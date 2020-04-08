#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

#ifdef __CUDACC__
#define KERNEL_ARGS2(grid, block) <<< grid, block >>>
#define KERNEL_ARGS3(grid, block, sh_mem) <<< grid, block, sh_mem >>>
#define KERNEL_ARGS4(grid, block, sh_mem, stream) <<< grid, block, sh_mem, stream >>>
#else
#define KERNEL_ARGS2(grid, block)
#define KERNEL_ARGS3(grid, block, sh_mem)
#define KERNEL_ARGS4(grid, block, sh_mem, stream)
#endif

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, char* file, int line, bool abort = true)
{
    if (code != cudaSuccess)
    {
        fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort) exit(code);
    }
}



__global__ void chaotic(double* X, double* R, int y)
{
    int index = threadIdx.x + blockIdx.x * 512;

	if (X[index]==0){X[index] = R[index]/4;}
	R[index] = R[index]+0.0001*X[index]+0.002;
	if (R[index]>=4){R[index] = 3.9+0.0025*R[index];}
        for (int i = 1; i < y; i++)
        {
            X[index] = X[index] * R[index]* (1 - X[index]);
        }
}

__global__ void postprocess(double* X, unsigned int* v)
{
    int index = threadIdx.x + blockIdx.x * 512;

    unsigned int M1 = 0, M2 = 0, M3 = 0, M4 = 0;

    if (blockIdx.x == 0)
    {
        unsigned long long* wsk;
        wsk = reinterpret_cast<long long unsigned*>(&X[index]);
        unsigned long long C1 = *wsk;
        wsk = reinterpret_cast<long long unsigned*>(&X[index + 33]);
        unsigned long long C2 = *wsk % 1024;

        unsigned int lsbmask = 0x80000000;

        for (int i = 0; i < 32; i++)
        {
            if ((lsbmask & C1) > 0)
            {
                M1 = M1 | lsbmask;
            }
            if ((lsbmask & C2) > 0)
            {
                M3 = M3 | lsbmask;
            }
            lsbmask = lsbmask >> 1;
        }

        lsbmask = 0x80000000;
        unsigned long long msbmask = 0x8000000000000000;

        for (int i = 0; i < 32; i++)
        {
            if ((msbmask & C1) > 0)
            {
                M2 = M2 | lsbmask;
            }
            if ((msbmask & C2) > 0)
            {
                M4 = M4 | lsbmask;
            }
            lsbmask = lsbmask >> 1;
            msbmask = msbmask >> 1;
        }

        v[index] = ((M1 + M4) ^ M3) + M2;
    }
}


int main(void)
{
    srand(time(NULL));

    const int Beta = 512;

    double* X = new double[Beta];
    double* R = new double[Beta];
    unsigned int* v = new unsigned int[Beta];

    double* dX = new double[Beta];
    double* dR = new double[Beta];
    unsigned int* dv = new unsigned int[Beta];

    int* check = new int[Beta];

    int size = Beta * sizeof(double);
    int sizev = Beta * sizeof(unsigned int);

    gpuErrchk(cudaMalloc((void**)&dX, size));
    gpuErrchk(cudaMalloc((void**)&dR, size));
    gpuErrchk(cudaMalloc((void**)&dv, sizev));

    X = (double*)malloc(size);
    R = (double*)malloc(size);
    v = (unsigned int*)malloc(sizev);

    for (int i = 0; i < Beta; i++)
    {
        check[i] = 0;
        v[i] = 0;

        X[i] = (rand() % 1000) / 1000.0;

        while (X[i] < 0.01)
        {
            X[i] = (rand() % 1000) / 1000.0;
        }

        R[i] = 3.86 + (X[i] * 0.14);
    }
	
    gpuErrchk(cudaMemcpy(dX, X, size, cudaMemcpyHostToDevice));
    gpuErrchk(cudaMemcpy(dR, R, size, cudaMemcpyHostToDevice));
	
    chaotic KERNEL_ARGS2(32, 512) (dX, dR, 50);
    postprocess KERNEL_ARGS2(32, 512) (dX, dv);
	
    gpuErrchk(cudaMemcpy(X, dX, size, cudaMemcpyDeviceToHost));
    gpuErrchk(cudaMemcpy(v, dv, sizev, cudaMemcpyDeviceToHost));

    gpuErrchk(cudaPeekAtLastError());
    gpuErrchk(cudaDeviceSynchronize());
	
    fstream plik;
    plik.open("plik.csv", ios::out);

    for (int i = 0; i < Beta; i++)
    {
        plik << X[i] <<";"<< v[i] <<endl;
    }
	
    return 0;
}