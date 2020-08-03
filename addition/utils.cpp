#include "utils.hpp"
#include <stdio.h>

namespace utils
{
    cl_int err;
    cl_platform_id getPlatformId()
    {
        cl_platform_id platformId;
        cl_uint numPlatforms;
        cl_uint num_entries = 1;
        err = clGetPlatformIDs(num_entries, &platformId, &numPlatforms);
        //oclErrorString is also defined in util.cpp and comes from the NVIDIA SDK
        printf("oclGetPlatformID: %s\n", oclErrorString(err));
        if (!checkAndPrint("fail to get platformID"))
        {
            return NULL;
        }
        return platformId;
    }

    cl_device_id getDeviceId(cl_platform_id platformId)
    {
        cl_device_id deviceId;
        cl_uint numDevices;
        err = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 1, &deviceId, &numDevices);
        printf("clGetDeviceIDs (get number of devices): %s\n", oclErrorString(err));
        printf("number of devices %d\n", numDevices);
        if (!checkAndPrint("fail to get deviceID"))
        {
            return NULL;
        }
        return deviceId;
    }

    cl_context createContext(cl_device_id deviceId)
    {
        cl_context context = clCreateContext(0, 1, &deviceId, NULL, NULL, &err);
        printf("clCreateContext: %s\n", oclErrorString(err));
        if (!checkAndPrint("fail to create context"))
        {
            return NULL;
        }
        return context;
    }

    cl_command_queue createCommandQueue(cl_context context, cl_device_id deviceId)
    {
        cl_command_queue command_queue = clCreateCommandQueue(context, deviceId, 0, &err);
        return command_queue;
    }

    bool checkAndPrint(string errMsg, string stdMsg)
    {
        if (err != CL_SUCCESS)
        {
            if (!errMsg.empty())
            {
                cerr << errMsg << endl;
            }
            return false;
        }
        else
        {
            if (!stdMsg.empty())
            {
                cout << stdMsg << endl;
            }
            return true;
        }
    }

    // Helper function to get error string
    // *********************************************************************
    const char *oclErrorString(cl_int error)
    {
        static const char *errorString[] = {
            "CL_SUCCESS",
            "CL_DEVICE_NOT_FOUND",
            "CL_DEVICE_NOT_AVAILABLE",
            "CL_COMPILER_NOT_AVAILABLE",
            "CL_MEM_OBJECT_ALLOCATION_FAILURE",
            "CL_OUT_OF_RESOURCES",
            "CL_OUT_OF_HOST_MEMORY",
            "CL_PROFILING_INFO_NOT_AVAILABLE",
            "CL_MEM_COPY_OVERLAP",
            "CL_IMAGE_FORMAT_MISMATCH",
            "CL_IMAGE_FORMAT_NOT_SUPPORTED",
            "CL_BUILD_PROGRAM_FAILURE",
            "CL_MAP_FAILURE",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "CL_INVALID_VALUE",
            "CL_INVALID_DEVICE_TYPE",
            "CL_INVALID_PLATFORM",
            "CL_INVALID_DEVICE",
            "CL_INVALID_CONTEXT",
            "CL_INVALID_QUEUE_PROPERTIES",
            "CL_INVALID_COMMAND_QUEUE",
            "CL_INVALID_HOST_PTR",
            "CL_INVALID_MEM_OBJECT",
            "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
            "CL_INVALID_IMAGE_SIZE",
            "CL_INVALID_SAMPLER",
            "CL_INVALID_BINARY",
            "CL_INVALID_BUILD_OPTIONS",
            "CL_INVALID_PROGRAM",
            "CL_INVALID_PROGRAM_EXECUTABLE",
            "CL_INVALID_KERNEL_NAME",
            "CL_INVALID_KERNEL_DEFINITION",
            "CL_INVALID_KERNEL",
            "CL_INVALID_ARG_INDEX",
            "CL_INVALID_ARG_VALUE",
            "CL_INVALID_ARG_SIZE",
            "CL_INVALID_KERNEL_ARGS",
            "CL_INVALID_WORK_DIMENSION",
            "CL_INVALID_WORK_GROUP_SIZE",
            "CL_INVALID_WORK_ITEM_SIZE",
            "CL_INVALID_GLOBAL_OFFSET",
            "CL_INVALID_EVENT_WAIT_LIST",
            "CL_INVALID_EVENT",
            "CL_INVALID_OPERATION",
            "CL_INVALID_GL_OBJECT",
            "CL_INVALID_BUFFER_SIZE",
            "CL_INVALID_MIP_LEVEL",
            "CL_INVALID_GLOBAL_WORK_SIZE",
        };

        const int errorCount = sizeof(errorString) / sizeof(errorString[0]);

        const int index = -error;

        return (index >= 0 && index < errorCount) ? errorString[index] : "";
    }
} // namespace utils