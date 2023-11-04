  //CODE NHẬN GIÁ TRỊ ĐỂ ĐIỀU KHIỂN XE BẰNG JOYSTICK THÔNG QUA MODULE nRF24L01

  // Thư viện cần thiết
  #include <SPI.h>
  #include <nRF24L01.h> 
  #include <RF24.h>

  // Định nghĩa các chân điều khiển động cơ 
  #define enA 9 // Chân Enable A
  #define in1 5 // Chân hướng quay động cơ 1
  #define in2 4 // Chân hướng quay động cơ 2  
  #define enB 10 // Chân Enable B
  #define in3 3 //Chân hướng quay động cơ 3  
  #define in4 2 //Chân hướng quay động cơ 4

  // Khởi tạo đối tượng radio
  RF24 radio(7, 13); // Chân CE, CSN

  // Địa chỉ cho giao tiếp radio
  const byte address[6] = "00001";

  // Biến lưu dữ liệu nhận được
  char receivedData[32] = "";   

  // Biến lưu giá trị trục joystick
  int  xAxis, yAxis;  
  int leftButton,rightButton;
  int topButton, bottomButton;
  // Biến lưu tốc độ động cơ
  int motorSpeedA = 0;  
  int motorSpeedB = 0;

  void setup() {

    // Đặt chế độ chân động cơ là OUTPUT
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    
    // Khởi động giao tiếp Serial
    Serial.begin(9600);
    //Kiểm tra giao tiếp radio
    if (!radio.begin()){
      Serial.write("Module không khởi động được...!");
      while (1){}
    }
    // Khởi động giao tiếp radio
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setChannel(67);
    radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate(RF24_250KBPS);
    radio.startListening();
    //Kiểm tra kết nối TX-RX
    if (!radio.available()){
      Serial.println("Chưa kết nối được với TX");
      Serial.println("CHỜ KẾT NỐI !");
    }
  }

  void loop() {

    // Kiểm tra có dữ liệu không
    if (radio.available()) {    
      
      // Đọc dữ liệu joystick
      radio.read(&receivedData, sizeof(receivedData));
      
      // Lưu giá trị trục X
      xAxis = atoi(&receivedData[0]);
      delay(10);
      
      // Lưu giá trị trục Y
      radio.read(&receivedData, sizeof(receivedData));
      yAxis = atoi(&receivedData[0]);
      delay(10);
         // Đọc giá trị nút trái 
      radio.read(&receivedData, sizeof(receivedData));
      leftButton = atoi(&receivedData[0]);  
      delay(10);
      // Đọc giá trị nút phải
      radio.read(&receivedData, sizeof(receivedData));  
      rightButton = atoi(&receivedData[0]);
      delay(10);
      // Đọc giá trị nút trên
      radio.read(&receivedData, sizeof(receivedData));
      topButton = atoi(&receivedData[0]);
      delay(10);
      // Đọc giá trị nút dưới
      radio.read(&receivedData, sizeof(receivedData));
      bottomButton = atoi(&receivedData[0]);
      delay(10);
    }
   
    // Điều khiển động cơ dựa trên giá trị trục Y
    if (yAxis < 470) {

      // Lùi
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW); 
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      
      // Ánh xạ giá trị Y sang tốc độ động cơ 
      motorSpeedA = map(yAxis, 470, 0, 0, 255);
      motorSpeedB = map(yAxis, 470, 0, 0, 255);

    } else if (yAxis > 550) {

      // Tiến
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);

      // Ánh xạ giá trị Y sang tốc độ động cơ
      motorSpeedA = map(yAxis, 550, 1023, 0, 255);
      motorSpeedB = map(yAxis, 550, 1023, 0, 255);

    } else {

      // Dừng động cơ
      Serial.println("!!!!!");
      motorSpeedA = 0;
      motorSpeedB = 0;
    }
    
    // Điều khiển động cơ dựa trên giá trị trục X
    if (xAxis < 470) {

      // Ánh xạ giá trị X sang chênh lệch tốc độ
      int xMapped = map(xAxis, 470, 0, 0, 255);

      // Điều chỉnh tốc độ động cơ để quay trái 
      motorSpeedA += xMapped;
      motorSpeedB -= xMapped;
      
      // Giới hạn tốc độ trong khoảng hợp lệ
      if(motorSpeedA < 0){
        motorSpeedA = 0;
      }
      if(motorSpeedB > 255){
        motorSpeedB = 255;
      }

    }
    
    if (xAxis > 550) {
    
      // Ánh xạ giá trị X sang chênh lệch tốc độ
      int xMapped = map(xAxis, 550, 1023, 0, 255);

      // Điều chỉnh tốc độ động cơ để quay phải
      motorSpeedA -= xMapped;
      motorSpeedB += xMapped;
      
      // Giới hạn tốc độ trong khoảng hợp lệ 
      if(motorSpeedA > 255){
        motorSpeedA = 255;
      }
      if(motorSpeedB < 0){
        motorSpeedB = 0;
      }
    }

    // Tránh kêu ở tốc độ thấp
    if(motorSpeedA < 70){
      motorSpeedA = 0;
    }
    if(motorSpeedB < 70){
      motorSpeedB = 0;
    }
    // Gửi tín hiệu PWM đến các động cơ
    analogWrite(enA, motorSpeedA);
    analogWrite(enB, motorSpeedB);
    
  }