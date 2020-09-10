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
    uint32_t img_size;
    Mat image;
};

void ConvolutionCL::prepare()
{
    image = imread("../assets/cat.jpeg");
    myutils::displayImage(image);
}

void ConvolutionCL::runKernel()
{
    prepare();
    auto inputBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, img_size * sizeof(uint8_t), NULL, &this->err);
    auto outputBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, img_size * sizeof(uint8_t), NULL, &this->err);

    // this->err = clEnqueueWriteBuffer(this->commandQueue, inputBuffer, CL_TRUE, 0, img_size * sizeof(uint8_t), img_info->pData, 0, NULL, NULL);
    // printf("enqueue buffer: %s\n", oclErrorString(this->err));

    // this->err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer);
    // printf("set arg0: %s\n", oclErrorString(this->err));
}

int main(int argc, char **argvs)
{
    cout << kernel_name << endl; // print Kernel Name
    ConvolutionCL cl(kernel_source, kernel_name);
    cl.runKernel();
}
