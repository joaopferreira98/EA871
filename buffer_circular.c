/* Teste de um buffer circular
 * João Pedro Martins Ferreira R.A: 218762
 
 * Este programa serve para implementar e testar uma implementacao de buffer
 * circular
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX 5 /* numero maximo de elementos */
char buffer[MAX]; /* define o buffer e seu tamanho */
int j = 0; /* parametro de insercao */
int k = 0; /* parametro de remocao */
int vazio = 1; /* variavel para checar se esta vazio (1) ou nao (0) */

/* Funcao para inserir elemento no buffer */
void adicionar_buffer(char c, int *i) {
	/* Essa condicao ira carregar o buffer ate o n MAX de elementos do buffer */
	if (*i < MAX){
		buffer[*i] = c; /* a posicao do buffer apontada por *i recebe o caracter */
		*i = *i+1; /* incremento o valor de insercao apontado por *i */
		j++; /* incremento j para a proxima insercao */
	}
	vazio = 0; /* nao esta mais vazio, pois inserimos caracter */
}
/* Funcao para remover elemento do buffer */
void remover_buffer(int *i) {
	if( *i != 0 && vazio == 0){ /* condicao em que o buffer tem pelo menos um elemento */
		buffer[k] = 0; /* remove elemento do buffer da posicao k */
		k ++; /* incremento k para a proxima remocao */
		if(k == MAX){ /* se k remover o maximo de elementos do buffer */
			*i = 0; /* volto para 0 o valor apontado por *i */
			k = 0; /* volto para 0 o parametro de remocao */
			vazio = 1;  /* o buffer esta vazio */
		}
	}
}

/* funcao para imprimir o buffer ordenadamente */
void imprimir_buffer(int *i) {
	int posicao = 0; /* define a posicao do buffer circular por padrao sera 0 */
	int tamanho = *i; /* define o tamanho do buffer atual */
	if (j-k != j) { /* condicao em que um elemento foi removido */
		posicao = k; /* defino a posicao como ultimo indice nao removido */
		tamanho = j-k; /* tamanho do buffer sera elementos inseridos menos os removidos */
	}
	/* loop para imprir os elementos do buffer em ordem */
	for (tamanho; tamanho>0 ; tamanho --){ /* a cada iteracao diminuimos o tamanho do buffer 
											* para saber a quantidade de elementos restante */	
		if(posicao < MAX && vazio == 0){ /* condicao de buffer com elementos e a posica menor 
										  * que o maximo do buffer */
			printf("%c", buffer[posicao]); /* imprimo o elemento da posicao atual */
			posicao ++; /* incremento a posicao a ser impressa */
			if( tamanho == 1 || j == k) tamanho = 0; /*se o tamanho do buffer for 1 ou removido 
													  * igual ao inseridos zeramos o tamanho do 
													  * buffer para nao entrarmos no looping na proxima iteracao*/
			else if (posicao == MAX) continue; /* se a posicao atinge o MAX de elementos do buffer 
												* apenas continuo o programa */
			else printf(" "); /* se nenhuma das condicoes acima for atingida, imprimo o espaco 
							   * entre os elementos */
		}
	}
	printf("\n"); //imprimo para pular a linha
}

int main(){
	/* Criacao de um ponteiro para auxilio no buffer */
	int l = 0;
	int *i;
	i = &l;
	
	char c; /* variavel que recebera elementos da entrada */

	do {
		scanf("%c", &c); /* recebe elemento por elemento da entrada */
		if (c == '\n') break; /* ate receber o \n(enter) e parar */

		/* Condicao numerica, onde receberemos o elemento no buffer */
		if (c >= '0' && c <= '9'){
			adicionar_buffer(c,i);
		}
		/* Condicao alfabetica, onde removeremos elemento do buffer */
		else if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'){
			remover_buffer(i);
		}
		imprimir_buffer(i); /* imprimo o conteudo do buffer atual */

	} while (1);

	return 0;
}