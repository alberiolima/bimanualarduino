/*
 * Acionamento bimanual
 * As portas de leitura estão configurada para usar os resistores internet de pull-up
 * Então as portas retornam HIGH normalmente e LOW quando o botão for acionado
 * O botão tem dois pinos, um pino vai direto para o arduino e o outro é ligado ao GND
 */
#define pBotao 2 //Pino digigal 2 e 3
#define saida  4 //Pino digital 4

boolean saidaAcionada = false;
boolean estadoBotao[2] = {HIGH,HIGH};
unsigned long tempoBotao = 0;
byte iBotao = 0;

void setup() {
  Serial.begin( 9600 );
  Serial.println( "Iniciado" );  

  pinMode( pBotao, INPUT_PULLUP ); 
  pinMode( pBotao + 1, INPUT_PULLUP ); 
  
  pinMode( saida, OUTPUT );
  digitalWrite( saida, LOW );   
}

void loop() {
  
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
      } else {
        if (estadoBotao[0]&&estadoBotao[1]){ //dois botões liberados
          tempoBotao = 0;
        } 
      }
    }

    //Liga/Desliga saída
    if (!estadoBotao[0]&&!estadoBotao[1]){ //Acionar a porta de saída quandos os dois botões estiverem pressionados
      if ((!saidaAcionada)&&(millis() - tempoBotao < 500)){ //se levar menos de 0,5 segundos entre o apertar de um botão e o outro
        saidaAcionada = true;
        digitalWrite( saida, HIGH );        
      }        
    } else if (estadoBotao[0]||estadoBotao[1]){ //Desliga a porta de saída quando um dos botões ou os dois forem liberados
      if (saidaAcionada){
        saidaAcionada = false;
        digitalWrite( saida, LOW );        
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
