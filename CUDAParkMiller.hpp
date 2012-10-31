#ifndef ___CUDA_PARK_MILLER__HPP___
#define ___CUDA_PARK_MILLER__HPP___

template <class int_t, class real_t>
class CUDAParkMiller {
protected:
    static int_t const a = 16807;      //ie 7**5
    static int_t const m = 2147483647; //ie 2**31-1
    real_t reciprocal_m;
    real_t reciprocal_m_sub_1;
    int_t seed;
public:
    __device__ CUDAParkMiller() : seed(0) {
        reciprocal_m = 1.0 / (float)m;
        reciprocal_m_sub_1 = 1.0 / (float)(m - 1);
    }

    __device__ CUDAParkMiller(int_t seed) : seed(seed) {
        reciprocal_m = 1.0 / (float)m;
        reciprocal_m_sub_1 = 1.0 / (float)(m - 1);
    }

    __device__ void set_seed(int_t i_seed) {
        seed = i_seed;
    }

    __device__ int_t MOD(real_t value, real_t divisor, real_t divisor_inv) {
        return value - (int_t)floor(value * divisor_inv) * divisor;
    }

    __device__ int_t current_value() {
        return seed;
    }

    __device__ int_t get_value() {
        real_t temp = seed * a;
        seed = MOD(temp, m, reciprocal_m);
        return seed;
    }

    __device__ int_t rand_int(int_t max_value) {
        return get_value() % (max_value + 1);
    }

    __device__ real_t rand_real() {
        return get_value() * reciprocal_m_sub_1;
    }
};


#endif
