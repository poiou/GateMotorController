#include <RCSwitch.h>

#define Open1 D1
#define Close1 D2

#define Open2 D3
#define Close2 D4

#define Reciever433 0

int ActionTime=8;

int OpenCodes[] = {1500};

int CloseCodes[] = {1501};

unsigned long ActionStart=0;

//0 for open adn 1 for close
bool Action = true;

int RemoteSize=sizeof(OpenCodes);
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(Reciever433);  // Receiver on inerrupt 0 => that is pin #2
  pinMode(Open1,OUTPUT);
  pinMode(Close1,OUTPUT);
  pinMode(Open2,OUTPUT);
  pinMode(Close2,OUTPUT);
  //pinMode(Reciever433,INPUT);
}

void loop() {
   
   if((millis()/1000)==ActionStart+ActionTime){
    Stop();
   }
   if (mySwitch.available()) 
   {
    int value = mySwitch.getReceivedValue();
    if (value == 0) 
    {
      Serial.print("Unknown encoding");
    } 
    else {

      
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      int code=mySwitch.getReceivedValue();
      if(GetOpenCode(code)&Action==true){
        Action=false;
        Open();
      }
       if(GetCloseCode(code)&Action==false){
        Action=true;
        Close();
      }
    }

    mySwitch.resetAvailable();
  }

}

void Open(){
  ActionStart = millis()/1000;
  Action=false;
  digitalWrite(Close1, LOW);
  digitalWrite(Close2, LOW);
  
  digitalWrite(Open1, HIGH);
  delay(100);
  digitalWrite(Open2, HIGH);
  
}

void Close(){
  ActionStart = millis()/1000;
  Action=true;
  digitalWrite(Open1, LOW);
  digitalWrite(Open2, LOW);
  
  digitalWrite(Close1, HIGH);
  delay(100); 
  digitalWrite(Close2, HIGH);
}

void Stop(){
  digitalWrite(Close1, LOW);
  digitalWrite(Close2, HIGH);
  digitalWrite(Open1, LOW);
  digitalWrite(Open2, LOW);
}

bool GetOpenCode(int RecCode){
  for(int i = 0; i < RemoteSize; i++){
    if(RecCode==OpenCodes[i]){
      return true;
    }
    else{
      return false;
    }
  }
}
bool GetCloseCode(int RecCode){
  for(int i = 0; i < RemoteSize; i++){
    if(RecCode==CloseCodes[i]){
      return true;
    }
    else{
      return false;
    }
  }
}

