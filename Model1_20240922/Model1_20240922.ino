#include <Wire.h>
#include "seeed_bme680.h"
#include <SoftwareSerial.h>
#include <Bridge.h>

#define SOFT_TX  8   //to CO2 RX
#define SOFT_RX  9   //to CO2 TX
SoftwareSerial mySerial(SOFT_RX, SOFT_TX); // RX, TX

/* IIC PROTOCOL D2(SDA), D3(SCL)*/ 

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define IIC_ADDR  uint8_t(0x76)
Seeed_BME680 bme680(IIC_ADDR);        //大氣壓  D2(SDA), D3(SCL)


unsigned int fetch_CO2_ppm(){
    unsigned char CO2_Read[]={0x42,0x4d,0xe3,0x00,0x00,0x01,0x72};
    unsigned char CO2_val[24],num=0;
    mySerial.flush();
    mySerial.write(CO2_Read,7);
    for(unsigned char i=0;i<100;i++){
        delay(1);
        if(mySerial.available()==12){
            while(mySerial.available()){
                CO2_val[num] = mySerial.read();
                num++;
            }
            break;
        }
    }
    return CO2_val[4]*256+CO2_val[5]; 
}

void setup() {
    Serial.begin(115200); //for debug message
    mySerial.begin(9600);  //for CO2 communication    

    while (!bme680.init()) {
        Serial.println("bme680 init failed ! can't find device!");
        delay(10000);
    }

    pinMode(13,OUTPUT); //IoTtalk successful registration notification
    Bridge.begin();   // D0 and D1 are used by the Bridge library.
}

void loop() {
    char D13[2];
    char valueStr[23];
    unsigned int valueInt=0;
    
    Bridge.get("Reg_done",  D13, 2);
    digitalWrite(13, atoi(D13));    

    if (bme680.read_sensor_data()) {
        Serial.println("Failed to perform reading :(");
        return;
    }
    
    uint32_t AtmosphericPressure = bme680.sensor_result_value.pressure; //BME 大氣壓   D2, D3
    dtostrf(AtmosphericPressure, 8, 0, valueStr);
    Bridge.put("AtPressure", valueStr);     

    float Temperature = bme680.sensor_result_value.temperature;
    dtostrf(Temperature, 6, 2, valueStr);
    Bridge.put("Temperature", valueStr); 

    float Humidity = bme680.sensor_result_value.humidity;
    dtostrf(Humidity, 8, 0, valueStr);
    Bridge.put("Humidity", valueStr); 

    uint32_t Gas = bme680.sensor_result_value.gas;
    dtostrf(Gas, 8, 0, valueStr);
    Bridge.put("Gas", valueStr); 
       
    Serial.print("Temp: ");        Serial.print(Temperature);    Serial.println(" C");// unit: C
    Serial.print("Humidity: ");    Serial.print(Humidity);    Serial.println(" %"); // unit:%    
    Serial.print("Pressure: ");    Serial.print(AtmosphericPressure);    Serial.println(" Pa");     // unit: Pa
    Serial.print("Gas: ");    Serial.print(Gas);    Serial.println(" ohms");     // unit: Pa

    //CO2 DS-CO2-20
    valueInt = fetch_CO2_ppm();
    itoa(valueInt, valueStr, 10);
    Bridge.put("CO2", valueStr);
    Serial.print("CO2: ");    Serial.print(valueStr);   Serial.println(" ppm"); // unit:ppm
   
    Serial.println("=========================================================================");    
    
    delay(1000);
}