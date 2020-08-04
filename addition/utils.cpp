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

    cl_program loadProgram(string kernel_source, cl_context context)
    {
        const char *cSource = kernel_source.c_str();
        size_t lengths = (size_t)kernel_source.size();
        cl_program program = clCreateProgramWithSource(context, 1,
                                                       &cSource, &lengths, &err);
        printf("clCreateProgramWithSource: %s\n", oclErrorString(err));
        return program;

        // buildEecutable();

        //Free buffer returned by file_contents
        // free(cSourceCL);
    }

    void buildProgram(cl_program program, cl_device_id deviceId)
    {
        err = clBuildProgram(program, 1, &deviceId, NULL, NULL, NULL);
        printf("clBuildProgram: %s\n", oclErrorString(err));
        //if(err != CL_SUCCESS){
        cl_build_status build_status;
        err = clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &build_status, NULL);

        char *build_log;
        size_t ret_val_size;
        err = clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, 0, NULL, &ret_val_size);

        build_log = new char[ret_val_size + 1];
        err = clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, NULL);
        build_log[ret_val_size] = '\0';
        printf("BUILD LOG: \n %s", build_log);
        //}
        printf("program built\n");
    }

    cl_kernel createAndsetupKernel(cl_program program,
                                   cl_context context,
                                   cl_command_queue commandQueue,
                                   cl_mem *cl_c_ptr)
    {
        // initialize our kernel from the program
        cl_kernel kernel = clCreateKernel(program, "vec_add", &err);
        printf("clCreateKernel: %s\n", oclErrorString(err));

        //initialize our CPU memory arrays, send them to the device and set the kernel arguements
        int size = 10;
        float *a = new float[size];
        float *b = new float[size];
        float *c = new float[size];
        for (int i = 0; i < size; i++)
        {
            a[i] = 1.0f * i;
            b[i] = 1.0f * i;
            c[i] = 0.0f;
        }

        printf("Creating mem in device\n");
        // create mem in device
        size_t array_size = sizeof(float) * size;
        cl_mem cl_a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * size, NULL, &err);
        cl_mem cl_b = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * size, NULL, &err);
        *cl_c_ptr = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * size, NULL, &err);

        printf("Pushing data to the GPU\n");
        //push our CPU arrays to the GPU
        cl_event event;
        err = clEnqueueWriteBuffer(commandQueue, cl_a, CL_TRUE, 0, sizeof(float) * size, a, 0, NULL, &event);
        err = clEnqueueWriteBuffer(commandQueue, cl_b, CL_TRUE, 0, sizeof(float) * size, b, 0, NULL, &event);
        clReleaseEvent(event);

        printf("set the arguements of our kernel\n");
        //set the arguements of our kernel
        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cl_a);
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&cl_b);
        err = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)cl_c_ptr);
        err = clSetKernelArg(kernel, 3, sizeof(unsigned int), (void *)&size);
        //Wait for the command queue to finish these commands before proceeding
        clFinish(commandQueue);

        //clean up allocated space.
        delete[] a;
        delete[] b;
        delete[] c;

        return kernel;
    }
    void runKernel(cl_kernel kernel, cl_command_queue commandQueue, cl_mem cl_c)
    {
        printf("in runKernel\n");
        //execute the kernel
        cl_event event;
        const size_t globalWorkSize = 10;
        err = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, &event);
        clReleaseEvent(event);
        printf("clEnqueueNDRangeKernel: %s\n", oclErrorString(err));

        //lets check our calculations by reading from the device memory and printing out the results
        float c_done[globalWorkSize];
        err = clEnqueueReadBuffer(commandQueue, cl_c, CL_TRUE, 0, sizeof(float) * globalWorkSize, &c_done, 0, NULL, &event);
        printf("clEnqueueReadBuffer: %s\n", oclErrorString(err));
        clReleaseEvent(event);

        for (int i = 0; i < globalWorkSize; i++)
        {
            printf("c_done[%d] = %g\n", i, c_done[i]);
        }
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