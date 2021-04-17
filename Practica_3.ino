#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

#define ONE_WIRE_BUS 2

float temperaturaC = 0;
float temperaturaF=0;





OneWire red1(ONE_WIRE_BUS);
DallasTemperature sensores(&red1);
SoftwareSerial beeSerial(7, 6); // RX, TX
//************************************************************************Funciones**********************************************************************************
void configuracion()
{

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

*/
  
}

//************************************************************************Setup**************************************************************************************
void setup() {
 
  Serial.begin(9600);
  sensores.begin();
  

}
//************************************************************************LOOP**************************************************************************************
void loop() {
  unsigned long tiempo=millis();
  long detener=millis()+5000;
  sensores.requestTemperatures(); 
  temperaturaC=sensores.getTempCByIndex(0);
  Serial.println("La temperatura es: "+ String(temperaturaC));

  /*Serial.println(tiempo);
  Serial.println(detener);
  Serial.println("");*/
//Serial.println(DallasTemperature::toFahrenheit(temperaturaC)); // Converts tempC to Fahrenheit
}


//************************************************************************LOOP**************************************************************************************
