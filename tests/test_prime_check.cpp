#include <gtest/gtest.h>
#include <gmp.h>
#include "prime_check.h"


TEST(PrimeCheckTest, CompareGMPandME) {
    mpz_t n;
    gmp_randstate_t state;
    mpz_init(n);

    unsigned long int num_bits = 1024; 
    int reps = 15; // Nível de confiança para os testes de primalidade

    // Inicializa o estado do gerador de números aleatórios
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL)); // Seed com o tempo atual

    // Gera um número primo aleatório
    Generate_random_prime(primes, state, num_bits);

        for (const char prime : primes) 
        {
            mpz_set_str(n, prime, 10);
            EXPECT_EQ(Is_Prime_GMP(n, reps), Is_Prime_ME(n, reps));
        }
    mpz_clear(n);
}

