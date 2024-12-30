//
// Created by artem on 25.11.2024.
//

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SHT4x.h>

#include "device_data.h"
#include "src/network.h"

extern "C" {
    #include "src/utils.h"
}

#define I2C_SDA 33
#define I2C_SCL 32

#define SENSOR_READ_DELAY_MS 2000
#define LOOP_DELAY_MS 5 * 60 * 1000

#define IS_DEBUG_MODE false

bool isSht45Available = false;

NetworkService networkService(MEASURE_COUNT);

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

    isSht45Available = sht4.begin();

    if (isSht45Available) {
        sht4.setPrecision(SHT4X_HIGH_PRECISION);
        sht4.setHeater(SHT4X_NO_HEATER);
    } else {
        if(IS_DEBUG_MODE) {
            Serial.println("Couldn't find SHT4x");
        }
    }

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
    float valueOutdoorTemp = -100.0f;
    float valueOutdoorHum = -100.0f;

    if (isSht45Available) {
        sensors_event_t humidity, temp;
        sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
        valueOutdoorTemp = round_two_sings_after_point(temp.temperature);
        valueOutdoorHum = round_two_sings_after_point(humidity.relative_humidity);
    }

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
            !is_temperature_valid(valueOutdoorTemp), // is server error (not valid value)
    };
    measurement.measures[1] = (measure) {
            "sht-45_hum", // id
            "sht-45_app_55", // sensor name
            "outdoor", //place
            "humidity", // measure name
            valueOutdoorHum, // value
            "%", // value unit
            !is_humidity_valid(valueOutdoorHum),  // is server error (not valid value)
    };
    delay(SENSOR_READ_DELAY_MS);
}

void readBme() {
    float temperature = round_two_sings_after_point(bme.readTemperature());
    delay(SENSOR_READ_DELAY_MS);
    float humidity = round_two_sings_after_point(bme.readHumidity());
    delay(SENSOR_READ_DELAY_MS);
    float pressure = bme.readPressure() * 0.0075F;
    pressure = round_two_sings_after_point(pressure);

    if (IS_DEBUG_MODE) {
        Serial.print("Bme temperature: t ℃");
        Serial.println(temperature);
        Serial.print("Bme humidity: %");
        Serial.println(humidity);
        Serial.print("Bme pressure: %");
        Serial.println(pressure);
    }

    delay(SENSOR_READ_DELAY_MS);

    measurement.measures[2] = (measure) {
            "bme_0_temp", // id
            "bme_0_app_55", // sensor name
            "room", //place
            "temperature", // measure name
            temperature, // value
            "℃", // value unit
            !is_temperature_valid(temperature), // is server error (not valid value)
    };

    measurement.measures[3] = (measure) {
            "bme_0_hum", // id
            "bme_0_app_55", // sensor name
            "room", //place
            "humidity", // measure name
            humidity, // value
            "%", // value unit
            !is_humidity_valid(humidity), // is server error (not valid value)
    };

    measurement.measures[4] = (measure) {
            "bme_0_press", // id
            "bme_0_app_55", // sensor name
            "room", //place
            "atm. pressure", // measure name
            pressure, // value
            "mmHg", // value unit
            !is_atm_pressure_valid(pressure), // is server error (not valid value)
    };
}
