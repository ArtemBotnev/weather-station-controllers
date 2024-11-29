//
// Created by artem on 25.11.2024.
//

#include <Wire.h>
#include <OneWire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DallasTemperature.h>

#include "network.h"

#define I2C_SDA 33
#define I2C_SCL 32

#define ONE_WIRE_BUS 4

#define SENSOR_READ_DELAY_MS 500
#define LOOP_DELAY_MS 60 * 1000

#define IS_DEBUG_MODE false

NetworkService networkService;

Adafruit_BME280 bme;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

uint8_t  dallasOutdoorSensorAddr[8] = { 0x28, 0xFF, 0x71, 0x38, 0x90, 0x15, 0x03, 0x65 };
//uint8_t  dallasBalconySensorAddr[8] = { 0x28, 0xFF, 0x84, 0x3A, 0x90, 0x15, 0x03, 0x16 };

measurement measurement = {
        3, // time zone
        { // device
                0, // device id
                "ESP-32", // type
                "ESP-32 id_0", // name
                "app_55", // location
        },
};

void setup() {
    Wire.begin(I2C_SDA, I2C_SCL);
    bme.begin();

    if (IS_DEBUG_MODE) {
        Serial.begin(115200);
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(networkService.establishConnection());
    } else {
        networkService.establishConnection();
    }
}

void loop() {
    readDallas();
    readBme();

    if (IS_DEBUG_MODE) {
        Serial.print("HTTP Response code: ");
        Serial.println(networkService.sendData(measurement));
    } else {
        networkService.sendData(measurement);
    }
    delay(LOOP_DELAY_MS);
}

void readDallas() {
    sensors.requestTemperatures();

    float valueOutdoor = readDallasSensor(dallasOutdoorSensorAddr);
    measurement.measures[0] = (measure) {
            "dallas_0", // id
            "dallas_app_55", // sensor name
            "outdoor", //place
            "temperature", // measure name
            valueOutdoor, // value
            "℃", // value unit
    };
    delay(SENSOR_READ_DELAY_MS);
}

void readBme() {
    float temperature = bme.readTemperature();
    delay(SENSOR_READ_DELAY_MS);
    float humidity = bme.readHumidity();
    delay(SENSOR_READ_DELAY_MS);
    float pressure = bme.readPressure() * 0.0075F;
    delay(SENSOR_READ_DELAY_MS);

    measurement.measures[1] = (measure) {
            "bme_0", // id
            "bme_0_app_55", // sensor name
            "room", //place
            "temperature", // measure name
            temperature, // value
            "℃", // value unit
    };

    measurement.measures[2] = (measure) {
            "bme_0", // id
            "bme_0_app_55", // sensor name
            "room", //place
            "humidity", // measure name
            humidity, // value
            "%", // value unit
    };

    measurement.measures[3] = (measure) {
            "bme_0", // id
            "bme_0_app_55", // sensor name
            "room", //place
            "atm. pressure", // measure name
            pressure, // value
            "mmHg", // value unit
    };
}

float readDallasSensor(uint8_t addr[8]) {
    return sensors.getTempC(addr);
}
