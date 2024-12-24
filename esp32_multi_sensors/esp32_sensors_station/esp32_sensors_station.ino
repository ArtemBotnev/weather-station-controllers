//
// Created by artem on 25.11.2024.
//

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DallasTemperature.h>
#include <Adafruit_SHT4x.h>

#include "network.h"

#define I2C_SDA 33
#define I2C_SCL 32

#define SENSOR_READ_DELAY_MS 500
#define LOOP_DELAY_MS 5 * 60 * 1000

#define IS_DEBUG_MODE false

NetworkService networkService;

Adafruit_BME280 bme;
Adafruit_SHT4x sht4;

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
    }

    if (!sht4.begin()) {
        if(IS_DEBUG_MODE) {
            Serial.println("Couldn't find SHT4x");
        }
        while (1) delay(1000);
    }
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);

    if (IS_DEBUG_MODE) {
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(networkService.establishConnection());
    } else {
        networkService.establishConnection();
    }
}

void loop() {
    readSht45();
    readBme();

    if (IS_DEBUG_MODE) {
        Serial.print("HTTP Response code: ");
        Serial.println(networkService.sendData(measurement));
    } else {
        networkService.sendData(measurement);
    }
    delay(LOOP_DELAY_MS);
}

void readSht45() {
    sensors_event_t humidity, temp;
    sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

    float valueOutdoorTemp = temp.temperature;
    float valueOutdoorHum = humidity.relative_humidity;
    if (IS_DEBUG_MODE) {
        Serial.print("Rht45 temperature: t ℃");
        Serial.println(valueOutdoorTemp);
        Serial.print("Rht45 humidity: %");
        Serial.println(valueOutdoorHum);
    }
    measurement.measures[0] = (measure) {
            "sht-45_temp", // id
            "sht-45_app_55", // sensor name
            "outdoor", //place
            "temperature", // measure name
            valueOutdoorTemp, // value
            "℃", // value unit
    };
    measurement.measures[1] = (measure) {
            "sht-45_hum", // id
            "sht-45_app_55", // sensor name
            "outdoor", //place
            "humidity", // measure name
            valueOutdoorHum, // value
            "%", // value unit
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

    measurement.measures[2] = (measure) {
            "bme_0_temp", // id
            "bme_0_app_55", // sensor name
            "room", //place
            "temperature", // measure name
            temperature, // value
            "℃", // value unit
    };

    measurement.measures[3] = (measure) {
            "bme_0_hum", // id
            "bme_0_app_55", // sensor name
            "room", //place
            "humidity", // measure name
            humidity, // value
            "%", // value unit
    };

    measurement.measures[4] = (measure) {
            "bme_0_press", // id
            "bme_0_app_55", // sensor name
            "room", //place
            "atm. pressure", // measure name
            pressure, // value
            "mmHg", // value unit
    };
}
