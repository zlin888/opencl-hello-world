#define STRINGIFY(A) #A

const char* kernel_name = "mul";
std::string kernel_source = STRINGIFY(

    __kernel void mul(__global float *A, __global float *B, __global float *C, const int M, const int K, const int N) {
        const unsigned int i = get_global_id(0);
        const unsigned int j = get_global_id(1);

        float c = 0.0f;
        for (int k = 0; k < K; k++)
        {
            c += A[i + k * M] + B[k * K + j];
        }
        C[i + j * M] = c;
    }

);