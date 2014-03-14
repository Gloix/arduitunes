/*  Este programa hace uso del timer 2 para reproducir varias notas simultáneas, y del
  timer 1 para reproducir notas según su duración. La melodía de cada voz se ingresa en forma de string,
  y debe cumplir cierta estructura.
  
  Setup: -4 buzzers conectados a pins 2, 3, 4 y 5. No olvidar resistencias para controlar corriente (330 ohms en este caso).
         -2 leds en pins 11 y 12. No olvidar resistencias para controlar corriente (330 ohms en este caso).
*/

#include <avr/pgmspace.h> //para almacenar constantes en memoria flash
#define _BV(b) (1 << (b)) //para manejo directo de pins

/* Tempo. Se admitirá por ahora desde 30 hasta 400*/
word tempo=200;

/* Definición de melodías. 
    Detalles importantes:  -NO puede contener errores. Cualquier error hará que el programa se comporte raro.
                           -Notación: cada nota se escribe como ABC, donde A en {1, 2, 4, 8, 16, 32} representa la duración de la nota (redonda, blanca, negra, corchea, semicorchea, fusa)
                                                                           B en {C, CS, D, DS, E, F, FS, G, GS, A, AS, B, S} representa la nota. Las notas con sufijo "S" son sostenidas (CS == C#) La S es un silencio.
                                                                           C en {1, 2, 3, 4, 5, 6, 7, 8} representa la octava de la nota. El LA = 440Hz se encuentra en octava 4 (es A4). El silencio no necesita este valor.
                           -Al final de cada melodía se debe terminar con el caracter X, anteponiendo un espacio al mismo. Ejemplo: "4C3 X".
                           -Las repeticiones se hacen dentro de un bloque encerrado por " ||: " y " :|| " en ese orden. Notar los espacios. Ejemplo: "4C4 ||: 4C4 4A3 4F3 4G3 :|| X"
                           -NO se puede anidar repeticiones. NO es válido cosas como "||: 4C4 ||: 4G4 :|| :|| X".
                           */
                           
/* SUPER MARIO*/
                         //comiemzo                                          // primera parte                                                                                             //"coro" A                                                                                                                                                                                                   //intermedio que vuelve a la intro                                                                                                                                                                  //Acá repite la intro                                                                                         //Coro B
