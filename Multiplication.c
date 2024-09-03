#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>


void convert_to_saturated(uint32_t D[], int size) 
{
    uint32_t carry;
    
#if 1
    // Itera sobre cada elemento do vetor
    for (int i = 0; i < size - 1; i++) {
        // Calcula a quantidade de bits a deslocar
        int shift_amount = (i + 1) * 4;

        // Ajusta o valor atual de D[i] para a direita
        D[i] >>= (i * 4);

        // Prepara o "carry" a partir do próximo valor de D
        carry = D[i + 1] << (32 - shift_amount);

        // Adiciona o "carry" ao valor atual de D[i]
        D[i] += carry;
    }

    // Ajusta o último valor de D para a direita (não há carry para adicionar)
    D[size - 1] >>= ((size - 1) * 4);

    #else

    D[0] = D[0] >> 0;

    carry = D[1] << 28;

    D[0] = D[0] + carry;


    carry = D[2] << 24;

    D[1] = D[1] >> 4;

    D[1] = D[1] + carry;


    carry =  D[3] << 20;

    D[2] = D[2] >> 8;

    D[2] = D[2] + carry;


    D[3] = D[3] >> 12;

  #endif
}

// Função que converte um número de 64 bits em uma representação de dois números
// de 32 bits, manipulando os bits superiores e inferiores de forma específica.
void convert_to_unsaturated(uint32_t U[], uint32_t I[]) 
{
  uint32_t temp;

  // U[1] recebe os 32 bits superiores de I[1], deslocados para a esquerda por 4
  // bits.
  U[1] = I[1] << 4;

  // temp recebe os 4 bits mais significativos de I[0], deslocados 28 bits para
  // a direita.
  temp = I[0] >> 28;

  // U[1] é atualizado com a combinação de U[1] e temp, usando a operação OR.
  U[1] = U[1] | temp;

  // U[0] recebe os 28 bits menos significativos de I[0].
  U[0] = I[0] & 0x0FFFFFFF;
}

// Função que multiplica dois números de 64 bits representados como dois arrays
// de 32 bits. O resultado da multiplicação é armazenado no array D.
void mul(uint32_t D[], uint32_t A[], uint32_t B[]) {
  // Multiplica A0*B0 => C00, A0*B1 => C01, A1*B0 => C10, A1*B1 => C11 (64 bits)
  uint64_t C00, C01, C10, C11;

  C00 = ((uint64_t)A[0]) * B[0];
  C01 = ((uint64_t)A[0]) * B[1];
  C10 = ((uint64_t)A[1]) * B[0];
  C11 = ((uint64_t)A[1]) * B[1];

  // Separar C00 em C00L || C00H, C01 em C01L || C01H, C10 em C10L || C10H, C11
  // em C11L || C11H
  uint32_t c00[2], c01[2], c10[2], c11[2];

  convert_to_unsaturated(c00, (uint32_t *)&C00);
  convert_to_unsaturated(c01, (uint32_t *)&C01);
  convert_to_unsaturated(c10, (uint32_t *)&C10);
  convert_to_unsaturated(c11, (uint32_t *)&C11);

  // Gera D0 = C00L, D1 = C00H + C10L + C01L, D2 = C10H + C01H + C11L, D3 = C11H
  //   uint32_t D0, D1, D2, D3;

  D[0] = (c00[0]);
  D[1] = (c00[1] + c10[0] + c01[0]);
  D[2] = (c11[0] + c10[1] + c01[1]);
  D[3] = (c11[1]);

  uint32_t carry;

  // Nao pode ter carry no D[0], porque contem apenas o c00[0], sem somar com
  // mais nada

  carry = D[1] >> 28;
  D[1] = D[1] & 0x0FFFFFFF;

  D[2] = D[2] + carry;
  carry = D[2] >> 28;
  D[2] = D[2] & 0x0FFFFFFF;

  D[3] = D[3] + carry;
}

int main() {
  // Declara dois arrays de 32 bits para armazenar as partes de a e b.
  uint32_t A[2], B[2], c[2], d[2];
  uint32_t a[2] = {0x4F3C4F30, 0xC4F3C};
  uint32_t b[2] = {0x1F3B1F35, 0xB1F3B};

  // Converte a variável a em sua forma "unsaturated" e armazena o resultado em
  // A.
  convert_to_unsaturated(A, a);
  convert_to_unsaturated(B, b);

  // Array para armazenar o resultado final
  uint32_t D[4];

  // Multiplica A e B, armazenando o resultado em D
  mul(D, A, B);
  printf("D = 0x0000088e8077886083055a1c52334f0\n");

  // Imprime o resultado da multiplicação
  printf("D[0] = %08x\n", D[0]);
  printf("D[1] = %08x\n", D[1]);
  printf("D[2] = %08x\n", D[2]);
  printf("D[3] = %08x\n", D[3]);
  printf("D = 0x%07x%07x%07x%07x\n", D[3], D[2], D[1], D[0]);

  // Variável de 128 bits para armazenar o valor combinado
  __uint128_t result = 0;

  convert_to_saturated(D, 4);

  printf("D[0] = %08x\n", D[0]);
  printf("D[1] = %08x\n", D[1]);
  printf("D[2] = %08x\n", D[2]);
  printf("D[3] = %08x\n", D[3]);
  printf("D = 0x%07x%07x%07x%07x\n", D[3], D[2], D[1], D[0]);

  // Combina os valores do vetor D em um único número de 128 bits
  result |= (__uint128_t)D[3] << 84;
  result |= (__uint128_t)D[2] << 56;
  result |= (__uint128_t)D[1] << 28;
  result |= (__uint128_t)D[0];

  // Dividindo em High e Low, pois o limite é 64 bits
  uint64_t high = (uint64_t)(result >> 64);
  uint64_t low = (uint64_t)(result);

  // Imprime o número de 128 bits como dois valores de 64 bits
  //printf("Result (128 bits) = %016" PRIx64 "%016" PRIx64 "\n", high, low);

  return 0;
}