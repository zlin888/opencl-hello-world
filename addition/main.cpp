/**
 * @author Zhitao Lin
 * @email zhitaolin@outlook.com 
 * @create date 2020-08-02 23:30:28
 * @modify date 2020-08-02 23:30:28
 * @desc addition on OpenCL
 */
#include <CL/cl.hpp>
#include "utils.hpp"
#include "addition.cl" //const char* kernel_source is defined in here

using namespace std;

int main(int argc, char **argvs)
{
    cl_mem memObjects[3] = {0, 0, 0};
    cl_int err;

    cl_platform_id platformId = utils::getPlatformId();
    cl_device_id deviceId = utils::getDeviceId(platformId);
    cl_context context = utils::createContext(deviceId);
    cl_command_queue commandQueue = utils::createCommandQueue(context, deviceId);
    cl_program program = utils::loadProgram(kernel_source, context);
    utils::buildProgram(program, deviceId);
    cl_mem cl_c;
    cl_kernel kernel = utils::createAndsetupKernel(program, context, commandQueue, &cl_c);
    utils::runKernel(kernel, commandQueue, cl_c);

    // if(kernel)clReleaseKernel(kernel);
    if (program)
        clReleaseProgram(program);
    if (commandQueue)
        clReleaseCommandQueue(commandQueue);
}
