

// asier@artecodigo.pt 24


// OLED --------------------------------------------------------------------- //

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET     4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);


// Servos ------------------------------------------------------------------- //

#include <Servo.h>

#define SERVO_1 9
#define SERVO_2 6
#define SERVO_3 5

Servo servo1;
Servo servo2;
Servo servo3;

// values to write to servo
int dataServo1 = 90;
int dataServo2 = 90;
int dataServo3 = 90;

// servo position targets
int dstServo1 = 90;
int dstServo2 = 90;
int dstServo3 = 90;

// for controlling motion speed
int dstTime = 100; // frame s
unsigned long long ttime = 0;

// for changing modes
unsigned long long starttime = 0;
unsigned long long sectiondur = 3000;

enum {
   IDLE,
   IDLEPOSE,
   SWEEPCLEAN,
   AS_0,
   AS_1,
} MODES;
int num_modes = 5;

int idletime = 0;
int mode = IDLEPOSE;


// -------------------------------------------------------------------------- //

void setup(){

   Serial.begin(9600);

   // init servos
   servo1.attach(SERVO_1);
   servo2.attach(SERVO_2);
   servo3.attach(SERVO_3);
   writeServos(dataServo1, dataServo2, dataServo3);

   // init OLED
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
   display.setTextColor(WHITE);
   display.clearDisplay();

}


void loop(){

   ttime++;

   switch(mode) {
      case IDLE:
         break;
      case IDLEPOSE:
         do_IDLEPOSE();
         break;
      case SWEEPCLEAN:
         break;
   }

   updateDsts();
   updateOLED();
   updateMode();
   // Serial.println((int)ttime);

   delay(30);
}


// utilities ---------------------------------------------------------------- //

void writeServos(int x, int y, int z) {

   servo1.write(x);
   servo2.write(y);
   servo3.write(z);

   // Serial.print("wrote ");Serial.print((int)ttime);Serial.print(" servos : ");
   // Serial.print(x);
   // Serial.print("  ");Serial.print(y);
   // Serial.print("  ");Serial.print(z);
   // Serial.println("  ");

}


// low pass filters for smooth motion
void updateDsts() {

   int f = 10;
   bool wr = false;
   int md = 2; // mindist equal

   if (dstServo1 != dataServo1 ) {
      dataServo1 += (dstServo1 - dataServo1) / f;
      wr = true;
      if (abs(dstServo1 - dataServo1 ) < md) dataServo1 = dstServo1;
   }

   if (dstServo2 != dataServo2 ) {
      dataServo2 += (dstServo2 - dataServo2) / f;
      wr = true;
      if (abs(dstServo2 - dataServo2 ) < md) dataServo2 = dstServo2;
   }

   if (dstServo3 != dataServo3 ) {
      dataServo3 += (dstServo3 - dataServo3) / f;
      if (abs(dstServo3 - dataServo3 ) < md) dataServo3 = dstServo3;
      wr = true;
   }

   // Serial.println((int)ttime);

   if(wr){
      writeServos(dataServo1, dataServo2, dataServo3);
   }
}


// Modes -------------------------------------------------------------------- //

void updateMode() {
   if(millis() - starttime > sectiondur) {
      int pmode = mode;
      while (mode == pmode) {
         mode = random(0,num_modes);
      }
      Serial.print("changing to mode: ");
      Serial.println(mode);
      starttime = millis();
   }
}


void do_IDLEPOSE(){

   if (ttime % 60 == 0) {

   idletime++;

   dstServo1 = random(60, 80);
   dstServo2 = random(60, 80);
   dstServo3 = random(60, 80);

      if (idletime > 2) {
         idletime = 0;

         dstServo1 = random(40, 120);
         dstServo2 = random(30, 20);
         dstServo3 = random(20, 40);
      }
   }
}


void updateOLED() {
   display.clearDisplay();
   display.setTextSize(2);
   display.setCursor(0,0);
   display.println("CALCULAR");
   display.println("NA");
   display.println("AREIA");
   display.setCursor(110,40);
   display.setTextSize(3);
   display.print(mode);
   display.display();
}