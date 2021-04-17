#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

#define ONE_WIRE_BUS 2

float temperaturaC = 0;
float temperaturaF=0;
int pulsador= 8;
int salirConfiguracion=0;
String informacion[5];




OneWire red1(ONE_WIRE_BUS);
DallasTemperature sensores(&red1);
SoftwareSerial beeSerial(7, 6); // RX, TX
//************************************************************************Funciones**********************************************************************************
void tiempo(int proposito)
{
int x=0;
long tiempoEspera = millis()+5000;

while(x==0)
{
  unsigned long tiempoActual=millis();
  if( tiempoEspera==0)
  {
    Serial.println("5 Segundos cumplidos");
    x=1;
    if(proposito == 1)
    {
    configuracion();
    }
    delay(2000);
    
  }
  else
  {
    if(digitalRead(pulsador)== LOW)
    {
      x=1;
    }
    if(tiempoEspera-tiempoActual>2)
    {
    Serial.println(String(tiempoEspera-tiempoActual));
    }
    else
    {
      tiempoEspera=0;
    }
    
  }

 
  
}
  
}

void configuracion()
{
int x=0;
/*
//Desconectamos la red local guardada
beeSerial.println("AT+CWMODE=1");
delay(1000);
  
//Desconectamos la red wifi que tenga guardado
beeSerial.println("AT+CWQAP");
delay(1000);

//configuramos modo estacion
beeSerial.println("AT+CWMODE=3");
delay(1000);

//Iniciamos la red local
//Serial.println("AT+CWJAP=\"ARRIS-5222\",\"macosay3099\"");//
beeSerial.println("AT+CWSAP=\"smartcd35\",\admin1234\",6,2");
delay(3000);

//Activamos el modo server para esperar los sensores
beeSerial.println("AT+CIPSERVER=1,9001");


*/
Serial.println("Entraste en modo configuracion");
delay(3000);
while(x==0)
{
  char datos[50];
  String cadena;
  String cad1 ="";
  int posicion_inicial = 0;
  int posicion_final= 0;
  int cont=0;
      
  if(Serial.available())
  {
    if(Serial.find("~"))
    {
      
      Serial.readBytesUntil("~",datos,50);
      for (int i=0; i<strlen(datos);i++)
      {
        cadena += String(datos[i]);
      }

    do
    {
    
      posicion_final = cadena.indexOf("/",posicion_inicial);
      cad1 = cadena.substring(posicion_inicial,posicion_final);
      informacion[cont] = String(cad1);
       
      posicion_inicial = posicion_final + 1;
      cont++;
    
    
     } 
     while(posicion_final > 0);

     x=1;
      
    }
    else
    {
      x=0;
    }
  }
  
  else
   {
   x=0;
   }
}

  for(int j=0;j<5;j++)
  {
    Serial.println(informacion[j]);
  }
}

//************************************************************************Setup**************************************************************************************
void setup() {
 
  Serial.begin(9600);
  sensores.begin();
  pinMode(pulsador,INPUT);
  

}
//************************************************************************LOOP**************************************************************************************
void loop() {

  if(digitalRead(pulsador)==HIGH)
  {
   // Serial.println("Presionado");
   tiempo(1);//1 es para Entrar al modo configuracion , 2 es para cuando queremos salir
  }
  else{
 
  sensores.requestTemperatures(); 
  temperaturaC=sensores.getTempCByIndex(0);
  Serial.println("La temperatura es: "+ String(temperaturaC));
  }
  /*Serial.println(tiempo);
  Serial.println(detener);
  Serial.println("");*/
//Serial.println(DallasTemperature::toFahrenheit(temperaturaC)); // Converts tempC to Fahrenheit
}


//************************************************************************LOOP**************************************************************************************
