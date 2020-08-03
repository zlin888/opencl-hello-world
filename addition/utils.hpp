/**
 * @author Zhitao Lin
 * @email zhitaolin@outlook.com 
 * @create date 2020-08-03 11:14:17
 * @modify date 2020-08-03 11:14:17
 * @desc utils for opencl 
 */
#ifndef UTILS_HPP 
#define UTILS_HPP 

#include "CL/cl.hpp"
#include <iostream> 

cl_context createContext() {
    cl_int errNum;
    cl_uint numPlatforms;
    cl_platform_id firstPlatformId;
    cl_context context = NULL;

    // errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
    std::cout << numPlatforms << std::endl;
} 

#endif