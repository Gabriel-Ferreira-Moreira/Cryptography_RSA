#include <stdio.h>
#include <gmp.h>
#include <time.h>

void key_generation() {

}

void encrypt(mpz_t m, mpz_t n) {

}

void decrypt() {

}

int main(void) {
//RSA Key Generation

//public Key Kpub=(n,e)
//private Key Kpr = (d)

//choose two larges primes p and q

    mpz_t rand_num, rand_nump, rand_numq, p, q, n, phi, e, gcd, lower_bound, d, x, y, h;
    gmp_randstate_t state;

    mpz_init_set_ui(lower_bound, 3);
    mpz_init(rand_num);
    mpz_inits(rand_nump, rand_numq, d, x, y, h, NULL);
    mpz_inits(p, q, n, phi, e, NULL);

    //Gerador de numero aleatorio
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    unsigned long int num_bits = 1024; 
    mpz_urandomb(rand_nump, state, num_bits); 
    mpz_urandomb(rand_numq, state, num_bits); 

    //numero primo mais proximo do numero aleatorio
    mpz_nextprime(p, rand_nump);
    mpz_nextprime(q, rand_numq);

    //gmp_printf("Número primo gerado:p = %Zd\n", p);
    //gmp_printf("Número primo gerado:q = %Zd\n", q);

    mpz_mul(n, p, q);// n = p*q
    
    // Calcula phi = (p - 1) * (q - 1)
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q);

    
    //Public exponent Generation (public Key)
    //Select public exponent e ∈ {1,2,...,phi−1} such that
    // "gcd(e,phi) = 1"
    //xgcd(e,phi)
    //e = phi;

    mpz_init_set_ui(gcd, 0);// gcd = 0

    mpz_set(e, phi);// e = phi

    // Loop enquanto o gcd de e e phi não for 1
    while (mpz_cmp_ui(gcd, 1) != 0) 
    {
        // Gera um número aleatório entre 3 e phi
        mpz_sub(rand_num, phi, lower_bound);// rand_num = phi - 3
        mpz_urandomm(e, state, rand_num); // gera um e aleatório de 0 a phi - 4
        // Ajusta e para o intervalo de lower_bound a phi - 1
        mpz_add(e, e, lower_bound); // e agora está no intervalo de 3 a phi - 1
        // Recalcula o gcd
        mpz_gcd(gcd, e, phi);
    }
    

    //Private exponent Generation (private Key: d) 
    // e*d = 1 mod phi
    // e^(-1) ≡ d mod phi
    mpz_invert(d, e, phi); //d = e^(-1) % phi

    mpz_sub_ui(rand_num, n, 2);// rand_num = n - 2

    //message = x
    mpz_urandomm(x, state, rand_num);// gera uma mensagem aleatoria com tamanho {(p*q) - 2}

    //encrypted message = y
    mpz_powm(y, x, e, n);// y = x^e mod n

    //decrypted message = h
    mpz_powm(h, y, d, n);// h = y^d mod n

    gmp_printf("\nMensagem original x: %Zd\n", x);
    gmp_printf("\nMensagem criptografada y: %Zd\n", y);
    gmp_printf("\nMensagem descriptografada h: %Zd\n\n", h);

    //Teste de descriptação
    if (mpz_cmp(x, h) == 0)
     {
            printf("\n True! The message final is original\n");
     }else 
        {
            printf("\n False! The message final isn't original\n");
        }
    //gmp_printf("e = %Zd,\n phi = %Zd\n d = %Zd\n", e, phi, d);
    
    //LIMPAR MEMÓRIA
    mpz_clears(rand_num, rand_nump, rand_numq, x, y, d, h, e, n, NULL);
    mpz_clears(lower_bound, phi, gcd, p, q, NULL);
    gmp_randclear(state);

    return 0;
}

