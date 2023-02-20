#include <Servo.h>
#include <LiquidCrystal.h>

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
int coordinates;
int time_x;
int time_y;
int speedValue = 50;
int rotate_x;
int rotate_y;
String coord;

int current_hod = 0; // 1 - когда кушают фигуру, 0 - обычный ход

bool button_pos = true;
bool flag;

#define BUTTON_PIN  11

boolean buttonWasUp = true;  
boolean canmove = false; 

int hand_close = true;
int figure_in_hand = false;

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
  pinMode(BUTTON_PIN, INPUT_PULLUP);
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
  //if (button()) {      
    if (millis() - timer > 1000) {
      timer = millis();
      SEC = SEC - 1;
      lcd.setCursor(0, 0);

      if (MIN > -1) {
        if (SEC < 0) { 
          SEC = 59;
          MIN = MIN - 1;
        }

        if (SEC < 10) {
          lcd.print("Time left: 0" + String(MIN) + ":0" + String(SEC));
        } else {
          lcd.print("Time left: 0" + String(MIN) + ":" + String(SEC));
        } 
        Serial.println("ready");
        if (Serial.available()) {
          coord = Serial.readString();
          if (coord.startsWith("e")) {
            coord.remove(0,1);
            Serial.println(coord);
            if (time_y < 0){
              minus_movement(1);
            } else {
              movement(1);              
            }
           
          }
          if (time_y<0){
            minus_movement(0);
          } else{
             movement(0);
          }
         

          if (current_hod == 0) {
            lcd.clear();
            lcd.setCursor(0, 2);
            lcd.print("Last move: " + String(current_x) + String(current_y) + ">" + String(next_x) + String(next_y));        
            SEC = 0;
            MIN = 2;
          }
          Serial.println("next_turn");          // НЕ УДАЛЯТЬ - для переходна следующий ход - должно быть в конце хода
        }
      } else {
        flag = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Time is up,");
        lcd.setCursor(0, 2);
        lcd.print("game is over!");

        Serial.println("Time_up");
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
  // } else {
  //     start_pos();
  //     nowposition_x = 1;
  //     nowposition_y = 1;
  // }
}

int math(int value){                               //принимаем координаты с Raspberry void math
  //Serial.println("math");
  if (value == 0){ //x
    if (nowposition_x != current_x){  
      int time_x_coord = (to_figure(1)*340);
      Serial.println(String(time_x_coord/200)+"time_x_coord");       
      axisXControl(50);
      delay(abs(time_x_coord));
      axisXControl(0);
    }

    rotate_x = (next_x - current_x) / abs(next_x - current_x);
    save(0);
    return next_x - current_x;
  } else { //y
    if (nowposition_y != current_y) {
      int time_y_coord = (to_figure(2)*180);
      Serial.println(String(time_y_coord/200)+"time_y_coord");
      axisYControl(50);
      delay(abs(time_y_coord));
      axisYControl(0);
    }

    rotate_y = (next_y - current_y) / abs(next_y - current_y);
    save(1);
    return next_y - current_y;  
  } 
}

void save(int value){
  if (value == 0){
    nowposition_x = next_x;
  } else{
    nowposition_y = next_y;
  }
}

int to_figure(int value) {
  //Serial.println("to_figure");
  if (value == 1) {
    //Serial.println(current_x - nowposition_x);
    rotate_x = (current_x - nowposition_x) / abs(current_x - nowposition_x);
    Serial.println(String(current_x-nowposition_x) + "nowx"+String(nowposition_y)+String(current_y));
    return current_x - nowposition_x;
  } else {
    //Serial.println(current_y - nowposition_y);
    rotate_y = (current_y - nowposition_y) / abs(current_y - nowposition_y);
    Serial.println(String(current_y-nowposition_y) + "nowx" + String(nowposition_y)+String(current_y));
    return current_y - nowposition_y;
  }
}

bool button(){ 
  boolean buttonIsUp = digitalRead(BUTTON_PIN);//если передается значение 0 значит кнопка нажата
  if (buttonWasUp && !buttonIsUp) {// если кнопка нажата и до этого была разжата
    delay(10);//следовательно это не миссклик и кнопка реально нажата
    buttonIsUp = digitalRead(BUTTON_PIN);//подтверждаем нажатие кнопки
    if (!buttonIsUp) { 
      canmove = !canmove;//включаем или выключаем работу программы
      return(canmove);
    }
  }
  buttonWasUp = buttonIsUp;
}

int take(int value) {
  //Serial.println("take");
  if (value == 0){
    hand(50);
    motor_z1.write(map(speedValue,-100,100,1000,2000));//way - down
    motor_z2.write(map(speedValue,-100,100,1000,2000));
    delay(750);
    motor_z1.write(map(0,-100,100,1000,2000));
    motor_z2.write(map(0,-100,100,1000,2000));
    hand(-50);
    motor_z1.write(map(-speedValue,-100,100,1000,2000));//way - top
    motor_z2.write(map(-speedValue,-100,100,1000,2000));
    delay(910);
    motor_z1.write(map(0,-100,100,1000,2000));
    motor_z2.write(map(0,-100,100,1000,2000));
  } else {
    motor_z1.write(map(speedValue,-100,100,1000,2000));//way - down
    motor_z2.write(map(speedValue,-100,100,1000,2000));
    delay(750);
    motor_z1.write(map(0,-100,100,1000,2000));
    motor_z2.write(map(0,-100,100,1000,2000));
    hand(50);
    motor_z1.write(map(-speedValue,-100,100,1000,2000));//way - top
    motor_z2.write(map(-speedValue,-100,100,1000,2000));
    delay(910);
    motor_z1.write(map(0,-100,100,1000,2000));
    motor_z2.write(map(0,-100,100,1000,2000));
    hand(-50);
  }
}

int movement(int value){
  Serial.println("minus");
  if (value == 0){
    coordinates = coord.toInt();
    current_x = coordinates / 1000;     //текущая координата фигуры по x
    next_x = coordinates /100 % 10;     //будущая координата фигуры по x
    current_y = coordinates / 10 % 10;  //текущая координата фигуры по y
    next_y = coordinates % 10;          //будущая координата фигуры по y
  
  } else {
    coordinates = coord.toInt();
    current_x = coordinates / 10000;    //текущая координата фигуры по x
    next_x = coordinates /1000 % 10;    //будущая координата фигуры по x
    current_y = coordinates / 100 % 10; //текущая координата фигуры по y
    next_y = coordinates % 100;         //будущая координата фигуры по y
  
  }
  time_x = (math(0)*320);
  time_y = (math(1)*165);
  Serial.println(String(time_x/200)+"time_x");
  Serial.println(String(time_y/200)+"time_y");        
  take(0); // поднимает фигуру

  axisXControl(50);
  delay(abs(time_x));
  axisXControl(0); 

  axisYControl(50);
  delay(abs(time_y));
  axisYControl(0); 

  take(1);// опускает фигуру  
  Serial.println(String(nowposition_x)+"x");
  Serial.println(String(nowposition_y)+"y");                              
}
int minus_movement(int value){
  if (value == 0){
    coordinates = coord.toInt();
    current_x = coordinates / 1000;     //текущая координата фигуры по x
    next_x = coordinates /100 % 10;     //будущая координата фигуры по x
    current_y = coordinates / 10 % 10;  //текущая координата фигуры по y
    next_y = coordinates % 10;          //будущая координата фигуры по y
  
  } else {
    coordinates = coord.toInt();
    current_x = coordinates / 10000;    //текущая координата фигуры по x
    next_x = coordinates /1000 % 10;    //будущая координата фигуры по x
    current_y = coordinates / 100 % 10; //текущая координата фигуры по y
    next_y = coordinates % 100;         //будущая координата фигуры по y
  
  }
  time_x = (math(0)*207);
  time_y = (math(1)*307);
  Serial.println(String(time_x/200)+"time_x");
  Serial.println(String(time_y/200)+"time_y");        
  take(0); // поднимает фигуру

  axisXControl(50);
  delay(abs(time_x));
  axisXControl(0); 

  axisYControl(50);
  delay(abs(time_y));
  axisYControl(0); 

  take(1);// опускает фигуру  
  Serial.println(String(nowposition_x)+"x");
  Serial.println(String(nowposition_y)+"y");                              
}

int hand(int value) {
  //Serial.println("hand");
  claw.write(map(value,-100,100,1000,2000));
  delay(90);
  claw.write(map(0,-100,100,1000,2000));
}

int start_pos(){
  int time_start_pos = (1 - nowposition_x) * 210;
  rotate_x = (1 - nowposition_x) / abs(1 - nowposition_x);
  axisXControl(50);
  delay(abs(time_start_pos));
  axisXControl(0);

  time_start_pos = (1 - nowposition_y) * 300;
  rotate_y = (1 - nowposition_y) / abs(1 - nowposition_y);
  axisYControl(50);
  delay(abs(time_start_pos));
  axisYControl(0);
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
