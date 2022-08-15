#include <WiFiNINA.h>         // Wifi library
#include <WiFiUdp.h>  


// UDP library
/*
char ssid[] = "WelderXR";        //  ENTER Wifi SSID (network name) BETWEEN QUOTES
char pass[] = "39910646";        // ENTER Wifi password BETWEEN QUOTES
*/

char ssid[] = "IASTATE";
char pass[] = "";
// UDP Variables
unsigned int localPort = 2390;        // local port to listen on
const char* computerIP = "oculus2-alpha.vrac.iastate.edu";
//const char* computerIP = "192.168.0.96";         // ENTER YOUR COMPUTER'S IP BETWEEN QUOTES
const unsigned int TXPort = 65013;
String Message;
String Test = "Test";

// these variables will be used to see if there is a change
int OldS1;
int OldS2;
int OldS3;
int OldP1;
int OldP2;
int OldP3;


String S1Type = "S1";
String S2Type = "S2";
String S3Type = "S3";
String P1Type = "P1";
String P2Type = "P2";
String P3Type = "P3";
String R1Type = "R1";
int counter =0;
String currentDir = "";
int status = WL_IDLE_STATUS;      // Status of WiFi connection
WiFiUDP Udp;                          // Instantiate UDP class
// Define the pins for inputs
/********************************************************************/
// these are the 2 position switches
const int Switch1 = 4;
const int Switch2 = 5;
// These are for the rotary encoder
const int Switch3 = 6;
const int EncoderA = 19;
const int EncoderB = 10;
// These are for the choke
const int Pot1 = A0;
// This is for the other POT
const int Pot2 = A1;
const int Pot3 = A2;

// These values are how the sensor data is averaged to avoid false signals. 
int IterationCount = 5;
int ReadingCount =0;

// This is where the Switch values will be stored
int S1Val;
int S2Val;
int S3Val;

float P1Val;
float P2Val;
float P3Val;

int currentStateCLK;
int lastStateCLK;
unsigned long lastButtonPress = 0;
void setup() 
{

  lastStateCLK = digitalRead(EncoderA);

  
  OldP1 = analogRead(Pot1);
  OldP2 = analogRead(Pot2);
    // setup the pin modes
  /************************************************/
  // put your setup code here, to run once:
  pinMode(Switch1, INPUT_PULLUP);
  pinMode(Switch2, INPUT_PULLUP);
  pinMode(Switch3, INPUT_PULLUP);
  pinMode(EncoderA, INPUT);
  pinMode(EncoderB, INPUT);
  //pinMode(Pot1, INPUT);
  //pinMode(Pot2, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Setup wifi connection
  /**********************************************/
  // Check for Wifi Module. If no module, don't continue
  if (WiFi.status() == WL_NO_MODULE)
  {
    while (true);
  }
  // Connect to Wifi Access Point
  connectToAP();
  // UDP Connect with report via serial
  Udp.begin(localPort);
}
void loop() {

    //Serial.print("A");
    //Serial.println(digitalRead(EncoderA));
    //Serial.print("B" );
    //Serial.println(digitalRead(EncoderB));
  currentStateCLK = digitalRead(EncoderA);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    Serial.print("A");
    Serial.println(digitalRead(EncoderA));
    Serial.print("B" );
    Serial.println(digitalRead(EncoderB));
    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(EncoderB) != lastStateCLK) {
      counter --;
      currentDir ="L";
    } else {
      // Encoder is rotating CW so increment
      counter ++;
      currentDir ="R";
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }
    SendData(R1Type, counter);
    
    
    /*Message = R1Type + String(counter);
    char buf1[Message.length() + 1];
    Message.toCharArray(buf1, 50);
    Udp.beginPacket(computerIP, TXPort);
    Udp.write(buf1);
    Udp.endPacket();
    */
    
  // Remember last CLK state
  lastStateCLK = currentStateCLK;

    P1Val = analogRead(Pot1);
    P2Val = analogRead(Pot2);
    P3Val = analogRead(Pot3);
    SendData(P1Type, P1Val);
    SendData(P2Type, P2Val-300);
    SendData(P3Type, P3Val);

    S1Val = digitalRead(Switch1)
    S2Val = digitalRead(Switch2)
    S3Val = digitalRead(Switch3)

    SendData(S1Type, S1Val);
    SendData(S2Type, S2Val);
    SendData(S3Type, S3Val);

  // Read the sensors
  /*******************************************/


  if (digitalRead(Switch1) == HIGH){
    S1Val += 1;
  }


  if (digitalRead(Switch2) == HIGH){
    S2Val += 1;
  }

  if (digitalRead(Switch3) == HIGH){
    S3Val += 1;
  }



    if (ReadingCount == IterationCount){
      P1Val = P1Val/IterationCount;
      P2Val = P2Val/IterationCount;
      P3Val = P3Val/IterationCount;
      SendData(P1Type, P1Val);
      SendData(P2Type, P2Val-300);
      SendData(P3Type, P3Val);

      Serial.print("P2: ");
      Serial.println(P2Val);

      
      S1Val = floor(S1Val/IterationCount);
      S2Val = floor(S2Val/IterationCount);
      S3Val = floor(S3Val/IterationCount);
      SendData(S1Type, S1Val);
      SendData(S2Type, S2Val);
      SendData(S3Type, S3Val);

      ReadingCount = 0;
    }

    ReadingCount++;
}

// Connect to wifi network
void connectToAP() 
{
  // Try to connect to Wifi network
  while ( status != WL_CONNECTED) 
  {
    status = WiFi.begin(ssid, pass);
    // wait 1 second for connection:
    delay(1000);
  }
}

void SendData(String Source, float Value){
    String Content = Source + String(Value);
    char buf[Content.length() + 1];
    Content.toCharArray(buf, 50);
    Udp.beginPacket(computerIP, TXPort);
    Udp.write(buf);
    Udp.endPacket();
}

void SendData(String Source, int Value){
    String Content = Source + String(Value);
    char buf[Content.length() + 1];
    Content.toCharArray(buf, 50);
    Udp.beginPacket(computerIP, TXPort);
    Udp.write(buf);
    Udp.endPacket();
    
}
