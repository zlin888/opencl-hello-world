#ifndef JPEG_LOADER_H
#define JPEG_LOADER_H
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <stdint.h>
#include <iostream>
#include <cstring>

JSAMPLE * image_buffer;	/* Points to large array of R,G,B-order data */
int image_height;	/* Number of rows in image */
int image_width;		/* Number of columns in image */

GLOBAL(void)
write_JPEG_file(char *filename, int quality);
class JpegLoader
{
public:
    struct ImageInfo
    {
        uint32_t nWidth;
        uint32_t nHeight;
        uint8_t nNumComponent;
        uint8_t *pData;

        ImageInfo() = default;
        ImageInfo(const ImageInfo &SImageInfo) : nWidth(SImageInfo.nWidth),
                                                 nHeight(SImageInfo.nHeight), nNumComponent(SImageInfo.nNumComponent)
        {
            pData = (uint8_t *)malloc(sizeof(uint8_t) * nWidth * nHeight);
            memcpy(pData, SImageInfo.pData, sizeof(sizeof(uint8_t) * nWidth * nHeight));
            std::cout << "call ImageInfo's copy constructor" << std::endl;
        }
        // ~ImageInfo() {
        //     delete[] pData;
        //     std::cout << "call ImageInfo's destructor" << std::endl;
        // }
    };

    JpegLoader();
    ~JpegLoader();

    const ImageInfo *Load(const char *szFileName);

private:
    ImageInfo *m_pImageInfo;
    void Cleanup();

    struct ErrorManager
    {
        jpeg_error_mgr defaultErrorManager;
        jmp_buf jumpBuffer;
    };

    static void ErrorExit(j_common_ptr cinfo);
    static void OutputMessage(j_common_ptr cinfo);
};
#endif