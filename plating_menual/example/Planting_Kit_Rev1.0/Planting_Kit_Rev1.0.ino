#include <LiquidCrystal_I2C.h>/*LiquidCrystal_I2C*/

#define A_SOIL_HUMI A0
#define O_PUMP_A 6
#define O_PUMP_B 5
#define O_RGB_R 9
#define O_RGB_G 10
#define O_RGB_B 11
#define On_Time 500
#define LCD_I2C_ADDR 0x27


LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);

// 습도 변수
int soilHumidity;

// 타이머 변수

int h;    // 시
int m;    // 분
int s;    // 초

int hh;   // 타이머 시
int mm;   // 타이머 분
unsigned long ss;  // 타이머 초

int vm;   // 화면 노출용 분
int vh;   // 화면 노출용 시

int lightType = 3 ;   // 불빛 타입

/*디지털핀 초기화하기*/
void initPin() {
  pinMode(O_PUMP_A, OUTPUT);
  pinMode(O_PUMP_B, OUTPUT);
  pinMode(O_RGB_R, OUTPUT);
  pinMode(O_RGB_G, OUTPUT);
  pinMode(O_RGB_B, OUTPUT);

  digitalWrite(O_RGB_R, LOW);
  digitalWrite(O_RGB_G, LOW);
  digitalWrite(O_RGB_B, LOW);
  analogWrite(O_PUMP_A, 0);
  analogWrite(O_PUMP_B, 0);
}

/*LCD INTRO출력하기*/
void introLcd() {
  lcd.print("Planting Kit");
  lcd.setCursor(0, 1);
  lcd.print("Rev1.0");
}

/*LCD 습도 프린트하기*/
void printLcd() {
  ss = round(millis()*0.001);
  s = ss % 60;
  mm = (ss / 60)%60;
  hh = (ss / 3600)%24;

  vm = (m + mm)%60;
  vh = (h + hh + (m+mm)/60)%24;

  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moisture : ");
  lcd.print(soilHumidity);
  lcd.print("%");
  lcd.setCursor(0, 1);

  if ( vh < 10 ) lcd.print(0);
  lcd.print(vh);
  lcd.print(":");
  if ( vm < 10 ) lcd.print(0);
  lcd.print(vm);
  lcd.print(":");
  if ( s < 10 ) lcd.print(0);
  lcd.print(s);

  lcd.print(" |Type:");
  lcd.print(lightType);
  
  delay(1000);
}

/*LCD 초기화하기*/
void initLcd() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  // INTRO LCD 출력
  introLcd();
}

/*토양습도 계산하기*/
void calcSoilHumidity() {
  soilHumidity = map(analogRead(A_SOIL_HUMI), 1000, 400, 0, 100);
  // if(soilHumidity > 100) soilHumidity = 100;
  // else if(soilHumidity < 0) soilHumidity = 0;
}

void writeRGB(bool R, bool G, bool B) {
  digitalWrite(O_RGB_R, R);
  digitalWrite(O_RGB_G, G);
  digitalWrite(O_RGB_B, B);
}

// 펌프 작동
void pumpOn(int mois) {
  // 습도가 변수(mois) 미만일 때 펌프 작동
  if ( soilHumidity < mois ) {
    delay(2000);
    lcd.clear();
    lcd.noBacklight();
    
    // 230*5 = 1.150초
    for ( int i = 0; i < 230; i++ ) {
      analogWrite(O_PUMP_A, i);
      delay(5);
    }

    delay(mois == 80 ? On_Time*4:On_Time);    // 0.5초
    analogWrite(O_PUMP_A, 0);
    analogWrite(O_PUMP_B, 0);
    delay(100);
  } else {
    analogWrite(O_PUMP_A, 0);
    analogWrite(O_PUMP_B, 0);
    delay(1000);
  }
}

// LED 작동
void LEDOn() {
  if ( 7 <= vh && vh <= 18 ) {
    lightType = 1;
  } else if ( 19 <= vh && vh <= 22 ) {
    lightType = 2;
  } else {
    lightType = 3;
  }

  writeRGB(lightType == 1 ? HIGH : LOW, LOW, lightType == 1 ? HIGH : LOW);
  lightType != 3 ? lcd.backlight() : lcd.noBacklight();
}

// 전원 들어왔을 때 한 번 실행
void setup() {
  // 핀 초기화
  initPin();

  // 토양 습도 계산
  calcSoilHumidity();

  // 펌프 작동
  // pumpOn(80);
  
  // LCD 초기화
  initLcd();
  
  h = 11;    // 시
  m = 33;    // 분
  s = 0;    // 초
}


  
void loop() {
  // 토양 습도 계산
  calcSoilHumidity();

  // 습도 프린트
  printLcd();

  // LED 작동
  LEDOn();

  // 펌프 작동
  // pumpOn(60);
}
