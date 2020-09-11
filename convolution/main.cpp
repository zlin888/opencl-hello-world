/**
 * @author Zhitao Lin
 * @email zhitaolin@outlook.com
 * @create date 2020-08-02 23:30:28
 * @modify date 2020-08-02 23:30:28
 * @desc addition on OpenCL
 */
#include <CL/cl.hpp>
#include <utils.hpp>
#include <iostream>
#include <math.h>
#include "kernel.hpp" //const char* kernel_source is defined in here
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace std;
using namespace myutils;
using namespace cv;

class ConvolutionCL : public myutils::CL
{
public:
    using myutils::CL::CL;
    void prepare();
    void runKernel();
    Mat image;
};

void ConvolutionCL::prepare()
{
    image = imread("../assets/cat.jpeg");
    // myutils::displayImage(image);
}

void ConvolutionCL::runKernel()
{
    prepare();
    int H = image.size[0];
    int W = image.size[1];
    int Wd = floor(W / 3);
    int Hd = floor(H / 3);
    int img_size = H * W;

    auto inputBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, img_size * sizeof(uint8_t), NULL, &this->err);
    auto outputBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, Wd * Hd * sizeof(uint8_t), NULL, &this->err);

    this->err = clEnqueueWriteBuffer(this->commandQueue, inputBuffer, CL_TRUE, 0, img_size * sizeof(uint8_t), image.data, 0, NULL, NULL);
    printf("enqueue buffer: %s\n", oclErrorString(this->err));

    this->err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer);
    printf("set arg0: %s\n", oclErrorString(this->err));
    this->err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&outputBuffer);
    printf("set arg1: %s\n", oclErrorString(this->err));
    this->err = clSetKernelArg(kernel, 2, sizeof(int), (void *)&W);
    printf("set arg2: %s\n", oclErrorString(this->err));
    this->err = clSetKernelArg(kernel, 3, sizeof(int), (void *)&H);
    printf("set arg3: %s\n", oclErrorString(this->err));
    this->err = clSetKernelArg(kernel, 4, sizeof(int), (void *)&Wd);
    printf("set arg4: %s\n", oclErrorString(this->err));
    this->err = clSetKernelArg(kernel, 5, sizeof(int), (void *)&Hd);
    printf("set arg5: %s\n", oclErrorString(this->err));

    const size_t globalWorkSize[2] = {(size_t)W, (size_t)H};
    cl_event event;
    printf("in runKernel\n");
    this->err = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL, globalWorkSize, NULL, 0, NULL, &event);
    printf("clEnqueueNDRangeKernel: %s\n", oclErrorString(this->err));

    //Wait for the command queue to finish these commands before proceeding
    clFinish(commandQueue);

    myutils::printRuntimeInfo(event);
    //lets check our calculations by reading from the device memory and printing out the results
    float output[W * H];
    this->err = clEnqueueReadBuffer(commandQueue, outputBuffer, CL_TRUE, 0, sizeof(uint8_t) * Wd * Hd, &output, 0, NULL, NULL);
    printf("clEnqueueReadBuffer: %s\n", oclErrorString(this->err));

    myutils::printMatrix(Wd, Hd, output);

    if (inputBuffer)
        clReleaseMemObject(inputBuffer);
    if (outputBuffer)
        clReleaseMemObject(outputBuffer);
    printf("finish!\n");
}

int main(int argc, char **argvs)
{
    cout << kernel_name << endl; // print Kernel Name
    ConvolutionCL cl(kernel_source, kernel_name);
    cl.runKernel();
}