PROGMEM prog_char voz1[]={"8E5 8E5 8S 8E5 8S 8C5 8E5 8S 8G5 8S 4S 8G4 4S 8S ||: 8C5 4S 8G4 4S 8E4 4S 8A4 8S 8B4 8S 8AS4 8A4 8S 8G4 8E5 8S 8G5 8A5 8S 8F5 8G5 8S 8E5 8S 8C5 8D5 8B4 4S :|| ||: 4S 8G5 8FS5 8F5 8DS5 8S 8E5 8S 8GS4 8A4 8C5 8C5 8A4 8C5 8D5 4S 8G5 8FS5 8F5 8DS5 8S 8E5 8S 8C6 8S 8C6 8C6 8S 2S 8G5 8FS5 8F5 8DS5 8S 8E5 8S 8GS4 8A4 8C5 8C5 8A4 8C5 8D5 4S 8DS5 4S 8D5 4S 8C5 4S 8S 2S :|| 8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8C5 8C5 8S 8C5 8S 8C5 8D5 8E5 2S 2S 8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8E5 8E5 8S 8E5 8S 8C5 8E5 8S 8G5 8S 4S 8G4 8S 4S ||: 8C5 4S 8G4 4S 8E4 4S 8A4 8S 8B4 8S 8AS4 8A4 8S 8G4 8E5 8S 8G5 8A5 8S 8F5 8G5 8S 8E5 8S 8C5 8D5 8B4 4S :|| ||: 8E5 8C5 8S 8G4 4S 8GS4 8S 8A4 8F5 8S 8F5 8A4 8S 4S 8B4 8A5 8S 8A5 8A5 8G5 8S 8F5 8E5 8C5 8S 8A4 8G4 8S 4S 8E5 8C5 8S 8G4 4S 8GS4 8S 8A4 8F5 8S 8F5 8A4 8S 4S 8B4 8F5 8S 8F5 8F5 8E5 8S 8D5 8C5 8S 4S 2S :|| X"};//8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8C5 8C5 8S 8C5 8S 8C5 8D5 8E5 2S 2S 8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8E5 8E5 8S 8E5 8S 8C5 8E5 8S 8G5 8S 4S 8G4 8S 4S X"};
PROGMEM prog_char voz2[]={"8FS4 8FS4 8S 8FS4 8S 8FS4 8FS4 8S 8D5 8S 4S 8B3 4S 8S ||: 8G4 4S 8E4 4S 8C4 4S 8F4 8S 8G4 8S 8FS4 8F4 8S 8E4 8C5 8S 8E5 8F5 8S 8D5 8E5 8S 8C5 8S 8A4 8G4 8G4 4S :|| ||: 4S 8E5 8DS5 8D5 8B4 8S 8C5 8S 8E4 8F4 8G4 8S 8C4 8E4 8F4 4S 8E5 8DS5 8D5 8B4 8S 8C5 8S 8G5 8S 8G5 8G5 8S 2S 8E5 8DS5 8D5 8B4 8S 8C5 8S 8E4 8F4 8G4 8S 8C4 8E4 8F4 4S 8GS4 4S 8F4 4S 8E4 4S 8S 2S :|| 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 8S 4S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8G4 2S 2S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 8S 4S 8FS4 8FS4 8S 8FS4 8S 8FS4 8FS4 8S 8D5 4S 8S 8D4 8S 4S ||: 8G4 4S 8E4 4S 8C4 4S 8F4 8S 8G4 8S 8FS4 8F4 8S 8E4 8C5 8S 8E5 8F5 8S 8D5 8E5 8S 8D5 8S 8A4 8G4 8D4 4S :|| ||: 8C5 8G4 8S 8E4 4S 8E4 8S 8F4 8C5 8S 8C5 8F4 8S 4S 8G4 8F5 8S 8F5 8F5 8E5 8S 8D5 8C5 8F4 8S 8F4 8E4 8S 4S 8C5 8G4 8S 8E4 4S 8E4 8S 8F4 8C5 8S 8C5 8F4 8S 4S 8G4 8D5 8S 8D5 8D5 8C5 8S 8B4 8G4 8E4 8S 8E4 8C4 8S 4S :|| X"};//8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 8S 4S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8G4 2S 2S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 X"};
PROGMEM prog_char voz3[]={"8D3 8D3 8S 8D3 8S 8D3 8D3 8S 8B4 8S 4S 8G3 4S 8S ||: 8E4 4S 8C4 4S 8G3 4S 8C4 8S 8D4 8S 8CS4 8C4 8S 8C4 8G4 8S 8B4 8C5 8S 8A4 8B4 8S 8A4 8S 8E4 8F4 8D4 4S :|| ||: 8C3 4S 8G3 4S 8C4 8S 8F3 4S 8C4 8C4 8C4 8F3 8S 8C3 4S 8E3 4S 8G3 8C4 8S 8F5 8S 8F5 8F5 8S 8G3 8S 8C3 4S 8G3 4S 8C4 8S 8F3 4S 8C4 8C4 8C4 8F3 8S 8C3 8S 8GS3 4S 8AS3 4S 8C4 4S 8G3 8G3 8S 8C3 8S :|| 8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8D3 8D3 8S 8D3 8S 8D3 8D3 8S 8B4 4S 8S 8G3 8S 4S ||: 8E4 4S 8C4 4S 8G3 4S 8C4 8S 8D4 8S 8CS4 8C4 8S 8C4 8G4 8S 8B4 8C5 8S 8A4 8B4 8S 8A4 8S 8E4 8F4 8D4 4S :|| ||: 8C3 4S 8FS3 8G3 8S 8C4 8S 8F3 8S 8F3 8S 8C4 8C4 8F3 8S 8D3 4S 8F3 8G3 8S 8A3 8S 8G3 8S 8G3 8S 8C4 8C4 8G3 8S 8C3 4S 8FS3 8G3 8S 8C4 8S 8F3 8S 8F3 8S 8C4 8C4 8F3 8S 8G3 4S 8G3 8G3 8A3 8S 8B3 8C4 8S 8G3 8S 8C3 8S 4S :|| X"};
PROGMEM prog_char voz4[]={"X"};

/* */
/*
PROGMEM prog_char voz1[]={""};
PROGMEM prog_char voz1[]={""};
PROGMEM prog_char voz1[]={""};
PROGMEM prog_char voz1[]={""};*/

PROGMEM const char* voces[]={voz1,voz2,voz3,voz4};

