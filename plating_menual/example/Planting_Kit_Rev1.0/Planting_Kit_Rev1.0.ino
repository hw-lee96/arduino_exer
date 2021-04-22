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

int h = 21;    // 시
int m = 40;    // 분
int s = 00;    // 초
unsigned long ss;  // 밀리세컨드

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
  if ( ss >= 60 ) {
    s = 0;
    m++;
  } else {
    s = ss;
  }
  if ( m >= 60 ) {
    m = 0;
    h++;
  }
  if ( h >= 24 ) {
    h = 0;
  }
  
	lcd.init();
	lcd.clear();
//	lcd.backlight();
	lcd.setCursor(0, 0);
	lcd.print("Moisture : ");
	lcd.print(soilHumidity);
	lcd.print("%");
	lcd.setCursor(0, 1);

  if ( h < 10 ) lcd.print(0);
  lcd.print(h);
  lcd.print(":");
  if ( m < 10 ) lcd.print(0);
  lcd.print(m);
  lcd.print(":");
  if ( s < 10 ) lcd.print(0);
  lcd.print(s);
//  lcd.print(", ");
//  lcd.print(ss);
  
  delay(1000);
  
//	if (soilHumidity < 20)
//		lcd.print("Water Pump Start");
//	else if (soilHumidity < 40)
//		lcd.print("Soil is Dry");
//	else if (soilHumidity < 70)
//		lcd.print("Soil is Normal");
//	else if (soilHumidity < 90)
//		lcd.print("Soil is Wet");
//	else
//		lcd.print("In the Water");


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

		delay(mois == 80 ? On_Time*4:On_Time);		// 0.5초
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
  lcd.noBacklight();
  if ( 6 <= h && h < 12 ) { // 처음 가동 후 8시간 뒤 LED 작동하게 함
    lcd.backlight();
    writeRGB(HIGH, LOW, HIGH);
  } else if ( 12 <= h && h < 16 ) {  // LED 작동 4시간 뒤 LED 끔
    lcd.noBacklight();
    writeRGB(LOW, LOW, LOW);
  } else if ( 16 <= h && h < 20 ) {  // 다시 4시간 뒤 LED 켬
    lcd.backlight();
    writeRGB(HIGH, LOW, HIGH);
  } else {
    lcd.noBacklight();
    writeRGB(LOW, LOW, LOW);
  }
}

// 전원 들어왔을 때 한 번 실행
void setup() {
	// 핀 초기화
	initPin();

  // 토양 습도 계산
  calcSoilHumidity();

  // 펌프 작동
  pumpOn(80);
  
	// LCD 초기화
	initLcd();
}


  
void loop() {
	// 토양 습도 계산
	calcSoilHumidity();

	// 습도 프린트
	printLcd();

  // LED 작동
  LEDOn();

	// 펌프 작동
	pumpOn(60);
}
