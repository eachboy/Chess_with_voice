#define R_PIN 3
#define G_PIN 5
#define B_PIN 6

String data_t;
int data = 5;

void setup(){
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  if (Serial.available()) {
    data_t = Serial.readString();
    data = data_t.toInt();
  }

  if (data == 1) {
    analogWrite(R_PIN, 255);
    analogWrite(G_PIN, 0);
    analogWrite(B_PIN, 0);
  } else if (data == 2) {
    analogWrite(R_PIN, 0);
    analogWrite(G_PIN, 255);
    analogWrite(B_PIN, 0);
  } else if (data == 3) {
    analogWrite(R_PIN, 0);
    analogWrite(G_PIN, 0);
    analogWrite(B_PIN, 255);
  } else if (data == 4) {
    analogWrite(R_PIN, 255);
    analogWrite(G_PIN, 255);
    analogWrite(B_PIN, 0);
  } else if (data == 5) {
    analogWrite(R_PIN, 139);
    analogWrite(G_PIN, 0);
    analogWrite(B_PIN, 255);
  } else if (data == 6) {
    blink_red(5);
  }
}

void blink_red(int yw) {
    while (yw != 0) {
      analogWrite(R_PIN, 255);
      analogWrite(G_PIN, 0);
      analogWrite(B_PIN, 0);
      delay(500);
      analogWrite(R_PIN, 0);
      analogWrite(G_PIN, 0);
      analogWrite(B_PIN, 0);
      delay(500);
      yw = yw - 1;
    }
    data = 1;
}
