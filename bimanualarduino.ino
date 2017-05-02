/*
 * Acionamento bimanual
 * As portas de leitura estão configurada para usar os resistores internet de pull-up
 * Então as portas retornam HIGH normalmente e LOW quando o botão for acionado
 * O botão tem dois pinos, um pino vai direto para o arduino e o outro é ligado ao GND
 * 
 * Saídas digitais:
 * - Saída para acionar equipamento
 * - RESET-OUT, Saída de RESET, acionado quando o botão reset for pressionado, para comunicar o RESET a outros equipamentos
 * - FREIO-OUT, Aciona sistema de freios
 * 
 * Entradas digitais
 * - RESET-IN, Entrada de RESET, receber informação de outro equipamento para também fazer reset nesse equipamento
 * - FREIO-IN, Recebe comando de FREIO de outro equipamento
 */
#define pBotao   2 //Pino digigal 2 e 3
#define saida    4 //Pino digital 4
#define freioOut 5 //Pino digital 5

boolean saidaAcionada = false;
boolean estadoBotao[2] = {HIGH,HIGH};
unsigned long tempoBotao = 0;
unsigned long tempoFreio = 0;
byte iBotao = 0;

void setup() {
  Serial.begin( 9600 );
  Serial.println( "Iniciado" );  

  pinMode( pBotao, INPUT_PULLUP ); 
  pinMode( pBotao + 1, INPUT_PULLUP ); 
  pinMode( freioOut, OUTPUT );
  digitalWrite( freioOut, LOW );
  
  pinMode( saida, OUTPUT );
  digitalWrite( saida, LOW );   
}

void loop() {
  
  //desliga Frenagem depois do tempo necessário
  //Deve ficar no inicio do loop por se tratar se rotina crítica
  if ( tempoFreio > 0 ) {
    if ( millis() - tempoFreio >= 2000 ) {
      digitalWrite( freioOut, LOW );
      tempoFreio = 0;
    }
  }
  
  //Leitura das portas dos botões
  boolean estadoBotao_temp = digitalRead( pBotao + iBotao );
  if(estadoBotao_temp != estadoBotao[iBotao]){
    delay(10);//Aguarda um pouco para refazer a leitura dos botões  
    estadoBotao_temp = digitalRead( pBotao + iBotao );
    if(estadoBotao_temp != estadoBotao[iBotao]){
      estadoBotao[iBotao] = estadoBotao_temp;      
      if (!estadoBotao[iBotao]){//botão apertado
        if ( tempoBotao == 0 ){
          tempoBotao = millis();
        } 
      } else if (estadoBotao[0]&&estadoBotao[1]){ //só reiniciar o ciclo quando os dois botões são liberados
        tempoBotao = 0;
      }       
    }

    //Liga/Desliga saída
    if (( tempoFreio == 0 )&&(!estadoBotao[0]&&!estadoBotao[1])){ //Acionar a porta de saída quandos os dois botões estiverem pressionados
      if ((!saidaAcionada)&&(millis() - tempoBotao < 500)){ //se levar menos de 0,5 segundos entre o apertar de um botão e o outro
        saidaAcionada = true;
        digitalWrite( saida, HIGH );        
      }        
    } else if (( tempoFreio == 0 )&&(estadoBotao[0]||estadoBotao[1])){ //Desliga a porta de saída quando um dos botões ou os dois forem liberados
      if (saidaAcionada){
        saidaAcionada = false;
        digitalWrite( saida, LOW );        

        //inicia frenagem
        if ( tempoFreio == 0 ) {
          tempoFreio = millis();
          digitalWrite( freioOut, HIGH );
        }
      }  
    }    
    if (saidaAcionada) {
      Serial.println( "Saída ON" );
    } else {
      Serial.println( "Saída OFF" );
    }
  }

  //troca entre botões
  iBotao++;
  if ( iBotao > 1 ) {
    iBotao=0;
  }  
}
