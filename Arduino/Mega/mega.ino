#include <Servo.h>
#include <LiquidCrystal.h>
#include "chess.h"

giveTime Time;

const int rs = 27, en = 26, d4 = 25, d5 = 24, d6 = 23, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int SEC = 0;
int MIN = 2;
unsigned long timer;

int leftMotorx = 2;//первый мотор
int rightMotorx = 4;//второй мотор
int leftMotorx_1 = 5;//третий мотор 
int rightMotorx_1= 3;// четвертый мотор
int leftMotorY = 6;  
int rightMotorY = 7;
int clawmotor = 8;
int motor_zl = 9;
int motor_zr = 10;
int nowposition_x = 1;
int nowposition_y = 1;

int next_x;
int current_x;
int next_y;
int current_y;
int eat_current_x;
int eat_current_y;
int eat_next_x;
int eat_next_y;
long coordinates;
int time_x;
int time_y;
int time_x_coord;
int time_y_coord;
int speedValue = 40;
int rotate_x;
int rotate_y;
String coord;
String hod;
bool x;

int current_hod = 0; // 1 - когда кушают фигуру, 0 - обычный ход

bool button_pos = true;
bool flag;



#define button_control  11
#define button_speech  12
bool buttonWasUp = true;  
bool canmove = false; 
bool buttonWasUp1 = true;  
bool canmove1 = false; 

bool hand_close = true;
bool figure_in_hand = false;

Servo motor_x1;
Servo motor_x2;
Servo motor_x3;
Servo motor_x4;
Servo motor_y1;
Servo motor_y2;
Servo motor_z1;
Servo motor_z2;
Servo claw;

void setup() {
  pinMode(button_control, INPUT_PULLUP);
  pinMode(button_speech, INPUT_PULLUP);
  pinMode(leftMotorx,OUTPUT);
  pinMode(rightMotorx,OUTPUT);
  pinMode(leftMotorx_1,OUTPUT);
  pinMode(rightMotorx_1,OUTPUT);
  pinMode(leftMotorY,OUTPUT);
  pinMode(rightMotorY,OUTPUT);
  pinMode(clawmotor,OUTPUT);
  pinMode(motor_zl,OUTPUT);
  pinMode(motor_zr,OUTPUT);

  motor_x1.attach(leftMotorx);
  motor_x2.attach(rightMotorx);
  motor_x3.attach(leftMotorx_1);
  motor_x4.attach(rightMotorx_1);
  motor_y1.attach(leftMotorY);
  motor_y2.attach(rightMotorY);
  motor_z1.attach(motor_zl);
  motor_z2.attach(motor_zr);
  claw.attach(clawmotor);

  lcd.begin(16,2);
  timer = millis();
  Serial.begin(9600);
}

void loop() {
  if (button()) { 
   if (speech())
    {
      Serial.println("speech");
      canmove1 = 0;
    }   
    if (millis() - timer > 1000) {
      timer = millis();
      SEC = SEC - 1;
      lcd.setCursor(0, 0);

      if (MIN >= 0) {
        if (SEC < 0) { 
          SEC = 59;
          MIN = MIN - 1;
        }
        lcd.clear();
        if (SEC < 10) {
          lcd.print("Time left: 0" + String(MIN) + ":0" + String(SEC));
        } else {
          lcd.print("Time left: 0" + String(MIN) + ":" + String(SEC));
        } 
        //Serial.println(digitalRead(button_speech));
        if (Serial.available()) {          
          coord = Serial.readString();        
          if (coord.startsWith("e")) {
            x = coord[1];
            coord.remove(0,2);
            movement(1);
            start_pos();
          } else{
            movement(0);
            start_pos();
          }      
        if (current_hod == 0) {
            lcd.clear();
            lcd.setCursor(0, 2);
            hod = transl(current_x) + String(current_y) + ">" + transl(next_x) + String(next_y);
            Serial.println(hod);
            lcd.print("Last move: " + hod);        
            SEC = 0;
            MIN = 2;
          }
          Serial.println("next_turn");          // НЕ УДАЛЯТЬ - для переходна следующий ход - должно быть в конце хода
        }
      } else {
        flag = false;
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Time is up,");
        lcd.setCursor(0, 2);
        lcd.print("game is over!");

        Serial.println("Time_up");
        delay(100);
        exit(0);        
        movement(0);

        start_pos();
        nowposition_x = 1;
        nowposition_y = 1;
        while (flag == false) {
          if (button() == 1) {
            lcd.clear();
            MIN = 2;
            SEC = 0;
            flag = true;
          }
        } 
      }
    }
   } else {
      //Serial.println(canmove);
      // if (Serial.available()){
      //   movement(0);
      //   start_pos();        
      // }      
      MIN = 2;
      SEC = 0;
      lcd.setCursor(0, 0);
      lcd.print("Push button to start");
    }
    
}

