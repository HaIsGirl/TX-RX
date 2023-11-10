/*****************************/
// CHUONG TRINH DIEU KHIEN ROBOTICS THPT NGUYEN DU  //
// Create by Le Quyen                               //
// Date: 08/11/2023                                 //
/*****************************/
#include <Wire.h>
#include <gamepad.h>
#include <Servo.h>

Gamepad_Receiver gamepad;

// Định nghĩa chân nối các Servo
#define SERVO1_PIN 8   //Chân servo điều khiển cánh tay - nối 10 Shield V5.0 
#define SERVO2_PIN 9  //Chân servo điều khiển tay kẹp - nối 11 Shield V5.0         
#define MAX_SPEED 235 // tốc độ lớn nhất DC  motors

// Nối chân L298N với shield V5.0
#define enAPin 6 // (V5) chân 6 - ENA (L298N)
#define in1Pin 7 // (V5) chân 7 - IN1 (L298N)
#define in2Pin 5 // (V5) chân 5 - IN2 (L298N)
#define in3Pin 4 // (V5) chân 4 - IN3 (L298N)
#define in4Pin 2 // (V5) chân 2 - IN4 (L298N)
#define enBPin 3 // (V5) chân 3 - ENB (L298N)
        
Servo myservo1;     //Servo điều khiển cánh tay
//Servo myservo2;     //Servo điều khiển tay kẹp

int speedCar    = 150;     // toc do 50 - 255.
int speed_Coeff = 2;       // he so suy giam <=> giam may lan toc do
int setservo1 = 0;
//int setservo2 = 0;
int speedJS = 0;

void setup() {
  //Dừng robot
  stopRobot();  
  Serial.println("Robotic THPT Nguyễn Du - Begin!");
  //Khoi tao Servo
  myservo1.attach(SERVO1_PIN);         //servo1 nối chân số 8 Shield V5.0
  //myservo1.write(setservo1);
  myservo1.write(90);

  //Khoi tao L298N
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);
  
  //Khoi tao PS2_OhStem
  Serial.begin(115200);
  gamepad.begin();
  


}

void loop() {
  gamepad.update();   // hàm để cập nhật thông tin của gamepad
  if (!gamepad.isConnected) return; // hàm kiểm tra kết nối của gamepad
  else
    {
      //Joystick bên trái
    if (gamepad.aLx>=5 and gamepad.aLx<=512) {
      Serial.print("Joy trái qua phải ");
      speedJS = map(gamepad.aLx,5,512,50,250);
      Serial.print(speedJS); 
      RePhai_Line(speedJS,speedJS/speed_Coeff); 
      stopRobot();
      }
    if (gamepad.aLx<=-5 and gamepad.aLx>=-512) {
      Serial.print("Joy trái qua trái "); 
      speedJS = map(gamepad.aLx,-5,-512,50,250);
      Serial.print(speedJS); 
      ReTrai_Line(speedJS/speed_Coeff,speedJS); 
      stopRobot();
      }
    if (gamepad.aLy>=5 and gamepad.aLy<=512) {
      Serial.print("Joy trái xuống dưới "); 
      speedJS = map(gamepad.aLy,5,512,50,250);
      Serial.print(speedJS); 
      Lui_Line(speedJS,speedJS); 
      stopRobot();
      }
    if (gamepad.aLy<=-5 and gamepad.aLy>=-512) {
      Serial.print("Joy trái lên trên "); 
      speedJS = map(gamepad.aLy,-5,-512,50,250);
      Serial.print(speedJS); 
      Tien_Line(speedJS,speedJS); 
      stopRobot();
      }

    //Joystick bên trái
    if (gamepad.aRx>=5 and gamepad.aRx<=512) {
      Serial.print("Joy phải qua phải ");
      speedJS = map(gamepad.aRx,5,512,50,250);
      RePhai_Line(speedJS,speedJS/speed_Coeff); 
      stopRobot();
      }
    if (gamepad.aRx<=-5 and gamepad.aRx>=-512) { 
      Serial.print("Joy phải qua trái "); 
      speedJS = map(gamepad.aRx,-5,-512,50,250);
      ReTrai_Line(speedJS/speed_Coeff,speedJS); 
      stopRobot();
      }  
    if (gamepad.aRy>=5 and gamepad.aRy<=512) { 
      Serial.print("Joy phải xuống dưới "); 
      speedJS = map(gamepad.aRy,5,512,50,250);
      Lui_Line(speedJS,speedJS); 
      stopRobot();}
    if (gamepad.aRy<=-5 and gamepad.aRy>=-512) { 
      Serial.print("Joy phải lên trên ");
       speedJS = map(gamepad.aRy,-5,-512,50,250); 
       Tien_Line(speedJS,speedJS); 
       stopRobot();}

    //Các nút điều hướng bên trái
    if (gamepad.dpad_left!=0) {Serial.print("QUA TRÁI "); ReTrai(); stopRobot();}
    if (gamepad.dpad_right==1) {Serial.print("QUA PHẢI "); RePhai(); stopRobot();}
    if (gamepad.dpad_up==1) {Serial.print("LÊN TRÊN "); Tien(); stopRobot();}
    if (gamepad.dpad_down==1) {Serial.print("XUỐNG DƯỚI "); Lui(); stopRobot();}

    //Các nút điều hướng bên phải
    if (gamepad.a==1) {Serial.print("Nút X "); Ha();}
    //if (gamepad.b==1) {Serial.print("Nút hình tròn "); Serial.println(gamepad.b);}
    //if (gamepad.x==1) {Serial.print("Nút hình vuông "); Serial.println(gamepad.x);}
    if (gamepad.y==1) {Serial.print("Nút tam giác "); Nang();}

    }
 
 
}
   
