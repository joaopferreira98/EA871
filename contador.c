/* Microcontrolador usado como contador de 0-F no display de 7 segmentos
 
 * João Pedro Martins Ferreira R.A: 218762
 
 * Este programa serve para implementar e testar um contador de 0-F
 * no display de 7 segmentos atraves do acionamento de um push-button
 */

#define F_CPU 16000000UL 

#include <avr/io.h> 
#include <util/delay.h>

/* Definicao dos ponteiros para os registradores da porta C*/
unsigned char * p_pinc;
unsigned char * p_ddrc;
unsigned char * p_portc;

/* Definicao dos ponteiros para os registradores da porta D */
unsigned char * p_pind;
unsigned char * p_ddrd;
unsigned char * p_portd;

/* Definicao do ponteiro para o registrador da porta MCUCR */
unsigned char * p_mcucr;

/* Variavel do botao */
unsigned char button = 0x01;

/* Variavel do digito do display */
unsigned char digit = 0x00;

/* Definicao do vetor com os valores que serão colocados no display */ 
const unsigned char MASK[16] = {0x7E,0x0C,0xB6,0x9E,
                                0xCC,0xDA,0xFA,0x0E,
                                0xFE,0xDE,0xEE,0xF8,
                                0x72,0xBC,0xF2,0xE2};

/* Funcao para definicoes e configuracoes iniciais dos ponteiros */
void setup(void){
    /* Definicao do endereco de cada registrador */
    p_pinc = (unsigned char *) 0x26;
    p_ddrc = (unsigned char *) 0x27;
    p_portc = (unsigned char *) 0x28;
    p_ddrd = (unsigned char *) 0x2A;
    p_portd = (unsigned char *) 0x2B;
    p_mcucr = (unsigned char *) 0x55;

    /* Configuracao dos registradores */
    *p_mcucr &= ~(1 << 4); /* mascara que garante que o pull-up das entradas estejam ativas */
    *p_ddrc &= 0xFE; /* mascara para definir PC0 como entrada */
    *p_portc |= 0x01; /* mascara para ativar o pull-up da porta em questao PC0 */
    *p_ddrd |= 0xFF; /* mascara para definir PD0 a PD7 como saidas */
    *p_portd |= MASK[0]; /* definir inicialmente com zero */
    *p_portd &= 0xFE; /* com dp desligado */
}

/* Funcao de debounce */
void debounce(){
    /* Verifica se o botao foi acionado */
    if ((*p_pinc & 0x01) != button){
        _delay_ms(80); /* delay de debounce */
        if((*p_pinc & 0x01) != button) /* se o botao continuar pressionado */
            button = *p_pinc & 0x01;
    }      
}

/* Funcao para definir o digito a ser mostrado */
void def_digit(){
    unsigned char aux_button = button; /* guardar o estado atual do botao */
    debounce(); /* chama funcao de debounce */
    /* Atualiza o digito a ser mostrado no display, caso aconteca borda de descida */
    if ((aux_button == 0x01) && (button == 0x00)){
        /* Digito for menor que F, incrementamos o digito */
        if (digit < 0x0F) digit ++;
        /* Digito igual a F, zeramos o digito e recomecamos a contagem*/
        else digit = 0x00;
    }
}

/* Funcao para mostrar o valor guardado em digit no display, p_portd recebera o valor 
 * em hexadecimal que ligara os segmentos referentes ao numero a ser mostrado */
void display (){
    unsigned char aux_digit = digit; /* guardar o estado atual do digito */
    def_digit(); /* chama a funcao para atualizar o digito a ser mostrado */
    /* Se ocorrer a troca do digito */ 
    if (aux_digit != digit){
        /* Limpa o display, menos o dp */
        *p_portd &= 0x01;
        /* Seta o display, menos o dp */
        *p_portd |= MASK[digit];
    }
    /* Se o botao nao esta mantido pressionado, desligamos o dp */
    if (button == 0x01) *p_portd &= 0xFE;
    /* Se o botao nao esta mantido pressionado, ligamos o dp */
    else *p_portd |= 0x01;
}

/* Funcao principal */
int main(void){
    setup(); /* chama funcao de configuracoes iniciais */
    /* Laco infinito */
    while(1){
      _delay_ms(5); /* delay para corrigir bug do Tinkercad */
      display(); /* chama funcao para mostrar digito no display */
    }
      
    return 0;
}
