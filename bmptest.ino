#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <dht.h>

/*
В примере показано использование датчика атмосферного давления BMP180
*/

#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8


// Color definitions
#define  BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst); 
 
#include <Wire.h>
#include <Adafruit_BMP085.h>
/***************************************************
  This is an example for the BMP085 Barometric Pressure & Temp Sensor
   These displays use I2C to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries. 
  BSD license, all text above must be included in any redistribution
 ****************************************************/
// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here
Adafruit_BMP085 bmp;
DHT hum = DHT();

int soilpin = A0;
int dthpin  = A1;
int hum_ar,hum_soil,dth_temp;
char msg[128];
  
void setup() {
  Serial.begin(9600);
  display.begin();
  display.fillScreen(BLACK);
  display.setCursor(0,0);
  display.print("START TEST GY-68");
  display.setCursor(0,10);
  display.print("Jan 2016");
  delay(500);
  
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
  display.setCursor(0,20);
  display.print("DHT init");
  hum.attach(A1);
  delay(1000); //ждем готовности сенсора
  display.fillRect(0, 20 , 320, 240, BLACK);
}
  
void loop() {
    /*Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
     
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
     
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");
    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");
  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
     
    Serial.println();
  */

    hum.update();
 
    switch (hum.getLastError())
    {
        case DHT_ERROR_OK:
            
            // данные последнего измерения можно считать соответствующими
            // методами Humidity
            sprintf(msg, "T = %dC, H = %d%%", 
                    hum.getTemperatureInt(), hum.getHumidityInt());
            Serial.println(msg);
            break;
        case DHT_ERROR_START_FAILED_1:
            Serial.println("Error: start failed (stage 1)");
            break;
        case DHT_ERROR_START_FAILED_2:
            Serial.println("Error: start failed (stage 2)");
            break;
        case DHT_ERROR_READ_TIMEOUT:
            Serial.println("Error: read timeout");
            break;
        case DHT_ERROR_CHECKSUM_FAILURE:
            Serial.println("Error: checksum error");
            break;
    }   
    sprintf(msg, "T = %dC, H = %d%%", 
                    hum.getTemperatureInt(), hum.getHumidityInt());
    hum_soil = analogRead(A0);
    
    //display.fillRect(0, 20 , 320, 240, BLACK);
    display.setCursor(0,20);
    display.println("T=              ");
    display.println("P=              ");
    display.println("                ");
    display.println("                ");
    display.fillRect(0, 20 , 80, 40, BLACK);
    display.setCursor(0,20);
    display.print("T=  "); display.println(bmp.readTemperature());
    display.print("P=  "); display.print(bmp.readPressure()); display.println(" Pa");
    display.print("P= ");display.print(bmp.readPressure()*0.0075); display.println(" mm");
    display.print("Soil HUM= "); display.print(hum_soil); display.println("%");
    //display.print("A=  "); display.print(bmp.readAltitude()); display.println(" m");
    //display.print("Ar= "); display.print(bmp.readAltitude(101500)); display.println(" m");
    //display.print("Ps= "); display.print(bmp.readSealevelPressure()); display.println(" Pa");
    display.fillTriangle(75, 35, 80, 30, 85, 35, GREEN);
    display.fillTriangle(75, 20, 80, 25, 85, 20, RED);
    delay(500);
}


// Подробнее: http://xrobot.by/sensors/bmp180#ixzz3wD3eJ7Qa
