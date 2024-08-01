#include "prime_check.h"

void Generate_random_prime(mpz_t prime, gmp_randstate_t state, unsigned long int num_bits) 
{
    mpz_t random_number;
    mpz_init(random_number);

    // Gera um número aleatório com o número especificado de bits
    mpz_urandomb(random_number, state, num_bits);

    // Encontra o próximo número primo a partir do número aleatório
    mpz_nextprime(prime, random_number);

    mpz_clear(random_number);
}
int Is_Prime_GMP(const mpz_t n, int reps) {
    return mpz_probab_prime_p(n, reps);
}

int Is_Prime_ME(const mpz_t n, int reps) {
    mpz_t a, x, y, n_1, d;
    size_t i, r, s = 0;

    gmp_randstate_t state;
    gmp_randinit_default(state);

    mpz_inits(a, x, y, n_1, d, NULL);
    mpz_sub_ui(n_1, n, 1); // n_1 = n - 1

    // Calcula s (máximo r tal que 2^r divide (n-1))
    s = mpz_scan1(n_1, 0);
    mpz_fdiv_q_2exp(d, n_1, s); // d=temp/2^s

    for (i = 0; i < reps; i++) {
        do {
            mpz_urandomm(a, state, n_1); // número aleatório de 0 até n-1
        } while (mpz_cmp_ui(a, 1) <= 0); // Repete até que a > 1

        mpz_powm(x, a, d, n); // x = a^d mod n

        for (r = 0; r < s; r++) {
            mpz_powm_ui(y, x, 2, n); // y = x^2 mod n
            if (mpz_cmp_ui(y, 1) == 0 && mpz_cmp_ui(x, 1) != 0 && mpz_cmp(x, n_1) != 0) {
                return 0;
            }
            mpz_set(x, y); // x = y
        }

        if (mpz_cmp_ui(y, 1) != 0) {
            return 0;
        }
    }
    return 1;
}
