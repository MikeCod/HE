#include <fhe/ckks/ckks.hpp>
#include <iostream>

#define PRECISION_BITS 30

using namespace hehub;
using std::cout;
using std::cerr;
using std::endl;

inline bool equal(const double a, const double b, const double error_factor = 0.0000001) {
    return abs(a - b) < error_factor;
}

int main() {
    try {
        constexpr double input = 4.55566;
        constexpr double expected = input * input; // Expect square

        CkksParams params = ckks::create_params(4096, PRECISION_BITS);
        CkksSk sk(params); // Generate secret key
        RlweKsk relin_key = hehub::get_relin_key(sk, params.additional_mod); // Generate relinearization key
        CkksCt ct = ckks::encrypt(ckks::encode(input, params), sk); // Encode before encrypting

        // Make any kind of operation
        // Here's a square ct * ct
        ct = ckks::mult(ct, ct, relin_key);

		// Decrypt before decoding
        const double decryptedOutput = ckks::decode(ckks::decrypt(ct, sk));

        cout << "Input:_____" << input << endl << endl;
        cout << "Decrypted:_" << decryptedOutput << endl;
        cout << "Expected:__" << expected << endl;
        cout << (equal(decryptedOutput, expected) ? "Success !" : "Failed") << endl;
    } catch (const std::string &err) {
        cerr << err << endl;
    }
}
