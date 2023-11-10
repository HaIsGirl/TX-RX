/*
CODE NÀY DÙNG ĐỂ ĐIỀU KHIỂN XE BẰNG JOYSTICK THÔNG QUA MODULE RF24
*/

// 1. Khai báo thư viện cần dùng
#include <SPI.h> // Thư viện giao tiếp SPI để nói chuyện với module RF24
#include <nRF24L01.h> // Thư viện điều khiển module RF24
#include <RF24.h> // Thư viện điều khiển module RF24

// 2. Khởi tạo đối tượng radio để điều khiển module RF24 
//    - Đấu nối tới chân CE và CSN của module RF24
RF24 radio(7,8); 

// 3. Đặt địa chỉ kết nối giữa 2 module RF24
//    - Địa chỉ càng ngắn thì tốc độ kết nối càng nhanh
//    - Nhưng cũng cần tránh bị trùng địa chỉ với thiết bị khác
const byte address[6] = "00001";

// 4. Khai báo biến để lưu dữ liệu
//    - xyData[]: mảng chứa dữ liệu sẽ gửi đi
//    - xAxis, yAxis: chuỗi chứa giá trị đọc được từ joystick
//    - leftButton,rightButton chứa giá trị của nút nhấn bên trái, bên phải
//    - topButton, bottomButton chứa giá trị nút nhấn bên trên, bên dưới
char xyData[64] = "";    
String xAxis, yAxis;
byte leftButton,rightButton;
byte topButton, bottomButton;
// 5. Hàm khởi tạo
void setup() {

  // Mở cổng Serial ở baud 9600 để in ra màn hình máy tính
  Serial.begin(9600);  
  //------------------------------------------------------
  if (!radio.begin()){                                
      Serial.write("Module không khởi động được...!");
      while (1){}
    }
  if (radio.begin() == true ){
    Serial.write("Khoi dong hoat tat");
  }
  //------------------------------------------------------
  // Khởi tạo chức năng của module RF24
  radio.begin();
  radio.openWritingPipe(address); 
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(67); 
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening(); // Chế độ truyền dữ liệu
  //Kiểm tra kết nối
    if (!radio.available()){
      Serial.println("Chưa kết nối được với RX");
      Serial.println("CHỜ KẾT NỐI !");
    }
}

// 6. Vòng lặp chính
void loop() {
  if (radio.available() == true){
  // Đọc giá trị joystick 
  xAxis       = analogRead(A1);  // Trục X
  yAxis       = analogRead(A2);  // Trục Y
  leftButton  = analogRead(A5);  //Nút Trái
  rightButton = analogRead(A3);  //Nút Phải
  topButton   = analogRead(A0);  //Nút Trên
  bottomButton= analogRead(A4);  //Nút Dưới
  // Chuyển đổi dữ liệu về dạng mảng ký tự
  // Để có thể gửi qua RF24 dễ dàng   
  xAxis.toCharArray(xyData, 5);
  yAxis.toCharArray(xyData, 5);  
  // Gửi dữ liệu 
  // Cập nhật nội dung xyData[] trước mỗi lần gọi
  radio.write(&xyData, sizeof(xyData)); // Gửi X
  radio.write(&xyData, sizeof(xyData)); // Gửi Y
  
  sprintf(xyData, "%d", leftButton); 
  radio.write(&xyData, sizeof(xyData)); 

  sprintf(xyData, "%d", rightButton);
  radio.write(&xyData, sizeof(xyData));

  sprintf(xyData, "%d", topButton);
  radio.write(&xyData, sizeof(xyData));

  sprintf(xyData, "%d", bottomButton); 
  radio.write(&xyData, sizeof(xyData));

  delay(20); // Delay 20ms trước khi lặp lại
}
}