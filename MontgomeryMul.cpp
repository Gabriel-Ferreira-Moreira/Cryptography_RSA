#include <stdio.h>
#include <gmp.h>
#include <benchmark/benchmark.h>

// Função para multiplicação de Montgomery
void MontgomeryMul(mpz_t resultado, const mpz_t a, const mpz_t b, const mpz_t mod, const mpz_t R_inv, const mpz_t R, const mpz_t mu);
void ajustar_R(mpz_t R, const mpz_t mod);


static void BM_Montg(benchmark::State& state) 
{
    mpz_t a, b, mod, R, R_inv, mu, resultado;

    mpz_inits(a, b, mod, R, R_inv, mu, resultado, NULL);

    // Defina os valores de a, b, mod e R aqui
    mpz_set_ui(a, 1232);
    mpz_set_ui(b, 124);
    mpz_set_ui(mod, 97);
    mpz_set_ui(R, 100);

    // Precompute R_inv and mu
    mpz_invert(R_inv, R, mod);
    mpz_invert(mu, mod, R);
    mpz_neg(mu, mu);
    mpz_mod(mu, mu, R);

        for (auto _ : state)
      {
        // Calcula a exponenciação modular
        MontgomeryMul(resultado, a, b, mod, R_inv, R, mu);
      }

       
    // Imprime o resultado
    gmp_printf("Resultado Mont: %Zd\n", resultado);

    // Libera a memória das variáveis
    mpz_clears(a, b, mod, R, R_inv, mu, resultado, NULL);

}
static void BM_MulMod_GMP(benchmark::State& state) 
{
       mpz_t a, b, mod, resultado, temp;

        mpz_inits(a, b, mod, resultado, temp, NULL);

        // Defina os valores de a, b, mod e R aqui
          mpz_set_ui(a, 1232);
        mpz_set_ui(b, 124);
        mpz_set_ui(mod, 97);

         for (auto _ : state) 
        {
            mpz_mul(temp, a, b);
            mpz_mod(resultado, temp, mod); // resultado = (resultado * pot) % mod        
        }


            gmp_printf("Resultado GMP: %Zd\n", resultado);


    // Libera a memória alocada para base, expoente, mod e resultado
    mpz_clears(a, b, mod, resultado, temp, NULL);
}

    BENCHMARK(BM_Montg);
    BENCHMARK(BM_MulMod_GMP);
    BENCHMARK_MAIN();


   void MontgomeryMul(mpz_t resultado, const mpz_t a, const mpz_t b, const mpz_t mod, const mpz_t R, const mpz_t R_inv, const mpz_t mu) 
   {
    mpz_t A_prime, B_prime, T, m, u;
    mpz_inits(A_prime, B_prime, T, m, u, NULL);

    // Passo 1: Transforma a e b para o domínio de Montgomery
    mpz_mul(A_prime, a, R);
    mpz_mod(A_prime, A_prime, mod); // A' = (a * R) % mod

    mpz_mul(B_prime, b, R);
    mpz_mod(B_prime, B_prime, mod); // B' = (b * R) % mod

    // Passo 2: Multiplicação de A' e B' (sem mod ainda)
    mpz_mul(T, A_prime, B_prime);

    // Passo 3: Redução de Montgomery
    mpz_mul(m, T, mu); 
    mpz_mod(m, m, R); // m = (T * mu) mod R

    // u = (T + m * mod) / R
    mpz_mul(u, m, mod);
    mpz_add(u, u, T);
    mpz_fdiv_q(u, u, R);

    // Passo 4: Ajuste se necessário
    if (mpz_cmp(u, mod) >= 0) {
        mpz_sub(u, u, mod);
    }

    // Passo 5: Converte de volta do domínio de Montgomery
    mpz_mul(resultado, u, R_inv);
    mpz_mod(resultado, resultado, mod);

    // Liberação de variáveis temporárias
    mpz_clears(A_prime, B_prime, T, m, u, NULL);
}

void ajustar_R(mpz_t R, const mpz_t mod) 
{
    mpz_t gcd;
    mpz_init(gcd);

    // Inicialmente, defina R como uma potência de 2 maior que Z
    mpz_set_ui(R, 1);
    while (mpz_cmp(R, mod) <= 0) 
    {
        mpz_mul_ui(R, R, 2); // R = R * 2 (próxima potência de 2)
    }

    // Ajusta R até que gcd(R, Z) seja igual a 1
    do {
        mpz_gcd(gcd, R, mod);
        if (mpz_cmp_ui(gcd, 1) != 0) {
            // Se gcd(R, Z) não for 1, continue aumentando R
            mpz_mul_ui(R, R, 2); // Próxima potência de 2
        }
    } while (mpz_cmp_ui(gcd, 1) != 0);

    // Libera a memória da variável gcd
    mpz_clear(gcd);
}
