# Homomorphic encryption

## Introduction

#### What is homomorphic encryption?
Homomorphic encryption (HE) is a cryptographic primitive which allows computation on encrypted messages without the need of decryption or revealing of any infomation about the messages. The notion of fully homomorphic encryption (FHE) is specified by those HE schemes that allow to evaluate circuits of arbituary depth composed with any type of gates, and was first instantiated by Craig Gentry in 2009. Ever since the breakthrough of Gentry, the field of FHE has gone through years of rapid development, and has become a crucial part of state-of-the-art privacy enhancing technologies. In view of the great impact FHE technology has had on cryptography and privacy technology, the authors of papers BV11 and BGV12 about the BGV scheme were awarded Gödel Prize recently.

There are four generations of FHE schemes so far. The schemes of BFV, BGV, and CKKS are currently often applied in leveled mode, in which no bootstrapping procedure will be run and the circuits to evaluate need to be of limited depth. These schemes support SIMD evalutation natively. On the other hand, the scheme of FHEW/TFHE has practical solution of bootstrapping, which enables FHE mode natively. 


## Building and Installation
Currently the library only requires header-only [third-party dependencies](third-party/), which need no manual pre-installation. The library is built with CMake (>= 3.14), and tested on Linux with toolchain GCC (>= 8.0) and on MacOS with toolchain Clang (>= 12.0).

To build the library, use the following command to configure and build:
```bash
cmake -S . -B build
cmake --build build
```

To install the library, run the following command:
```bash
sudo cmake --install build
```
You can now compile your C program with `-lhehub` to use this library.

## Usage

The usage of homomorphic encryption with HEhub is very simple. Below is [an example](example.cpp) computing the Basel series with the CKKS scheme.
```cpp
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
```

### Compilation
```sh
g++ example.cpp -lhehub -Wall
```

## Copyrights

This library is an improvement of [this repository by PrimiHub](https://github.com/primihub/hehub).
