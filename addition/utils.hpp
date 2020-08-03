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
    cl_context createContext();
    bool checkAndPrint(string errMsg = string(), string stdMsg = string());
    const char *oclErrorString(cl_int error);
} // namespace Utils

#endif