#include <CL/opencl.h>
#include <CL/cl.hpp>

#include <iostream>
#include <vector>

#include "ClUtils.h"
#include "PI.hpp"

using namespace std;
using namespace cl;

int main()
{
    vector<Platform> vp;
    vector<Device> vd;

    int err = CL_SUCCESS;

    // get platforms
    checkCLError(Platform::get(&vp));
    printf("platform size = %lld\n", vp.size());
    for(int i = 0; i < vp.size(); ++i)
    {
        string info;
        vp[i].getInfo(CL_PLATFORM_NAME, &info);
        printf("%s\n", info.c_str());
    }

    // get devices
    vp[1].getDevices(CL_DEVICE_TYPE_ALL, &vd);
    printf("device number = %lld\n", vd.size());
    for(int i = 0; i < vd.size(); ++i)
    {
        string info;
        vd[i].getInfo(CL_DEVICE_NAME, &info);
        printf("%s\n", info.c_str());
    }
    
    // get context
    cl_context_properties properties[] = {CL_CONTEXT_PLATFORM, 
        (cl_context_properties)(vp[1])(), 0};
    Context context(vd, properties, NULL, NULL, &err);
    checkCLError(err);

    // make program
    //string source = clLoadProgramSource("../PI.cl");
    string source(PI_SourceCode);
    vector<pair<const char*, ::size_t>> ps(1, 
        make_pair(source.c_str(), source.size()));
    Program program(context, ps, &err);
    checkCLError(err);

    // build program
    err = program.build(vd);
    if(err != CL_SUCCESS)
    {
        string errString;
        checkCLError(program.getBuildInfo(vd[0], 
            CL_PROGRAM_BUILD_LOG, &errString));
        printf("%s\n", errString.c_str());
        exit(1);
    }

    // get kernel
    Kernel pi1(program, "PI1", &err);
    checkCLError(err);

    Kernel pi2(program, "PI2", &err);
    checkCLError(err);

    int groupSize = 128;
    int num = 1000000;

    // get buffer
    Buffer buf(context, CL_MEM_READ_WRITE, groupSize * sizeof(int));

    // make command queue
    CommandQueue cq(context, vd[0], CL_QUEUE_PROFILING_ENABLE, &err);
    checkCLError(err);

    // enqueue kernel
    NDRange zero(0);
    NDRange block(groupSize);
    NDRange grid(groupSize * groupSize);

    checkCLError(pi1.setArg(0, num));
    checkCLError(pi1.setArg(1, __local(groupSize * sizeof(float))));
    checkCLError(pi1.setArg(2, buf));
    checkCLError(cq.enqueueNDRangeKernel(pi1, zero, grid, block));

    checkCLError(pi2.setArg(0, groupSize));
    checkCLError(pi2.setArg(1, __local(groupSize * sizeof(float))));
    checkCLError(pi2.setArg(2, buf));
    checkCLError(cq.enqueueNDRangeKernel(pi2, zero, block, block));

    // finish queue
    float r;
    checkCLError(cq.enqueueReadBuffer(buf, true, 0, sizeof(float), &r));
    checkCLError(cq.finish());

    printf("%f\n", r);

    return 0;
}
