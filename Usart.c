/* Microcontrolador usando USART para transmissao de uma
 * mensagem ao computador que informe o estado do botao
 
 * João Pedro Martins Ferreira R.A: 218762
 
 * Este programa serve para utilizar a USART a fim de transmitir uma
 * mensagem ao computador que informe o estado do botao
 */

#define F_CPU 16000000UL 
#include <avr/io.h> 
#include <util/delay.h>

/* Definicao dos ponteiros dos registradores a serem usados no programa */
unsigned char *p_ubrr0h; /* usado para definir o Baud Rate do protocolo */
unsigned char *p_ubrr0l; /* usado para definir o Baud Rate do protocolo */
unsigned char *p_ucsr0a; /* usado para verificar se os dados foram transferidos, configurar 
                          * a velocidade de transmissao dos dados e tambem a funcao de multiprocessamento */
unsigned char *p_ucsr0b; /* usado para definir as configuracoes de interrupcoes, transmissao 
                          * e recepcao de dados e o numero de bits a serem transmitidos. */
unsigned char *p_ucsr0c; /* usado para definir modo de operacao da USART, configuracoes referentes a paridade
                          * quantidade de bits de parada, quantidade de bits a serem transmitidos */
unsigned char *p_udr0; /* usado para transferir os dados pela interface serial */
unsigned char *p_mcucr; /* usado para configuracao do resistor de pull-up */

/* Definicao dos ponteiros referentes ao uso da porta B */
unsigned char *p_pinb;
unsigned char *p_ddrb;
unsigned char *p_portb;

/* Definicao das mensagens dependendo do estado do botao */
char msg_p[] = "Transmissao serial utilizando a USART: verificacao do termino da transmissao por varredura. O botao esta pressionado.\n\n";
char msg_np[] = "Transmissao serial utilizando a USART: verificacao do termino da transmissao por varredura. O botao nao esta pressionado.\n\n";

/* Funcao para definicoes de enderecos e configuracoes iniciais dos ponteiros */
void setup(){
    /* Definicao dos enderecos dos registradores */
    p_ucsr0a = (unsigned char *) 0xC0;
    p_ucsr0b = (unsigned char *) 0xC1;
    p_ucsr0c = (unsigned char *) 0xC2;
    p_ubrr0l = (unsigned char *) 0xC4;
    p_ubrr0h = (unsigned char *) 0xC5;
    p_udr0 = (unsigned char *) 0xC6;
    p_mcucr = (unsigned char *) 0x55;   
    p_pinb = (unsigned char *) 0x23;
    p_ddrb = (unsigned char *) 0x24;
    p_portb = (unsigned char *) 0x25;
    /* Configuracao dos registradores de USART */
    
    /* Para definir o Baud rate de 115200 bps, o valor a ser setado nos registradores ubrr0 'l' e 'h'
     * sera 8, logo no registador de bits mais significativo, deve ser setado o valor 0x00, enquanto  
     * o menos significativo devera ser setado o valor 0x08 */
    *p_ubrr0h &= 0x00;
    *p_ubrr0l &= 0x00;
    *p_ubrr0l |= 0x08;
    /* Desabilita a taxa dobrada de transmissão do dados e a função de multiprocessamento com '0' 
     * nos bits 0 e 1, os demais bits nao importam para essa configuracao inicial */
    *p_ucsr0a &= 0xFC;
    /* Bit 7, 6 e 5 sera '0' para desabilitar as interrupções, bits 4 eh '0' para desabilitar o receptor
     * e o bit 3 eh '1' para habilitar o transmissor, o bit 2 eh 0 para que a quantidade de transmitidos
     * seja 8. Ainda eh preciso configurar bits do ucsr0c para de fato ser 8 bits transmitidos
     * bits 1 e 0 nao importam para essa atividade */
    *p_ucsr0b &= 0x03;
    *p_ucsr0b |= 0x08;
    /* Bits 7 e 6 serao '0' para configurar no modo assincrono, como a paridade eh par o valor dos 
     * bits 5 e 4 serao '1' e '0', bit 3 eh '1' pois temos 2 bits de parada, os bits 1 e 2 serao '1' e '1'
     * para transmitir 8 bits, o bit 0 sera '0' pois a transmissao eh assincrona */
    *p_ucsr0c &= 0x00;                                                                                                                                                                                                                                                                                                                                                                          
    *p_ucsr0c |= 0x2E;
    /* Mascara para definir PB1 como entrada */
    *p_ddrb &= 0xFD; 
    /* Resetar o bit PUD para garantir que setando PB1 como entrada e setando o bit X de PORTB,
     * sera habilitado o resistor de pull-up de PB1 */
    *p_mcucr &= ~(1 << 4); /* mascara que garante que o pull-up das entradas estejam ativas */  
    *p_portb |= 0x02; /* mascara para ativar o pull-up da porta em questao PB1 */
}

/* Funcao para exibir mesnsagem */
void mensagem (char msg[]){
    unsigned int k=0; /* variavel utilizada para a posicao dentro do vetor mensagem */
    while(msg[k]!='\0'){ /* enquanto nao chegar no caracter de fim da mensagem */
        if((*p_ucsr0a & 0x20) == 0x20){ /* se o buffer estiver vazio, recebera dados */
            *p_udr0 = msg[k]; /* o caracter da mensagem sera inserida no registrador afim de 
                               * visualiza-lo na interface serial */
            k++; /* atualiza a posicao a ser lida */
        }
    }
}

/* Funcao principal */
int main(){
    setup(); /* chama funcao de configuracao */
    /* Laco infinito */
    while(1){
        if((*p_pinb & 0x02) == 0x00){ /* caso em que o botao foi acionado */
            mensagem(msg_p); /* chama a funcao para exibir mensagem referente ao estado, 
                              * nesse caso, pressionado */
            _delay_ms(500); /* espera para transmitir os proximos dados */
        }
        else if((*p_pinb &  0x02) == 0x02){ /* caso em que o botao nao esta acionado */
            mensagem(msg_np); /* chama a funcao para exibir mensagem referente ao estado, 
                               * nesse caso, nao pressionado */
            _delay_ms(500); /* espera para transmitir os proximos dados */
        }
    }
    return 0;
}
