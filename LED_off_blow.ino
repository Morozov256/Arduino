//Extinguish the LED by blow
//On the idea Paul H. Dietz
//The main goal of improvements is noise immunity
//Applied white LED SMD 0603 and resistor 100 - 200 ohms
//LED connect: anode A1, catode A0
//Resistor connect: A0 and com

//The voltage on the diode depends on the temperature. 
//During operation, the LED heats up. 
//If you blow on it, it will cool slightly and the tension will change.
//The controller registers this change and turns off the LED for 2 seconds.

#define PLUS A1
#define DATA A0
#define SENS 100 //Set the sensitivity threshold

long int oldData=0; //Previous readings (reference level)
boolean dataReady=false; //Availability of raw data
int repOk=10; //The number of measurements to exceed the threshold (noise reduction)
int rep=0; //Current number of measurements

void setup() {
  pinMode(DATA, INPUT);
  pinMode(PLUS, OUTPUT);
  digitalWrite(PLUS, HIGH); //Turn on the LED
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  long int sum=0; //Data accumulation variable
  for(int i=0; i<256; i++) sum+=analogRead(DATA); //Accumulate measurements
  Serial.println(sum);
  if(dataReady && sum>(oldData+SENS)){ //Check on the deviation above the limit value
    rep++;
    if(rep==repOk){ //Check for the required number of overruns in a row
      digitalWrite(PLUS, LOW); //Turn off the LED
      dataReady=false; //Set the initial value
      delay(2000);
      digitalWrite(PLUS, HIGH); //Turn on the LED
      rep=0; //Reset the repetition counter
    }
  }
  else{
    oldData=sum; //Assign the current measurement result to the control level
    dataReady=true; //Show that the data is ready
    rep=0; //Reset the repetition counter
  }
}

