// #include "catch2/catch.hpp"
#include "fhe/ckks/ckks.hpp"
#include "fhe/common/bigint.hpp"
#include "fhe/common/mod_arith.hpp"
#include "fhe/common/ntt.hpp"
#include "fhe/common/permutation.hpp"
#include "fhe/common/sampling.hpp"
#include <type_traits>

using namespace hehub;

namespace hehub {
namespace ckks {
void fft_negacyclic_natural_inout(cc_double *coeffs, size_t log_dimension,
                                  bool inverse = false);
} // namespace ckks
} // namespace hehub

int main() {
    size_t dimension = 8;
    int scaling_bits = 30;
    CkksParams ct_params = ckks::create_params(dimension, {40}, 40, pow(2.0, scaling_bits));
    RlweSk sk(ct_params);

    // random ckks plain data
    auto data_count = dimension / 2;
    std::vector<double> plain_data(data_count);
    std::default_random_engine generator;
    std::normal_distribution<double> data_dist(0, 1);
    for (auto &d : plain_data) {
        d = data_dist(generator);
    }

    // encode
    auto pt = ckks::simd_encode(plain_data, ct_params);

    // encrypt
    auto ct = ckks::encrypt(pt, sk);

    // decrypt & decode
    auto data_recovered = ckks::simd_decode(ckks::decrypt(ct, sk));

    // check
    // REQUIRE(data_recovered.size() == dimension / 2);
    double eps = std::pow(2.0, 5 - scaling_bits); // noise's 6σ = 19.2 < 2^5
    REQUIRE_ALL_CLOSE(plain_data, data_recovered, eps);

	return 0;
}