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
int led=4;
bool estadoLed= false;
int salirConfiguracion=0;
String informacion[5];
String salto="NO";
bool Conectado=false;
bool Configuracion=true;

//*******Variables que se guardaran en la eeprom
char red[15];
char password[20];
char nombreSensor[20];
char tipoSensor[20];
char ubicacionSensor[20];
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

//La funcion tiempo establece que hacer cuando se pulsa el boton un determinado tiempo
void tiempo(int tiempo,int proposito)
{
int x=0;
long tiempoEspera = millis()+tiempo;
long parpadeo;

while(x==0)
{
  unsigned long tiempoActual=millis();
  
  if( tiempoEspera==0)
  {
    Serial.println( String(tiempo)+" MiliSegundos cumplidos");
    x=1;
    if(proposito == 1)
    {
      Conectado=false;
      Configuracion=true;
      configuracion();
      
    }
    else if(proposito ==2)
    {
      Serial.println("Saliendo del modo configuracion");
      Configuracion=false;
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
    //Serial.println(String(tiempoEspera-tiempoActual));
    
    if(millis()-parpadeo>=500)
    {
      Serial.print("Parpadeo");
      estadoLed= !estadoLed;
      digitalWrite(led,estadoLed);
      parpadeo = millis();
    }
    }
    else
    {
      tiempoEspera=0;
    }
    
  }

 
  
}
  
}

//*********************
//Esta funcion escribe en el puerto serial si los comandos At dieron OK o no
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
        Serial.println("Hubo un error en la conexion al wifi");
        Conectado=false;
        configuracion();
      }
    }
  
  }
  else
  {
    Serial.println(Proceso+": Fail");
    if(Proceso=="WIFI")
    {
      salto="SI";  
      Serial.println("Hubo un error de conexion con la tajeta WIFI, revisa la conexion");
    }
   }
   delay(1000);
}

//***********************
//Es el modo en el cual establecemos la nueva red mediante una estacion que crea el bee al princpio
void configuracion()
{
int x=0;
Serial.println("Entraste en modo configuracion");
digitalWrite(led,HIGH);
delay(3000);

  
  

  
  
      
     
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
     
  
  
  
  
   char datos[95];
   String cadena; 
    while(x==0)
    {
     
      String cad1 ="";
      int posicion_inicial = 0;
      int posicion_final= 0;
      int cont=0;
     
      
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
    
     /* informacion[0]="\"ARRIS-5222\"";
      informacion[1]="\"macosay3099\"";*/
    
      
      //informacion[0].toCharArray(red,15);
      //informacion[1].toCharArray(password,20);
      informacion[2].toCharArray(nombreSensor,20);
      informacion[3].toCharArray(tipoSensor,20);
      informacion[4].toCharArray(ubicacionSensor,20);
      
      
    
      
    //Serial.println(cadena);
     
    
      delay(5000);
    
      conectar();
      if(Conectado==true)
      {
     // EEPROM.update(0,red);
     //EEPROM.update(20,password);
      EEPROM.put(40,nombreSensor);
      EEPROM.put(60,tipoSensor);
      EEPROM.put(80,ubicacionSensor);
      }
      
      digitalWrite(led,LOW);
       salto="SI";
      
    }
} 


//**********
//Leer los datos de la memoria EEPROM
void leerEeprom()
{
 
  EEPROM.get(0,red);
  EEPROM.get(20,password);
  EEPROM.get(40,nombreSensor);
  EEPROM.get(60,tipoSensor);
  EEPROM.get(80,ubicacionSensor);

  RED = String(red);
  pass= String(password);
  sensorName = String(nombreSensor);
  sensorType = String(tipoSensor);
  location = String(ubicacionSensor);
  
  Serial.println(RED);
  Serial.println(pass);
  Serial.println(sensorName);
  Serial.println(sensorType);
  Serial.println(ubicacionSensor);
}

//*******************************
//Es la funcion que hace la conexion a internet, en caso de que la memoria eeprom no tenga la informacion correcta pasara al modo configuracion automaticamente
void conectar()
{
  int x=0;

  Serial.println("Buscando conectarse al modem");
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
    delay(3000);
    Conectado=true;
    /*while(x==0)
    {
      if (beeSerial.available()) 
      { 
        if (beeSerial.find("CONNECT"))
        {
          Serial.println("Servidor Conectado");
          delay(1000);
          enviarDatos("-Conectado al modulo",0);
          x=1;
        } 
      }
      
    }*/
  }
  
}

//****************************
//Actualiza el valor del sensor 
float valorSensor()
{
  float valor =0.0;
  sensores.requestTemperatures(); 
  valor=sensores.getTempCByIndex(0);
  
  return valor;
}
//*******************
//Envia los datos del sensor por wifi
void enviarDatos(String Mensajefinal,int NumeroDeConexion)
  { 
  int tam;
  String mensaje2;
  tam=Mensajefinal.length();
  mensaje2="AT+CIPSEND="+String(NumeroDeConexion)+ "," + String(tam);

  beeSerial.println(mensaje2);
  delay(500);
  
  beeSerial.println(Mensajefinal);
  
  delay(500);
}
//************************************************************************Setup**************************************************************************************
void setup() {

 
  Serial.begin(115200);
  beeSerial.begin(115200);
  sensores.begin();
  pinMode(pulsador,INPUT);
  pinMode(led,OUTPUT);
  /*delay(5000);
  
  /*informacion[0]="\"ARRIS-5222\"";
  informacion[1]="\"macosay3099\"";
  informacion[0].toCharArray(red,15);
  informacion[1].toCharArray(password,20);
  EEPROM.put(0,red);
  EEPROM.put(20,password);
  Serial.println("EEprom Actualizada");
  delay(5000);
  
  leerEeprom();
  conectar();// Esperar 25 segundos para llegar al modo servidor   */
  temperaturaC=valorSensor();
  AtemperaturaC=valorSensor();
  Serial.println(temperaturaC);

  

}
//************************************************************************LOOP**************************************************************************************
void loop() 
{

  
  
  if(digitalRead(pulsador)==HIGH)
  {
   // Serial.println("Presionado");
   tiempo(5000,1);//1 es para Entrar al modo configuracion , 2 es para cuando queremos salir
   
  }
  else
  {
 
    temperaturaC=valorSensor();
    if(temperaturaC-AtemperaturaC==1 || temperaturaC-AtemperaturaC==-1 )
    {
      Serial.println("La temperatura es: "+ String(temperaturaC));
      AtemperaturaC=temperaturaC;
      enviarDatos(String(temperaturaC),0);
      
    }

    else if(beeSerial.available())
    {
      if (beeSerial.find("~"))
      {
         delay(1000);
        Serial.println("se solicito la temperatura: "+ String(temperaturaC));
        AtemperaturaC=temperaturaC;
        enviarDatos(String(temperaturaC),0);
               
      }
    }
  
  }

  

//Serial.println(DallasTemperature::toFahrenheit(temperaturaC)); // Converts tempC to Fahrenheit
}


//************************************************************************LOOP**************************************************************************************
