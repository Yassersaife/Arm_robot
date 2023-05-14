

//////////////////////////////////////////////
//        RemoteXY Arm Robot               //
//////////////////////////////////////////////

#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>
#include <Servo.h>  
#include <AccelStepper.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial1
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "Robot arm"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 224 bytes
   { 255,12,0,117,0,198,1,16,31,5,129,0,10,3,41,8,0,202,82,111,
  98,111,116,32,65,114,109,0,130,3,0,11,63,26,1,41,129,0,2,15,
  27,6,1,37,76,105,110,101,32,49,0,71,56,42,18,20,20,1,0,6,
  36,165,0,0,0,0,0,0,122,68,0,0,72,67,0,0,200,66,0,0,
  72,66,24,0,130,3,0,38,63,27,1,41,129,0,2,40,27,6,1,36,
  76,105,110,101,32,50,0,71,56,41,45,21,21,1,0,6,35,165,0,0,
  0,0,0,0,122,68,0,0,72,67,0,0,200,66,0,0,72,66,24,0,
  10,56,44,76,17,17,1,147,36,31,79,78,0,31,79,70,70,0,4,128,
  4,79,39,9,1,204,8,129,0,13,74,20,4,1,6,115,112,101,101,100,
  32,65,114,109,0,4,128,2,52,39,9,1,204,26,4,128,2,25,39,9,
  1,203,26,67,4,5,66,55,6,1,24,16,101,69,0,56,11,6,6,1,
  1,69,0,55,38,6,6,1,1,65,104,23,39,8,8,1,65,104,24,14,
  8,8,1,131,5,2,93,22,6,1,203,31,65,117,116,111,109,97,116,105,
  99,0,131,4,27,93,19,6,2,203,31,109,97,110,117,97,108,0,129,0,
  2,37,17,5,2,217,83,101,114,118,111,32,52,0,129,0,2,61,17,5,
  2,217,83,101,114,118,111,32,50,0,129,0,2,73,17,5,2,217,83,101,
  114,118,111,32,49,0,129,0,2,14,17,5,2,217,83,101,114,118,111,32,
  54,0,129,0,2,49,17,5,2,217,83,101,114,118,111,32,51,0,129,0,
  2,25,17,5,2,217,83,101,114,118,111,32,53,0,4,128,24,72,38,7,
  2,34,24,4,128,24,60,38,7,2,34,24,4,128,24,36,38,7,2,34,
  24,4,128,24,48,38,7,2,34,24,4,128,24,24,38,7,2,34,24,4,
  128,24,13,38,7,2,34,24,10,56,51,82,10,10,2,12,35,31,79,78,
  0,31,79,70,70,0,1,8,39,82,10,10,2,36,31,82,101,115,101,116,
  0 };
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t pushSwitch_1; // =1 if state is ON, else =0 
  int8_t slider_1; // =0..100 slider position 
  int8_t slider_2; // =0..100 slider position 
  int8_t slider_3; // =0..100 slider position 
  int8_t slider_s1; // =0..100 slider position 
  int8_t slider_s2; // =0..100 slider position 
  int8_t slider_s4; // =0..100 slider position 
  int8_t slider_s3; // =0..100 slider position 
  int8_t slider_s5; // =0..100 slider position 
  int8_t slider_s6; // =0..100 slider position 
  uint8_t pushSwitch_2; // =1 if state is ON, else =0 
  uint8_t button_1; // =1 if button pressed, else =0 

    // output variables
  float speed1;  // from 0 to 1000 
  float speed2;  // from 0 to 1000 
  char text_1[101];  // string UTF8 end zero 
  int16_t sound_1; // =0 no sound, else ID of sound, =1001 for example, look sound list in app 
  int16_t sound_2; // =0 no sound, else ID of sound, =1001 for example, look sound list in app 
  uint8_t led_2_r; // =0..255 LED Red brightness 
  uint8_t led_2_g; // =0..255 LED Green brightness 
  uint8_t led_1_r; // =0..255 LED Red brightness 
  uint8_t led_1_g; // =0..255 LED Green brightness 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////
