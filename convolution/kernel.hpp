/**
 * @author Zhitao Lin
 * @email zhitao.lin@mail.mcgill.ca
 * @create date 2020-09-01 18:33:53
 * @modify date 2020-09-01 18:33:53
 * @desc Chapter-4---Basic-OpenCL-Examples_2013_Heterogeneous-Computing-with-OpenCL
 */

#define STRINGIFY(A) #A

const char *kernel_name = "convolution";
std::string kernel_source = STRINGIFY(
    __kernel void convolution(__global float *src_data, __global float *dest_data, int W, int H, int Wd, int Hd) //Rotation Parameters
    {
        //Work-item gets its index within index space
        const int xi = get_global_id(0);
        const int yi = get_global_id(1);

        if ((xi + 2) % 3 == 0 && (yi + 2) % 3 == 0) {
            int xd = (xi + 2) / 3 - 1;
            int yd = (yi + 2) / 3 - 1;
            dest_data[yd * Wd + xd] = src_data[yi * W + xi];
        }
    });
