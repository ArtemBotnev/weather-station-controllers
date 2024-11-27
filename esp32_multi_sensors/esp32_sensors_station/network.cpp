//
// Created by artem on 25.11.2024.
//

#include "network.h"

NetworkService::NetworkService() = default;

IPAddress NetworkService::establishConnection() {
    WiFi.begin(SSID, PASSWORD);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    return WiFi.localIP();
}

int16_t NetworkService::sendData(measurement measurement) {
    HTTPClient http;
    int16_t httpResponseCode = 0;

    if (WiFi.status()== WL_CONNECTED) {
        HTTPClient http;

        http.begin(PUSH_MEASUREMENT_REQUEST);

        serializeMeasurement(measurement);
        http.addHeader("Content-Type", "application/json");
        httpResponseCode = http.POST(jsonResult);

        http.end();
    }

    return httpResponseCode;
}

void NetworkService::serializeMeasurement(measurement measurement) {
    JsonDocument deviceJson;
    deviceJson["id"] = measurement.device.id;
    deviceJson["type"] = measurement.device.type;
    deviceJson["name"] = measurement.device.name;
    deviceJson["location"] = measurement.device.location;

    JsonDocument measurementJson;
    measurementJson["timeZoneHours"] = measurement.timeZoneHours;
    measurementJson["device"] = deviceJson;

    JsonArray array = measurementJson["measures"].to<JsonArray>();
    for(uint8_t i = 0; i < MEASURE_COUNT; i++) {
        measure measure = measurement.measures[i];

        JsonDocument measureJson;
        measureJson["sensorId"] = measure.sensorId;
        measureJson["sensorName"] = measure.sensorName;
        measureJson["sensorPlace"] = measure.sensorPlace;
        measureJson["measureName"] = measure.measureName;
        measureJson["measureValue"] = measure.measureValue;
        measureJson["measureUnit"] = measure.measureUnit;

        array.add(measureJson);
    }

    serializeJson(measurementJson, jsonResult);
}
