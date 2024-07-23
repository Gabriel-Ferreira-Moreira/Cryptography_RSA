#include <stdio.h>
#include <gmp.h>
#include <benchmark/benchmark.h>


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

static void ExpoM(benchmark::State& state) 
{
    mpz_t base, expoente, mod, resultado;
    mpz_inits(base, expoente, mod, resultado, NULL);

        // Inicializa base, expoente e mod com valores grandes
        mpz_set_str(base, "12345678901234567890", 10);
        mpz_set_str(expoente, "98765432109876543210", 10);
        mpz_set_str(mod, "13579135791357913579", 10);

        for (auto _ : state)
      {
        // Calcula a exponenciação modular
        ExpoM(resultado, base, expoente, mod);
      }

        // Imprime o resultado
        //gmp_printf("ExpoM(%Zd, %Zd, %Zd) = %Zd\n", base, expoente, mod, resultado);

    // Libera a memória alocada para base, expoente, mod e resultado
    mpz_clears(base, expoente, mod, resultado, NULL);

}

    BENCHMARK(ExpoM);
    BENCHMARK_MAIN();

