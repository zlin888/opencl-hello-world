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
#include <jpeg_loader.hpp>
#include <iostream>

using namespace std;
using namespace utils;

class ConvolutionCL : public utils::CL
{
public:
    using CL::CL;
    void runKernel();
};

void ConvolutionCL::runKernel()
{
    JpegLoader jpeg_loader;
    const JpegLoader::ImageInfo *img_info = jpeg_loader.Load("../assets/cat-640-426.jpeg");


}

int main(int argc, char **argvs)
{
    cout << kernel_name << endl; // print Kernel Name
    ConvolutionCL cl(kernel_source, kernel_name);
    cl.runKernel();
}
