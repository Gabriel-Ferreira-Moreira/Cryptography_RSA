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

static void BM_Is_Prime_GMP(benchmark::State &state) 
{
    mpz_t n;
    mpz_init_set_str(n, "1296005097246682578520326409", 10); // Número para teste
      for (auto _ : state) 
      {
        // Is_Prime_GMP(n, 5); // 5 iterações do teste de primalidade
        benchmark::DoNotOptimize(mpz_probab_prime_p(n, 5));
      }
      
    mpz_clear(n);
}

BENCHMARK(Is_Prime_ME);
BENCHMARK(BM_Is_Prime_GMP);

BENCHMARK_MAIN();


int Is_Prime_GMP(const mpz_t n, int reps) 
{
  mpz_probab_prime_p(n, reps);
}
int Is_Prime_ME(const mpz_t n, int reps) 
{
    mpz_t a, x, y, n_1, d;
    size_t i, r, s = 0;

    gmp_randstate_t state;
    gmp_randinit_default(state);

    mpz_inits(a, x, y, n_1, d, NULL);
    mpz_sub_ui(n_1, n, 1); // n_1 = n - 1

    // Calcula s (máximo r tal que 2^r divide (n-1))
    //n − 1 = 2s.d

    s = mpz_scan1(n_1, 0);
    mpz_fdiv_q_2exp(d, n_1, s);//d=temp/2^s

    for (i = 0; i < reps; i++) 
    {
      // Gera um número aleatório a no intervalo [2, n − 2]
        do {
          mpz_urandomm(a, state, n_1);     // número aleatório de 0 até n-1
        } while (mpz_cmp_ui(a, 1) <= 0); // Repete até que a > 1

        mpz_powm(x, a, d, n);//x = a^d mod n

          for (r = 0; r < s; r++) 
          {
            mpz_powm_ui(y, x, 2, n);//y = x^2 mod n
              if (mpz_cmp_ui(y, 1) == 0 && mpz_cmp_ui(x, 1) != 0 && mpz_cmp(x, n_1) != 0) 
              {
                return 0;
              }
            mpz_set(x, y);// x = y
          }

        if (mpz_cmp_ui(y, 1) != 0) 
        {
          return 0;
        }
    }
    return 1;
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
