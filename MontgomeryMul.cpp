#include <stdio.h>
#include <gmp.h>
#include <benchmark/benchmark.h>

// Função para multiplicação de Montgomery
void MontgomeryMul(mpz_t resultado, mpz_t P, const mpz_t mod, const mpz_t R, const mpz_t mu);
void ajustar_R(mpz_t R, const mpz_t mod);


static void BM_Montg(benchmark::State& state) 
{
    mpz_t a, b, mod, R, R_inv, mu, resultado, P;

    mpz_inits(a, b, mod, R, R_inv, mu, resultado, NULL);

    // Defina os valores de a, b, mod e R aqui
    mpz_set_ui(a, 42);
    mpz_set_ui(b, 17);
    mpz_set_ui(mod, 97);
    mpz_set_ui(R, 128);

    // Precompute R_inv and mu
    //mpz_invert(R_inv, R, mod);
    mpz_invert(mu, mod, R);
    mpz_neg(mu, mu);
    mpz_mod(mu, mu, R);
    mpz_mul(P, a, b);           // P = a * b

        for (auto _ : state)
      {
        // Calcula a exponenciação modular
        MontgomeryMul(resultado, P, mod, R, mu);
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
        mpz_set_ui(a, 42);
        mpz_set_ui(b, 17);
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


 void MontgomeryMul(mpz_t resultado, mpz_t P, const mpz_t mod, const mpz_t R, const mpz_t mu) 
   {
    mpz_t q, temp;

    // Inicializa as variáveis
    mpz_inits(q, temp, NULL);

    
    // Passo 1: q ← mu * (a * b % R) % R
    mpz_fdiv_q_2exp(P, P, mpz_scan1(P, 0));           // P = P % R
    mpz_mul(q, mu, P);          // q = mu * (a * b % R)
    mpz_fdiv_q_2exp(q, q, mpz_scan1(P, 0));           // q = q % R
 

    // Passo 2: C ← (P + N * q) / R
    mpz_mul(temp, mod, q);        // temp = N * q
    mpz_add(temp, temp, P);     // temp = P + N * q
    mpz_fdiv_q_2exp(resultado, temp, mpz_scan1(P, 0)); // C = (P + N * q)  % R


    // Passo 3: Ajuste se C ≥ N
    if (mpz_cmp(resultado, mod) >= 0) 
    {
        mpz_sub(resultado, resultado, mod);       // C = C - N
    }

    // Libera a memória das variáveis temporárias
    mpz_clears(q, temp, NULL);
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
