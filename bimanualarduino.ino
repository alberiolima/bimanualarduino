#define botao1 2 //Pino digigal 2
#define botao2 3 //Pino digital 3
#define saida  4 //Pino digital 4

boolean saidaAcionada = false;

void setup() {
  Serial.begin( 9600 );
  Serial.println( "Iniciado" );  

  pinMode( botao1, INPUT ); //usar resistor poll-down
  pinMode( botao2, INPUT ); //usar resistor poll-down
  
  pinMode( saida, OUTPUT );
  
}

void loop() {
  //Liga ou desliga equipamento na porta de saída
  if (saidaAcionada) { 
    digitalWrite( saida, HIGH );
  } else {
    digitalWrite( saida, LOW ); 
  }

  //Leitura das portas dos botões
  boolean estadoBotao1 = digitalRead( botao1 );
  boolean estadoBotao2 = digitalRead( botao2 );
  delay(10);//Aguarda um pouco para refazer a leitura dos botões
  
  //Refaz as leituras, caso as novas leituras seja diferentes das leituras anteriores ele refaz o teste, as duas leituras, da mesma porta, tem que ser iguais
  //Ele refaz a leituras dos dois botões caso um ou os dois deem erro
  while (( estadoBotao1 != digitalRead( botao1 ) )&&( estadoBotao2 != digitalRead( botao2 ) )) {
    delay(10);//Aguarda um pouco para refazer a leitura dos botões
    estadoBotao1 = digitalRead( botao1 );
    estadoBotao2 = digitalRead( botao2 );
  }

  //Acionar a porta quandos os dois botões estiverem perssionados
  saidaAcionada = (( estadoBotao1 == HIGH )&&( estadoBotao2 == HIGH ));

}
