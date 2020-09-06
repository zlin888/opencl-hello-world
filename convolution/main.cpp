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
    void prepare();
    void runKernel();
    void showImage(const JpegLoader::ImageInfo *img_info);

    const JpegLoader::ImageInfo *img_info;
};
void ConvolutionCL::prepare()
{
    JpegLoader jpeg_loader;
    img_info = jpeg_loader.Load("../assets/cat-640-426.jpeg");
}

void ConvolutionCL::runKernel()
{
    prepare()
    showImage(img_info);
    


    showImage(img_info);
}

void ConvolutionCL::showImage(const JpegLoader::ImageInfo *img_info)
{
    uint8_t *data = img_info->pData;
    uint32_t width = img_info->nWidth;

    cout << "\n";
    cout << (int)data[300 + 120 * width] << "|" << (int)data[300 + 121 * width] << "|" << (int)data[300 + 122 * width] << endl;
    cout << (int)data[301 + 120 * width] << "|" << (int)data[301 + 121 * width] << "|" << (int)data[301 + 122 * width] << endl;
    cout << (int)data[302 + 120 * width] << "|" << (int)data[302 + 121 * width] << "|" << (int)data[302 + 122 * width] << endl;
}

int main(int argc, char **argvs)
{
    cout << kernel_name << endl; // print Kernel Name
    ConvolutionCL cl(kernel_source, kernel_name);
    cl.runKernel();
}
