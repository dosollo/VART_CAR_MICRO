#include <Arduino.h>
#include <ESP8266WiFi.h>
//=========================================================================================================
// VART
//Driver Version2.0
//
// WiFi Code by dosollo
// Temp Code by miguelt
// ulstrasonic and motor basic by camilo
//
// 
// "Failure is not an option"
//
//Change log:
// camilo@                  08/08/19      Created v.2
//
//=========================================================================================================



///========================================================================================================
#define PIN_D1 5 // gpio5 = D1 PWM_A
#define PIN_D2 4 // gpio4 = D2 PWM_B
#define PIN_D3 0 // gpio0 = D3 DA (A- A+) 
#define PIN_D4 2 // gpio2 = D4 DB (B- B+) 



//Commands from movil
#define CMD_STOP      '0' 
#define CMD_FORWARD   '1' 
#define CMD_BACKWARD  '2' 
#define CMD_LEFT      '3' 
#define CMD_RIGHT     '4' 
#define CMD_AUTO      '5' 
#define CMD_MEASURE   '6'



#ifndef APSSID
#define APSSID "SmartCar"
#define APPSK  "SCar2019"
#endif


int port = 8888;  //Port number
WiFiServer server(port);

// this constant won't change. It's the pin number of the sensor's output:

// Pins for ultrasonic sensors.
//frontal sensors 
const int pingPin = 12; //D6
const int echoPin = 14; //D5
//back sensors
const int pingPin2 = 13; //D7
const int echoPin2 = 15; //D8

// defines variables for distance measurement
long duration;
int distance;

// For WiFi connection
const char *ssid = "HUAWEIDDOSO74";  //Enter your wifi SSID
const char *password = "Tuka73QQ";  //Enter your wifi Password
 
//const char *ssid = "pegasusnet";  //Enter your wifi SSID
//const char *password = "b0g0ta2019";  //Enter your wifi Password

//boolean var for auto fordward
bool fautonomus = false;
int outputpin= A0;
  WiFiClient client;
 //=========================================================================================================

//=========================================================================================================
// Fucntions
//=========================================================================================================



 bool stopmotor() {
digitalWrite(PIN_D1, LOW); // PWM_A HIGH
digitalWrite(PIN_D3, LOW); // DA HIGH
digitalWrite(PIN_D2, LOW); // PWM_B HIGH
digitalWrite(PIN_D4, LOW); // DB HIGH
//delay(microseconds); // wait
Serial.println("stop");


return true;
}


bool reversemotor(){
 //fordward
 Serial.println("starting reverse");


digitalWrite(PIN_D1, HIGH); // PWM_A HIGH
digitalWrite(PIN_D3, LOW); // DA HIGH
digitalWrite(PIN_D2, HIGH); // PWM_B HIGH
digitalWrite(PIN_D4, LOW); // DB HIGH
delay(1000); // wait
 Serial.println("reverse");
return true;
}


bool fordwardmotor(){
 //fordward
 Serial.println("starting fordward");


digitalWrite(PIN_D1, HIGH); // PWM_A HIGH
digitalWrite(PIN_D3, HIGH); // DA HIGH
digitalWrite(PIN_D2, HIGH); // PWM_B HIGH
digitalWrite(PIN_D4, HIGH); // DB HIGH
delay(1000); // wait
 Serial.println("fordward");
return true;
}

bool fordwardautomotor(){
 //fordward
 Serial.println("starting fordward auto");
 Serial.print("Distance in auto: ");
 Serial.println(distance);
while(client.read()!= CMD_STOP )
{
  Serial.println(client.read());
//if ((distance > 20) && fautonomus == true)
//{
 while(distance >50 )
  {
    digitalWrite(PIN_D1, HIGH); // PWM_A HIGH
    digitalWrite(PIN_D3, HIGH); // DA HIGH
    digitalWrite(PIN_D2, HIGH); // PWM_B HIGH
    digitalWrite(PIN_D4, HIGH); // DB HIGH
    delay(1000); // wait
    Serial.println("fordward auto");
    distanceobst();
    ////////Serial.println(client.read());
    if (client.read() == CMD_STOP)
    {
      Serial.println("Stop - Exit");
      stopmotor();
      delay(800);
      return false;
    }
  
  }  
    stopmotor();
    delay(800);

    //turing right 90 degrees
    turnrightmotor();
    delay(300);
    stopmotor();
    delay(200);

    
    //fordward 1 seg
    fordwardmotor();
    delay(1000);
    stopmotor();
    delay(200);

    //turing left 90 degrees
    turnleftmotor();
    delay(300);
    stopmotor();
    delay(200);
    
    distanceobst();
  
//} 
}

 Serial.print("finish auto ");
stopmotor();
return true;
}


