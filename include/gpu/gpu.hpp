#ifndef GPU_RAYTRACE_GPU_HPP
#define GPU_RAYTRACE_GPU_HPP

#if defined(__CUDACC__)

#define KERNEL __global__ void
#define CPU __host__
#define GPU __device__
#define CPU_GPU CPU GPU

#define CONSTANT __constant__
#define SHARED __shared__
#define MANAGED __managed__

#else

#define DECLARE_KERNEL
#define CPU
#define GPU
#define CPU_GPU

#define CONSTANT
#define SHARED
#define MANAGED

#endif

#endif //GPU_RAYTRACE_GPU_HPP
