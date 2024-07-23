#include <gmp.h>
#include <benchmark/benchmark.h>
#include <time.h>
#include <stdio.h>

int Is_Prime_ME(const mpz_t n, int reps); // Miller–Rabin primality test
void ExpoM(mpz_t resultado, const mpz_t base, const mpz_t expoente, const mpz_t mod); // Modular Exponentiation
int Is_Prime_GMP(const mpz_t n, int reps); // primality test GMP

  static void Is_Prime_ME(benchmark::State& state) 
  {
    mpz_t n;
    mpz_init_set_str(n, "1296005097246682578520326409", 10); // Número para teste
      for (auto _ : state)
      {
        Is_Prime_ME(n, 5); // 5 iterações do teste de primalidade
      }
    mpz_clear(n);
  }

  static void Is_Prime_GMP(benchmark::State& state) 
  {
    mpz_t n;
    mpz_init_set_str(n, "1296005097246682578520326409", 10); // Número para teste
      for (auto _ : state) 
      {
        Is_Prime_GMP(n, 5); // 5 iterações do teste de primalidade
      }
    mpz_clear(n);
  }

BENCHMARK(Is_Prime_ME);
BENCHMARK(Is_Prime_GMP);

BENCHMARK_MAIN();

int Is_Prime_GMP(const mpz_t n, int reps) // primality test GMP
{
  return mpz_probab_prime_p(n, reps); 
}
int Is_Prime_ME(const mpz_t n, int reps)
{
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
    
    while (mpz_even_p(temp)) // enquanto temp for par (temp % 2 = 0)
    {
        mpz_fdiv_q_2exp(temp, temp, 1); // temp = temp / 2
        s++;
        mpz_mul_2exp(d, d, 1); // d = d * (2^1)
    }

    // d = (n-1) / 2^s
    mpz_divexact(d, n_1, d); // d = (n - 1) / 2^s [divisão exata de n_1 por d]

    for (i = 0; i < reps; i++)
    {
        // Gera um número aleatório a no intervalo ]1, n-1[
        do
        {
            mpz_urandomm(a, state, n); // número aleatório de 0 até n-1
        } while (mpz_cmp_ui(a, 1) <= 0); // Repete até que a > 1

        // Exponenciação modular: y = a^d mod n
        ExpoM(y, a, d, n);

        pode_ser_primo = 0; // Assume que é composto a princípio

        // Verifica s vezes
        for (r = 0; r < s; r++) 
        {
            // Exponenciação modular: (2^r) * d
            mpz_set_ui(temp, 2);
            mpz_pow_ui(exp_r_d, temp, r); // exp_r_d = 2^r
            mpz_mul(exp_r_d, exp_r_d, d); // exp_r_d = (2^r) * d
            ExpoM(x, a, exp_r_d, n); // x = a^(2^r * d) % n

            if (mpz_cmp_ui(y, 1) == 0 || mpz_cmp(x, n_1) == 0)
            {
                pode_ser_primo = 1; // Não é composto se y == 1 ou x == n - 1
                break;
            }

            mpz_set(y, x); // Atualiza y
        }

        if (pode_ser_primo == 0)
        {
            break;
        }
    }

    mpz_clears(a, x, y, n_1, d, temp, exp_r_d, NULL);
    gmp_randclear(state);

    return pode_ser_primo;
}

void ExpoM(mpz_t resultado, const mpz_t base, const mpz_t expoente, const mpz_t mod)
{
    mpz_t pot, exp, res;
    mpz_inits(pot, exp, res, NULL);

    mpz_mod(pot, base, mod); // pot = base % mod
    mpz_set_ui(res, 1);      // resultado = 1

    mpz_set(exp, expoente); // exp = expoente

    while (mpz_cmp_ui(exp, 0) > 0)
    {
        // Se o bit menos significativo de exp é 1
        if (mpz_odd_p(exp)) // if (expoente % 2 == 1)
        {
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
