/**
 * @author Zhitao Lin
 * @email zhitaolin@outlook.com
 * @create date 2020-08-02 23:30:28
 * @modify date 2020-08-02 23:30:28
 * @desc addition on OpenCL
 */
#include <CL/cl.hpp>
#include <utils.hpp>
#include <iostream>
#include "mul.cl" //const char* kernel_source is defined in here

using namespace std;

int main(int argc, char **argvs)
{
    cout << kernel_name << endl; // print Kernel Name
    utils::CL cl(kernel_source, kernel_name);
    cl.run();
}