/* Buzzers activos */
#define N_BUZZERS 3

/* Número de pin para cada buzzer. Caben en 8 bits (un char) */
char buzz[N_BUZZERS];

/* Leds indicadores*/
char rythmLed=11;
char tempoLed=12;

/* Contador para el led de tempo*/
char tempoLedCounter=0;

/* Tener las notas en un arreglo ayuda a automatizar cálculos para otros valores importantes */
             
prog_uint16_t NOTAS[] PROGMEM={31,33,35,37,39,41,44,46,49,52,55,58, //12
                               62,65,69,73,78,82,87,93,98,104,110,117,  //24
                               123,131,139,147,156,165,175,185,196,208,220,233, //36
                               247,262,277,294,311,330,349,370,392,415,440,466,  //48
                               494,523,554,587,622,659,698,740,784,831,880,932,  //60
                               988,1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,1865,  //72
                               1976,2093,2217,2349,2489,2637,2794,2960,3136,3322,3520,3729,  //84
                               3951,4186,4435,4699,4978,0};  //90


int delaytempo=7500/(constrain((int)(tempo),30,400)); //Equivale a (60 * 1000/tempo)/8. El 8 sale de que la fusa será la unidad básica de tiempo, y es 1/8 de negra.

/* Nota. Tiene la nota en forma de String que tocará cada buzzer*/
String nota[]={"S", "S", "S", "S"};

/* Duracion. Cada nota tiene una duración asociada. Este es un contador para cada nota.
  Cuando se acabe la duración de una nota para alguna voz, se le carga la siguiente nota.*/
unsigned char duracion[]={0,0,0,0};

/* Buffer. Acá se cargará cada nota desde PROGMEM, para cada voz, una por una.
  Ojo que cabe sólo una nota a la vez. Su tamaño se fija dado que es el máximo
  tamaño que puede tener en texto cualquier nota. (Ejemplo: 16CS5 es de tamaño máximo)
  Notar que debe existir también un fin de caracter "\0".*/

char buffer[6];

/* Head. Cabezales de cada voz que indica dónde quedó la última lectura.*/
int head[]={0,0,0,0};

/* Rep. Almacena posicion donde el cabezal debiera devolverse en caso de 
  una repetición. Si no hay repetición vale -1*/
int rep[]={-1, -1, -1, -1};

/* Arreglo con los períodos de cada nota en el buzzer, en microsegundos */
int NOTAS_P[N_BUZZERS];
const char mcd=30; //Propuesto por Jeremy Blum: 64. Se obligará a los períodos a ser un múltiplo de esto
//ha funcionado bien para 71. Probando con 30, que es más preciso.

/* Inicializador de los períodos */
void initNotas_p(){
  for (char i=0;i<N_BUZZERS;i++){
    int progint2int=pgm_read_word_near(NOTAS + parseNota(nota[i]));//NOTAS[parseNota(nota[i])];//
    int periodo=(int)(500000.0/progint2int);//nota[i])]); //periodo real de cada nota
    NOTAS_P[i]=periodo + ((periodo%mcd)<=(mcd/2) ? -(periodo%mcd) : (mcd-(periodo%mcd)));     //nuevo período es múltiplo de 64 más cercano al período original
  }
}

/* Count. Lleva la cuenta de ticks para cada buzzer. Números pequeños, 
  caben en un char, pero si se usa char, por alguna razón no funcionan los
  silencios. Ojo, estos valores son VARIABLES.*/
unsigned int count[N_BUZZERS];

/* Cantidad de cuentas por cada nota. El máximo es 252 asi que cabe en 8 bits.
  Las cuentas son el período total dividido por el tiempo que se demora en reiniciar el contador
  del timer. Como se quiere que el timer se demore "mcd" microsegundos (y se forzará que esto
  se cumpla), cuentas es período/mcd. Ojo, estos valores son CONSTANTES.
*/
unsigned char counts[N_BUZZERS];

/* Inicializador de las cuentas para cada nota de cada voz*/
void initCounts(){
  for (char i=0;i<N_BUZZERS;i++){
    counts[i]=NOTAS_P[i]/mcd;
    count[i]=0;
  }
}

