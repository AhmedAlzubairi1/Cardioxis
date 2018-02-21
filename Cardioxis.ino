#include <SoftwareSerial.h>
#include <Servo.h>

//
#define MAX_RATE 400
#define MIN_RATE 200
#define END_DELAY 100
#define MAX_BUFFER 250
#define oxyPin 11
#define pulsePin 10 


#define rtsPin 3
#define ctsPin 4
#define extra5VPin 8

#define rxPin 7
#define txPin 6
SoftwareSerial mySerial(rxPin, txPin); //232_TX,232_RX

byte rtsVal;
byte ctsVal;
 
int alertAlarm = 11;
int index=7;

int oxyRate;
byte pulseOne;
byte pulseHun;
int pulseRate;
byte x;
byte id;
boolean endSequence;
Servo OxyRateOut;
Servo HeartRateOut;

#define HEARTPIN 9
#define OXYPIN 10
#define OUT_MAX 165
#define OUT_MIN 15

   
int heartRateNumberRaw = -1;
int oxygenRateNumberRaw = -1;
                       


void setup()
{
    pinMode(rtsPin, INPUT);
    pinMode(ctsPin, OUTPUT);
    pinMode(extra5VPin, OUTPUT);
    digitalWrite(extra5VPin, HIGH);
    
    OxyRateOut.attach(OXYPIN);
    HeartRateOut.attach(HEARTPIN);
  
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }
 
 
    //    Serial.println("Goodnight moon!");
 
    // set the data rate for the SoftwareSerial port
    mySerial.begin(9600);
    mySerial.println("Hello, world?");
    
    handshake();
  }
 
void loop() // run over and over
{

    
    //handshake();
  
    //only run code if handshake worked...
   // if(ctsVal == 1){
      //Serial.println ("Inside ctsLoop\t");
          /*
           
          1. Get digital signals
          2. Analyze 1 at a time
          3. Convert each byte to hex (print, dec) println(x, hex)
          4. make a 2d array with each row having 6 bytes
          5. orrrrrr if u get 255 then a 180, store the following byte. have 3 boolean and 3 numeracl to track the information 
          6. Make the 99s and 100s into one int for oxy and pulse.
          7. store this as final info for oxy and pulse, at this point 
          8. nancy, if it hits a point make it buzz
          8. give the information to sally on two seperate wires (pins)
           
           */
          
          //for loop to examine each incoming byte
          
          if(mySerial.available()>MAX_BUFFER){
           flush();
          }
          //    if (mySerial.available()){
            //  Serial.print("buffer\t");
              //Serial.print(mySerial.available());

                
                  Serial.print ("\tMS: \t");

                  x = mySerial.read();
                  //try to do 
                  Serial.print(x); //Serial.print(x,DEC);


    
                  //find & track the index
                  if(x==255) {
                    index=5;
                    Serial.print("\t255 found");
                    }
                  if (index==6 && x==180){ index = 0; }
                  index++;
                      
                  //keep track of the index
                  if(index==2){   id=x;   
                  Serial.print("\tindex = 2");
                    }
                  
                  if(index==3){
                    Serial.print("\tindex = 3");
 
                       if(id==244){
                        pulseHun = x;
                        Serial.print("\tfound hun (244)");
                    
                       }
                       else if(id==245){
                        pulseOne=x;
                        Serial.print("\tfound tens (245)");
                    
                       }
                       else if (id==246){
                        oxyRate=x;
                        endSequence=true;
                        Serial.print("\tfound oxy(246)");
                    
                       }
                  }

                  if (endSequence){
                        Serial.println();
                        Serial.println("\t---------StartEndSequenceCalcs---------");
                            //Do math and send info
                          pulseRate = (pulseHun *100)+ pulseOne;
                          oxyRate=oxyRate;
                    /*      
                          heartRateNumberRaw = map(pulseRate,0, 500, OUT_MIN, OUT_MAX);
                          oxygenRateNumberRaw = map(oxyRate,0, 100, OUT_MIN, OUT_MAX);
                          
                          OxyRateOut.write(oxygenRateNumberRaw);
                          HeartRateOut.write(heartRateNumberRaw);
                        */
                              float voltOxyRate = (((oxyRate*5) - 1.5)/2.015)+.05;
                              float voltPulseRate = ((pulseRate - 1.5)/2.015)+.05;
                              analogWrite(pulsePin,voltPulseRate);
                              analogWrite(oxyPin,voltOxyRate);
                              Serial.print("\t Pulse Rate: ");
                              Serial.print(pulseRate);
                              Serial.print("\t Oxygen Rate: ");
                              Serial.print(oxyRate);
                              Serial.println();
                              //Alert if heart Rate is too high or too low
                          if (pulseRate>=MAX_RATE || pulseRate <=MIN_RATE){
                              tone(alertAlarm, 100, 100);
                              Serial.println();
                              Serial.println("ALERT!!! CHECK HEART RATE");
                          }
                     
                        endSequence=false;
                   delay(END_DELAY);
                     flush();   
                 pinMode(12,OUTPUT);
                pinMode(12,INPUT);
                   }   //ends if for endsequence
/*
            OxyRateOut.write(oxygenRateNumberRaw);
            HeartRateOut.write(heartRateNumberRaw);
  */                            
            Serial.println();                       
      //    } //ends myserial if
   // }   //ends big cts if
    
    


   
}   //end loop




boolean handshake(){
    rtsVal = digitalRead(rtsPin);
   //Serial.print("rtsVal: "); Serial.print(rtsVal);
    
    if (rtsVal == HIGH){
      digitalWrite(ctsPin, HIGH);
     // Serial.print("\tctsVal: HIGH\t");
      //Serial.print("Success...\t");
      ctsVal = 1;
    } else {
      digitalWrite(ctsPin, LOW);
      //Serial.print("\tctsVal: LOW\t");
      //Serial.print("Fail...\t");
      ctsVal = 0;
    }

    
}



void flush2(){
  while(Serial.available()>0){
    Serial.read();
  }
  delay(100);
  handshake();
}

void flush3(){
  mySerial.end();
  delay(100);
  mySerial.begin(9600);
  Serial.println();
  Serial.println("-------Flushed data-------");
}

void flush(){
  mySerial.flush();
}

"# Cardioxis" 
