#define STRINGIFY(A) #A
std::string kernel_source = STRINGIFY(

__kernel void vec_add(__global const float* a, __global const float* b, __global float* c, const unsigned int n)
{
    unsigned int i = get_global_id(0);

    if (id < n) { // check overflow
        c[i] = a[i] + b[i];
    }
}

);