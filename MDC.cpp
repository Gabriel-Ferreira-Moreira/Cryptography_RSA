#include <gmp.h>
#include <benchmark/benchmark.h>
#include <stdio.h>

void MDC(mpz_t gcd, const mpz_t n1, const mpz_t n2) 
{
    mpz_t resto, a, b;
    mpz_inits(a, b, resto, NULL); // Aloca memória para as variáveis

    // Inicializa a e b com os valores de n1 e n2
    mpz_set(a, n1);
    mpz_set(b, n2);

    do {
        // Calcula resto = a % b
        mpz_mod(resto, a, b);
        // Atualiza a e b
        mpz_set(a, b);
        mpz_set(b, resto);
    } while (mpz_cmp_ui(resto, 0) != 0); // Continua até resto ser 0

    // O MDC está em a
    mpz_set(gcd, a);

    // Libera a memória alocada para as variáveis temporárias
    mpz_clears(resto, a, b, NULL);
}
    static void MDC(benchmark::State& state) 
  {
        mpz_t n1, n2, gcd;
        mpz_inits(n1, n2, gcd, NULL);

        // Inicializa n1 e n2 com valores grandes
        mpz_set_str(n1, "1296005097246682578520326409", 10);
        mpz_set_str(n2, "1234567890123456789012345678901234567890", 10);

        for (auto _ : state)
      {
        // Calcula o MDC de n1 e n2
        MDC(gcd, n1, n2);
      }

        // Imprime o resultado
        //gmp_printf("MDC(%Zd, %Zd) = %Zd\n", n1, n2, gcd);

        // Libera a memória alocada para n1, n2 e gcd
        mpz_clears(n1, n2, gcd, NULL);

    }

    BENCHMARK(MDC);
    BENCHMARK_MAIN();

