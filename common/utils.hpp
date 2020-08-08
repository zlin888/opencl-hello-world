/**
 * @author Zhitao Lin
 * @email zhitaolin@outlook.com
 * @create date 2020-08-03 11:14:17
 * @modify date 2020-08-03 11:14:17
 * @desc utils for opencl
 */
#ifndef UTILS_HPP
#define UTILS_HPP

#include <CL/cl.hpp>
#include <iostream>
#include <string>

using namespace std;
namespace utils
{
    const char *oclErrorString(cl_int error);
    void printMatrix(int M, int N, float *matrix);
    void printRuntimeInfo(cl_event event);
    class CL {
    public:
        string kernel_source;
        const char* kernel_name;
        CL(string kernel_source, const char* kernel_name) :
            kernel_source(kernel_source), kernel_name(kernel_name) {};

        ~CL() {
            // if(kernel)clReleaseKernel(kernel);
            if (program)
                clReleaseProgram(program);
            if (commandQueue)
                clReleaseCommandQueue(commandQueue);
            if (kernel)
                clReleaseKernel(kernel);
            if (cl_a)
                clReleaseMemObject(cl_a);
            if (cl_b)
                clReleaseMemObject(cl_b);
            if (cl_c)
                clReleaseMemObject(cl_c);
            if (context)
                clReleaseContext(context);
        };

        cl_platform_id platformId;
        cl_device_id deviceId;
        cl_context context;
        cl_int err;
        cl_command_queue commandQueue;
        cl_program program;
        cl_mem cl_a;
        cl_mem cl_b;
        cl_mem cl_c;
        cl_kernel kernel;
        void getPlatformId();
        void getDeviceId();
        void createContext();
        void createCommandQueue();
        void loadProgram();
        void buildProgram();
        void runKernel();
        void run();
        bool checkAndPrint(string errMsg, string stdMsg);
        bool checkAndPrint(string errMsg);
    };
} // namespace utils

#endif