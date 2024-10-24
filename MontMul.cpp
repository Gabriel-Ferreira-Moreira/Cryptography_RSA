#include <stdio.h>
#include <gmp.h>
#include <benchmark/benchmark.h>

// Função para multiplicação de Montgomery
void MontgomeryMul(mpz_t resultado, const mpz_t a, const mpz_t b, const mpz_t mod, const mpz_t R_inv, const mpz_t R, const mpz_t mu);
void ajustar_R(mpz_t R, const mpz_t mod);

#if 0
void ExpoM(mpz_t resultado, const mpz_t base, const mpz_t expoente, const mpz_t mod) 
{

mpz_t pot, exp, res;
    mpz_inits(pot, exp, res, NULL);

    mpz_mod(pot, base, mod); // pot = base % mod
    mpz_set_ui(res, 1); // resultado = 1

    mpz_set(exp, expoente); // exp = expoente

    while (mpz_cmp_ui(exp, 0) > 0) 
    {
        // Se o bit menos significativo de exp é 1 (se é impar)
        if (mpz_odd_p(exp)) { // if (expoente % 2 == 1)
            mpz_mul(res, res, pot);
            mpz_mod(res, res, mod); // resultado = (resultado * pot) % mod
        }

        mpz_mul(pot, pot, pot);
        mpz_mod(pot, pot, mod); // pot = (pot * pot) % mod

        mpz_fdiv_q_2exp(exp, exp, 1); // exp = exp/2 (arredonda pra baixo)
    }

    mpz_set(resultado, res); // resultado = res

    mpz_clears(pot, exp, res, NULL);
}

#else
 void ExpoM(mpz_t resultado, const mpz_t base, const mpz_t expoente, const mpz_t mod) 
{

    mpz_t pot, exp, res, temp;
    mpz_inits(pot, exp, res, temp, NULL);

    mpz_t R, R_inv, mu;
    mpz_inits(R, R_inv, mu, NULL);

        mpz_mod(pot, base, mod); // pot = base % mod
        mpz_set_ui(res, 1); // resultado = 1

        mpz_set(exp, expoente); // exp = expoente

        ajustar_R(R, mod);

        // Precompute R_inv and mu
        mpz_invert(R_inv, R, mod);
        mpz_invert(mu, mod, R);
        mpz_neg(mu, mu);
        mpz_mod(mu, mu, R);

        while (mpz_cmp_ui(exp, 0) > 0) 
        {
            // Se o bit menos significativo de exp é 1 (se é impar)
            if (mpz_odd_p(exp)) { // if (expoente % 2 == 1)
                MontgomeryMul(temp, res, pot, mod, R_inv, R, mu);
                mpz_set(res,temp);
            }

            MontgomeryMul(temp, pot, pot, mod, R_inv, R, mu);
            mpz_set(pot,temp);


            mpz_fdiv_q_2exp(exp, exp, 1); // exp = exp/2 (arredonda pra baixo)
        }

        mpz_set(resultado, res); // resultado = res

        mpz_clears(pot, exp, res, NULL);
    }


#endif



static void BM_ExpoM_Montgomery(benchmark::State& state) 
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
        //gmp_printf("ExpoM_Montgomery(%Zd, %Zd, %Zd) = %Zd\n", base, expoente, mod, resultado);

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
        //gmp_printf("ExpoM_GMP(%Zd, %Zd, %Zd) = %Zd\n", base, expoente, mod, resultado);

    // Libera a memória alocada para base, expoente, mod e resultado
    mpz_clears(base, expoente, mod, resultado, NULL);
}

    BENCHMARK(BM_ExpoM_Montgomery);
    BENCHMARK(BM_ExpoM_GMP);
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
    mpz_mul(m, T, mu); // m = (T * mu) mod R
    mpz_mod(m, m, R);

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

    // Inicialmente, defina R como uma potência de 2 maior que mod
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
