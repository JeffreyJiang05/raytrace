#include <cstdio>

#include "gpu/gpu.hpp"
#include "math/functions.hpp"
#include "math/linalg/vec.hpp"
#include "math/sampling.hpp"

KERNEL test_vectors()
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    math::vector<int, 2> v0{ index, index + 1 }, v1{ index - 1, index - 2 };
    auto res = v0 + v1;
    printf("%d - [%d %d]\n", index, res[0], res[1]);
}

int main()
{
    test_vectors<<<4, 4>>>();
    cudaDeviceSynchronize();
    return 0;
}