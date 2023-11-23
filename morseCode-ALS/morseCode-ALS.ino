//#include <string>   -not included in Arduino IDE
typedef struct _tuple { 
  const char key; 
 String val; 
} Tuple;  // tuple with keys of type char and values of type String(coz later we need to iterate over the sensorStatus recv)

unsigned long endDuration = 5000;  // Time duration to end a letter [5 seconds]
unsigned long dotDuration = 600 ;  // Time duration for a dot(low signal) between 2 signals [0.6sec]
unsigned long dashDuration = 2000;  // Time duration for a dash(high signal) between 2 signals [2 sec]

int IRSensor = 9; // connect IR sensor module to Arduino pin D9
int LED = 13; // connect LED to Arduino pin 13

Tuple kvp[ ]= { { 'A',"01"},{'B',"1000"},{'C',"1010"},{'D', "100"},{'E', "0"}}; //defined only for 5 letters for now

String letter="";
  String sentence="";
  int timeArray[5]={0,0,0,0,0};
  int duration_check[5]={};
  int flag=0;
  int duration=0;
  int wordEnd=0; //to add a space after word is finished
  int sentEnd=0; //to add a PERIOD(dramatically) after a sentence is completed
  int OUT=0; //to get out of the prog as sntence is finished

char findKeyByValue(const String& value) {
  for (const auto& entry : kvp) {
    if (entry.val.equals(value)) {
      return entry.key;
    }
  }
  return '\0'; // Return null character if not found
}  //to find the alphabet given the morse code generated in the string letter


void setup(){
  Serial.begin(9600); // Init Serial at 9600 Baud Rate.
  Serial.println("Serial Working"); // Test to check if serial is working or not
  pinMode(IRSensor, INPUT); // IR Sensor pin INPUT
  pinMode(LED, OUTPUT); // LED Pin Output

  
  /*dict1['A'] = "01";
  dict1['B'] = "1000"
  dict1['C'] = "1010"
  dict1['D'] = "100"
  dict1['E'] = "0"
  dict1['F'] = "0010"
  dict1['G'] = "110"
  dict1['H'] = "0000"
  dict1['I'] = "00"
  dict1['J'] = "0111"
  dict1['K'] = "101"
  dict1['L'] = "0100"
  dict1['M'] = "11"
  dict1['N'] = "10"
  dict1['O'] = "111"
  dict1['P'] = "0110"
  dict1['Q'] = "1101"
  dict1['R'] = "010"
  dict1['S'] = "000"
  dict1['T'] = "1"
  dict1['U'] = "001"
  dict1['V'] = "0001"
  dict1['W'] = "011"
  dict1['X'] = "1001"
  dict1['Y'] = "1011"
  dict1['Z'] = "1100"*/

  
}



void loop(){
  
  //sensorStatus 1 : no obj in front


  while(OUT!=1){//run till sent doesn't end
  int sensorStatus = digitalRead(IRSensor); // Set the GPIO as Input

  if (sensorStatus == 1) // Check if the pin high or not
  {
    // if the pin is high turn off the onboard Led
    digitalWrite(LED, LOW); // LED LOW
    //Serial.println("No motion"); // print Motion Detected! on the serial monitor window
  }
  else  {
    //else turn on the onboard LED
    digitalWrite(LED, HIGH); // LED High
    //Serial.println("Motion Detected!"); // print Motion Ended! on the serial monitor window
    Serial.println("obj detected");
    if (timeArray[0]==0){
    Serial.println("inside first start time loop");

    unsigned long startTime = millis();
    timeArray[0]=startTime;
    }
    else{
      Serial.println("inside end time loop");
      unsigned long endTime = millis();
      duration=endTime-timeArray[0];
      Serial.println(duration);
      timeArray[0]=0;
      //checks condition if it is word end so that a space could be added in the sentence string
      if(wordEnd==1 && duration>endDuration){
         flag=1;
        wordEnd=0;
        
      }
      else{
       wordEnd=0;
      }

      //checks if a sentence end so that period can be added and the prog can be halted
      if(sentEnd==1 && duration>endDuration){
          flag=2;
          sentEnd=0;
      }
      else{
        sentEnd=0;
      }


      //actual condition checcking to decide if dash,dot or end
      if (duration>endDuration){
        Serial.println("greater than 5 sec detected");
        if (flag==0){
          //indicates a letter is finished transmitting
          char foundKey = findKeyByValue(letter);
          if (foundKey != '\0') {
            //Serial.print("Found key: ");
            //Serial.println(foundKey);
            sentence=sentence+foundKey;
            letter="";
          } else {
            Serial.println("Value not found in the tuple.");
          }



          wordEnd=1;
        }
        else if(flag==1){//word end
          sentence=sentence+" ";

          sentEnd=1;

        }
        else{//sent end
          sentence=sentence+".";
            OUT=1;//to end the while loop
        }
      }

      else if(duration>dashDuration && duration<endDuration){//dash
        Serial.println("dash detected");
        letter=letter+"1";
      }
      else{//dot
      Serial.println("dot detected");
        letter=letter+"0";
      }
      
    }



  }
  }
}
