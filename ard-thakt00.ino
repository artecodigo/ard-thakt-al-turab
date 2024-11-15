

// asier@artecodigo.pt 24


#include <Servo.h>

Servo servo1;//create servo object to control a servo
Servo servo2;//create servo object to control a servo
Servo servo3;//create servo object to control a servo

int dataServo1 = 90; // Servo 1 rotation range(dataServo1=0~180)
int dataServo2 = 90; // Servo 2 rotation range(dataServo2=0~180) 
int dataServo3 = 90; // Servo 3 rotation range(dataServo3=0~180)

int dstServo1 = 90; // Servo 1 rotation range(dataServo1=0~180)
int dstServo2 = 90; // Servo 2 rotation range(dataServo2=0~180) 
int dstServo3 = 90; // Servo 3 rotation range(dataServo3=0~180)

int dstTime = 100; // frame s
unsigned long long ttime = 0;
unsigned long long starttime = 100;
unsigned long long sectiondur = 100;


enum {
    IDLE,
    IDLEPOSE,
    SWEEPCLEAN,
    AS_0,
    AS_1,


} MODES;


int mode = IDLEPOSE;


void setup(){

    Serial.begin(9600);

  servo1.attach(9);//attachs the servo1 on pin 9 to servo object
  servo2.attach(6);//attachs the servo2 on pin 6 to servo object
  servo3.attach(5);//attachs the servo3 on pin 5 to servo object

    writeServos(dataServo1, dataServo2, dataServo3);

}

void writeServos(int x, int y, int z){
  servo1.write(x);//goes to dataServo1 degrees 
  servo2.write(y);//goes to dataServo2 degrees 
  servo3.write(z);//goes to dataServo3 degrees 
 
  Serial.print("wrote ");Serial.print((int)ttime);Serial.print(" servos : ");
  Serial.print(x);
  Serial.print("  ");Serial.print(y);
  Serial.print("  ");Serial.print(z);
  Serial.println("  ");
}



void loop(){

    ttime++;

    switch(mode){

        case IDLE:
            break;

        case IDLEPOSE:  do_IDLEPOSE();
            break;

        case SWEEPCLEAN:
            break;



    }
    

    updateDsts();

    // Serial.println((int)ttime);

}


void updateDsts(){

    int f = 10;
    bool wr = false;
    int md = 2; // mindist equal

    if (dstServo1 != dataServo1 ){
        dataServo1 += (dstServo1 - dataServo1) / f;
        wr = true;

        if (abs(dstServo1 - dataServo1 ) < md) dataServo1 = dstServo1;
    }

    if (dstServo2 != dataServo2 ){
        dataServo2 += (dstServo2 - dataServo2) / f;
        wr = true;
        if (abs(dstServo2 - dataServo2 ) < md) dataServo2 = dstServo2;

    }

    if (dstServo3 != dataServo3 ){
        dataServo3 += (dstServo3 - dataServo3) / f;
        if (abs(dstServo3 - dataServo3 ) < md) dataServo3 = dstServo3;
        wr = true;
    }


    // Serial.println((int)ttime);

    if(wr){
            writeServos(dataServo1, dataServo2, dataServo3);
    }

}




int idletime = 0;

void do_IDLEPOSE(){

    if (ttime % 60 == 0){

        idletime++;

        dstServo1 = random(60, 80);
        dstServo2 = random(60, 80);
        dstServo3 = random(60, 80);

        if (idletime > 2){
            idletime = 0;

            dstServo1 = random(40, 120);
            dstServo2 = random(30, 20);
            dstServo3 = random(20, 40);
        }

    }




}