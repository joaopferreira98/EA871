/* Contador de bits
 * João Pedro Martins Ferreira RA. 218762
 * Este programa conta o numero de bits em um byte
 */

#include <stdio.h>

/* Funcao para contagem dos bits. Varemos o Byte, bit a bit usando uma 
   * mascara para ver qual bit eh 0 ou 1. Se for 1 contabilizamos. 
   * Ao final retornamos o n de bits iguais a 1.*/
int varredura_bit (char entrada){
  unsigned char resultado; /* variavel auxiliar para efeito de comparacao */
  unsigned int n_bits = 0; /* variavel para guardar o n de bits iguais a 1 */
  /* Loop para varrer o byte */
  for (int i = 0; i < 8; i++){
    resultado = entrada & 0x01; /* atraves da mascara 0x01, identificamos qual o valor do bit atual */
    if (resultado == 1) n_bits += 1; /* se for 1, contabilizamos */
    entrada = entrada >> 1; /* movemos a cadeia para a direita, afim de observarmos o proximo bit */
  }
  return n_bits; /* retorna a quantidade de bits iguais a 1 */
}

int main() {
  unsigned char entrada; /* variavel responsavel por guardar o valor da entrada */
  unsigned int tmp;
  /* Ler entrada em hexadecimal */
  scanf("%x", &tmp);
  entrada = (unsigned char)tmp;
  /* Chamamos a funcao para varrer o byte e ja imprimos o valor do numero de bits 1 da entrada */
  printf("%d\n", varredura_bit(entrada)); 
  return 0;
}