bool turnrightmotor(){
//turn right

digitalWrite(PIN_D1, HIGH); // PWM_A HIGH
digitalWrite(PIN_D2, HIGH); // PWM_B HIGH
digitalWrite(PIN_D3, HIGH); // DA HIGH
digitalWrite(PIN_D4, LOW); // DB HIGH
//digitalWrite(PIN_D4, HIGH); // DB HIGH
delay(600); // wait
Serial.println("Right");

return true;
}



bool turnleftmotor(){
//turn left

digitalWrite(PIN_D1, HIGH); // PWM_A HIGH
digitalWrite(PIN_D2, HIGH); // PWM_B HIGH
digitalWrite(PIN_D3, LOW); // DA HIGH
digitalWrite(PIN_D4, HIGH); // DB HIGH

delay(600); // wait
Serial.println("left");

return true;
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
void distanceobst() {

// Clears the trigPin
digitalWrite(pingPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(pingPin, HIGH);
delayMicroseconds(10);
digitalWrite(pingPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
delay(200);

}


void medirtemperatura()
{
// en el loop

char sCade[100];
        //float ss = 13.1;

int analogValue = analogRead(outputpin);
float millivolts = (analogValue/1024.0) * 3300.0; 
float celsius = millivolts/10;
Serial.print("in DegreeC=   ");
Serial.println(celsius);


   sprintf(sCade,"Medicion=%f",celsius);
   client.write(sCade);
   client.write("\n");

delay(1000);
}
//=========================================================================================================
// Setup
//=========================================================================================================
void setup() 
{
pinMode(PIN_D1, OUTPUT); 
pinMode(PIN_D2, OUTPUT); 
pinMode(PIN_D3, OUTPUT); 
pinMode(PIN_D4, OUTPUT); 


//Set up ultrasonic variables
pinMode(pingPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input


  /////////////FROM BASIC SERIAL
  Serial.begin(115200);
  while(!Serial) { delay(100); }
  Serial.println("Starting Serial");


// ================starting WiFi==================
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi
  //WiFi.config(staticIP, gateway, subnet);


 
  // Wait for connection  
  Serial.println("Connecting to Wifi");
  WiFi.softAP(ssid, password);
    

  while (WiFi.status() != WL_CONNECTED)
  {   
     delay(500);
     Serial.print(".");
     delay(500);
  }
 
   Serial.println("");
   Serial.print("Connected to ");
   Serial.println(ssid);
 
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());  
   Serial.print("Open Telnet and connect to IP:");
   Serial.print(WiFi.localIP());
   Serial.print(" on port ");
   Serial.println(port);
   server.begin();

}



//=========================================================================================================
// Main Loop
//=========================================================================================================

void loop()
{
//inicializando
 Serial.println("Hello VART!");

//list of commands (testing commands )

//fordwardmotor();
//stopmotor();
//delay(2000);
//turnrightmotor();
//stopmotor();
//delay(2000); 

///////////////////////////////////
  client = server.available();

  char CarCommand;
  
  if (client) 
  {
     if(client.connected())
     {
        Serial.println("Client Connected");
        client.write("Client Connected\n");
     }
    
     while(client.connected())
     {      
         while(client.available()>0)
         {
             // read data from the connected client
             //Serial.write(client.read()); 

             // Read distance
              Serial.println("Main loop waiting cmd");
             distanceobst();

             //==========Reading commands===========
             CarCommand=client.read();
             switch(CarCommand)
             {
                case CMD_STOP:    
                     Serial.println("CMD_STOP");
                     stopmotor();
                     delay(2000); 
                     fautonomus = false;
                     break;
                case CMD_FORWARD:    
                     Serial.println("CMD_FORWARD");
                     fordwardmotor();
                     fautonomus = false;
                     break;
                case CMD_BACKWARD:    
                     Serial.println("CMD_BACKWARD");
                     reversemotor();
                     fautonomus = false;
                     break;
                case CMD_LEFT: 
                     Serial.println("CMD_LEFT");
                     turnleftmotor();
                     fautonomus = false;
                     break;
                case CMD_RIGHT:    
                     Serial.println("CMD_RIGHT");
                     turnrightmotor();
                     fautonomus = false;
                     break;
                case CMD_AUTO:    
                     Serial.println("CMD_AUTO");
                     fautonomus = true;
                     fordwardautomotor();
                     break;
                case CMD_MEASURE:   
                     Serial.println("CMD_MEASURE");
                     client.write("Client Connected\n");
                     medirtemperatura();
                     fautonomus = false;
                     break; 
             }
         }
         //Send Data to connected client
         while(Serial.available()>0)
         {
            client.write(Serial.read());
         }
     }
     client.stop();
     Serial.println("Client disconnected");
  }




}
