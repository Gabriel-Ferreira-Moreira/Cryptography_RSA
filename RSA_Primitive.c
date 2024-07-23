#include <stdio.h>
#include <gmp.h>
#include <time.h>

void ExpoM(mpz_t resultado, const mpz_t base, const mpz_t expoente, const mpz_t mod); // Modular Exponentiation
int Is_Prime_ME(const mpz_t n, int reps); // Miller–Rabin primality test
void Smart_guess_primes(mpz_t prime, mpz_t rand_num, int reps);
void MDC(mpz_t gcd, const mpz_t n1, const mpz_t n2); // Greatest Common Divisor
void Public_key(mpz_t e, mpz_t phi, gmp_randstate_t state);// Gera a chave publica
void Message(mpz_t x, const mpz_t n, gmp_randstate_t state);// Gera uma mensagem aleatoria de intervalo [0, n - 3]
void Gerator_n_phi(mpz_t n, mpz_t phi ,  mpz_t p,  mpz_t q);// Gera n e phi
void Gerator_2_randnumber(mpz_t rand_nump, mpz_t rand_numq, unsigned long int num_bits, gmp_randstate_t state);
void Private_key(mpz_t d, mpz_t e, mpz_t phi); // Gera a chave privada
void Encrypted_message(mpz_t y, mpz_t x, mpz_t e, mpz_t n);// Encripta a mensagem
void Decrypted_message(mpz_t h, mpz_t y, mpz_t d, mpz_t n);// Descripta a mensagem
void Print_message(mpz_t x, mpz_t y, mpz_t h);// Printa a mensagem inicial, Encriptada e Descriptada 
void Teste_de_descriptação(mpz_t x, mpz_t h);

int main(void) 
{
    //RSA Key Generation

    //public Key Kpub=(n,e)
    //private Key Kpr = (d)

    //choose two larges primes p and q
    mpz_t rand_nump, rand_numq;
    mpz_t p, q, n, phi, e;
    mpz_t d, x, y, h;
    gmp_randstate_t state;
    int reps = 10; // Número de iterações do teste de primalidade
    unsigned long int num_bits = 1024; // numero de bits

    mpz_inits(rand_nump, rand_numq, NULL);
    mpz_inits(p, q, n, phi, e, NULL);
    mpz_inits(d, x, y, h, NULL);

    // Gerador de numero aleatorio
    Gerator_2_randnumber(rand_nump, rand_numq, num_bits, state);

    // Chute inteligente de primo
    Smart_guess_primes(p, rand_nump, reps);
    Smart_guess_primes(q, rand_numq, reps);

    // n = p*q
    // Calcula phi = (p - 1) * (q - 1)
    Gerator_n_phi(n, phi ,  p,  q);

    // Public exponent Generation (public Key)
    // Select public exponent e ∈ {1,2,...,phi−1} such that "gcd(e,phi) = 1"
    // e = phi;
    Public_key(e, phi, state);

    // Private exponent Generation (private Key: d) 
    // e*d = 1 mod phi
    // e^(-1) ≡ d mod phi
    //d = e^(-1) % phi  
    Private_key(d, e, phi);

    // Message = x
    Message(x, n, state);// gera uma mensagem aleatoria com tamanho {(p*q) - 2}

    // Encrypted message = h
    Encrypted_message(y, x, e, n);// y = x^e mod n
        
    // Decrypted message = h
    Decrypted_message(h, y, d, n);// h = y^d mod n

    Print_message(x, y, h);

    // Teste de descriptação
    Teste_de_descriptação(x, h);
            
    // LIMPAR MEMÓRIA
    mpz_clears(phi, p, q, rand_nump, rand_numq, x, y, d, h, e, n, NULL);
    gmp_randclear(state);

    return 0;
}
void Teste_de_descriptação(mpz_t x, mpz_t h) 
{
    if (mpz_cmp(x, h) == 0)
    {
        printf("\nTrue! The message final is original\n");
    } 
    else 
    {
        printf("\nFalse! The message final isn't original\n");
    }
}
void Print_message(mpz_t x, mpz_t y, mpz_t h) 
{
    gmp_printf("\nMensagem original x: %Zd\n", x);
    gmp_printf("\nMensagem criptografada y: %Zd\n", y);
    gmp_printf("\nMensagem descriptografada h: %Zd\n", h);

}
void Decrypted_message(mpz_t h, mpz_t y, mpz_t d, mpz_t n) 
{
    ExpoM(h, y, d, n); // h = y^d mod n
}
void Encrypted_message(mpz_t y, mpz_t x, mpz_t e, mpz_t n) 
{
    ExpoM(y, x, e, n); // y = x^e mod n
}
   
