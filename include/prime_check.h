#ifndef PRIME_CHECK_H
#define PRIME_CHECK_H

#include <gmp.h>

int Is_Prime_GMP(const mpz_t n, int reps);
int Is_Prime_ME(const mpz_t n, int reps);
void Generate_random_prime(mpz_t prime, gmp_randstate_t state, unsigned long int num_bits);

#endif
    