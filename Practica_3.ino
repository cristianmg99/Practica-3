#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

#define ONE_WIRE_BUS 2

float temperaturaC = 0;
float temperaturaF=0;





OneWire red1(ONE_WIRE_BUS);
DallasTemperature sensores(&red1);
SoftwareSerial beeSerial(7, 6); // RX, TX


void setup() {
 
  Serial.begin(9600);
  

}

void loop() {
  
  temperaturaC=sensores.getTempCByIndex(0);
  Serial.println("La temperatura es: "+ String(temperaturaC));
//Serial.println(DallasTemperature::toFahrenheit(temperaturaC)); // Converts tempC to Fahrenheit
}