char parseNota(String s){
  if (s[0]=='S') {
    PORTB &= ~(_BV(rythmLed-8));
    return 89;
  }
  /* recibe un string con el nombre de la nota y retorna su posición en el arreglo de notas.
   Se asumirá que los inputs son correctos*/
  
  /* Para parsear el string, hay que observar las siguientes cosas:
  El string tiene dos formas posibles: 
                      -XY
                      -XSY
    donde X en {A, B, C, D, E, F}
          Y en {0,1,2,3,4,5,6,7,8}
  
  Notar que: el número fija particiones de tamaño 12. El 1 envía a la primera partición, y
  así sucesivamente. La letra (y su sostenido siesque tiene) ubica la nota dentro de
  cada partición.*/
  /* Recordando que A=65, ..., G=71 en ASCII: */
  PORTB |= _BV(rythmLed-8);
  char pos;
  if (s[1]!='S'){//era natural, sin sostenido
    switch (s[0]){
      case 65: //A
        pos=9;
        break;
      case 66: //B
        pos=11;
        break;
      case 67: //C
        pos=0;
        break;
      case 68: //D
        pos=2;
        break;
      case 69: //E
        pos=4;
        break;
      case 70: //F
        pos=5;
        break;
      case 71: //G
        pos=7;
        break;
    }
  }
  else{
    switch (s[0]){ //era nota sostenida
      case 65: //A#
        pos=10;
        break;
      //no existe B#
      case 67: //C#
        pos=1;
        break;
      case 68: //D#
        pos=3;
        break;
      //no existe E#
      case 70: //F#
        pos=6;
        break;
      case 71: //G#
        pos=8;
        break;
    }
  }
  /* Fórmula mágica que retorna el subíndice del arreglo de notas según el nombre de la nota*/
  return (((s[(s[1]=='S') ? 2 : 1]-1) - '0')*12)+1+pos;
}

void initTimer1(){
  /* Desactivar interrupciones por overflow del contador*/
  TIMSK1  &= ~(1<<TOIE1);
  /* Activar interrupciones por Compare Match A*/
  TIMSK1 |= (1<<OCIE1A);
  /* Limpiar registros TCCR2A y TCCR2B*/
  TCCR1A = 0;
  TCCR1B = 0;
  /* Setear modo CTC (Clear Timer on Compare match): se resetea
  automáticamente el timer al igualarse al valor OCR2A por fijar*/
  TCCR1B |= (1<<WGM12);
  /* Configurar el prescaler a 64*/
  TCCR1B |= ((1<<CS11) | (1<<CS10));
  /* Setear el valor para el cual se generará una interrupción.
  El contador del timer 1 llega a 65535. Se quiere que genere
  una interrupción cada "delaytempo" ms. Como se demora 4us en cada
  click, se quiere que cuente hasta: 
  0.004ms·X=delaytempo.
  X=delaytempo/0.004.
  Ojo que dividir por 0.004 es multiplicar por 250.*/
  OCR1A=delaytempo*250;
}

void initTimer2(){
  /* Desactivar interrupciones por overflow del contador*/
  TIMSK2  &= ~(1<<TOIE2);
  /* Activar interrupciones por Compare Match A*/
  TIMSK2 |= (1<<OCIE2A);
  /* Limpiar registros TCCR2A y TCCR2B*/
  TCCR2A = 0;
  TCCR2B = 0;
  /* Setear modo CTC (Clear Timer on Compare match): se resetea
  automáticamente el timer al igualarse al valor OCR2A por fijar*/
  TCCR2A |= (1<<WGM21);
  /* Configurar el prescaler a 8*/
  TCCR2B |= (1<<CS21);
  /* Setear el valor para el cual se generará una interrupción.
  El contador del timer 2 llega a 255. Se llegará a mcd*2 -1.
  Usando un mcd=71 (valor empírico) se tiene que este valor es
  igual a 141.*/
  OCR2A=mcd*2 -1;
}

void initBuzzers(){
  for (char i=0; i<N_BUZZERS; i++){
    buzz[i]=i+2;
    pinMode(i+2,OUTPUT);
  }
}

/* Definición del handler de interrupciones del timer 1: ISR (Interrupt Service Routine)*/
ISR(TIMER1_COMPA_vect){
  /* Comportamiento del led de tempo*/
  PORTB = (tempoLedCounter==7 ? PORTB | _BV(tempoLed -8) : PORTB & ~(_BV(tempoLed-8)));
  tempoLedCounter=((tempoLedCounter+1)%8);
  /* Actualizar notas*/
  update();
}

