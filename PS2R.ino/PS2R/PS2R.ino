/*****************************/
// CHUONG TRINH DIEU KHIEN ROBOTICS THPT NGUYEN DU
// Create by Le Quyen
// Date: 23/04/2021
/*****************************/

//Ket noi Bluetooth HC-06,HC-05 (neu HC-05 chi dung 4 pin: +5V,GND,TX,RX)
// Bluetooth HC-06/HC-05   <----> Arduino SENSor Shield V5.0 (4pin TX,RX,-,+)
//      RX                 -NOI-            TX
//      TX                 -NOI-            RX
//      GND                -NOI-            -
//      +5V                -NOI-            +

#include <Wire.h> //Includes I2C Lib 
#include <NewPing.h> 
#include <Servo.h>
#include <PS2X_lib.h>

#define PS2_DAT A3 // data
#define PS2_CMD A2 //command
#define PS2_SEL A1 // attention
#define PS2_CLK A0 //clock
#define SERVO_PIN 9   //Chân servo điều khiển cánh tay - nối 9 Shield V5.0         
#define MAX_DISTANCE 200 
#define MAX_SPEED 235 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20
        

Servo myservo;     //Servo điều khiển cánh tay
PS2X ps2x;
// Nối chân L298N với shield V5.0
const int enAPin = A5; // (V5) chân A5 - ENA (L298N)
const int in1Pin = 11; // (V5) chân 7 - IN1 (L298N)
const int in2Pin = 5; // (V5) chân 5 - IN2 (L298N)
const int in3Pin = 3; // (V5) chân 4 - IN3 (L298N)
const int in4Pin = 2; // (V5) chân 2 - IN4 (L298N)
const int enBPin = A4; // (V5) chân A4 - ENB (L298N)

int command;               //bien luu trang thai Bluetooth
int speedCar = 0;     // toc do 50 - 255.
int speed_Coeff = 4;       // he so suy giam <=> giam may lan toc do
int setservo1 = 0;
int error = 0; 
byte type = 0;
byte vibrate = 0;

void setup(){
    // Khởi tạo chân đầu ra và chân PWM cho động cơ và servo
  pinMode(enAPin,OUTPUT);
  pinMode(enBPin,OUTPUT);
  pinMode(in1Pin,OUTPUT);
  pinMode(in2Pin,OUTPUT);
  pinMode(in3Pin,OUTPUT);
  pinMode(in4Pin,OUTPUT);
  Serial.begin(9600); // Khởi tạo giao tiếp Serial với tốc độ 9600 bps
  myservo.attach(SERVO_PIN); // Kết nối đối tượng servo với chân SERVO_PIN

  // Khởi tạo bộ điều khiển PS2 và kiểm tra lỗi
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT,false, false);


  //  // Kiểm tra lỗi và loại bộ điều khiển PS2
  if (error == 0){
    Serial.println("Found Controller, configured successful");
  }
   
  else if (error == 1)
   Serial.println("No controller found, check wiring or reset the Arduino");
   
  else if (error == 2)
   Serial.println("Controller found but not accepting commands");
  
  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it.");
   
  // // Xác định loại bộ điều khiển PS2
  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }
} 

