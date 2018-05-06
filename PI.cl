__kernel void PI1(const int num, __local float* loc, __global float* r)
{
    unsigned int id = get_global_id(0);
    unsigned int globalSize = get_global_size(0);
    unsigned int lid = get_local_id(0);
    unsigned int localSize = get_local_size(0);

    float tmp = 0.0f;
    float rnum = 1.0f / num; // length of each step

    for(int i = id; i < num; i += globalSize)
    {
        float x = (0.5f + i) * rnum; // x coordinate at i-th step
        tmp += 4.0f / (1.0f + x * x); // dirivative at x
    }

    loc[lid] = tmp * rnum; // square of rectangle
    barrier(CLK_LOCAL_MEM_FENCE);

	// binary reduction within a group
    for(int i = localSize / 2; i > 0; i >>= 1)
    {
        if(lid < i)
        {
            loc[lid] += loc[lid + i];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

	// sum of each group
    if(lid == 0)
    {
        r[get_group_id(0)] = loc[0];
    }
}

__kernel void PI2(const int groupNum, __local float* loc, __global float* r)
{
    unsigned int lid = get_local_id(0);
    unsigned int localSize = get_local_size(0);

    float tmp = 0.0f;

	// groupNum is the length of r
    for(int i = lid; i < groupNum; i += localSize)
    {
        tmp += r[i];
    }

    loc[lid] = tmp;
    barrier(CLK_LOCAL_MEM_FENCE);

	// binary reduction within groups
    for(int i = localSize / 2; i > 0; i >>= 1)
    {
        if(lid < i)
        {
            loc[lid] += loc[lid + i];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

	// final result
    if(lid == 0)
    {
        r[0] = loc[0];
    }
}