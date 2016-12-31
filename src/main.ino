#include <Arduino.h>
#include <SoftwareSerial.h>

#define RX 0
#define TX 1
#define WAKESW 2
#define MLDP 3
#define CONN 4
#define MLDPEV 5
#define ACTIVITY 6
#define WAKEHW 7
#define BUTTON 8
#define LED 13

#define DEVICES 5
#define MACLENGTH 12
#define PRIVLENGTH 1
#define DEVLENGTH 16
#define UUIDLENGTH 32
#define RSSILENGTH 3

//Arrays for device information
char MAC[DEVICES][MACLENGTH];
char priv[DEVICES][PRIVLENGTH];
char dev_name[DEVICES][DEVLENGTH];
char UUID[DEVICES][UUIDLENGTH];
char RSSIHex[DEVICES][RSSILENGTH];

struct integerIndex{
  int value = 0;
  int index = 0;
};

//RSSI values
integerIndex RSSIInt[DEVICES];
int highestRSSIIndex[DEVICES];

//Read from BT-module and echo to pc.
void BTRead(){
  char temp;
  delay(100);
  while(Serial.available() && temp != '\n'){
    temp = Serial.read();
    //Serial.write(temp);
  }
}

//Read from BT-module and echo to pc (idem but without delay).
void BTReadNoDelay(){
  char temp;
  while(Serial.available() /*&& temp != '\n'*/){
    temp = Serial.read();
    //Serial.write(temp);
  }
}

void BTSetup(){
  // Setup RN4020
  digitalWrite(WAKESW, HIGH);
  digitalWrite(WAKEHW, LOW);
  digitalWrite(MLDP, LOW);
  delay(500);
  BTRead();                          //read the "CMD" + empty buffer
  BTReadNoDelay();
  Serial.println("derp");           //first command always gives error (don't know why, get the ERR message back)
  BTRead();
  BTReadNoDelay();
  //Serial.println("+");            //echo
  BTRead();
  Serial.println("SF,1");           //factory reset
  BTRead();
  Serial.println("SS,C0000000");    //set supported services
  BTRead();
  Serial.println("SR,10000000");    //set features (peripheral, mldp enabled)
  BTRead();
  BTRead();
  Serial.println("R,1");            //reboot so actions take effect
  delay(1500);                      //delay after reboot important!
  BTRead();                         //read "reboot" and "CMD"
  //Serial.println("");
  digitalWrite(WAKESW, LOW);
  BTRead();
}

void changePower(){
  digitalWrite(WAKESW, HIGH);
  delay(500);
  Serial.println("SP,4");
  BTRead();
  Serial.println("N,00000004");
  BTRead();
  Serial.println("A");
  BTRead();
  digitalWrite(WAKESW, LOW);
  delay(5000);
  digitalWrite(WAKESW, HIGH);
  delay(500);
  Serial.println("SP,3");
  BTRead();
  Serial.println("N,00000003");
  BTRead();
  Serial.println("A");
  BTRead();
  digitalWrite(WAKESW, LOW);
  delay(5000);
  digitalWrite(WAKESW, HIGH);
  delay(500);
  Serial.println("SP,2");
  BTRead();
  Serial.println("N,00000002");
  BTRead();
  Serial.println("A");
  BTRead();
  digitalWrite(WAKESW, LOW);
  delay(5000);
}

void setup(){
  // Pin configuration
  pinMode(WAKESW, OUTPUT);
  pinMode(MLDP, OUTPUT);
  pinMode(WAKEHW, OUTPUT);
  /*pinMode(CONN, INPUT);
  pinMode(MLDPEV, INPUT);
  pinMode(ACTIVITY, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);*/

  // Start Serial
  Serial.begin(115200);


  BTSetup();
}

void loop(){
  //scanDevices();
  //connectToDevice();
  changePower();
}
