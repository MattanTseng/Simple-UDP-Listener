#include <WiFiNINA.h>         // Wifi library
#include <WiFiUdp.h>          // UDP library
char ssid[] = "WelderXR";        //  ENTER Wifi SSID (network name) BETWEEN QUOTES
char pass[] = "39910646";        // ENTER Wifi password BETWEEN QUOTES
// UDP Variables
unsigned int localPort = 2390;        // local port to listen on
const char* computerIP = "192.168.0.103";         // ENTER YOUR COMPUTER'S IP BETWEEN QUOTES
const unsigned int TXPort = 65013;
String Message;
String Test = "Test";

// These are the identifiers for the information being sent over UDP
String S1Type = "S1";
String S2Type = "S2";
String S3Type = "S3";
String P1Type = "P1";
String P2Type = "P2";
String P3Type = "P3";
String R1Type = "R1";

//These are variables that are used for the rotary encoder
int counter =0;
String currentDir = "";

// WiFi setup stuff. 
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

// This is where the Switch values will be stored
int S1Val;
int S2Val;
int S3Val;

// This is where the potentiometer values will be stored
float P1Val;
float P2Val;
float P3Val;

// These variables will determine direction of the rotary encoder. 
int currentStateCLK;
int lastStateCLK;
void setup() 
{


    // setup the pin modes
  /************************************************/
  // put your setup code here, to run once:
  // I used pullup inputs because they are more stable than pull down
  pinMode(Switch1, INPUT_PULLUP);
  pinMode(Switch2, INPUT_PULLUP);
  pinMode(Switch3, INPUT_PULLUP);
  pinMode(EncoderA, INPUT);
  pinMode(EncoderB, INPUT);
  //pinMode(Pot1, INPUT);
  //pinMode(Pot2, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

// Read out the initial states of the encoder.
  lastStateCLK = digitalRead(EncoderA);
  
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
  currentStateCLK = digitalRead(EncoderA);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    //Serial.print("A");
    //Serial.println(digitalRead(EncoderA));
    //Serial.print("B" );
    //Serial.println(digitalRead(EncoderB));
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

    //Serial.print("Direction: ");
    //Serial.print(currentDir);
    //Serial.print(" | Counter: ");
    //Serial.println(counter);
  }
    Message = R1Type + String(counter);
    char buf1[Message.length() + 1];
    Message.toCharArray(buf1, 50);
    Udp.beginPacket(computerIP, TXPort);
    Udp.write(buf1);
    Udp.endPacket();

  // Remember last CLK state
  lastStateCLK = currentStateCLK;

  P1Val = analogRead(Pot1);
  P2Val = analogRead(Pot2);
  P3Val = analogRead(Pot3);

  P1Val = P1Val; //map(P1Val, 0, 1023, 0, 1);
  Message = P1Type + String(P1Val);
  char buf2[Message.length() + 1];
  Message.toCharArray(buf2, 50);
  Udp.beginPacket(computerIP, TXPort);
  Udp.write(buf2);
  Udp.endPacket();

  P2Val = P2Val; //map(P2Val, 0, 1023, 0, 1);
  Message = P2Type + String(P2Val);
  char buf3[Message.length() + 1];
  Message.toCharArray(buf3, 50);
  Udp.beginPacket(computerIP, TXPort);
  Udp.write(buf3);
  Udp.endPacket();

  P3Val = P3Val; //map(P3Val, 0, 1023, 0, 1);
  Message = P3Type + String(P3Val);
  char buf4[Message.length() + 1];
  Message.toCharArray(buf4, 50);
  Udp.beginPacket(computerIP, TXPort);
  Udp.write(buf4);
  Udp.endPacket();


  
  // Read the sensors
  /*******************************************/
  if (digitalRead(Switch1) == HIGH){
    S1Val = 1;
  }
  else {
    S1Val = 0;
  }

  if (digitalRead(Switch2) == HIGH){
    S2Val = 1;
  }
  else {
    S2Val = 0;
  }

  if (digitalRead(Switch3) == HIGH){
    S3Val = 1;
  }
  else {
    S3Val = 0;
  }
    
    Message = S1Type + String(S1Val);
    char buf5[Message.length() + 1];
    Message.toCharArray(buf5, 50);
    Udp.beginPacket(computerIP, TXPort);
    Udp.write(buf5);
    Udp.endPacket();

    Message = S2Type + String(S2Val);
    char buf6[Message.length() + 1];
    Message.toCharArray(buf6, 50);
    Udp.beginPacket(computerIP, TXPort);
    Udp.write(buf6);
    Udp.endPacket();

    Message = S3Type + String(S3Val);
    char buf7[Message.length() + 1];
    Message.toCharArray(buf7, 50);
    Udp.beginPacket(computerIP, TXPort);
    Udp.write(buf7);
    Udp.endPacket();



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
