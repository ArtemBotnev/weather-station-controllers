//
// Created by artem on 25.11.2024.
//

#include "ESP8266WiFi.h"
#include "Adafruit_Sensor.h"
#include "DHT.h"

#include "device_data.h"
#include "src/network.h"

extern "C" {
    #include "src/utils.h"
}

#define DHT_PIN 2
#define DHT_TYPE DHT22

#define IS_DEBUG_MODE false

#define SENSOR_READ_DELAY_MS 2000
#define LOOP_DELAY_MS 5 * 60 * 1000

NetworkService networkService(MEASURE_COUNT);

DHT dht(DHT_PIN, DHT_TYPE);

measurement measurement = {
        3, // time zone
        { // device
                1, // device id
                "ESP-01", // type
                "ESP-01 id_1", // name
                "app_55", // location
        },
};

void setup() {
    dht.begin();

    if (IS_DEBUG_MODE) {
        Serial.begin(115200);
    }

    if (IS_DEBUG_MODE) {
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(networkService.establishConnection());
    } else {
        networkService.establishConnection();
    }
}

void loop() {
    readDht();

    if (IS_DEBUG_MODE) {
        Serial.print("HTTP Response code: ");
        Serial.println(networkService.sendData(measurement));
    } else {
        networkService.sendData(measurement);
    }
    delay(LOOP_DELAY_MS);
}

void readDht() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    temp = round_two_sings_after_point(temp);
    humidity = round_two_sings_after_point(humidity);

    measurement.measures[0] = (measure) {
            "dht-22_temp", // id
            "dht-22_app_55", // sensor name
            "balcony", //place
            "temperature", // measure name
            temp, // value
            "℃", // value unit
            !is_temperature_valid(temp) || isZero(temp), // is server error (not valid value)
    };
    measurement.measures[1] = (measure) {
            "dht-22_hum", // id
            "dht-22_app_55", // sensor name
            "balcony", //place
            "humidity", // measure name
            humidity, // value
            "%", // value unit
            !is_humidity_valid(humidity),  // is server error (not valid value)
    };
    delay(SENSOR_READ_DELAY_MS);
}
