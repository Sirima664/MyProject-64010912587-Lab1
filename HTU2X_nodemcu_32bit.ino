/**
 * www.arduinona.com
 * วัดอุณหภูมิ และความชื้นภายในห้องด้วย HTU21D
 */

/**
 * Arduino macro and library 
 */

    #include <Arduino.h>



/**
 *
 * HTU21D module
 *  
 */

    #include <Adafruit_HTU21DF.h>

    // Connect Vin to 3-5VDC
    // Connect GND to ground
    // Connect SCL to I2C clock pin (A5 on UNO)
    // Connect SDA to I2C data pin (A4 on UNO)
    Adafruit_HTU21DF htu = Adafruit_HTU21DF();



/**
 * 
 * Blynk module 
 * 
 */
    #define BLYNK_PRINT Serial


    #include <BlynkSimpleEsp8266.h>

    /**
     * Note : virtual pin
     * V0 = temperature report
     * V1 = humidity report
     */
    char        auth[] = "LsmuqUa3IGrOLJdVx7TY1FQegPizMRFu";
    const char* domain = "blynk.iot-cm.com";
    uint16_t    port   = 8080;



/**
 * 
 * WiFi module
 * 
 */
    #include <ESP8266WiFi.h>
    char ssid[] = "Kanuengnit_kiss2G";
    char pass[] = "3213213213";


void setup()
{
  /**
   * ไว้ debug กับ Serial monitor
   */
      Serial.begin(115200);


  /**
   * เชื่อมต่ออินเตอร์เน็ตและ Blynk server
   */

      Serial.println("Blynk connecting..");
      Blynk.begin(auth, ssid, pass, domain, port);
      Serial.println("Blynk connected!!");

  /**
   * เชื่อมต่อกับ HTU21D
   */ 
      if (!htu.begin()) {
        Serial.println("Couldn't find sensor! please check wiring");
        while (1);
      }
}

void loop()
{

  /**
   * 
   * อัพเดทค่า Blynk กับทาง Server
   */
      Blynk.run();

  

  /**
   * อัพเดททุกๆ 1000 มิลลิวินาที
   */
      static uint64_t last_update = micros64();
      uint64_t time_now = micros64();
      if( time_now - last_update > 1000000) {
        last_update = time_now;


        /**
         * 
         * ดึงค่าจาก HTU21D
         */ 
            float temp = htu.readTemperature();
            float rel_hum = htu.readHumidity();

            Serial.printf("%.2f degC, %.2f %\n", temp, rel_hum);

        /**
         * ป้อนค่าขึ้น Blynk 
         */

            Blynk.virtualWrite(V0, temp);
            Blynk.virtualWrite(V1, rel_hum);
          

      }
}