int math(int value){                  
  //Serial.println("math");
  if (value == 0){ 
    if (nowposition_x != current_x){  
      //Serial.println(nowposition_x,current_x);      
      time_x_coord = (to_figure(0)*Time.forwardTime_x(current_x)); //тут нужен новый метод
      //Serial.println(time_x_coord);
      Serial.println(String(Time.forwardTime_x(current_x))+"time_x_coord");       
      axisXControl(30);
      delay(abs(time_x_coord));
      axisXControl(0);
    }

    rotate_x = (next_x - current_x) / abs(next_x - current_x);
    save(0);
    return next_x - current_x;

  } else if (value == 1){

    if (nowposition_y != current_y) {
      time_y_coord = (to_figure(1)*Time.forwardTime_y(current_y)); //тут нужен новый метод
      Serial.println(String(Time.forwardTime_y(current_y))+"time_y_coord");
      //Serial.println(time_y_coord); 
      axisYControl(30);
      delay(abs(time_y_coord));
      axisYControl(0);
    }

    rotate_y = (next_y - current_y) / abs(next_y - current_y);
    save(1);
    return next_y - current_y;  
    
  } else if (value == 2){
    
    if (nowposition_x != eat_current_x){  
      time_x_coord = (to_figure(2)*Time.forwardTime_x(eat_current_x)); //тут нужен новый метод
      
      //Serial.println(time_x_coord);
      Serial.println(String(Time.forwardTime_x(eat_current_x))+"etime_x_coord");       
      axisXControl(30);
      delay(abs(time_x_coord));
      axisXControl(0);
    }
    rotate_x = (eat_next_x - eat_current_x) / abs(eat_next_x - eat_current_x);
    save(2);
    return eat_next_x - eat_current_x; 

  } else if (value == 3){

    if (nowposition_y != eat_current_y) {
      time_y_coord = (to_figure(3)*Time.forwardTime_y(eat_current_y)); //тут нужен новый метод
      Serial.println(String(Time.forwardTime_y(eat_current_y))+"etime_y_coord");
      axisYControl(30);
      delay(abs(time_y_coord));
      axisYControl(0);
    }

    rotate_y = (eat_next_y - eat_current_y) / abs(eat_next_y - eat_current_y);
    save(3);
    return eat_next_y - eat_current_y;      
  }
}

void save(int value){
  if (value == 0){
    nowposition_x = next_x;
  } else if (value == 1){
    nowposition_y = next_y;
  } else if (value == 2){
    nowposition_x = eat_next_x;
  } else if (value == 3){
    nowposition_y = eat_next_y;
  }
}

int to_figure(int value) {
  //Serial.println("to_figure");
  if (value == 0) {
    //Serial.println(current_x - nowposition_x);
    rotate_x = (current_x - nowposition_x) / abs(current_x - nowposition_x);
    //Serial.println(String(current_x-nowposition_x) + "nowx"+String(nowposition_y)+String(current_y));
    //Serial.println(current_x - nowposition_x);
    return current_x - nowposition_x;
  } else if (value == 1) {
    //Serial.println(current_y - nowposition_y);
    rotate_y = (current_y - nowposition_y) / abs(current_y - nowposition_y);
    //Serial.println(String(current_y-nowposition_y) + "nowx" + String(nowposition_y)+String(current_y));
    return current_y - nowposition_y;
  } else if (value == 2){
    rotate_x = (eat_current_x - nowposition_x) / abs(eat_current_x - nowposition_x);
    //Serial.println(String(current_x-nowposition_x) + "nowx"+String(nowposition_y)+String(current_y));
    return eat_current_x - nowposition_x;    
  } else if (value == 3){
    //Serial.println(current_y - nowposition_y);
    rotate_y = (eat_current_y - nowposition_y) / abs(eat_current_y - nowposition_y);
    //Serial.println(String(current_y-nowposition_y) + "nowx" + String(nowposition_y)+String(current_y));
    return eat_current_y - nowposition_y;
  }
}