void Tien(){ 
      digitalWrite(in1Pin,LOW);      
      digitalWrite(in2Pin,HIGH);         // LEFT tien
      digitalWrite(in3Pin,LOW);      
      digitalWrite(in4Pin,HIGH);         // RIGHT tien
      analogWrite(enAPin, speedCar);
      analogWrite(enBPin, speedCar);
      Serial.println("Tiến tới");
      delay(200);
}

void Lui(){ 
      digitalWrite(in1Pin,HIGH);      
      digitalWrite(in2Pin,LOW);         // LEFT lui
      digitalWrite(in3Pin,HIGH);      
      digitalWrite(in4Pin,LOW);         // RIGHT lui
      analogWrite(enAPin, speedCar);
      analogWrite(enBPin, speedCar);         
      Serial.println("Lùi lại");
      delay(200);
}

void RePhai(){ 
      digitalWrite(in1Pin,LOW);      
      digitalWrite(in2Pin,HIGH);         // LEFT tien
      digitalWrite(in3Pin,HIGH);      
      digitalWrite(in4Pin,LOW);          // RIGHT lui
      analogWrite(enAPin, speedCar);
      analogWrite(enBPin, speedCar);
      Serial.println("Rẽ phải");
      delay(200);
}

void ReTrai(){
      digitalWrite(in1Pin,HIGH);      
      digitalWrite(in2Pin,LOW);         // LEFT lui
      digitalWrite(in3Pin,LOW);      
      digitalWrite(in4Pin,HIGH);         // RIGHT tien
      analogWrite(enAPin, speedCar);
      analogWrite(enBPin, speedCar);
      Serial.println("Rẽ Trái");
      delay(200);
}


void stopRobot(){  
      digitalWrite(in1Pin,LOW);      
      digitalWrite(in2Pin,LOW);      
      digitalWrite(in3Pin,LOW);      
      digitalWrite(in4Pin,LOW);         
      analogWrite(enAPin, 0);
      analogWrite(enBPin, 0);
      Serial.println("STOP ROBOT");
}
 /* Đối với Servo MG 996R 360 độ thì:
   *  -Hàm gServo.write(pos) 
   *  + Với pos=90 servo đứng im
   *  + <90 thì Servo quay theo chiều kim đồng hồ với tốc độ tăng dần khi pos càng nhỏ
   *  + >90 thì Servo quay ngược chiều kim đồng hồ với tốc độ tăng dần khi pos càng lớn
   *  - Hàm Delay(), nếu giá trị càng lớn thì thời gian quay càng nhiều (tương ứng góc quay càng lớn)
   *  - Tốc độ càng chậm, thời gian càng nhanh: Đồng nghĩa với servo quay góc nhỏ
   *  Đối với Servo 180 độ thì:
   *  - Hàm gServo.write(pos) là quay servo đến góc pos.
   *  - Tốc độ quay phụ thuộc hàm delay()
   */

void Ha(){
  int tocdo = 80; //Theo chiều kim đồng hồ
  myservo1.write(tocdo);
  delay(200);
  myservo1.write(90);  //Dừng servo nếu không Servo xoay tròn
  Serial.println("Hạ");
}
void Nang(){
  int tocdo = 100; //Theo chiều kim đồng hồ
  myservo1.write(tocdo);
  delay(200);
  myservo1.write(90); //Dừng servo nếu không Servo xoay tròn
  Serial.println("Nâng");
  
} 
void ReTrai_Line(int speedL,int speedR){
  digitalWrite(in1Pin,HIGH);      
  digitalWrite(in2Pin,LOW);         // LEFT lui
  digitalWrite(in3Pin,LOW);      
  digitalWrite(in4Pin,HIGH);         // RIGHT tien
  analogWrite(enAPin, speedL);
  analogWrite(enBPin, speedR);
  Serial.println("Rẽ trái");
  delay(200);
}
void RePhai_Line(int speedL,int speedR){
  digitalWrite(in1Pin,LOW);      
  digitalWrite(in2Pin,HIGH);         // LEFT tien
  digitalWrite(in3Pin,HIGH);      
  digitalWrite(in4Pin,LOW);          // RIGHT lui
  analogWrite(enAPin, speedL);
  analogWrite(enBPin, speedR);
  Serial.println("Rẽ phải");
  delay(200);
}
void Tien_Line(int speedL,int speedR){
  digitalWrite(in1Pin,LOW);      
  digitalWrite(in2Pin,HIGH);         // LEFT tien
  digitalWrite(in3Pin,LOW);      
  digitalWrite(in4Pin,HIGH);         // RIGHT tien
  analogWrite(enAPin, speedL);
  analogWrite(enBPin, speedR);
  Serial.println("Tiến lên");
  delay(200);
}
void Lui_Line(int speedL,int speedR){
  digitalWrite(in1Pin,HIGH);      
  digitalWrite(in2Pin,LOW);         // LEFT lui
  digitalWrite(in3Pin,HIGH);      
  digitalWrite(in4Pin,LOW);         // RIGHT lui         
  analogWrite(enAPin, speedL);
  analogWrite(enBPin, speedR);
  Serial.println("Lùi lại");
  delay(200);
}