void loop(){
   // Đọc giá trị từ các cảm biến analog trên bộ điều khiển PS2
  int lx = ps2x.Analog(PSS_LX);
  int ly = ps2x.Analog(PSS_LY);
  int rx = ps2x.Analog(PSS_RX);
  int ry = ps2x.Analog(PSS_RY);

  // Tắt tất cả các động cơ trước khi xử lý lệnh mới
  digitalWrite(in1Pin,LOW);      
  digitalWrite(in2Pin,LOW);      
  digitalWrite(in3Pin,LOW);      
  digitalWrite(in4Pin,LOW);     

  // Khởi tạo tốc độ động cơ là 0.     
  analogWrite(enAPin, 0);
  analogWrite(enBPin, 0);       //Initialize with motors stopped.
  
  // Kiểm tra lỗi kết nối với bộ điều khiển PS2
  if (error == 1) 
    return;
  
  else {
    // Đọc dữ liệu từ bộ điều khiển PS2
    ps2x.read_gamepad(false, vibrate);
    //MOVE FORWARD
    if (ps2x.Button(PSB_PAD_UP) == 1){
      tienlen(255);
      delay(200);
      stopRobot();
    }
    //MOVE DOWN
    if(ps2x.Button(PSB_PAD_DOWN)){
      luixuong(255);
      delay(200);
      stopRobot();        
    }
    //TURN RIGHT
    if(ps2x.Button(PSB_PAD_RIGHT)){
      sangphai(126);
      delay(200);
      stopRobot();
    }
    // TURN LEFT
    if(ps2x.Button(PSB_PAD_LEFT)){
      sangtrai(126);
      delay(200);
      stopRobot();
    }
    //STICK UP
    if (ry < 118){
      speedCar = map(ry,0,118,100,255);
      tienlen(speedCar);
      delay(200);
      stopRobot();
    }
    //STICK DOWN
    if (ry > 138){
      speedCar = map(ry,138,255,100,255);
      luixuong(speedCar);   
      delay(200);
      stopRobot();
    }
    //STICK RIGHT
    if (rx > 128){
      speedCar = map(rx, 138,255,100,255);
      sangphai(speedCar);
      delay(200);
      stopRobot();    
    }
    //STCK LEFT
    if (rx < 128){
      speedCar = map(rx,0,128,100,255);
      sangtrai(speedCar);
      delay(200);
      stopRobot();
    }
    //NÂNG SERVO
    if (ps2x.ButtonPressed(PSB_BLUE) == 1) {
      nangservo();
      stopRobot();
      }
    //HẠ SERVO
    if (ps2x.ButtonPressed(PSB_GREEN)) {
      haservo();
      }
  }
}

void stopRobot(){
  digitalWrite(in1Pin,LOW);      
  digitalWrite(in2Pin,LOW);      
  digitalWrite(in3Pin,LOW);      
  digitalWrite(in4Pin,LOW);         
  analogWrite(enAPin, 0);       
  analogWrite(enBPin, 0);  
}

void tienlen(int speedCar){
  digitalWrite(in1Pin,LOW);
  digitalWrite(in2Pin,HIGH);
  digitalWrite(in3Pin,LOW);
  digitalWrite(in4Pin,HIGH);
  analogWrite(enAPin, speedCar);
  analogWrite(enBPin, speedCar);
}

void luixuong(int speedCar){
      digitalWrite(in1Pin,HIGH);      
      digitalWrite(in2Pin,LOW);         // LEFT lui
      digitalWrite(in3Pin,HIGH);                    
      digitalWrite(in4Pin,LOW);         // RIGHT lui
      analogWrite(enAPin, speedCar/2);    
      analogWrite(enBPin, speedCar/2);   
}

void sangtrai(int speedCar){
      digitalWrite(in1Pin,HIGH);      
      digitalWrite(in2Pin,LOW);         // LEFT lui
      digitalWrite(in3Pin,LOW);      
      digitalWrite(in4Pin,HIGH);         // RIGHT tien
      analogWrite(enAPin, speedCar/2);      
      analogWrite(enBPin, speedCar/2);
}

void sangphai(int speedCar){
      digitalWrite(in1Pin,LOW);      
      digitalWrite(in2Pin,HIGH);         // LEFT tien
      digitalWrite(in3Pin,HIGH);      
      digitalWrite(in4Pin,LOW);          // RIGHT lui
      analogWrite(enAPin, speedCar/2);
      analogWrite(enBPin, speedCar/2);
}

void nangservo(){
  myservo.attach(SERVO_PIN);
  int setservo = 140;
  setservo = map(setservo,180,0,225,0);
  myservo.write(setservo);
  delay(200);
  myservo.detach();
  delay(200);
}

void haservo(){
    myservo.attach(SERVO_PIN);  
      int setservo = 30;
      setservo = map(setservo,0,180,0,225);
      myservo.write(setservo);
      delay(200);
      myservo.detach();
      delay(200);
}



