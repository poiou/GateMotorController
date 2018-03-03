#include <RCSwitch.h>


#define Open1 D1
#define Close1 D2

#define Open2 D3
#define Close2 D4

#define Reciever433 14

int ActionTime=2;
//A
int OpenCodes[] = {7843080,13939976,10590472,2350344,3931400 };
//B
int CloseCodes[] = {7843076,13939972,10606852,2350340,3931396};

unsigned long ActionStart=0;

//0 for open and 1 for close 3 for stop
int Action = 3;
int prevAction = 1;

int RemoteSize=sizeof(OpenCodes);
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  mySwitch.enableReceive(Reciever433);  // Receiver on inerrupt 0 => that is pin #2
  pinMode(Open1,OUTPUT);
  pinMode(Close1,OUTPUT);
  pinMode(Open2,OUTPUT);
  pinMode(Close2,OUTPUT);
  //pinMode(Reciever433,INPUT);
  
 digitalWrite(Close1, LOW);
  digitalWrite(Close2, LOW);
   digitalWrite(Open1, LOW); 
   digitalWrite(Open2, LOW);


  
   Serial.begin(9600);
}

void loop() {
   
    if((millis()/1000)==ActionStart+ActionTime && Action!=3){
      Stop();
           Action=3;
  }
   if (mySwitch.available()) 
   {
    int value = mySwitch.getReceivedValue();
        Serial.println(value);
    if (value == 0) 
    {
      Serial.print("Unknown encoding");
    } 
    else {
      if(GetOpenCode(value)&& (prevAction+Action==4)){
        Action=3;
        prevAction=0;
        Open();
      }
       if(GetCloseCode(value)&& (prevAction+Action==3)){
        Action=3;
        prevAction=1;
        Close();
      }
    }

    mySwitch.resetAvailable();
  }

}

void Open(){
  ActionStart = millis()/1000;
  Action=false;
  Serial.println("Open");
  digitalWrite(Close1, LOW);
  digitalWrite(Close2, LOW);
  
  digitalWrite(Open1, HIGH);
  delay(100);
  digitalWrite(Open2, HIGH);
  
}

void Close(){
  ActionStart = millis()/1000;
  Action=true;
   Serial.println("Close");
  digitalWrite(Open1, LOW);
  digitalWrite(Open2, LOW);
  
  digitalWrite(Close1, HIGH);
  delay(100); 
  digitalWrite(Close2, HIGH);
}

void Stop(){
   Serial.println("Stop");
  digitalWrite(Close1, LOW);
  digitalWrite(Close2, LOW);
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