AccelStepper stepper1(1, 2, 3); // (Type of driver: with 2 pins, STEP, DIR)
AccelStepper stepper2(1, 4, 5); // (Type of driver: with 2 pins, STEP, DIR)

Servo myservo1; 
Servo myservo2; 
Servo myservo3;
Servo myservo4; 
Servo myservo5; 
Servo myservo6;  

const int indexx=10;
int IRSensor = 6; // connect ir sensor to arduino pin 6
int IRSensor2 = 7; //connect ir sensor to arduino pin 7
int val = 0;

int servo1PPos, servo2PPos, servo3PPos, servo4PPos, servo5PPos, servo6PPos; // previous position
int servo1Pos, servo2Pos, servo3Pos, servo4Pos, servo5Pos, servo6Pos; // current position

//                    { 0    1    2     3    4     5     6      7      8     9    10     11     12 }
int servo01SP[indexx]={124 ,125  ,99   ,99  ,99  ,161  ,161   ,161  ,161  ,125};
int servo02SP[indexx]={27  ,25   ,25   ,1   ,20   ,20   ,20    ,15   ,25   ,25 };
int servo03SP[indexx]={59  ,60   ,70  ,70   ,60   ,60   ,90    ,90   ,60   ,60 };
int servo04SP[indexx]={109 ,110  ,110 ,110  ,110  ,110  ,110   ,110  ,110  ,110};
int servo05SP[indexx]={169 ,170  ,131 ,131  ,150  ,160  ,160   ,160  ,170  ,170};
int servo06SP[indexx]={100  ,120   ,120  ,20    ,20    ,20    ,20     ,120   ,120   , 120 };

int speedDelay = 30;


void setup() 
{
  RemoteXY_Init (); 
  RemoteXY.slider_3=30;
  RemoteXY.slider_2=30;
  RemoteXY.slider_1=50;
  RemoteXY.pushSwitch_1=0;
  RemoteXY.pushSwitch_2=0;

   myservo1.attach(8);
   myservo2.attach(9); 
   myservo3.attach(10); 
   myservo4.attach(11);
   myservo5.attach(12); 
   myservo6.attach(13); 

  Arm_robot(0,1);
  pinMode(IRSensor, INPUT); // sensor pin INPUT
  pinMode(IRSensor2, INPUT); // sensor pin INPUT
  stepper1.setMaxSpeed(1000);// Set maximum speed value for the stepper
  stepper2.setMaxSpeed(1000);// Set maximum speed value for the stepper
  Serial.begin(9600);
  servo1PPos = 125;
  servo2PPos = 25;
  servo3PPos = 60;
  servo4PPos = 110;
  servo5PPos = 170;
  servo6PPos = 50;

  delay(speedDelay);

}

