/*  Author: Seamus Anderson, Diego Briceno, Jeff Strange Jr.
 *  Date: 2017-11-27
 *  Purpose: 
 *    Test the integration of various experimental apparatus components for the
 *    SLOPE Undergraduate Student Instrumentation Project from 
 *    UCF's Center for Microgravity Research.
 *    Components include a DM422 stepper driver, a 17HS08-1004S stepper motor,
 *    a KWB-R8445W/1W LED backlight, and a 1201 vibrating motor.
 *  References:
 *  - DM422 Stepper Driver User's Manual (http://www.leadshine.com/UploadFile/Down/DM422m.pdf)
 *  - 17HS08-1004S Stepper Motor Datasheet (https://www.omc-st  epperonline.com/download/17HS08-1004S.pdf)
 *  - KWB-R8445W/1W White LED Backlight Datasheet (https://cdn-shop.adafruit.com/datasheets/KWB-R8445W-1W.pdf)
 *  - 1201 Vibrating Motor Product Page (https://www.adafruit.com/product/1201)
 *  
 *  Wiring for the motor:
 *    Black: A+
 *    Green: A-
 *    Red: B+
 *    Blue: B-
 */

// Assign functions to pins:
#define PUL 11  // Pulse signal for the stepper driver on pin 11.
#define DIR 10  // Direction signal for the stepper driver on pin 10.
#define OPTO 9  // Power supply for the stepper driver's opto-couplers on pin 9.
#define ENA 8   // Enable (HIGH) signal for the driver on pin 8.
#define LED 7   // Power supply for the LED backlight on pin 4.
#define VIB 5   // Power supply for the vibration motor on pin 2.
#define BUT 12  // Pushbutton
volatile int stepCount = 0;  // Counts the number of pulses provided to the stepper.
volatile int numberPressed = 0; // amount of times the button has been pressed


void setup()
{
Serial.begin(9600);
  pinMode(ENA, OUTPUT);     // Configure the ENA pin as an output.
  digitalWrite(ENA, HIGH);  // Enable the stepper driver.

  pinMode(OPTO, OUTPUT);    // Configure the OPTO pin as an output.
  digitalWrite(OPTO, HIGH); // Provide power to the stepper driver's opto-couplers.

  pinMode(DIR, OUTPUT);     // Configure the DIR pin as an output.
  digitalWrite(DIR, HIGH);  // Set the stepper to rotate clockwise (viewed from the top).

  pinMode(PUL, OUTPUT);     // Configure the PUL pin as an output.
  digitalWrite(PUL, LOW);   // Initialize the pulse signal to LOW.

  pinMode(LED, OUTPUT);     // Configure the LED pin as an output.
  digitalWrite(LED, HIGH);  // Provide power to the LED.

  pinMode(VIB, OUTPUT);     // Configure the LED pin as an output.
  digitalWrite(VIB, LOW);   // Ensure the vibration motor is off.  \

  pinMode(BUT,INPUT);
  delay(500);

  // Code added to set up so it only runs once
  initialStep(10); // This will turn the stepper 45 degrees at 6 rpm
  delay(1000); // delay to see it working
  
  
  
}

void loop(){
  /* Notes!!!*/
  //This loop is not used now because there is no code that will run more than once, each function has its own\
  respective loop.So I am using this space as place for updates!\
  -This code will make the stepper move 45 degrees, delay a second. move another 5 degrees and then go back to inital position.\
  -The rpm calculation was based on the 1.8 resolution of the stepper, so now we can just type how fast we want it\
   instead of guessing the delay or playing with the dip swithes.\
  -Function Definitions are at the bottom in case you want to see how it works or even add functionality,\
  -we could maybe add the switch code into this loop if needed
   volatile int button = 1;
  
  button = digitalRead(BUT);
  
  Serial.println(button);
  
 
 
  if(button == 1)
  {
    fiveSteps(10); // This will turn the stepper 5 degrees at 6 rpm
    numberPressed++;
    Serial.println(numberPressed);
  }
  
  if( numberPressed == 10)
  {
    restartStep(40,stepCount); // This function will restart to original position at a given rpm 
    numberPressed = 0; // amount of times the button has been pressed
  }
    digitalWrite(LED, HIGH);
   
}

/* Function Definitions! */
// restart to inital position
void restartStep(unsigned long rpm, int counterp){
  digitalWrite(DIR,LOW);
  unsigned long dtime = 0;
  int steps;
  dtime = 3000/(20*rpm); // rpm higher than 150 won't work since it will read the number as a 0 value
   for(steps=0; steps< counterp;steps++) // go back to initial condition
   {
    digitalWrite(PUL, HIGH);
    delay(dtime); 
    digitalWrite(PUL, LOW);
    delay(dtime);
   }
   digitalWrite(DIR,HIGH);
   stepCount = 0;
}

//initial 45 Degrees
void initialStep(unsigned long rpm){
  unsigned long dtime = 0;
  int steps;
  dtime = 3000/(20*rpm);
  digitalWrite(VIB, HIGH);
   for(steps=0;steps<=50;steps++) // 45/1.8 gives 25 steps
   {
    stepCount++;
    digitalWrite(PUL, HIGH);
    delay(dtime); 
    digitalWrite(PUL, LOW);
    delay(dtime);
   }
  digitalWrite(VIB,LOW);
}

// Optional 5 degrees
void fiveSteps(unsigned long rpm){
  unsigned long dtime = 0;
  int steps;
  dtime = 3000/(20*rpm);
  if(stepCount <= 100){
   digitalWrite(VIB, HIGH);
   for(steps=0;steps<=3;steps++) // 5/1.8 gives 3 about steps
   {
    stepCount++;
    digitalWrite(PUL, HIGH);
    delay(dtime); //wait half a second
    digitalWrite(PUL, LOW);
    delay(dtime);
    }
   digitalWrite(VIB, LOW);
  }
  else
  {
    restartStep(1,stepCount);
  }
}
