#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>


#define ONE_WIRE_BUS 2

float temperaturaC = 0;
float AtemperaturaC=0;
float AtemperaturaF=0;
float temperaturaF=0;
int pulsador= 8;
int salirConfiguracion=0;
String informacion[5];
String salto="NO";

//*******Variables que se guardaran en la eeprom
 String pass;
 String RED;
 String sensorName;
 String sensorType;
 String location;



OneWire red1(ONE_WIRE_BUS);
DallasTemperature sensores(&red1);
SoftwareSerial beeSerial(7, 6); // RX, TX
//************************************************************************Funciones**********************************************************************************
void tiempo(int tiempo,int proposito)
{
int x=0;
long tiempoEspera = millis()+tiempo;

while(x==0)
{
  unsigned long tiempoActual=millis();
  if( tiempoEspera==0)
  {
    Serial.println( String(tiempo)+" MiliSegundos cumplidos");
    x=1;
    if(proposito == 1)
    {
    configuracion();
    }
    else if(proposito ==2)
    {
      Serial.println("Saliendo del modo configuracion");
      delay(2000);
    }
    
    
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

//*********************
void respuestaComandos(String Proceso)
{
  delay(2000);
  if (beeSerial.available()) 
  { 
    
    if (beeSerial.find("OK"))
    {
      
      Serial.println(Proceso+": OK");
     } 
    else if(beeSerial.find("no change"))
    {
      Serial.println(Proceso+": no change");
    }
    else
    {
      Serial.println(Proceso+": No");
      if(Proceso == "WIFI")
      {
        Serial.println("Hubo un error en la conexion al wifi , volviendo al modo configuriacion");
        
        configuracion();
      }
    }
  
  }
  else
  {
    Serial.println(Proceso+": Fail");
   }
   delay(1000);
}

//***********************
void configuracion()
{
int x=0;
Serial.println("Entraste en modo configuracion");
  
  if(digitalRead(pulsador)==HIGH)
  {
    tiempo(3000,2);
    
  }
  else{
    
   
    beeSerial.println("ATE0");
    delay(1000);
    respuestaComandos("ATE0");
    
    //Desconectamos la red local guardada
    beeSerial.println("AT+CWMODE=1");
    delay(2000);
    respuestaComandos("Mode1");
      
    //Desconectamos la red wifi que tenga guardado
    beeSerial.println("AT+CWQAP");
    delay(2000);
    respuestaComandos("Desconectar wifi");
    
    //configuramos modo estacion
    beeSerial.println("AT+CWMODE=3");
    delay(2000);
    respuestaComandos("ModoEstacion");
    
    
    
    //Iniciamos la red local
    //Serial.println("AT+CWJAP=\"ARRIS-5222\",\"macosay3099\"");//
    beeSerial.println("AT+CWSAP=\"smartcd35\",\admin1234\",6,2");
    delay(3000);
    respuestaComandos("Red Local");
    
    beeSerial.println("AT+CIPMUX=1");
    delay(2000);
    respuestaComandos("Conexion multiple");
    
    
    
    //Activamos el modo server para esperar los sensores
    beeSerial.println("AT+CIPSERVER=1,9001");
    delay(3000);
    respuestaComandos("ModoServer");
    delay(10000);
    
    Serial.println("Esperando por los datos del sensor");
  
    delay(1000);
   }
/*informacion[0]="\"ARRIS-5222\"";
informacion[1]="\"macosay3099\"";

char red[15];
informacion[0].toCharArray(red,15);
char password[20];
informacion[1].toCharArray(password,20);

String RED = String(red);
String pass= String(password);

Serial.println(RED);
Serial.println(pass); */
//beeSerial.println("AT+CWJAP="+RED+","+pass);//


 char datos[95];
 String cadena; 
while(x==0)
{
 
  String cad1 ="";
  int posicion_inicial = 0;
  int posicion_final= 0;
  int cont=0;
  if(digitalRead(pulsador)==HIGH)
  {
    tiempo(3000,2);
    
  }
  
  if(beeSerial.available())
  {
    if(beeSerial.find(":"))
    {
      //+IPD,0,67:~"ARRIS-5222"/"macosay3099"/TemperaturaDallas/Temperatura/recamara.
      beeSerial.readBytesUntil("~",datos,95);
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

   
}

  informacion[0]="\"ARRIS-5222\"";
  informacion[1]="\"macosay3099\"";

  char red[15];
  informacion[0].toCharArray(red,15);
  char password[20];
  informacion[1].toCharArray(password,20);
  
  char nombreSensor[20];
  informacion[2].toCharArray(nombreSensor,20);
  char tipoSensor[20];
  informacion[3].toCharArray(tipoSensor,20);
  char ubicacionSensor[20];
  informacion[4].toCharArray(ubicacionSensor,20);
  
  
  RED = String(red);
  pass= String(password);
  sensorName = String(nombreSensor);
  sensorType = String(tipoSensor);
  location = String(ubicacionSensor);
  
  Serial.println(cadena);
  Serial.println(RED);
  Serial.println(pass);
  Serial.println(sensorName);
  Serial.println(sensorType);
  Serial.println(ubicacionSensor);

  delay(8000);

  conectar();
   salto="SI";
  //EEPROM.update(0,red);
  //EEPROM.update(20,password);

}
//*******************************
void conectar()
{
  int x=0;
  delay(2000);
  beeSerial.println("AT+CWJAP="+RED+","+pass);
  delay(6000);
  respuestaComandos("WIFI");
  
  if(salto=="NO")
  {
    beeSerial.println("AT+CWMODE=1");
    delay(2000);
    respuestaComandos("Mode1");
  
    beeSerial.println("AT+CIPMUX=1");
    delay(2000);
    respuestaComandos("Conexion multiple");
  
    beeSerial.println("AT+CIPSERVER=1,9001");
    delay(2000);
    respuestaComandos("Esperando por conexion");
    while(x==0)
    {
      if (beeSerial.available()) 
      { 
        if (beeSerial.find("CONNECT"))
        {
          Serial.println("Servidor Conectado");
          x=1;
        } 
      }
      
    }
  }
  
}


float valorSensor()
{
  float valor =0.0;
  sensores.requestTemperatures(); 
  valor=sensores.getTempCByIndex(0);
  
  return valor;
}
//************************************************************************Setup**************************************************************************************
void setup() {
 
  Serial.begin(115200);
  beeSerial.begin(115200);
  sensores.begin();
  pinMode(pulsador,INPUT);
  conectar();
  temperaturaC=valorSensor();
  AtemperaturaC=valorSensor();
  Serial.println(temperaturaC);

  

}
//************************************************************************LOOP**************************************************************************************
void loop() {

  
  
  if(digitalRead(pulsador)==HIGH)
  {
   // Serial.println("Presionado");
   tiempo(5000,1);//1 es para Entrar al modo configuracion , 2 es para cuando queremos salir
  }
  else
  {
 
    temperaturaC=valorSensor();
    if(temperaturaC-AtemperaturaC==0.5)
    {
      Serial.println("La temperatura es: "+ String(temperaturaC));
      AtemperaturaC=temperaturaC;
      
    }
  
  }

//Serial.println(DallasTemperature::toFahrenheit(temperaturaC)); // Converts tempC to Fahrenheit
}


//************************************************************************LOOP**************************************************************************************
