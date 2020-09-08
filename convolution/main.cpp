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
#include <memory>

using namespace std;
using namespace utils;

class ConvolutionCL : public utils::CL
{
public:
    using CL::CL;
    ~ConvolutionCL()
    {
        delete[] img_info->pData;
    };
    void prepare();
    void runKernel();
    void showImage(const JpegLoader::ImageInfo *img_info);

    unique_ptr<const JpegLoader::ImageInfo> img_info;

    uint32_t img_size;
};
void ConvolutionCL::prepare()
{
    JpegLoader jpeg_loader;
    // memcpy((void *)img_info, jpeg_loader.Load("../assets/cat-640-426.jpeg"), sizeof(JpegLoader::ImageInfo));
    img_info = make_unique<const JpegLoader::ImageInfo>(*jpeg_loader.Load("../assets/cat-640-426.jpeg"));
    img_size = img_info->nWidth * img_info->nHeight;
}

void ConvolutionCL::runKernel()
{
    prepare();
    // showImage(img_info.get());
    auto inputBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, img_size * sizeof(uint8_t), NULL, &this->err);
    auto outputBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, img_size * sizeof(uint8_t), NULL, &this->err);
    // showImage(img_info.get());
}

void ConvolutionCL::showImage(const JpegLoader::ImageInfo *img_info)
{

    uint8_t *data = img_info->pData;
    for (int i = 0; i < img_info->nHeight; i++)
    {
        for (int j = 0; j < img_info->nWidth; j++)
        {
            cout << (int)data[i * img_info->nWidth + j] << "|";
        }
        cout << endl;
    }
}

int main(int argc, char **argvs)
{
    cout << kernel_name << endl; // print Kernel Name
    ConvolutionCL cl(kernel_source, kernel_name);
    cl.runKernel();
}
