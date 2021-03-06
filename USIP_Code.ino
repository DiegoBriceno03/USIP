/*  Authors: Diego Briceno & Seamus Anderson
 *  Date:        6  July   2017
 *  Last Update: 11/1/2017
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
 *    Red:   B+
 *    Blue:  B-
 *  Motor Resolution: 1.8 deg/step
 */

     // Define pin functions
          //Push-button pins
#define BUT  12   // Pushbutton Logic input
     
          //Driver Pins
#define PUL   8   // Pulse signal for the stepper driver on pin 11.
#define DIR   9   // Direction signal for the stepper driver on pin 10.
#define OPTO 10   // Power supply for the stepper driver's opto-couplers on pin 9.
#define ENA  11   // Enable (HIGH) signal for the driver on pin 8.

          //Vibration Motor Pins

#define VIB0  5   // Signal output for vib motor
#define SWI   4   // Logic for the switch
#define CON   2   // Logic for the contact/limit switch

     // Global Variables
int  step_count  = 0;    // Counts the number of pulses provided to the stepper.
bool switch_hist = 0;    // History of the switch, 0 if un-switched, 1 if it has been switched



void setup(){

       // Beginning Serial Output
  Serial.begin(9600);
  
       // Configuring Input/Output pins
  pinMode(ENA,   OUTPUT);    
  pinMode(OPTO,  OUTPUT);    
  pinMode(DIR,   OUTPUT);    
  pinMode(PUL,   OUTPUT);    
  pinMode(VIB2,  OUTPUT);
  pinMode(BUTP,  OUTPUT);

  
  pinMode(SWI,   INPUT);
  pinMode(BUT,   INPUT_PULLUP); // Now this has an internal pull up resistor and (HIGH = OFF ; LOW = ON)
  pinMode(CON,   INPUT_PULLUP); // Same as before    

         // Starting Logic for each pin
  digitalWrite(VIB0,  LOW);   // Ensure the vibration motor is off. 
  digitalWrite(PUL,   LOW);   // Initialize the pulse signal to LOW. (Not moving)

  digitalWrite(ENA,  HIGH);  // Enable the stepper driver.
  delay(1);
  digitalWrite(OPTO, HIGH);  // Provide power to the stepper driver's opto-couplers.
  delay(1);
  digitalWrite(DIR,  HIGH);  // Set the stepper to rotate clockwise (viewed from the top).
  digitalWrite(BUTP, HIGH);  // Turn on Button's power supply

   
  delay(100);            }



     // Subroutines //
     //*************//

// Initial Rotation Subroutine//
//---------------------------------//
/*  Parameters
 *  ----------
 *  int steps:  number of steps to iterate (n_deg = 1.8 * n_steps)
 *  int rpm=10: revolutions per minute of the motor
 */
void SUB_ROTATE(int n_steps, int rpm=10){
  
  int   i = 0;
  float dtime = 3000 / (20 * rpm);    
   
  switch_hist = 1;
       // Iterate through steps
  for(i=0; i < n_steps; i++){ 
       if(digitalRead(SWI)==0){
            return;             }
       step_count++;
       digitalWrite(PUL, HIGH);
       delay(dtime); 
       digitalWrite(PUL, LOW);
       delay(dtime);        }   }



// Subroutine for iterating motor by 5.4 deg //
//-----------------------------------------//
/*  Parameters
 *  ----------
 *  int n_steps =  3: number of steps to move through
 *  int rmp     = 10: revolutions per minute of the motor 
 */
void SUB_ITERATE(int n_steps=3, int rpm=10){
  
  int   i = 0;
  float dtime = 3000 / (20 * rpm);       
  
  for(i=0; i < n_steps ; i++){          // Iterate 5.4 deg or 3 steps
       step_count++;
       digitalWrite(PUL, HIGH);
       delay(dtime);                    // wait half a second
       digitalWrite(PUL, LOW);
       delay(dtime);            }    }



// Reseting to Inital Position Subroutine //
//--------------------------------------------//
/*  Parameters
 *  ----------
 *  int rpm=10:   revolutions per minute
 */

void SUB_RESET(int rpm=10){

  float dtime = 3000 / (20 * rpm);

       // Reverse direction of the motor to counter-clockwise
  digitalWrite(DIR, LOW);
  delay(1);

       // Rotate counter-clockwise until box hits limit switch
  while(digitalRead(CON)!= 1){  
       digitalWrite(PUL, HIGH);
       delay(dtime); 
       digitalWrite(PUL, LOW);
       delay(dtime);           }

       // Reset step counter and switch history and direction of motor
  step_count  = 0;    
  switch_hist = 0;
  digitalWrite(DIR, HIGH);     }

 



// Main Function Loop //
//--------------------//

void loop(){
  /* Notes!!!
  respective loop.So I am using this space as place for updates!\
  -This code will make the stepper move 45 degrees, delay a second. move another 5 degrees and then go back to inital position.\
  -The rpm calculation was based on the 1.8 resolution of the stepper, so now we can just type how fast we want it\
   instead of guessing the delay or playing with the dip swithes.\
  -Function Definitions are at the bottom in case you want to see how it works or even add functionality,\
  -we could maybe add the switch code into this loop if needed
  
  -Seamus Update (8.16.17)- I reorganized some of the funcitons and cut down down on the parameters
  -Seamus Update (8.31.17)- Added some delays for the push button and some of the driver logics  
  */
       // Turn on vibration motor
  digitalWrite(VIB0, HIGH);
  
       // Reading Switch and Button Logic
  int button_logic = digitalRead(BUT);  // Both of these logics should be '0' or '1'
  int switch_logic = digitalRead(SWI);

       // Printing user inputs and number of steps
  Serial.println("Button Logic:");
  Serial.println(button_logic);
  Serial.print("Switch Logic: ");
  Serial.println(switch_logic);
  Serial.println("Steps: ");
  Serial.println(step_count);
  Serial.print("----------------- \n");


       // Initial Rotation event (x * 1.8 degrees, x steps)
       // if switch is on, but hasn't been turned on in the past
  if((switch_hist==0) && (switch_logic==1)){
       SUB_RESET();
       SUB_ROTATE(25);
       delay(500);       }  // Rotate 25 steps or 45 deg, entered parameter is number of steps (n_steps)   
    
       // Button Iterating event (5.4 degrees, 3 steps)
       // if push button is being pressed
  if(button_logic == 0){
       SUB_ITERATE();   
       delay(500);       }

       // Reseting event 
       // if switch is off, but has been turned on in the past
  if((switch_hist == 1) && (switch_logic==0)){
       SUB_RESET();
       delay(500);       }
  }