void loop() 
{ 
  RemoteXY_Handler ();

if (RemoteXY.pushSwitch_1!=0) {
    /*  button pressed */
  
 int Sensordata = digitalRead(IRSensor);
 int Sensordata2 = digitalRead(IRSensor2);
 int SpeedS1=RemoteXY.slider_3 * 10;
 int SpeedS2=RemoteXY.slider_2 * -10;



   if((Sensordata == 1)&&(Sensordata2 == 1)){
   stepper1.setSpeed(SpeedS1);
   stepper1.runSpeed();
   stepper2.setSpeed(SpeedS2);
   stepper2.runSpeed();
   RemoteXY.led_1_g = 255;
   RemoteXY.led_2_g = 255;
   RemoteXY.led_1_r = 0;
   RemoteXY.led_2_r = 0;
   RemoteXY.speed1=SpeedS1;
   RemoteXY.speed2=SpeedS2;
   if(SpeedS2==0){
    RemoteXY.led_2_g = 0;
   RemoteXY.led_2_r = 255;
     }
 if(SpeedS1==0){
    RemoteXY.led_1_g = 0;
   RemoteXY.led_1_r = 255;
             }

   } 

   if (Sensordata == 0){  
   stepper1.setSpeed(0);
   stepper1.runSpeed();
   RemoteXY.led_1_r = 255;
   RemoteXY.led_1_g = 255;
   RemoteXY.speed1=0;
   RemoteXY.sound_1 = 2012;

   }
   if (Sensordata2 == 0){  
   stepper2.setSpeed(0);
   stepper2.runSpeed();
   RemoteXY.led_2_r = 255;
   RemoteXY.led_2_g = 255;
   RemoteXY.speed2=0;
   RemoteXY.sound_2 = 2012;

   }
   if (Sensordata == 1){  
   stepper1.setSpeed(SpeedS1);
   stepper1.runSpeed();
   RemoteXY.led_1_g = 255;
   RemoteXY.led_1_r = 0;
   RemoteXY.speed1=SpeedS1;
   RemoteXY.sound_1 = 0;
    if(SpeedS1==0){
   RemoteXY.led_1_g = 0;
   RemoteXY.led_1_r = 255;
 }
 

   }
   if (Sensordata2 == 1){  
   stepper2.setSpeed(SpeedS2);
   stepper2.runSpeed();
   RemoteXY.led_2_g = 255;
   RemoteXY.led_2_r = 0;
   RemoteXY.speed2=SpeedS2;
   RemoteXY.sound_2 = 0;
   if(SpeedS2==0){
    RemoteXY.led_2_g = 0;
   RemoteXY.led_2_r = 255;
 }

   }
   if ((Sensordata==0)&&(Sensordata2==0)){
    RemoteXY.sound_2 = 2012;
    stepper1.setSpeed(0);
    stepper2.setSpeed(0);
    stepper1.runSpeed();
    stepper2.runSpeed();
    RemoteXY.speed1=0;
    RemoteXY.speed2=0;
    RemoteXY.led_1_r = 255;
    RemoteXY.led_2_r = 255;
    RemoteXY.led_1_g = 255;
    RemoteXY.led_2_g = 255;
    Arm_robot(1,indexx);
    RemoteXY.led_2_r = 0;
    stepper2.move(-450);
    stepper2.setAcceleration(200);
    RemoteXY.speed2=200;
    stepper2.runToPosition();
    char str[] = "done in";
    val++;
    sprintf (RemoteXY.text_1, "%s %d items", str, val); 

   }
}
   else if(RemoteXY.pushSwitch_2!=0){
     if (RemoteXY.button_1!=0) {
    /*  button pressed */
    RemoteXY.slider_s1=73;
    RemoteXY.slider_s2=25;
    RemoteXY.slider_s3=70;
    RemoteXY.slider_s4=65;
    RemoteXY.slider_s5=97;
    RemoteXY.slider_s6=60;
  }
  

      servo1Pos = RemoteXY.slider_s1 *1.7; 
      if (servo1PPos > servo1Pos) {
        for ( int j = servo1PPos; j >= servo1Pos; j--) {   // Run servo down
          myservo1.write(j);
          delay(speedDelay);    // defines the speed at which the servo rotates
        }
      }
      // If previous position is smaller then current position
      if (servo1PPos < servo1Pos) {
        for ( int j = servo1PPos; j <= servo1Pos; j++) {   // Run servo up
          myservo1.write(j);
          delay(speedDelay);
        }
      }
      servo1PPos = servo1Pos;   // set current position as previous position
    

     servo2Pos = RemoteXY.slider_s2 ;

      if (servo2PPos > servo2Pos) {
        for ( int j = servo2PPos; j >= servo2Pos; j--) {
          myservo2.write(j);
          delay(speedDelay);
        }
      }
      if (servo2PPos < servo2Pos) {
        for ( int j = servo2PPos; j <= servo2Pos; j++) {
          myservo2.write(j);
          delay(speedDelay);
        }
      }
      servo2PPos = servo2Pos;
    

     servo3Pos = 120 - RemoteXY.slider_s3 ;
      if (servo3PPos > servo3Pos) {
        for ( int j = servo3PPos; j >= servo3Pos; j--) {
          myservo3.write(j);
          delay(speedDelay);
        }
      }
      if (servo3PPos < servo3Pos) {
        for ( int j = servo3PPos; j <= servo3Pos; j++) {
          myservo3.write(j);
          delay(speedDelay);
        }
      }
      servo3PPos = servo3Pos;
    

     servo4Pos = RemoteXY.slider_s4 *1.7 ;
      if (servo4PPos > servo4Pos) {
        for ( int j = servo4PPos; j >= servo4Pos; j--) {
          myservo4.write(j);
          delay(speedDelay);
        }
      }
      if (servo4PPos < servo4Pos) {
        for ( int j = servo4PPos; j <= servo4Pos; j++) {
          myservo4.write(j);
          delay(speedDelay);
        }
      }
      servo4PPos = servo4Pos;
    

     servo5Pos = RemoteXY.slider_s5 *1.75 ;
      if (servo5PPos > servo5Pos) {
        for ( int j = servo5PPos; j >= servo5Pos; j--) {
          myservo5.write(j);
          delay(speedDelay);
        }
      }
      if (servo5PPos < servo5Pos) {
        for ( int j = servo5PPos; j <= servo5Pos; j++) {
          myservo5.write(j);
          delay(speedDelay);
        }
      }
      servo5PPos = servo5Pos;
    

     servo6Pos = RemoteXY.slider_s6 ;
      if (servo6PPos > servo6Pos) {
        for ( int j = servo6PPos; j >= servo6Pos; j--) {
          myservo6.write(j);
          delay(speedDelay);
        }
      }
      if (servo6PPos < servo6Pos) {
        for ( int j = servo6PPos; j <= servo6Pos; j++) {
          myservo6.write(j);
          delay(speedDelay);
        }
      }
      servo6PPos = servo6Pos; 
    

   }
  else {
    /*  button not pressed */
    stepper1.setSpeed(0);
    stepper2.setSpeed(0);
    stepper1.runSpeed();
    stepper2.runSpeed();
    Arm_robot(0,1);
    RemoteXY.speed1=0;
    RemoteXY.speed2=0;
    RemoteXY.led_1_r = 255;
    RemoteXY.led_2_r = 255;
    RemoteXY.led_1_g = 0;
    RemoteXY.led_2_g = 0;
    val=0;
    sprintf (RemoteXY.text_1,"the project is off");
   
    RemoteXY.slider_s1=77;
    RemoteXY.slider_s2=25;
    RemoteXY.slider_s3=70;
    RemoteXY.slider_s4=65;
    RemoteXY.slider_s5=97;
    RemoteXY.slider_s6=100;
  

  }


}

