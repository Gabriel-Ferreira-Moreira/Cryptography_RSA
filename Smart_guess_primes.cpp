#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <benchmark/benchmark.h>


void ExpoM(mpz_t resultado, const mpz_t base, const mpz_t expoente, const mpz_t mod);
int Is_Prime_ME(const mpz_t n, int reps);
void Smart_guess_primes(mpz_t prime, mpz_t rand_num, int reps);

static void Smart_guess_primes(benchmark::State& state) 
{
    mpz_t n, prime, rand_num;
    int reps = 5;

    // Inicializa as variáveis
    mpz_inits(n, prime, rand_num, NULL);
    mpz_set_str(n, "1296005097246682578520326409", 10);
    mpz_set_str(rand_num, "12345678901234567890", 10);

    // Testa a primalidade do número n
        int result = Is_Prime_ME(n, reps);
    printf("Is_Prime_ME(%s, %d) = %d\n", mpz_get_str(NULL, 10, n), reps, result);

    // Gera um número primo próximo a rand_num
    for (auto _ : state)
    {
    Smart_guess_primes(prime, rand_num, reps);
    }
    gmp_printf("Smart_guess_primes: %Zd\n", prime);

    // Limpa a memória
    mpz_clears(n, prime, rand_num, NULL);

}

    BENCHMARK(Smart_guess_primes);
    BENCHMARK_MAIN();

void ExpoM(mpz_t resultado, const mpz_t base, const mpz_t expoente, const mpz_t mod) 
{
    mpz_t pot, exp, res;
    mpz_inits(pot, exp, res, NULL);

    mpz_mod(pot, base, mod); // pot = base % mod
    mpz_set_ui(res, 1); // resultado = 1

    mpz_set(exp, expoente); // exp = expoente

    while (mpz_cmp_ui(exp, 0) > 0) {
        // Se o bit menos significativo de exp é 1
        if (mpz_odd_p(exp)) { // if (expoente % 2 == 1)
            mpz_mul(res, res, pot);
            mpz_mod(res, res, mod); // resultado = (resultado * pot) % mod
        }

        mpz_mul(pot, pot, pot);
        mpz_mod(pot, pot, mod); // pot = (pot * pot) % mod

        mpz_fdiv_q_2exp(exp, exp, 1); // exp = exp/2
    }

    mpz_set(resultado, res); // resultado = res

    mpz_clears(pot, exp, res, NULL);
}


int Is_Prime_ME(const mpz_t n, int reps) {
    mpz_t a, x, y, n_1, d, temp, exp_r_d;
    size_t i, r, s = 0;
    int pode_ser_primo = 1; // 1 = provavelmente primo, 0 = com certeza composto

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_inits(a, x, y, n_1, d, temp, exp_r_d, NULL);
    mpz_sub_ui(n_1, n, 1); // n_1 = n - 1

    // Calcula s (máximo r tal que 2^r divide (n-1))
    mpz_set(temp, n_1);
    mpz_set_ui(d, 1);
    
    while (mpz_even_p(temp)) { // enquanto temp for par (temp % 2 = 0)
        mpz_fdiv_q_2exp(temp, temp, 1); // temp = temp / 2
        s++;
        mpz_mul_2exp(d, d, 1); // d = d * (2^1)
    }

    // d = (n-1) / 2^s
    mpz_divexact(d, n_1, d); // d = (n - 1) / 2^s [divisão exata de n_1 por d]

    for (i = 0; i < reps; i++) {
        // Gera um número aleatório a no intervalo ]1, n-1[
        do {
            mpz_urandomm(a, state, n); // número aleatório de 0 até n-1
        } while (mpz_cmp_ui(a, 1) <= 0); // Repete até que a > 1

        // Exponenciação modular: y = a^d mod n
        ExpoM(y, a, d, n);

        pode_ser_primo = 0; // Assume que é composto a princípio

        // Verifica s vezes
        for (r = 0; r < s; r++) {
            // Exponenciação modular: (2^r) * d
            mpz_set_ui(temp, 2);
            mpz_pow_ui(exp_r_d, temp, r); // exp_r_d = 2^r
            mpz_mul(exp_r_d, exp_r_d, d); // exp_r_d = (2^r) * d
            ExpoM(x, a, exp_r_d, n); // x = a^(2^r * d) % n

            if (mpz_cmp_ui(y, 1) == 0 || mpz_cmp(x, n_1) == 0) {
                pode_ser_primo = 1; // Não é composto se y == 1 ou x == n - 1
                break;
            }

            mpz_set(y, x); // Atualiza y
        }

        if (pode_ser_primo == 0) {
            break;
        }
    }

    mpz_clears(a, x, y, n_1, d, temp, exp_r_d, NULL);
    gmp_randclear(state);

    return pode_ser_primo;
}

void Smart_guess_primes(mpz_t prime, mpz_t rand_num, int reps) {
    mpz_t v[8];
    size_t i;
    mpz_t r, thirty, thirty_one, temp, teste;
    mpz_inits(r, thirty, thirty_one, temp, teste, NULL);
    mpz_init_set_ui(v[0], 0);
    mpz_init_set_ui(v[1], 6);
    mpz_init_set_ui(v[2], 4);
    mpz_init_set_ui(v[3], 2);
    mpz_init_set_ui(v[4], 4);
    mpz_init_set_ui(v[5], 2);
    mpz_init_set_ui(v[6], 4);
    mpz_init_set_ui(v[7], 6);

    mpz_set_ui(thirty, 30);
    mpz_set_ui(thirty_one, 31);

    // Calcula r = rand_num % 30
    mpz_mod(r, rand_num, thirty);

    // rand_num_copy = rand_num + (31 - r)
    mpz_sub(temp, thirty_one, r);
    mpz_add(rand_num, rand_num, temp);

    // Verifica se o número é primo
    
    // Loop para adicionar valores de v[i] e verificar primalidade
    for (i = 0; i < 8; i++) {
        mpz_add(rand_num, rand_num, v[i]);
        int result = Is_Prime_ME(rand_num, reps);
        if (result > 0) {
            break;
        } else if (i == 7) {
            i = 0;
        }
    }

    mpz_set(prime, rand_num);

    mpz_clears(r, thirty, thirty_one, temp, NULL);
    // Limpar a memória
    for (int i = 0; i < 8; i++) {
        mpz_clear(v[i]);
    }
}
