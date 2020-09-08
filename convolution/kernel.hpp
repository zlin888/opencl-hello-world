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
    __kernel void convolution(__global float *src_data, __global float *dest_data, int W, int H, float sinTheta, float cosTheta) //Rotation Parameters
    {
        //Work-item gets its index within index space
        const int xi = get_global_id(0);
        const int yi = get_global_id(1);

        float x0 = W / 2.0f;
        float y0 = H / 2.0f;
        float delta_x = xi - x0;
        float delta_y = yi - y0;

        int xpos = (int)(delta_x * cosTheta + delta_y * sinTheta + x0);
        int ypos = (int)(delta_y * cosTheta + delta_x * sinTheta + y0);

        //Bound Checking
        if (((int)xpos >= 0) && ((int)xpos < W) &&
            ((int)ypos >= 0) && ((int)ypos < H))
        {
            dest_data[yi * W + xi] = src_data[ypos * W + xpos];
        }
    });
