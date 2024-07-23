#include <stdio.h>
#include <gmp.h>
#include <time.h>

int is_prime(const mpz_t n, int reps) 
{
    // Verifica se o número é primo usando mpz_probab_prime_p
    // reps é o número de iterações do teste de primalidade (quanto maior, mais preciso)
    return mpz_probab_prime_p(n, reps);
}

int Smart_kick_primes(mpz_t prime, const mpz_t rand_num, int reps)
{
    int v[7] = {6, 4, 2, 4, 2, 4, 6};// Pulos necessários para o divisor 30 (2*3*5)
    size_t i;


        mpz_t r, thirty, thirty_one, temp, teste;
        mpz_inits(r, temp,  NULL);
        mpz_init_set_ui(teste, 0);
        mpz_init_set_ui(thirty, 30);
        mpz_init_set_ui(thirty_one, 31);

     // Calcula r = rand_num % 30  
        mpz_mod(r, rand_num, thirty); 

        // Verifica se o número é primo
    do {
            int result = is_prime(rand_num, reps);   
            if (result > 0) 
            {
                gmp_printf("O número é primo.\n");
                break;
                
            }   else
                {
                    gmp_printf("O número nao é primo.\n");
                    // rand_num = rand_num + (31 - r)
                    mpz_sub(temp, thirty_one, r);
                    mpz_add(rand_num, rand_num, temp);

                    // Loop para adicionar valores de v[i] e verificar primalidade
                    for (i = 0; i < 7; i++) 
                    {
                        mpz_add_ui(rand_num, rand_num, v[i]);
                        result = is_prime(rand_num, reps);

                            if (result > 0) 
                            {
                                break;
                            }
                    }
                }

        }while (teste != 1);

    mpz_set(prime, rand_num);

    mpz_clears(r, thirty, thirty_one, temp, teste, NULL);

}

int main(void) 
{
        int reps = 30; // Número de iterações do teste de primalidade

        mpz_t rand_num, prime;

        // Inicializa a variável
        mpz_inits(rand_num, prime, NULL);
        
        gmp_randstate_t state;
        //Gerador de número aleatorio com num_bits
        gmp_randinit_default(state);
        gmp_randseed_ui(state, time(NULL));
        unsigned long int num_bits = 1024;
        mpz_urandomb(rand_num, state, num_bits); 

        Smart_kick_primes(prime, rand_num, reps);
    
        gmp_printf("rand = %Zd\n", prime);

        // Libera a memória alocada
        mpz_clears(rand_num, prime, NULL);
        gmp_randclear(state);


    return 0;
}