void Private_key(mpz_t d, mpz_t e, mpz_t phi)
{
    mpz_invert(d, e, phi); //d = e^(-1) mod phi
}
void Gerator_2_randnumber(mpz_t rand_nump, mpz_t rand_numq, unsigned long int num_bits, gmp_randstate_t state)
{
    // Gerador de numero aleatorio
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    mpz_urandomb(rand_nump, state, num_bits); 
    mpz_urandomb(rand_numq, state, num_bits); 
}
void Gerator_n_phi(mpz_t n, mpz_t phi ,  mpz_t p,  mpz_t q) 
{
    mpz_mul(n, p, q); // n = p*q
                
    // Calcula phi = (p - 1) * (q - 1)
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q);
}
void Public_key(mpz_t e, mpz_t phi, gmp_randstate_t state) 
{        
    mpz_t lower_bound, temp, gcd;
    
    mpz_inits(lower_bound, temp, gcd, NULL);
    
    // lower_bound = 3
    mpz_set_ui(lower_bound, 3);
    // gcd = 0
    mpz_set_ui(gcd, 0);
    // temp = phi
    mpz_set(temp, phi);

    // Loop enquanto o gcd de e e phi não for 1
    do {
        // Gera um número aleatório entre 3 e phi - 1
        mpz_sub(temp, phi, lower_bound); // temp = phi - 3
        mpz_urandomm(e, state, temp);    // e aleatório entre 0 e phi - 4
        mpz_add(e, e, lower_bound);      // e agora está no intervalo de [3, phi-1]
        
        // Calcula o MDC de e e phi
        mpz_gcd(gcd, e, phi);
    } while (mpz_cmp_ui(gcd, 1) != 0);
    
    // Limpa a memória
    mpz_clears(lower_bound, temp, gcd, NULL);
}
void Message(mpz_t x, const mpz_t n, gmp_randstate_t state) 
{
    mpz_t temp;
    mpz_init(temp);
    mpz_sub_ui(temp, n, 2); // temp = n - 2

    // Gera um número aleatório no intervalo [0, temp - 1]
    mpz_urandomm(x, state, temp);

    mpz_clear(temp); // Libera a memória alocada para temp
}


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

void Smart_guess_primes(mpz_t prime, mpz_t rand_num, int reps) 
{
    mpz_t v[8];
    size_t i;
    mpz_t r, thirty, thirty_one, temp, teste;
    mpz_inits(r, thirty, thirty_one, temp, teste, NULL);
    mpz_init_set_ui(v[0], 0);
    mpz_init_set_ui(v[1], 6);
    mpz_init_set_ui(v[2], 4);
    mpz_init_set_ui(v[3], 2);
    mpz_init_set_ui(v[4], 4);
    mpz_init_set_ui(v[5], 2);
    mpz_init_set_ui(v[6], 4);
    mpz_init_set_ui(v[7], 6);

    mpz_set_ui(thirty, 30);
    mpz_set_ui(thirty_one, 31);

    // Calcula r = rand_num % 30
    mpz_mod(r, rand_num, thirty);

    // rand_num_copy = rand_num + (31 - r)
    mpz_sub(temp, thirty_one, r);
    mpz_add(rand_num, rand_num, temp);

    // Verifica se o número é primo
    
        // Loop para adicionar valores de v[i] e verificar primalidade
        for (i = 0; i < 8; i++) 
        {
            mpz_add(rand_num, rand_num, v[i]);
            int result = Is_Prime_ME(rand_num, reps);
            if (result > 0) 
            {
                break;
            }else if (i == 7)
            {
                i = 0;
            }
        }

    mpz_set(prime, rand_num);

    mpz_clears(r, thirty, thirty_one, temp, NULL);
    // Limpar a memória
    for (int i = 0; i < 8; i++) 
    {
        mpz_clear(v[i]);
    }
}

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
