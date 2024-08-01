#include <stdio.h>
#include <gmp.h>

// Função para verificar a primalidade usando o método Miller-Rabin modificado
int Is_Prime_ME(const mpz_t n, int reps) {
    mpz_t a, x, y, n_1, d;
    size_t i, r, s = 0;

    gmp_randstate_t state;
    gmp_randinit_default(state);

    mpz_inits(a, x, y, n_1, d, NULL);
    mpz_sub_ui(n_1, n, 1); // n_1 = n - 1

    // Calcula s (máximo r tal que 2^r divide (n-1))
    s = mpz_scan1(n_1, 0);
    mpz_fdiv_q_2exp(d, n_1, s); // d = (n - 1) / 2^s

    for (i = 0; i < reps; i++) {
        do {
            mpz_urandomm(a, state, n_1); // número aleatório de 0 até n-1
        } while (mpz_cmp_ui(a, 1) <= 0); // Repete até que a > 1

        mpz_powm(x, a, d, n); // x = a^d mod n

        if (mpz_cmp_ui(x, 1) != 0 && mpz_cmp(x, n_1) != 0) {
            for (r = 1; r < s; r++) {
                mpz_powm_ui(y, x, 2, n); // y = x^2 mod n
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(a, x, y, n_1, d, NULL);
                    gmp_randclear(state);
                    return 0; // n é composto
                }
                mpz_set(x, y); // x = y
            }
            if (mpz_cmp_ui(x, 1) != 0) {
                mpz_clears(a, x, y, n_1, d, NULL);
                gmp_randclear(state);
                return 0; // n é composto
            }
        }
    }

    mpz_clears(a, x, y, n_1, d, NULL);
    gmp_randclear(state);
    return 1; // n é primo
}

void find_next_prime(mpz_t result, const mpz_t start) 
{
    mpz_t candidate;
    mpz_init(candidate);

    // Inicia com o próximo número ímpar após start
    mpz_add_ui(candidate, start, 1);
    if (mpz_even_p(candidate)) {
        mpz_add_ui(candidate, candidate, 1);
    }

    // Incrementa candidate até encontrar o próximo primo
    while (!Is_Prime_ME(candidate, 25)) {
        mpz_add_ui(candidate, candidate, 2); // Incrementa por 2 para garantir números ímpares
    }

    mpz_set(result, candidate);
    mpz_clear(candidate);
}

int main() {
    mpz_t start, next_prime;
    mpz_inits(start, next_prime, NULL);

    printf("Digite o valor inicial: ");
    gmp_scanf("%Zd", start);

    find_next_prime(next_prime, start);

    gmp_printf("O próximo primo após %Zd é %Zd\n", start, next_prime);

    mpz_clears(start, next_prime, NULL);
    return 0;
}
