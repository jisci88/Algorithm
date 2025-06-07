// PrimeFactorizer - Miller-Rabin + Pollard Rho 기반 소인수분해 템플릿 클래스 (Supports up to 2^63 - 1)

#pragma once

#include <algorithm> // sort
#include <numeric> // gcd
#include <cmath> // llabs
#include <vector>
#include <random>
#include <chrono>



template<typename T>
class PrimeFactorizer {
public:
    PrimeFactorizer(T nn) 
    : rng{std::chrono::steady_clock::now().time_since_epoch().count()}, n{nn} {}

    std::vector<T> get_prime_factorization();
    
private:
    T Pollard_rho(T n);
    T safe_mul_mod(T a, T b, T mod); // use safe_mul_mod if system does not support __int128
    T mul_mod128(T a, T b, T mod);
    T pow_mod(T n, T e, T mod);
    bool MillerRabin(T n, T a);
    bool isPrime(T n);
    std::vector<T> calculate_prime_factorization(T n);
    
    /*
    Miller Rabin method definitely guarantees that it is a prime number in range of long long
    using primes range 2 from 37
    */
    static constexpr int BasePrimes[]{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    std::mt19937_64 rng;
    T n;
};



template<typename T>
std::vector<T> PrimeFactorizer<T>::get_prime_factorization() {
    return calculate_prime_factorization(n);
}

template<typename T>
std::vector<T> PrimeFactorizer<T>::calculate_prime_factorization(T n) {
    std::vector<T> factorization{};
    
    for (const T p : BasePrimes) {
        while (n % p == 0) {
            factorization.push_back(p);
            n /= p;
        }
    }

    while (n > 1) {
        factorization.push_back(Pollard_rho(n));
        n /= factorization.back();
    }
    
    std::sort(factorization.begin(), factorization.end());
    return factorization;
}

template<typename T>
T PrimeFactorizer<T>::Pollard_rho(T n) {
    if (isPrime(n)) return n;

    for (int tries{0}; tries < 200; ++tries) {
        std::uniform_int_distribution<T> dist(1, n - 1);

        T x{2}, y{2}, d{1};
        T c{dist(rng)}; // const of f(x) = (x^2 + c) mod n, 1 <= c < n

        auto f{[&](T x) { return (mul_mod128(x, x, n) + c ) % n; }};

        while (d == 1) {
            x = f(x);
            y = f(f(y));
            d = std::gcd(std::llabs(x-y), n);
        }
        if (d != n) return d;
    }
    return n; // faild to find gcd
}

template<typename T>
T PrimeFactorizer<T>::pow_mod(T n, T e, T mod) {
    T res{1};

    while (e > 0) {
        if (e & 1) res = mul_mod128(res, n, mod);
        n = mul_mod128(n, n, mod);
        e >>= 1;
    }
    return res;
}

template<typename T>
T PrimeFactorizer<T>::mul_mod128(T a, T b, T mod) {
    return (__int128)a * b % mod;
}

template<typename T>
T PrimeFactorizer<T>::safe_mul_mod(T a, T b, T mod) {
    if (a < b) std::swap(a, b);
    
    T res{0};
    a %= mod;
    while (b > 0) {
        if (b & 1) res = (res + a) % mod;
        a = (a + a) % mod;
        b >>= 1;
    }
    return res;
}

template<typename T>
bool PrimeFactorizer<T>::MillerRabin(T n, T a) { // a^n
/*
Step 1. n - 1 = d * 2^r. d is odd

Step 2. find what d and r are. repeating divide (n-1) from 2 to make d odd

Step 3. determine if `n` is likely prime

-> n can be a prime number if meets at least one of the following expression.

    3-1. a^d = 1 (mod n)  

    3-2. a^(d*2^i) = -1 (mod n), for some 0 <= i < r

*/
    
    T d{n - 1}; // n - 1 = d * 2^r
    int r{0};
    while ((d & 1) == 0) { // calculates d and r    ref.Step 2
        ++r;
        d >>= 1;
    }

    T x{pow_mod(a, d, n)}; // a^d % n    ref-Step3-1
    if (x == 1 || x == n - 1) return true; // replace -1 with n - 1 for math logic

    for (int i{1}; i < r; ++i) { // x = a^(d*2) % n, a^(d*2^2) % n, a^(d*2^3) % n, ...
        x = mul_mod128(x, x, n);
        if (x == n - 1) return true; // ref-Step3-2
        // what if x == a^(d*2^i) == 1? It is a clear evidence that n is not a prime number
    }
    return false;
}

template<typename T>
bool PrimeFactorizer<T>::isPrime(T n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    // cases of n == 1, n == p, n % p == 0 are resolved
    for (const int p: BasePrimes) if (!MillerRabin(n, p)) return false;
    return true;
}