/* Definición del handler de interrupciones del timer 2: ISR (Interrupt Service Routine)*/
ISR(TIMER2_COMPA_vect){
  //TCNT2 = 0; NO es necesario pues se configuro para reiniciarse al ocurrir match con OCR1A
  /* Recorrer cada buzzer. Para cada uno, aumentar su cuenta y ver si hay que cambiar toggle*/
  for (char i=0; i<N_BUZZERS; i++){
    count[i]++;
    if (count[i] == counts[i]){
      PORTD ^= (_BV(buzz[i])); //digitalWrite más rápido
      count[i]=0;
    }
  }
}

void getNext(char voz){
  /* Obtiene siguiente nota de la voz especificada y la sitúa en el buffer. Acá se identifica
    si la melodía posee indicación de repetición.*/
  if  (head[voz]==-1){
    buffer[0]='X';
    buffer[1]='\0';
  }
  else{
    /* obtengo puntero a string de la voz, en PROGMEM*/
    PGM_P prog_str=(prog_char*)pgm_read_word(&(voces[voz]));
    /* iterador para caracteres*/
    char j=0;
    /* ve cuando se acaba la melodía*/
    boolean fin=false;
    
    /* controla fin de repetición*/
    boolean endrep=false,beginrep=false;
    
    /* recorrer cada caracter hasta encontrar  un espacio. Si se estaba en el final, se verá una X.*/
    while (((buffer[j]=(char) pgm_read_byte(&prog_str[head[voz]+j]))!= ' ')){
      if (buffer[j]=='|'){//se encontró una repetición
        /* pongo el cabezal en la posicion de la nota siguiente */
        head[voz]+=4; // simbolo de repeticion es "||: " luego hay que saltarse 4 a la derecha para encontrar la siguiente nota.
        /* almaceno la posición para volver después */
        rep[voz]=head[voz];
        beginrep=true;
        /* obtengo inmediatamente la siguiente nota */
        break;
      }
      if (buffer[j]==':'){ //se encontró fin de repetición y efectivamente había repetición
        if (rep[voz]!=-1){ //si hay que repetir
          head[voz]=rep[voz];
          rep[voz]=-1;
        }
        else{  //ya se repitió y hay que ignorar el símbolo
          head[voz]+=4;
        }
        endrep=true;
        break;
      }
      if (buffer[j]=='X'){
        fin=true;
        break;
      }
      j++;
    }
    if (fin){
      head[voz]=-1;
      buffer[1]='\0';
    }
    else
    if (beginrep||endrep){//hubo repetición: tomar inmediatamente siguiente nota, o finalizo la repeticion y hay que tomar la siguiente nota.
        getNext(voz);
      }
    else{
        head[voz]+=(j+1);
        buffer[j]='\0';
    }
  }
}
boolean melodyOver(){
  char c=0;
  for (int i=0;i<N_BUZZERS;i++){
    c+=head[i];
  }
  return (c==(N_BUZZERS*-1))? true : false;
}

void endProgram(){
  cli();
  TIMSK1 &= ~(1<<OCIE1A);
  TIMSK2 &= ~(1<<OCIE2A);
  sei();
  digitalWrite(rythmLed,HIGH);
  digitalWrite(tempoLed,HIGH);
}
void seteaNota(char voz){
  /* Setea la nota en el buffer en la voz "voz".
    Ojo que la nota viene en un formato del tipo AB(S)C que hay que
    separar en A y B(S)C. Ojo que A puede tener 1 o 2 dígitos.*/
  if (buffer[0]=='X'){//Si ya no quedan notas, rellenar con silencios para siempre.
    nota[voz]="S";
    duracion[voz]=32;
    if (melodyOver()){
      endProgram();
    }
  }
  else{
    /* Establecer duración. Notar que duración de 32 vale 1, 16-> 2, 8->4, 4->8, 2->16, 1->32*/
    String numbers="26"; //caso 16 y 32
    int d;
    boolean dosdig=false; //true si la duración tenia 2 dígitos-
    if (numbers.indexOf(buffer[1])!=-1){//si la duración tiene 2 dígitos (16, 32)
      d=32/((buffer[0]-'0')*10 + (buffer[1]-'0'));
      dosdig=true;
    }
    else
      d=32/(buffer[0]-'0');
    duracion[voz]=d;
    
    /* Establecer la nota*/
    char j=dosdig ? 2 : 1; //se para en la posición de la nota
    //copiar el texto que representa a la nota
    nota[voz]=buffer;
    nota[voz]=nota[voz].substring(j);
  }
  /* Se ha actualizado la nota de la voz correspondiente*/
}
boolean loadVoices(){
  /* Leer las duraciones para cada voz. Si el contador es 0, leer la siguiente nota*/
  /*si ninguna voz cambiaba, sigue todo igual */
  boolean changed=false;
  for (char i=0; i<N_BUZZERS; i++){
    if (duracion[i]<=1){
      changed=true;
      /* Situar en el buffer la nota a situar*/
      getNext(i);
      /* Situa desde el buffer la nota en la voz que le corresponde*/
      seteaNota(i);
    }
    else duracion[i]--;//si la duración no se ha acabado, seguir con la misma nota en esa voz
  }
  return changed;
}