void Arm_robot(int begin, int end){
for (int i = begin; i < end-1; i++) {  // Run through all steps(index)
      
    int SpeedArm=100 - RemoteXY.slider_1;
    int SpeedS1=RemoteXY.slider_3 * 10;
    int Sensordata = digitalRead(IRSensor);



  if ((Sensordata == 0)&&(end>=3)){  
   stepper1.setSpeed(0);
   stepper1.runSpeed();
   RemoteXY.led_1_r = 255;
   RemoteXY.led_1_g = 255;
   RemoteXY.speed1=0;


   }
   if((Sensordata == 1)&&(end>=3)) {
   stepper1.setSpeed(SpeedS1);
   stepper1.runSpeed();
   RemoteXY.led_1_g = 255;
   RemoteXY.led_1_r = 0;
   RemoteXY.speed1=SpeedS1;
   RemoteXY.sound_1 = 0;
    if(SpeedS1==0){
   RemoteXY.led_1_g = 0;
   RemoteXY.led_1_r = 255;
   }
   }
   // Servo 1

       if (servo01SP[i] == servo01SP[i + 1]) {
               Serial.println(servo01SP[i]);

       }
      if (servo01SP[i] > servo01SP[i + 1]) {
        for ( int j = servo01SP[i]; j >= servo01SP[i + 1]; j--) {
          myservo1.write(j);
          delay(SpeedArm);
        }
              Serial.println(servo01SP[i]);

      }
      if (servo01SP[i] < servo01SP[i + 1]) {
        for ( int j = servo01SP[i]; j <= servo01SP[i + 1]; j++) {
          myservo1.write(j);
          delay(SpeedArm);
        }
              Serial.println(servo01SP[i]);

     }
    
// Servo 2
       if (servo02SP[i] == servo02SP[i + 1]) {
               Serial.println(servo02SP[i]);

       }
      if (servo02SP[i] > servo02SP[i + 1]) {
        for ( int j = servo02SP[i]; j >= servo02SP[i + 1]; j--) {
          myservo2.write(j);
          delay(SpeedArm);
        }
              Serial.println(servo02SP[i]);

      }
      if (servo02SP[i] < servo02SP[i + 1]) {
        for ( int j = servo02SP[i]; j <= servo02SP[i + 1]; j++) {
          myservo2.write(j);
          delay(SpeedArm);
        }
              Serial.println(servo02SP[i]);

     }
    

     // Servo 3
       if (servo03SP[i] == servo03SP[i + 1]) {
               Serial.println(servo03SP[i]);

       }
      if (servo03SP[i] > servo03SP[i + 1]) {
        for ( int j = servo03SP[i]; j >= servo03SP[i + 1]; j--) {
          myservo3.write(j);
          delay(SpeedArm);
        }
              Serial.println(servo03SP[i]);

      }
      if (servo03SP[i] < servo03SP[i + 1]) {
        for ( int j = servo03SP[i]; j <= servo03SP[i + 1]; j++) {
          myservo3.write(j);
          delay(SpeedArm);
        }
              Serial.println(servo03SP[i]);

     }

     // Servo 4
       if (servo04SP[i] == servo04SP[i+1]) {
               Serial.println(servo04SP[i]);

       }
      if (servo04SP[i] > servo04SP[i + 1]) {
        for ( int j = servo04SP[i]; j >= servo04SP[i + 1]; j--) {
          myservo4.write(j);
          delay(SpeedArm);
        }
              Serial.println(servo04SP[i]);

      }
      if (servo04SP[i] < servo04SP[i + 1]) {
        for ( int j = servo04SP[i]; j <= servo04SP[i + 1]; j++) {
          myservo4.write(j);
          delay(SpeedArm);
        }
              Serial.println(servo04SP[i]);

     }
    
     // Servo 5
       if (servo05SP[i] == servo05SP[i + 1]) {
               Serial.println(servo05SP[i]);

       }
      if (servo05SP[i] > servo05SP[i + 1]) {
        for ( int j = servo05SP[i]; j >= servo05SP[i + 1]; j--) {
          myservo5.write(j);
          delay(SpeedArm);
        }
                       Serial.println(servo05SP[i]);

      }
      if (servo05SP[i] < servo05SP[i + 1]) {
        for ( int j = servo05SP[i]; j <= servo05SP[i + 1]; j++) {
          myservo5.write(j);
          delay(SpeedArm);
        }
                       Serial.println(servo05SP[i]);

     }
    

     // Servo 6
       if (servo06SP[i] == servo06SP[i + 1]) {
                        Serial.println(servo06SP[i]);

       }
      if (servo06SP[i] > servo06SP[i + 1]) {
        for ( int j = servo06SP[i]; j >= servo06SP[i + 1]; j--) {
          myservo6.write(j);
          delay(SpeedArm/2);
        }
                                Serial.println(servo06SP[i]);

      }
      if (servo06SP[i] < servo06SP[i + 1]) {
        for ( int j = servo06SP[i]; j <= servo06SP[i + 1]; j++) {
          myservo6.write(j);
          delay(SpeedArm/2);
        }
                 Serial.println(servo06SP[i]);

     }
            Serial.print("...................");
            Serial.println(i);
     
  }
}
