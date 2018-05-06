#ifndef OPENCL_CLUTILS_H
#define OPENCL_CLUTILS_H

#include <cstdio>
#include <string>

#define checkCLError(errno) {\
    if(CL_SUCCESS != errno) {\
        printf("error in %s, line%d, error no. %d\n", __FILE__, __LINE__, errno);\
    }\
}

std::string clLoadProgramSource(const char* filename);

#endif // OPENCL_CLUTILS_H