#include "utils.hpp"
#include <stdio.h>
#include <math.h>
using namespace std;
namespace myutils
{

    CL::CL(string kernel_source, const char *kernel_name) : kernel_source(kernel_source), kernel_name(kernel_name)
    {
        setup();
    };

    void CL::getPlatformId()
    {
        cl_uint numPlatforms;
        cl_uint num_entries = 1;
        this->err = clGetPlatformIDs(num_entries, &this->platformId, &numPlatforms);
        //oclErrorString is also defined in util.cpp and comes from the NVIDIA SDK
        printf("number of platform %d\n", numPlatforms);
        printf("oclGetPlatformID: %s\n", oclErrorString(this->err));
        checkAndPrint("fail to get platformID");
    }

    void CL::getDeviceId()
    {
        cl_uint numDevices;
        this->err = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
        printf("clGetDeviceIDs (get number of devices): %s\n", oclErrorString(this->err));
        clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, numDevices,
                       &deviceId, NULL);
        printf("number of devices %d\n", numDevices);
        checkAndPrint("fail to get deviceID");

        size_t valueSize;
        clGetDeviceInfo(deviceId, CL_DEVICE_NAME, 0, NULL, &valueSize);
        char *value = (char *)malloc(valueSize);
        clGetDeviceInfo(deviceId, CL_DEVICE_NAME, valueSize, value, NULL);
        printf("%d. Device: %s\n", numDevices, value);
        free(value);
    }

    void CL::createContext()
    {
        this->context = clCreateContext(0, 1, &this->deviceId, NULL, NULL, &this->err);
        printf("clCreateContext: %s\n", oclErrorString(this->err));
        checkAndPrint("fail to create context");
    }

    void CL::createCommandQueue()
    {
        this->commandQueue = clCreateCommandQueue(this->context, this->deviceId, CL_QUEUE_PROFILING_ENABLE, &this->err);
    }

    void CL::loadProgram()
    {
        const char *cSource = this->kernel_source.c_str();
        size_t lengths = (size_t)this->kernel_source.size();
        this->program = clCreateProgramWithSource(this->context, 1,
                                                  &cSource, &lengths, &this->err);
    }

    void CL::buildProgram()
    {
        this->err = clBuildProgram(this->program, 1, &this->deviceId, NULL, NULL, NULL);
        printf("clBuildProgram: %s\n", oclErrorString(this->err));
        cl_build_status build_status;
        this->err = clGetProgramBuildInfo(this->program, this->deviceId, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &build_status, NULL);

        char *build_log;
        size_t ret_val_size;
        this->err = clGetProgramBuildInfo(this->program, this->deviceId, CL_PROGRAM_BUILD_LOG, 0, NULL, &ret_val_size);

        build_log = new char[ret_val_size + 1];
        this->err = clGetProgramBuildInfo(this->program, this->deviceId, CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, NULL);
        build_log[ret_val_size] = '\0';
        printf("BUILD LOG: \n %s", build_log);
        //}
        printf("program built\n");
        delete build_log;
    }

    void CL::runKernel()
    {
        //initialize our CPU memory arrays, send them to the device and set the kernel arguements
        int size = 10;
        int M = 5;
        int K = 2;
        int N = 10;
        unsigned int A_SIZE = M * K;
        unsigned int B_SIZE = K * N;
        unsigned int C_SIZE = M * N;

        float *A = new float[A_SIZE];
        float *B = new float[B_SIZE];
        float *C = new float[C_SIZE];

        for (int i = 0; i < A_SIZE; i++)
        {
            A[i] = 1.0f * i;
        }

        for (int i = 0; i < B_SIZE; i++)
        {
            B[i] = 1.0f * i;
        }

        for (int i = 0; i < C_SIZE; i++)
        {
            C[i] = 0.0f;
        }

        printf("Creating mem in device\n");
        // create mem in device
        size_t array_size = sizeof(float) * size;
        this->cl_a = clCreateBuffer(this->context, CL_MEM_READ_ONLY, sizeof(float) * A_SIZE, NULL, &this->err);
        this->cl_b = clCreateBuffer(this->context, CL_MEM_READ_ONLY, sizeof(float) * B_SIZE, NULL, &this->err);
        this->cl_c = clCreateBuffer(this->context, CL_MEM_WRITE_ONLY, sizeof(float) * C_SIZE, NULL, &this->err);

        printf("Pushing data to the GPU\n");
        //push our CPU arrays to the GPU
        this->err = clEnqueueWriteBuffer(this->commandQueue, this->cl_a, CL_TRUE, 0, sizeof(float) * A_SIZE, A, 0, NULL, NULL);
        this->err = clEnqueueWriteBuffer(this->commandQueue, this->cl_b, CL_TRUE, 0, sizeof(float) * B_SIZE, B, 0, NULL, NULL);

        printf("set the arguements of our kernel\n");
        //set the arguements of our kernel
        this->err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cl_a);
        printf("set arg0: %s\n", oclErrorString(this->err));
        this->err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&cl_b);
        printf("set arg1: %s\n", oclErrorString(this->err));
        this->err = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&cl_c);
        printf("set arg2: %s\n", oclErrorString(this->err));
        this->err = clSetKernelArg(kernel, 3, sizeof(unsigned int), (void *)&M);
        printf("set arg3: %s\n", oclErrorString(this->err));
        this->err = clSetKernelArg(kernel, 4, sizeof(unsigned int), (void *)&K);
        printf("set arg4: %s\n", oclErrorString(this->err));
        this->err = clSetKernelArg(kernel, 5, sizeof(unsigned int), (void *)&N);
        printf("set arg5: %s\n", oclErrorString(this->err));

        //clean up allocated space.
        delete[] A;
        delete[] B;
        delete[] C;

        //execute the kernel
        const size_t globalWorkSize = C_SIZE;
        cl_event event;
        printf("in runKernel\n");
        this->err = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL, &globalWorkSize, NULL, 0, NULL, &event);
        printf("clEnqueueNDRangeKernel: %s\n", oclErrorString(this->err));

        //Wait for the command queue to finish these commands before proceeding
        clFinish(commandQueue);

        printRuntimeInfo(event);
        //lets check our calculations by reading from the device memory and printing out the results
        float c_done[globalWorkSize];
        this->err = clEnqueueReadBuffer(commandQueue, cl_c, CL_TRUE, 0, sizeof(float) * globalWorkSize, &c_done, 0, NULL, NULL);
        printf("clEnqueueReadBuffer: %s\n", oclErrorString(this->err));

        printMatrix(M, N, c_done);
    }
    bool CL::checkAndPrint(string errMsg)
    {
        string stdMsg = "";
        return this->checkAndPrint(errMsg, stdMsg);
    }

    bool CL::checkAndPrint(string errMsg, string stdMsg)
    {
        if (this->err != CL_SUCCESS)
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

    void CL::setup()
    {
        getPlatformId();
        getDeviceId();
        createContext();
        createCommandQueue();
        loadProgram();
        buildProgram();

        // initialize our kernel from the program
        this->kernel = clCreateKernel(this->program, this->kernel_name, &this->err);
        printf("clCreateKernel: %s\n", oclErrorString(this->err));
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

    void printMatrix(int M, int N, float *matrix)
    {
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%g", matrix[i * M + j]);
                if (matrix[i * M + j] < 10)
                {
                    printf("  ");
                }
                else
                {
                    printf(" ");
                }
            }
            printf("\n");
        }
    }

    void printRuntimeInfo(cl_event event)
    {
        cl_ulong queued_time, end_time;
        clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &queued_time, NULL);
        clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end_time, NULL);
        double runtime = (double)(end_time - queued_time) * pow(10, -9);
        cout << "queued_time: " << queued_time << endl;
        cout << "end_time: " << end_time << endl;
        cout << "runtime: " << runtime << endl;
    }

    void displayImage(const cv::Mat &image)
    {
        cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
        cv::imshow("Display Image", image);
        cv::waitKey(0);
    }
} // namespace myutils