bool button(){ 
  bool buttonIsUp = digitalRead(button_control);//если передается значение 0 значит кнопка нажата
  if (buttonWasUp && !buttonIsUp) {// если кнопка нажата и до этого была разжата
    delay(10);//следовательно это не миссклик и кнопка реально нажата
    buttonIsUp = digitalRead(button_control);//подтверждаем нажатие кнопки
    if (!buttonIsUp) { 
      canmove = !canmove;//включаем или выключаем работу программы
    }
  }
  buttonWasUp = buttonIsUp;

  return(canmove);
 
}

int take(int value) {
  //Serial.println("take");
  if (value == 0){
    hand(60);
    motor_z1.write(map(-speedValue,-100,100,1000,2000));//way - down
    motor_z2.write(map(-speedValue,-100,100,1000,2000));
    delay(1500);
    motor_z1.write(map(0,-100,100,1000,2000));
    motor_z2.write(map(0,-100,100,1000,2000));
    hand(-60);
    delay(500);
    motor_z1.write(map(speedValue,-100,100,1000,2000));//way - top
    motor_z2.write(map(speedValue,-100,100,1000,2000));
    delay(1600);
    motor_z1.write(map(0,-100,100,1000,2000));
    motor_z2.write(map(0,-100,100,1000,2000));
  } else {
    motor_z1.write(map(-speedValue,-100,100,1000,2000));//way - down
    motor_z2.write(map(-speedValue,-100,100,1000,2000));
    delay(1300);
    motor_z1.write(map(0,-100,100,1000,2000));
    motor_z2.write(map(0,-100,100,1000,2000));
    delay(500);
    hand(60);
    motor_z1.write(map(speedValue,-100,100,1000,2000));//way - top
    motor_z2.write(map(speedValue,-100,100,1000,2000));
    delay(1600);
    motor_z1.write(map(0,-100,100,1000,2000));
    motor_z2.write(map(0,-100,100,1000,2000));
    hand(-60);
  }
}

void movement(int value){
  if (value == 0){
    coordinates = coord.toInt();
    current_x = coordinates / 1000;     //текущая координата фигуры по x
    next_x = coordinates /100 % 10;     //будущая координата фигуры по x
    current_y = coordinates / 10 % 10;  //текущая координата фигуры по y
    next_y = coordinates % 10;          //будущая координата фигуры по y
    time_x = (math(0)*380); 
    time_y = (math(1)*225);   
    take(0); // поднимает фигуру

    axisXControl(30);
    delay(abs(time_x));
    axisXControl(0); 

    delay(500);
    axisYControl(30);
    delay(abs(time_y));
    axisYControl(0); 

    take(1);// опускает фигуру 
  } else {
//    String s = String(coord[3]);
//    int ai = s.toInt();
    //Serial.println(coord.length());
    if (x == 0){
    coordinates = coord.toInt();
    eat_current_x = coordinates / long(pow(10,7));
    Serial.println(eat_current_x);
    eat_next_x = coordinates / long(pow(10,6)) % 10;
       Serial.println(eat_next_x);
    eat_current_y = coordinates /long(pow(10,5)) % 10;
       Serial.println(eat_current_y);
    eat_next_y = coordinates / int(pow(10,4)) % 10;
       Serial.println(eat_next_y);
    current_x = coordinates / int(pow(10,3)) % 10;    //текущая координата фигуры по x
       Serial.println(current_x);
    next_x = coordinates / int(pow(10,2)) % 10;    //будущая координата фигуры по x
    Serial.println(next_x);
    current_y = coordinates /int(pow(10,1)) % 10; //текущая координата фигуры по y
       Serial.println(current_y);
    next_y = coordinates % int(pow(10,1));         //будущая координата фигуры по y
    Serial.println(next_y);
    
    } else {
    coordinates = coord.toInt();
    eat_current_x = coordinates / long(pow(10,8));
    eat_next_x = coordinates / long(pow(10,7)) % 10;
    eat_current_y = coordinates /long(pow(10,6)) % 10;
    eat_next_y = coordinates / int(pow(10,4)) % 100;
    current_x = coordinates / int(pow(10,3)) % 10;    //текущая координата фигуры по x
    next_x = coordinates / int(pow(10,2)) % 10;    //будущая координата фигуры по x
    current_y = coordinates /int(pow(10,1)) % 10; //текущая координата фигуры по y
    next_y = coordinates % int(pow(10,1));         //будущая координата фигуры по y      
    Serial.println(coordinates);
    Serial.println("////////////////////");
    }
    time_x = (math(2)*375); 
    time_y = (math(3)*250);
    take(0); // поднимает фигуру
    Serial.println(time_x);
    axisXControl(30);
    delay(abs(time_x));
    axisXControl(0); 

    delay(500);
    axisYControl(30);
    delay(abs(time_y));
    axisYControl(0); 

    take(1);// опускает фигуру 
    
    delay(500);

    start_pos();   
    time_x = (math(0)*375); 
    time_y = (math(1)*225);
    take(0); // поднимает фигуру

    axisXControl(30);
    delay(abs(time_x));
    axisXControl(0); 

    delay(500);
    axisYControl(30);
    delay(abs(time_y));
    axisYControl(0); 

    take(1);// опускает фигуру 
  }
  //Serial.println(String(nowposition_x)+"x");
  //Serial.println(String(nowposition_y)+"y");                              
}

