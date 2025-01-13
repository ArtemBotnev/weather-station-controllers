//
// Created by artem on 25.11.2024.
//

#include "network.h"

NetworkService::NetworkService(uint8_t measureCount) {
    this->measureCount = measureCount;
}

IPAddress NetworkService::establishConnection() {
    WiFi.begin(SSID, PASSWORD);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    return WiFi.localIP();
}

uint16_t NetworkService::sendData(measurement measurement) {
    int16_t httpResponseCode = 0;

    if (WiFi.status()== WL_CONNECTED) {
        for (int i = 0; i < HOST_COUNT; i++) {
            char request[128];
            snprintf(request, sizeof(request), "%s%s%s", HTTP, hosts[i], PUSH_MEASUREMENT_REQUEST);

            HTTPClient http;
            http.begin(request);

            serializeMeasurement(measurement);
            http.addHeader("Content-Type", "application/json");
            httpResponseCode = http.POST(jsonResult);

            http.end();
        }
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
    for(uint8_t i = 0; i < measureCount; i++) {
        measure measure = measurement.measures[i];

        JsonDocument measureJson;
        measureJson["sensorId"] = measure.sensorId;
        measureJson["sensorName"] = measure.sensorName;
        measureJson["sensorPlace"] = measure.sensorPlace;
        measureJson["measureName"] = measure.measureName;
        measureJson["measureValue"] = measure.measureValue;
        measureJson["measureUnit"] = measure.measureUnit;
        measureJson["sensorError"] = measure.sensorError;

        array.add(measureJson);
    }

    serializeJson(measurementJson, jsonResult);
}
