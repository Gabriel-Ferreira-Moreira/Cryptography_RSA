#include <stdint.h>
#include <stdio.h>

// Função que converte um número de 64 bits em uma representação de dois números de 32 bits,
// manipulando os bits superiores e inferiores de forma específica.
void convert_to_unsaturated(uint32_t U[], uint32_t I[]) 
{
    uint32_t temp;

    // U[1] recebe os 32 bits superiores de I[1], deslocados para a esquerda por 4 bits.
    U[1] = I[1] << 4;

    // temp recebe os 4 bits mais significativos de I[0], deslocados 28 bits para a direita.
    temp = I[0] >> 28;

    // U[1] é atualizado com a combinação de U[1] e temp, usando a operação OR.
    U[1] = U[1] | temp;
    
    // U[0] recebe os 28 bits menos significativos de I[0].
    U[0] = I[0] & 0x0FFFFFFF;
}

// Função que multiplica dois números de 64 bits representados como dois arrays de 32 bits.
// O resultado da multiplicação é armazenado no array D.
void mul(uint32_t D[], uint32_t A[], uint32_t B[]) 
{
    // Multiplica A0*B0 => C00, A0*B1 => C01, A1*B0 => C10, A1*B1 => C11 (64 bits)
    uint64_t C00, C01, C10, C11;

    C00 = ((uint64_t)A[0]) * B[0];
    C01 = ((uint64_t)A[0]) * B[1];
    C10 = ((uint64_t)A[1]) * B[0];
    C11 = ((uint64_t)A[1]) * B[1];

    // Separar C00 em C00L || C00H, C01 em C01L || C01H, C10 em C10L || C10H, C11 em C11L || C11H
    uint32_t c00[2], c01[2], c10[2], c11[2];

    convert_to_unsaturated(c00, (uint32_t *)&C00);
    convert_to_unsaturated(c01, (uint32_t *)&C01);
    convert_to_unsaturated(c10, (uint32_t *)&C10);
    convert_to_unsaturated(c11, (uint32_t *)&C11);

    // Gera D0 = C00L, D1 = C00H + C10L + C01L, D2 = C10H + C01H + C11L, D3 = C11H
    uint64_t D0, D1, D2, D3;

    D0 = ((uint64_t)c00[0]);
    D1 = ((uint64_t)c00[1] + c10[0] + c01[0]);
    D2 = ((uint64_t)c11[0] + c10[1] + c01[1]);
    D3 = ((uint64_t)c11[1]);

    // Propagação de carries (vai-uns)
    uint64_t carry;

    // Adiciona carry de D1 para D2, e de D2 para D3
    carry = D1 >> 28;
    D1 = D1 & 0x0FFFFFFF;
    D2 = D2 + carry;

    carry = D2 >> 28;
    D2 = D2 & 0x0FFFFFFF;
    D3 = D3 + carry;

    // Organizar os valores
    carry = D1 & 0x0000000F;
    carry = carry << 28;
    D0 = carry | D0;
    carry = D2 & 0x0000000F;
    carry = carry << 28;
    D1 = carry | D1;
    D1 = D1 >> 4;
    D2 = D2 >> 8;
    carry = D3 & 0x00000FFF;
    carry = carry << 20;
    D2 = carry | D2;
    D3 = D3 >> 12;

    // Armazena os resultados finais no array D[]
    D[0] = (uint32_t) (D0 & 0xFFFFFFFF);
    D[1] = (uint32_t) (D1 & 0xFFFFFFFF);
    D[2] = (uint32_t) (D2 & 0xFFFFFFFF);
    D[3] = (uint32_t) (D3 & 0xFFFFFFFF);
}

int main() 
{
    uint64_t a = 0xC4F3C4F3C4F30; // numero de 52 bits declarado como 64 bits
    uint64_t b = 0xB1F3B1F3B1F35; // numero de 52 bits declarado como 64 bits

    // Declara dois arrays de 32 bits para armazenar as partes de a e b.
    uint32_t A[2], B[2];

    // Converte a variável a em sua forma "unsaturated" e armazena o resultado em A.
    convert_to_unsaturated(A, (uint32_t *)&a);
    convert_to_unsaturated(B, (uint32_t *)&b);

    // Array para armazenar o resultado final
    uint32_t D[4];

    // Multiplica A e B, armazenando o resultado em D
    mul(D, A, B);

    // Imprime o resultado da multiplicação
    //printf("D[0] = %08x\n", D[0]);
    //printf("D[1] = %08x\n", D[1]);
    //printf("D[2] = %08x\n", D[2]);
    //printf("D[3] = %08x\n", D[3]);

    // Variável de 128 bits para armazenar o valor combinado
    __uint128_t result = 0;

    // Combina os valores do vetor D em um único número de 128 bits
    result |= (__uint128_t)D[3] << 96;
    result |= (__uint128_t)D[2] << 64;
    result |= (__uint128_t)D[1] << 32;
    result |= (__uint128_t)D[0];

    // Dividindo em High e Low, pois o limite é 64 bits
    uint64_t high = (uint64_t)(result >> 64);
    uint64_t low = (uint64_t)(result);

    // Imprime o número de 128 bits como dois valores de 64 bits
    printf("Result (128 bits) = %016lx%016lx\n", high, low);

    return 0;
}