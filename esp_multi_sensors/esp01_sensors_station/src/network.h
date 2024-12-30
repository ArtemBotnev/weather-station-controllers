//
// Created by artem on 25.11.2024.
//

#ifndef NETWORK_H
#define NETWORK_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "../device_data.h"

#define SSID "YOUR_SSID"
#define PASSWORD "YOUR_PASSWORD"

#define HTTP "http://"
#define PUSH_MEASUREMENT_REQUEST "/send_measurement" // your_request_path

#define HOST_COUNT 2

#define BASE_JSON_CHAR_CAPACITY 1024 // could be extends for long strings
#define MEASURE_JSON_CHAR_CAPACITY 2048 // could be extends for long strings

class NetworkService {

public:
    NetworkService(uint8_t measureCount);

    /**
    * Establish WIFI Connection
    * @return device local IP
    */
    IPAddress establishConnection();

    /**
    * Send measurement json to server
    * @return response code
    */
    uint16_t sendData(measurement measurement);

private:
    uint8_t measureCount;

    char jsonResult[BASE_JSON_CHAR_CAPACITY + MEASURE_JSON_CHAR_CAPACITY * MEASURE_COUNT];

    void serializeMeasurement(measurement measurement);

    // your_hosts
    const char* hosts[HOST_COUNT] = {
            "192.168.1.85:8080",
            "192.168.1.66:8080",
    };
};

#endif //NETWORK_H