void update(){
  if (loadVoices()){
    initNotas_p();
    initCounts();
  }
}

void initLeds(){
  pinMode(rythmLed,OUTPUT);
  pinMode(tempoLed,OUTPUT);
  digitalWrite(tempoLed,HIGH);
}

void setup(){
  /* Establecer pins de buzzers como salida*/
  initBuzzers();
  /* Preparar leds indicadores*/
  initLeds();
  /* Cargar notas de voces en las notas actuales, calcular períodos de notas para poder
  reproducirlas y preparar duracion de cada nota*/
  update();
  /* Inicializar timers para reproducir notas*/
  cli();
  initTimer1();
  initTimer2();
  sei();
  
  
  pinMode(0,INPUT);
}

void loop(){
  /* Libre para hacer cualquier otra cosa. Ojo que el timer 0 sigue libre, y por ende
  funcionan cosas como delay(), millis(), etc. */
  
  tempo=map(analogRead(0),0,1023,30,400);
  delaytempo=7500/(int)(tempo);
  OCR1A=delaytempo*250;
  delay(250);
}


/* Review: 
-Día 1: Al parecer el asunto del mcd no es trivial: si alguna combinación de notas hace
que las interrupciones ocurran muy seguido, sencillamente no funciona. Deducción sacada a partir de
la experiencia de que para algunas tuplas de notas funciona y ambas suenan al unísono, para otras
hay sonido pero con notas extrañas y para otras sencillamente no hay sonido. Habría que de alguna
manera modificar los valores de las frecuencias para los casos donde las interrupciones ocurren
muy seguido. Habría que modificar la frecuencia de manera de que exista un mcd relativamente 
significativo. Por ejemplo, mmodificarlas todas para que siempre sea 5.
-Día 2: Se generan períodos aproximados de manera de cumplir con un mcd global. Con esto se 
elimina el problema anterior y se tienen notas bastante buenas. Se cambiará a timer 2 para no
desperdiciar la capacidad del timer 1 en términos de bits del counter.
Ahora se usa timer 2, y el modo de introducir notas es en texto. Se parsean con una función especial.
El reproductor funciona y puede tocar diversos acordes. La meta ahora es determinar un método de introducción
de la música que sea cómodo y considere duraciones de cada nota para todas las voces. Los silencios deberían
poder implementarse como frecuencias nulas. Silencios implementados
  Idea:
    Melodías como un string, para cada voz (cada Buzzer)
    Protocolo: AB(S)C o AS si es silencio.
          A=Duración (1=redonda, 2= blanca, 4=negra, 8=corchea, 16=semicorchea, 32=fusa. Se admitirá hasta fusa, luego, "loop()" debe actualizarse a esta velocidad, que es delaytempo/8)
          B=Nota (A, B, C, D, E, F, G)
          C=Octava de la nota. de 0 a 8, el La 440 es A4
    Separador: Espacio.
    Ejemplo: "8C5 8S 8G4 8G4 8A4 8S 8G4 4S 8S 8B4 8S 8C4"
    Eso para una sola voz.
    
    Cada voz carga su nota y debe durar lo que dice la información. 
    Cargar la nota significa: 
      -Actualizar las notas del vector de notas. Notar que el vector de notas tiene strings.
      -Establecer duración de nota. Para esto, crear un contador para cada nota, que 
        parte en el valor que le corresponde y decrece hasta llegar a 0, en cuyo caso
        se procede a leer la siguiente nota para esa voz, desde el string destinado a eso.
        Como idea para la lectura, ir "consumiendo" de alguna manera el string.
    Tras cargar la nota, actualizar los "counts" y los períodos "NOTAS_P" para cada nota. 
      Para eso existe función update();
    El delay de cada ciclo depende del tempo que se especifique.

-Día 3: implementado toque de melodías. Problema: se perdió polifonía, reproduce tan solo la última voz.
  Observación: Está actualizando TODAS LAS NOTAS con la última voz. Error de subindicación???
  NO. Se sobreescribe la referencia al char*, en seteaNota(). 
  Fix: Usar String en vez de Char*. Los String son mutables y cambian su tamaño a gusto.
  
  Problema: notas se repiten 5 veces al usar corcheas...
  Fixed. Problema con los contadores de duración.
  
-Día 4: El sistema funciona completamente. Único detalle desagradable: el delay produce pequeños
  silencios no deseados que a la larga estorban en la melodía. Posible arreglo: Usar otro timer.
  Este arreglo haría que el timer provoque una interrupción cada "delaytempo" segundos.
  "delaytempo" depende de "tempo". Asumiendo que "tempo" va de 30 a 300, delaytempo va de 250ms a 25ms
  Estos son los posibles tiempos que puede quererse para los interrupts. Si se usa un prescaler de 64, 
  se ve que se necesitan 6250 clicks y para 250ms 62500. Convendría usar el timer 1 que cubre estos
  valores por ser de 16 bits.
  
  -Se implementa con timer 1 en vez de delays en el loop. Los silencios siguen de todas maneras.
  Explicación: Si bien el timer 1 y el 2 son independientes entre sí, cada vez que ocurre una interrupción
  por el timer 1, el timer 2 pierde la cpu por las instrucciones que debe ejecutar el procesador por la
  interrupción del timer 1. Luego, la idea sería ejecutar cosas sólo cuando las notas cambien.
  Estrategia actual de detección de cambios de notas: interrupciones cada fusa. Cada nota tiene una duración
  definida por la cantidad de fusas que caben en su tempo. 
  El problema es que ahora, aunque NINGUNA nota cambie, se está evaluando la duración de cada nota y ejecutando varias instrucciones.
  La interrupción debería saber cuándo ejecutar realmente update. La idea es que sólo se ejecute cuando alguna nota
  debe cambiar. 
  Comprobado: funciona la continuidad del sonido aunque haya que evaluar un if.
  SOLUCIONADO
  
  Problema: ruidos extraños al terminar reproducción, y a veces en medio de la reproducción.
    Detalles: -Ocurre sólo cuando la melodía se hace larga. Si es corta, de a tres suenan perfecto.
              -Si agrego otra voz, crece la cantidad de datos y vuelve el ruido.
              
  IDENTIFICADO: El problema se produce por poca memoria RAM. Es posible ocupar la memoria flash PROGMEM, donde
    se almacena el programa. Esta memoria es mucho más grande que la memoria RAM. Un gran contra de usar esta
    memoria es que solo se almacenan valores constantes, y la implementación actual necesita hacer variar el
    texto de las voces. Para implementarel programa usando memoria flash, se debe redefinir los algoritmos
    de parseo del texto de las voces, dado que además hay limitados tipos de datos para la memoria flash, y
    String no es uno de ellos, aunque char si lo es. Luego, se pierden valiosos métodos para trabajar strings.
--Guardando versión actual 3.0 en respaldo de versiones--
    
    Se implementa versión con almacenamiento en PROGMEM y se solucionan los problemas descritos


Proyecto terminado. Única limitante hasta ahora es el largo posible de los strings. A partir de cierto largo, Java 
lanza StackOverflow. 

Ideas para el futuro: 
    -Con un opamp hacer un sumador y reproducir los sonidos en una única salida de audio, ya sea a un 
    parlante o a audífonos.
    DONE Implementar parseo de repeticiones, para no tener que copiar pegar todo un bloque. Ejemplo: "||:4C4 4D4 4E4 4F4:||4C4 X".
    -Implementar tresillos.
    -Crear software de inserción amigable de notas. El resultado del software es el resultado en texto que hay que copiar pegar para tener melodía.
    
--Guardando versión 4.0 en respaldo de versiones--
    -Se implementó repeticiones. Nomenclatura explicada al inicio.
    -Se testeó el jack de audio (chassis) y funciona bien mezclando directamente, pero podría quizás mejorarse
      la calidad del audio usando un sumador activo.
    -Se varía el tempo con un potenciómetro.
--Guardando versión 5.0 en respaldo de versiones--

    .*/
