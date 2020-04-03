
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;


__global__ void chaotic(double *X, double *R, int y)
{
    int index = threadIdx.x + blockIdx.x * 1024;
    
    double sum = 0.0;
    double temp[48];
    
    if (X[index] >= 0.5)
    {
        for (int i = 1; i < y; i++)
        {
           // X[index] = X[index] * R[index] * (1 - X[index]);
        }
        R[index] = 3.99;
    }
    else
    {
        for (int i = 1; i < y; i++)
        {
            X[index] = X[index] * R[index] * (1 - X[index]);
            temp[i] = 3.86 + (X[index] * 0.14);
            sum = sum + temp[i];
            
        }
        R[index] = sum / y;
    }
}

__global__ void postprocess(double* X, double* R, unsigned int* v)
{
    int index = threadIdx.x + blockIdx.x * 1024;

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
    fstream plik;
    plik.open("plik.txt", ios::out);

    const int Beta = 1024;

    double *X = new double[Beta];
    double *R = new double[Beta];
    unsigned int *v = new unsigned int[Beta];
   

    double* dX = new double[Beta];
    double* dR = new double[Beta];
    unsigned int* dv = new unsigned int[Beta];


    int size = Beta * sizeof(double);
    int sizev = Beta * sizeof(unsigned int);

    cudaMalloc((void**)&dX, size);
    cudaMalloc((void**)&dR, size);
    cudaMalloc((void**)&dv, sizev);

    X = (double *)malloc(size);
    R = (double *)malloc(size);
    v = (unsigned int*)malloc(sizev);

    for (int i = 0; i < Beta; i++)
    {
            X[i] = (rand() % 1000) / 1000.0;

            while (X[i] < 0.01)
            {
                X[i] = (rand() % 1000) / 1000.0;
            }

            R[i] = 3.86 + (X[i] * 0.14);
        // plik << X[i] << " " <<R[i]<< endl;
    }
    
    cudaMemcpy(dX, X, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dR, R, size, cudaMemcpyHostToDevice);

    chaotic <<< 32, 1024 >>> (dX, dR, 48);

    chaotic <<< 32, 1024 >>> (dX, dR, 2);
    postprocess <<< 32, 1024 >>> (dX, dR, dv);

    cudaMemcpy(X, dX, size, cudaMemcpyDeviceToHost);
    cudaMemcpy(R, dR, size, cudaMemcpyDeviceToHost);
    cudaMemcpy(v, dv, sizev, cudaMemcpyDeviceToHost);


    for (int i = 0; i < Beta; i++)
    {
       plik << v[i] << endl;
        //plik << X[i] << " " << R[i] << endl;
    }

    return 0;
}
