#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

void convert_to_saturated(uint32_t D[] , int size) 
{
    uint32_t carry;

  #if 0

    carry = D[1] << 28;

    D[1] = D[1] & 0xFFFFFFF0;

    D[1] = D[1] >> 4;

    D[0] = D[0] + carry;

  #else
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

    #endif
}
// Função que converte um número de 64 bits em uma representação de dois números
// de 32 bits, manipulando os bits superiores e inferiores de forma específica.
void convert_to_unsaturated(uint32_t U[], uint32_t I[]) {
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
void add(uint32_t D[], uint32_t A[], uint32_t B[]) {
  // Multiplica A0*B0 => C00, A0*B1 => C01, A1*B0 => C10, A1*B1 => C11 (64 bits)

  D[0] = ((uint32_t)A[0]) + B[0];
  D[1] = ((uint32_t)A[1]) + B[1];

  uint32_t carry;

  // Nao pode ter carry no D[0], porque contem apenas o c00[0], sem somar com
  // mais nada

  carry = D[0] >> 28;
  D[0] = D[0] & 0x0FFFFFFF;

  D[1] = D[1] + carry;
  
}

int main() {
  // Declara dois arrays de 32 bits para armazenar as partes de a e b.
  uint32_t A[2], B[2], c[2], d[2];
  uint32_t a[2] = {0x4F3C4F30, 0xC4F3C};
  uint32_t b[2] = {0x1F3B1F35, 0xB1F3B};

  // Converte a variável a em sua forma "unsaturated" e armazena o resultado em A
  convert_to_unsaturated(A, a);
  convert_to_unsaturated(B, b);

  // Array para armazenar o resultado final
  uint32_t D[2];

  // Soma A e B, armazenando o resultado em D
  add(D, A, B);

  // Imprime o resultado da multiplicação
  printf("D[0] = %08x\n", D[0]);
  printf("D[1] = %08x\n", D[1]);

  printf("D = 0x%07x%08x\n",D[1], D[0]);

  convert_to_saturated(D, 2);
  printf("D[0] = %08x\n", D[0]);
  printf("D[1] = %08x\n", D[1]);


  printf("D = 0x%06x%08x\n",D[1], D[0]);


  return 0;
}