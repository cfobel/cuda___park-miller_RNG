#ifndef ___CUDA_PARK_MILLER__HPP___
#define ___CUDA_PARK_MILLER__HPP___

class CUDAParkMiller {
typedef double real_t;
protected:
    static real_t const a = 16807;      //ie 7**5
    static real_t const m = 2147483647; //ie 2**31-1
    real_t const reciprocal_m;
    real_t const reciprocal_m_sub_1;
    unsigned int seed;
public:
    __device__ CUDAParkMiller(unsigned int seed) : seed(seed),
        reciprocal_m(1.0 / m), reciprocal_m_sub_1(1.0 / (m - 1)) {}

    __device__ int MOD(real_t value, real_t divisor, real_t divisor_inv) {
        return value - floor(value * divisor_inv) * divisor;
    }

    __device__ int get_value() {
        real_t temp = seed * a;
        seed = MOD(temp, m, reciprocal_m);
        return seed;
    }

    __device__ int rand_int(int max_value) {
        return get_value() % (max_value + 1);
    }

    __device__ real_t rand_double() {
        return get_value() * reciprocal_m_sub_1;
    }
};


#endif
