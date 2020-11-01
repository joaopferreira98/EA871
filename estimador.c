/* Microcontrolador usado como estimador da frequencia de rotação por minuto
 * de um motor CC com codificador
 
 * João Pedro Martins Ferreira R.A: 218762
 
 * Este programa serve para implementar e testar a frequencia de rotação de
 * um motor CC com codificador e visualizao da frequencia atraves do display
 * de sete segmentos
 
 * OBS: Trabalhei com as variaveis relacionadas a equacao com tipo float, 
 * pois para frequencias mais altas com tipo int tive problemas para calcular
 * corretamente */
#define F_CPU 16000000UL 

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h> 

/* Definicao dos ponteiros dos registradores a serem usados no programa*/
unsigned char * p_ddrc;
unsigned char * p_ddrd;
unsigned char * p_portd;
unsigned char * p_pcicr;
unsigned char * p_pcmsk1;
/* Definicao do vetor com os valores que serão colocados no display */ 
const unsigned char MASK[10] = {0x7E,0x0C,0xB6,0x9E,0xCC,
                                0xDA,0xFA,0x0E,0xFE,0xDE};
/* Variavel responsavel por guardar o numero de pulsos gerados pelo motor */
unsigned int conta_pulsos = 0;
/* define a constante nº de pulsos por volta */ 
const float N_ppv = 96;
/* Define a constante para transformar a medida em minutos 60*(1000ms) */
const float min = 60000;
/* Define o valor do tempo que pegaremos os dados */
const float tempo = 1000;
/* Subrotina da interrupcao, sera chamada quando ocorrer tanto bordas
 * de subida quanto descida */
ISR(PCINT1_vect){
    conta_pulsos ++; /* apenas incrementa a variavel para ser usada posteriormente */
}
/* Funcao para definicoes e configuracoes iniciais dos ponteiros */
void setup(){
    cli(); /* desabilita interrupcoes evitando interrupcoes durante o ajuste de configuracoes */
 	/* Definicao dos enderecos dos registradores */
    p_ddrc = (unsigned char *) 0x27;
    p_ddrd = (unsigned char *) 0x2A;
    p_portd = (unsigned char *) 0x2B;
    p_pcicr = (unsigned char *) 0x68;
    p_pcmsk1 = (unsigned char *) 0x6C;
    /* Configuracao dos registradores */
    *p_pcicr |= 2; /* habilitar o grupo 1 de interrupcoes */
    *p_pcmsk1 |= 1; /* habilitado o grupo 1, aqui habilita-se a PCINT8 */
    *p_ddrc &= 0xFE; /* mascara para definir PC0 como entrada, para garantir
                      * que nao aconteca interrupcoes erroneas pelo codigo 
                      * no PC0 */
    *p_ddrd |= 0xFE; /* mascara para definir PD1 a PD7 como saidas */  
    *p_portd &= 0x01; /* mascara para definir PD1 a PD7 em nivel logico 0 */
    sei(); /* habilita o funcionamento das interrupcoes */
}
/* Funcao responsavel por calcular a frequencia do motor por minuto */
//float
float equacao(){
    float f;
  	f = (conta_pulsos*min)/(N_ppv*tempo);
    return f;
}
/* Funcao para mostrar o digito no display, correspondente com a Frpm 
 * Para cada caso, ha uma condicao do valor da frequencia, para ser mostrado
 * o digito correspondente */
void display(float freq){
    if (freq < 100) {/* nesse caso mostra 0 no display */
        *p_portd &= 0x01;
        *p_portd |= MASK[0];
    }
    else if ((freq >= 100) & (freq < 200)) {/* nesse caso mostra 1 no display */
        *p_portd &= 0x01;
        *p_portd |= MASK[1];
    }
    else if ((freq >= 200) & (freq < 300)) {/* nesse caso mostra 2 no display */
        *p_portd &= 0x01;
        *p_portd |= MASK[2];
    }
    else if ((freq >= 300) & (freq < 400)) {/* nesse caso mostra 3 no display */
        *p_portd &= 0x01;
        *p_portd |= MASK[3];
    }
    else if ((freq >= 400) & (freq < 500)) {/* nesse caso mostra 4 no display */
        *p_portd &= 0x01;
        *p_portd |= MASK[4];
    }
    else if ((freq >= 500) & (freq < 600)) {/* nesse caso mostra 5 no display */
        *p_portd &= 0x01;
        *p_portd |= MASK[5];
    }
    else if ((freq >= 600) & (freq < 700)) {/* nesse caso mostra 6 no display */
        *p_portd &= 0x01;
        *p_portd |= MASK[6];
    }
    else if ((freq >= 700) & (freq < 800)) {/* nesse caso mostra 7 no display */
        *p_portd &= 0x01;
        *p_portd |= MASK[7];
    }
    else if ((freq >= 800) & (freq < 900)) {/* nesse caso mostra 8 no display */
        *p_portd &= 0x01;
        *p_portd |= MASK[8];
    }
    else if (freq >= 900) {/* nesse caso mostra 9 no display */
        *p_portd &= 0x01;
        *p_portd |= MASK[9];
    }
}
/* Funcao principal */
int main(){
  	float frpm = 0; /* variavel que recebera o valor da frequencia rpm */
    setup(); /* chamo funcao para fazer as configuracoes */
    /* Laco infinito */
    while(1){
      _delay_ms(5); /* delay para evitar bug no tinkercad */
      conta_pulsos = 0; /* zero variavel de contagem de voltas do motor */
      sei(); /* habilito interrupcao */
      _delay_ms(tempo); /* gasto tempo ja definido para que a interrupcao ative */
      cli(); /* desabilito interrupcao */
      frpm = equacao(); /* chamo funcao para realizar o calculo e coloco o resultado na
                         * na variavel */
      display(frpm); /* chamo funcao, passando o valor encontrado acima como parametro
                      * para amostragem do display */         
    }
    return 0;
}