int hand(int value) {
  //Serial.println("hand");
  claw.write(map(value,-100,100,1000,2000));
  delay(90);
  claw.write(map(0,-100,100,1000,2000));
}

int start_pos(){
  int time_start_pos = (1 - nowposition_x) * Time.forwardTime_x(nowposition_x);
  Serial.println(1-nowposition_x);
  rotate_x = (1 - nowposition_x) / abs(1 - nowposition_x);
  axisXControl(30);
  delay(abs(time_start_pos)+200);
  axisXControl(0);
  nowposition_x = 1;

  delay(1000);

  time_start_pos = (1 - nowposition_y) * Time.forwardTime_y(nowposition_y);
  rotate_y = (1 - nowposition_y) / abs(1 - nowposition_y);
  axisYControl(30);
  delay(abs(time_start_pos)+300);
  axisYControl(0);
  nowposition_y = 1;
}

int axisXControl(int value){
  //Serial.println("axisXControl");
  motor_x1.write(map(-rotate_x * value,-100,100,1000,2000));
  motor_x3.write(map(rotate_x * value,-100,100,1000,2000));
  motor_x2.write(map(-rotate_x * value,-100,100,1000,2000));
  motor_x4.write(map(rotate_x * value,-100,100,1000,2000));
}

int axisYControl(int value){
  //Serial.println("axisYControl");
  motor_y1.write(map(-rotate_y * value,-100,100,1000,2000));
  motor_y2.write(map(rotate_y * value,-100,100,1000,2000));
}

int allMotorStop(int value){
  motor_x1.write(map(value,-100,100,1000,2000));
  motor_x2.write(map(value,-100,100,1000,2000));
  motor_x3.write(map(value,-100,100,1000,2000));
  motor_x4.write(map(value,-100,100,1000,2000));
  motor_y1.write(map(value,-100,100,1000,2000));
  motor_y2.write(map(value,-100,100,1000,2000));
  motor_z1.write(map(value,-100,100,1000,2000));
  claw.write(map(value,-100,100,1000,2000));
}

String transl(int value) {
  if (value == 1) {
    return "A";
  }
  if (value == 2) {
    return "B";
  }
  if (value == 3) {
    return "C";
  }
  if (value == 4) {
    return "D";
  }
  if (value == 5) {
    return "E";
  }
  if (value == 6) {
    return "F";
  }
  if (value == 7) {
    return "G";
  }
  if (value == 8) {
    return "H";
  }
}
//
bool speech(){ 
    //Serial.println("continue");
    bool buttonIsUp1 = digitalRead(button_speech);//если передается значение 0 значит кнопка нажата
    if (buttonWasUp1 && !buttonIsUp1) {// если кнопка нажата и до этого была разжата
      delay(10);//следовательно это не миссклик и кнопка реально нажата
      buttonIsUp1 = digitalRead(button_speech);//подтверждаем нажатие кнопки
      if (!buttonIsUp1) { 
        canmove1 = !canmove1;//включаем или выключаем работу программы
        
      }
    }
    buttonWasUp1 = buttonIsUp1;
//    bool canmove1 = digitalRead(button_speech);
//    Serial.println(canmove1);
    return(canmove1); 
}
