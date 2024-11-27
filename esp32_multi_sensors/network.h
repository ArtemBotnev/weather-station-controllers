//
// Created by artem on 25.11.2024.
//

#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "device_data.h"

#define SSID "YOUR_SSID"
#define PASSWORD "YOUR_PASSWORD"
#define PUSH_MEASUREMENT_REQUEST "http://192.168.1.66:8080/send_measurement" // your_request_path

/**
 * Establish WIFI Connection
 * @return device local IP
 */
IPAddress establishConnection();

/**
 * Send measurement json to server
 * @return response code
 */
int16_t sendData(measurement measurement);

void serializeMeasurement(measurement measurement);

#endif //NETWORK_H
