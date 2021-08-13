#include <SPI.h>
#include <String.h>
#include <Ethernet.h>
#include <dht.h>
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,0, 80 };
byte gateway[] = { 192, 168, 0, 1 };
byte subnet[] = { 255, 255, 255, 0 };
dht DHT;
uint32_t timer = 0;
EthernetServer server(80); //Selecionar porta de saída (80)

//Conexão do módulo v3




int LuzInt = 6;
int LuzExt = 7;
int Ar = 8;
int Portao = 9;


String readString = String(30);
void Pagina();
String statusLed;

void setup() {
 Ethernet.begin(mac, ip);
 pinMode(LuzInt, OUTPUT);
 pinMode(LuzExt, OUTPUT);
 pinMode(Ar, OUTPUT);
 pinMode(Portao, OUTPUT);
 Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0){
    char c = Serial.read();
    if(c == 'A'){
      digitalWrite(LuzInt, digitalRead(LuzInt) ^ 1);
    }
    if(c == 'B'){
     digitalWrite(LuzExt, digitalRead(LuzExt) ^ 1);
    }
    if(c == 'C'){
      digitalWrite(Ar, digitalRead(Ar) ^ 1);
    }
    if(c == 'D'){
      digitalWrite(Portao, digitalRead(Portao) ^ 1);
    }
  }
  Pagina();
}

void Pagina(){  
 EthernetClient client = server.available();
 if(client)
 {
  while(client.connected())
  {
    if(client.available())
    {
       char c = client.read();

       if(readString.length() < 30) {
          readString += (c);
       }    
       
       if(c == '\n')
       {
         
         if(readString.indexOf("LuzInt") >= 0) {
           digitalWrite(LuzInt,!digitalRead(LuzInt));
         }
         
         if(readString.indexOf("LuzExt") >= 0) {
           digitalWrite(LuzExt,!digitalRead(LuzExt));
         }
         
         if(readString.indexOf("Ar") >= 0) {
           digitalWrite(Ar,!digitalRead(Ar));
         }
         
         if(readString.indexOf("Portao") >= 0) {
           digitalWrite(Portao,!digitalRead(Portao));
         }
         
                 
          // Cabeçalho http padrão
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          
          client.println("<!doctype html>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title> Automacao & Arduino </title>");
          client.println("<meta name=\"viewport\" content=\"width=320\">");
          client.println("<meta name=\"viewport\" content=\"width=device-width\">");
          client.println("<meta charset=\"utf-8\">");
          client.println("<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\">");
          client.println("</head>");
          client.println("<body>");
          client.println("<center>");
          
          client.println("<HR SIZE=40 font color=#4682B4>");
          client.println("<h1 style=\"color: #4682B4; font-family: arial; text-align: center;\">Reinaldo TCC</h1>"); 
          client.println("<h2 style=\"color: green; font-family: arial; text-align: center;\">Automacao Residencial</h2>");
          
          DHT.read11(A1); 
          client.println(DHT.humidity);
          client.println(" %");
          client.println(DHT.temperature);
          client.println(" °C");
          client.println();  
          if(digitalRead(LuzInt)) {
           statusLed = "Ligado" ;
          } else {
            statusLed = "Desligado";
          }
          client.println("<td> <form action=\"LuzInt\" method=\"get\">");
          client.println("<button type=submit style=\"width:400px;\">LuzInt - "+statusLed+"</button>");
          client.println("</form> <br /> </td>");
          
          if(digitalRead(LuzExt)) {
           statusLed = "Ligado" ;
          } else {
            statusLed = "Desligado";
          }
          client.println("<td> <form action=\"LuzExt\" method=\"get\">");
          client.println("<button type=submit style=\"width:400px;\">LuzExt - "+statusLed+"</button>");
          client.println("</form> <br /> </td>");
          
          if(digitalRead(Ar)) {
           statusLed = "Ligado" ;
          } else {
            statusLed = "Desligado";
          }
          client.println("<td> <form action=\"Ar\" method=\"get\">");
          client.println("<button type=submit style=\"width:400px;\">Ar - "+statusLed+"</button>");
          client.println("</form> <br /> </td>");
          
          if(digitalRead(Portao)) {
           statusLed = "Ligado" ;
          } else {
            statusLed = "Desligado";
          }
          client.println("<td> <form action=\"Portao\" method=\"get\">");
          client.println("<button type=submit style=\"width:400px;\">Portao - "+statusLed+"</button>");
          client.println("</form> <br /> </td>");  
          client.println("<HR SIZE=40 font color=#4682B4>");          
          client.println("</center>");
          client.println("<h4 style=\"color: green; font-family: arial; text-align: center;\">Direitos reservados</h4>");
          client.println("</body>");
          client.println("</html>");
         
         readString = "";
         client.stop();
         
       }
     }
  }
 }
}
