#include <stdio.h>
#include <gmp.h>
#include <benchmark/benchmark.h>

#if 1
void ExpoM(mpz_t resultado, const mpz_t base, const mpz_t expoente, const mpz_t mod) 
{

mpz_t pot, exp, res;
    mpz_inits(pot, exp, res, NULL);

    mpz_mod(pot, base, mod); // pot = base % mod
    mpz_set_ui(res, 1); // resultado = 1

    mpz_set(exp, expoente); // exp = expoente

    while (mpz_cmp_ui(exp, 0) > 0) 
    {
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

#else
 void ExpoM(mpz_t result, const mpz_t base, const mpz_t exponent, const mpz_t modulus) {
    // Initialize temporary variables
    mpz_t base_mod, exp, res;
    mpz_init(base_mod);
    mpz_init(exp);
    mpz_init_set_ui(res, 1);  // result := 1

    // base := base mod modulus
    mpz_mod(base_mod, base, modulus);

    // exp := exponent
    mpz_set(exp, exponent);

    // Main loop
    while (mpz_cmp_ui(exp, 0) > 0) {
        if (mpz_odd_p(exp)) {  // if (exponent mod 2 == 1)
            // result := (result * base) mod modulus
            mpz_mul(res, res, base_mod);
            mpz_mod(res, res, modulus);
        }

        // exponent := exponent >> 1
        mpz_fdiv_q_2exp(exp, exp, 1);

        // base := (base * base) mod modulus
        mpz_mul(base_mod, base_mod, base_mod);
        mpz_mod(base_mod, base_mod, modulus);
    }

    // result := res
    mpz_set(result, res);

    // Clear temporary variables
    mpz_clear(base_mod);
    mpz_clear(exp);
    mpz_clear(res);
}

#endif



static void BM_ExpoM(benchmark::State& state) 
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
        gmp_printf("ExpoM(%Zd, %Zd, %Zd) = %Zd\n", base, expoente, mod, resultado);

    // Libera a memória alocada para base, expoente, mod e resultado
    mpz_clears(base, expoente, mod, resultado, NULL);

}
static void BM_ExpoM_GMP(benchmark::State& state) 
{
    mpz_t base, expoente, mod, resultado;
    mpz_inits(base, expoente, mod, resultado, NULL);

        // Inicializa base, expoente e mod com valores grandes
        mpz_set_str(base, "12345678901234567890", 10);
        mpz_set_str(expoente, "98765432109876543210", 10);
        mpz_set_str(mod, "13579135791357913579", 10);

         for (auto _ : state) 
        {
        mpz_powm(resultado, base, expoente, mod);
        }

        // Imprime o resultado
        //gmp_printf("ExpoM(%Zd, %Zd, %Zd) = %Zd\n", base, expoente, mod, resultado);

    // Libera a memória alocada para base, expoente, mod e resultado
    mpz_clears(base, expoente, mod, resultado, NULL);
}

    BENCHMARK(BM_ExpoM);
    BENCHMARK(BM_ExpoM_GMP);
    BENCHMARK_MAIN();

