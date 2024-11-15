

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

// offsets to zero servos at vertical arm position
const int offset_1 = 60;
const int offset_2 = 115;
const int offset_3 = 90;

// regulate motion speed
const int main_frame_delay = 5;

// values to write to servo
float dataServo1, dataServo2, dataServo3;

// servo position targets
float dstServo1, dstServo2, dstServo3;

// for changing modes
unsigned long long starttime = 0;
unsigned long long sectiondur = 3000; // millis

// for changing mode actions
int dstTime = 1000; // millis
unsigned long long ttime = 0;

enum {
   IDLE,
   IDLEPOSE,
   SWEEPCLEAN,
   AS_0,
   AS_1,
} MODES;
int num_modes = 5;

int idletime = 0;
int mode = SWEEPCLEAN;// IDLEPOSE;


// -------------------------------------------------------------------------- //

void setup(){

   Serial.begin(115200);

   // init servos
   servo1.attach(SERVO_1);
   servo2.attach(SERVO_2);
   servo3.attach(SERVO_3);
   // writeServos(dataServo1, dataServo2, dataServo3);
   updateServos(0,0,0);
   delay(5000);

   // init OLED
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
   display.setTextColor(WHITE);
   display.clearDisplay();

}


void loop(){

   switch(mode) {
      case IDLE:
         break;
      case IDLEPOSE:
         do_IDLEPOSE();
         break;
      case SWEEPCLEAN:
         do_SWEEP();
         break;
   }

   updateDsts(main_frame_delay);
   updateOLED();
   //updateMode();
   // Serial.println((int)ttime);

}


// utilities ---------------------------------------------------------------- //

void writeServos(int x, int y, int z) {

   servo1.write(x);
   servo2.write(y);
   servo3.write(z);

   // Serial.print("wrote ");Serial.print((int)ttime);   (" servos : ");
   // Serial.print(x);
   // Serial.print("  ");Serial.print(y);
   // Serial.print("  ");Serial.print(z);
   // Serial.println("  ");

}

void updateServos(int a1, int a2, int a3) {
   servo1.write(-a1 + offset_1);
   servo2.write(180 - (-a2 + offset_2)); // servo 2 is inverted
   servo3.write(-a3 + offset_3);
}


// low pass filters for smooth motion
bool updateDsts(int frame_delay) {

   printPos();

   int f = 10;
   bool wr = false;
   int md = 2; // mindist equal

   // at target
   bool at1, at2, at3;
   at1 = at2 = at3 = false;

   if (dstServo1 != dataServo1 ) {
      dataServo1 += (dstServo1 - dataServo1) / f;
      wr = true;
      if (abs(dstServo1 - dataServo1 ) < md) {
         dataServo1 = dstServo1;
         at1 = true;
      }
   } else {
      at1 = true;
   }

   if (dstServo2 != dataServo2 ) {
      dataServo2 += (dstServo2 - dataServo2) / f;
      wr = true;
      if (abs(dstServo2 - dataServo2 ) < md) {
        dataServo2 = dstServo2;
        at2 = true;
      }
   } else {
      at2 = true;
   }

   if (dstServo3 != dataServo3 ) {
      dataServo3 += (dstServo3 - dataServo3) / f;
      if (abs(dstServo3 - dataServo3 ) < md) {
         dataServo3 = dstServo3;
         at3 = true;
      }
      wr = true;
   } else {
      at3 = true;
   }

   // Serial.println((int)ttime);

   if(wr){
      //writeServos(dataServo1, dataServo2, dataServo3);
      updateServos((int)dataServo1, (int)dataServo2, (int)dataServo3);
   }

   delay(frame_delay);

   if(at1 && at2 && at3) {
      // Serial.print("TRUE\t");
      // Serial.print(abs(dstServo1 - dataServo1 ));
      // Serial.print("\t");
      // Serial.print(abs(dstServo2 - dataServo2 ));
      // Serial.print("\t");
      // Serial.print(abs(dstServo3 - dataServo3 ));
      // Serial.print("\t");
      return true;
   } else {
      // Serial.print("FALSE\t");
      // Serial.print(abs(dstServo1 - dataServo1 ));
      // Serial.print("\t");
      // Serial.print(abs(dstServo2 - dataServo2 ));
      // Serial.print("\t");
      // Serial.print(abs(dstServo3 - dataServo3 ));
      // Serial.print("\t");
      return false;
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

void printPos() {
   Serial.print(dstServo1);
   Serial.print('\t');
   Serial.print(dstServo2);
   Serial.print('\t');
   Serial.print(dstServo3);
   Serial.print("\t:\t");
   Serial.print(dataServo1);
   Serial.print('\t');
   Serial.print(dataServo2);
   Serial.print('\t');
   Serial.print(dataServo3);
   Serial.println();
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

   if (millis() - ttime > dstTime) {

      ttime = millis();;
      idletime++;

      int a, b, c;

      if (idletime > 2) {
         idletime = 0;
         a = random(-40, 40);
         b = random(-40, 40);
         c = random(-40, 40);
         Serial.print("IDLE_B\t");
      } else {
         a = random(-60, 60);
         b = random(-60, 60);
         c = random(-60, 60);
         Serial.print("IDLE_A\t");
      }

      Serial.print(a);
      Serial.print('\t');
      Serial.print(b);
      Serial.print('\t');
      Serial.println(c);

      dstServo1 = a;
      dstServo2 = b;
      dstServo3 = c;
   }
}


void do_SWEEP() {
   int a2_min_pos = 30;
   int a3_min_pos = 90;
   int a2_max_pos = 70;
   int a3_max_pos = 40;

   int w = 50;
   int s = 50;

   int num_sweeps = 5;

   int a2d = (a2_max_pos - a2_min_pos) / num_sweeps;
   int a3d = (a3_max_pos - a3_min_pos) / num_sweeps;

   dstServo2 = a2_min_pos;
   dstServo3 = a3_min_pos;

   for (int i=0; i<num_sweeps; i++) {
      dstServo2 += a2d;
      dstServo3 += a3d;
      dstServo1 = -w;
      while(!updateDsts(s));
      dstServo1 = w;
      while(!updateDsts(s));
   }

   dstServo1 = 0;
   dstServo3 = -35;
   while(!updateDsts(s));

   dstServo2 = -20;
   dstServo3 = 90;
   while(!updateDsts(s));

   delay(3000);

}



