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

using namespace std;
using namespace utils;

class ImgRotationCL : public utils::CL
{
public:
    using CL::CL;
    void runKernel(float theta);
};

void ImgRotationCL::runKernel(float theta)
{
    int W = 10;
    int H = 10;

    float inputArray[W * H];
    for (int i = 0; i < W * H; i++)
    {
        inputArray[i] = 1.0f * i;
    }
    utils::printMatrix(W, H, inputArray);

    printf("Creating mem in device\n");
    auto inputBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, W * H * sizeof(float), NULL, &this->err);
    auto outputBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, W * H * sizeof(float), NULL, &this->err);

    printf("enqueue memory\n");
    this->err = clEnqueueWriteBuffer(this->commandQueue, inputBuffer, CL_TRUE, 0, W * H * sizeof(float), inputArray, 0, NULL, NULL);

    float cos_theta = cos(theta);
    float sin_theta = sin(theta);

    printf("set the arguements of our kernel\n");
    //set the arguements of our kernel
    this->err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer);
    printf("set arg0: %s\n", oclErrorString(this->err));
    this->err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&outputBuffer);
    printf("set arg1: %s\n", oclErrorString(this->err));
    this->err = clSetKernelArg(kernel, 2, sizeof(int), (void *)&W);
    printf("set arg2: %s\n", oclErrorString(this->err));
    this->err = clSetKernelArg(kernel, 3, sizeof(int), (void *)&H);
    printf("set arg3: %s\n", oclErrorString(this->err));
    this->err = clSetKernelArg(kernel, 4, sizeof(float), (void *)&cos_theta);
    printf("set arg4: %s\n", oclErrorString(this->err));
    this->err = clSetKernelArg(kernel, 5, sizeof(float), (void *)&sin_theta);
    printf("set arg5: %s\n", oclErrorString(this->err));

    //execute the kernel
    const size_t globalWorkSize[2] = {(size_t)W, (size_t)H};
    cl_event event;
    printf("in runKernel\n");
    this->err = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL, globalWorkSize, NULL, 0, NULL, &event);
    printf("clEnqueueNDRangeKernel: %s\n", oclErrorString(this->err));

    //Wait for the command queue to finish these commands before proceeding
    clFinish(commandQueue);

    utils::printRuntimeInfo(event);
    //lets check our calculations by reading from the device memory and printing out the results
    float output[W * H];
    this->err = clEnqueueReadBuffer(commandQueue, outputBuffer, CL_TRUE, 0, sizeof(float) * W * H, &output, 0, NULL, NULL);
    printf("clEnqueueReadBuffer: %s\n", oclErrorString(this->err));

    utils::printMatrix(W, H, output);

    if (inputBuffer)
        clReleaseMemObject(inputBuffer);
    if (outputBuffer)
        clReleaseMemObject(outputBuffer);
    printf("finish!\n");
}

int main(int argc, char **argvs)
{
    cout << kernel_name << endl; // print Kernel Name
    ImgRotationCL cl(kernel_source, kernel_name);
    cl.runKernel(3.14 / 2);
}
