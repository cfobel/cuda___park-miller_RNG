#ifndef ___CUDA_PARK_MILLER_RNG__HPP___
#define ___CUDA_PARK_MILLER_RNG__HPP___


#include <vector>
#include <iostream>
#include <cutil_inline.h>
#include "random_number_generator/ParkMillerRNG.hpp"
using namespace std;


extern "C"
void parkmillerGPU(int *d_Output, unsigned int seed, int cycles,
       unsigned int grid_size, unsigned int block_size, unsigned int N);


class CUDAParkMillerRNG {
protected:
    int *d_Output;
    int data_size;
    unsigned int seed;
public:
    CUDAParkMillerRNG(unsigned int seed=1) : seed(seed), data_size(0), d_Output(NULL) {}

    vector<int> get_values(int cycles, unsigned int grid_size,
                            unsigned int block_size, unsigned int N){
        vector<int> output(N);
        int output_size = output.size() * sizeof(int);
        if(data_size < output_size) {
            reset();
            cout << "Allocating " << output_size << " bytes..." << endl;
            cutilSafeCall(cudaMalloc((void **)&d_Output, output_size));
            cout << "  DONE" << endl;
            data_size = output_size;
        }
        cutilSafeCall(cudaMemset(d_Output, 0, output_size));
        cutilSafeCall(cudaThreadSynchronize());
        parkmillerGPU(d_Output, seed, cycles, grid_size, block_size, N);
        cutilSafeCall(cudaThreadSynchronize());
        cutilSafeCall(cudaMemcpy(&output[0], d_Output, output_size, cudaMemcpyDeviceToHost));

        return output;
    }

    void reset() {
        if(NULL != d_Output) {
            cudaFree(d_Output);
        }
    }

    ~CUDAParkMillerRNG() {
        reset();
    }
};

#endif
