const long baudrate=115200;
//const long baudrate=9600;
const unsigned int timeout=500; //milisecons since return data
const byte numRelays=8; //how many relays in system. Maximum: 8

//assing relay to arduino pin
const byte relay1=9;
const byte relay2=7;
const byte relay3=5;
const byte relay4=3;
const byte relay5=2;
const byte relay6=8;
const byte relay7=6;
const byte relay8=4;

const byte sensor=A5; //+9V detect

//releay sensing inputs
const byte sensor1=A0;
const byte sensor2=12;
const byte sensor3=10;
const byte sensor4=0;
const byte sensor5=A4;
const byte sensor6=13;
const byte sensor7=11;
const byte sensor8=1;

const byte relays[]={relay1,relay2,relay3,relay4,relay5,relay6,relay7,relay8}; //Relay-to-pin assing array
const byte sensors[]={sensor1,sensor2,sensor3,sensor4,sensor5,sensor6,sensor7,sensor8}; //Sensor-to-pin assing array
const int address=0; //EEPROM storage address

#include <EEPROM.h>

long timervar; //delay timer

byte state; //relay state
byte sensing; //sensor state
byte i,a;

void setup()
{
  Serial.begin(baudrate); //Start Serial port
  pinMode(sensor,INPUT_PULLUP); //sensor pin to input
  for (i=0;i<numRelays;i++){pinMode(relays[i],OUTPUT);} //set pins to output
    state=EEPROM.read(address); //read eeprom
    if (state<1 or state>8){
      state=1;
  }
  setState(); //set relays
  while(!Serial){;} //for Leonardo
}
void loop()
{
  if (Serial.available()) //check for serial data
  {
    a=Serial.parseInt(); //get serial data from PC
    if ((a>0) && (a<=numRelays)) //check for good data
    {
      state=a-1;
      setState(); //set relays
      EEPROM.write(address,state); //store to eeprom
    }
  }
  if (timervar<millis()) //check for replay timeout
  {
    timervar=millis()+timeout;  //reset timout counter
    Serial.print("ON: ");
    Serial.println(state+1); //send active releay number
        
    Serial.print("OFFLINE: "); //send offline relays
    for (i=0;i<numRelays;i++) //loop thru bits
    {
      if (!(sensing | (1<<i))) //if bit is 0 then problems with relay.
                               //status: 1011     status: 1011
                               //i=2 shl 0100     i=1 shl 0010
                               //and     0000     and     0010
      {
        Serial.print(i+1); //send fault relay number
      }
    }
    Serial.print("\n");
    if (!(digitalRead(sensor))) //check for PSU. If low, then no PSU
    {
      Serial.println("No +12V supply");  
    }
  }
  setState();  //set relays in case if something fucked up
}

//Set relay state
void setState()
{
  sensing=0;
  for (i=0;i<numRelays;i++)
  {
    if (digitalRead(i) << i);
    {
      sensing= sensing | 1 << i;
    }
    if (i==state){
      digitalWrite(relays[i],HIGH);
      sensing= sensing | 1 << i;
    }
    else {digitalWrite(relays[i],LOW);}
    
  }
}
