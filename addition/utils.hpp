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
    extern cl_int err;

    cl_platform_id getPlatformId();
    cl_device_id getDeviceId(cl_platform_id platformId);
    cl_context createContext(cl_device_id deviceId);
    cl_command_queue createCommandQueue(cl_context context, cl_device_id deviceId);
    cl_program loadProgram(std::string kernel_source, cl_context context);
    cl_kernel createAndsetupKernel(cl_program program,
                                   cl_context context,
                                   cl_command_queue commandQueue,
                                   cl_mem *cl_c);
    void runKernel(cl_kernel kernel, cl_command_queue commandQueue, cl_mem cl_c);
    void buildProgram(cl_program program, cl_device_id deviceId);
    bool checkAndPrint(string errMsg = string(), string stdMsg = string());
    const char *oclErrorString(cl_int error);
} // namespace utils

#endif