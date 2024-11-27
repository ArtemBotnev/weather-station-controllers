//
// Created by artem on 25.11.2024.
//

#include "network.h"

NetworkService networkService;

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
//    establishConnection();
Serial.begin(115200);
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(networkService.establishConnection());
  delay(3000);
}

void loop() {
    readOutdoorDallas();
    readRoom();
//    sendData(measurement);

    Serial.print("HTTP Response code: ");
    Serial.println(networkService.sendData(measurement));
    delay(5000);
}

void readOutdoorDallas() {
    measurement.measures[0] = (measure) {
            "dallas_0", // id
            "dallas_app_55", // sensor name
            "outdoor", //place
            "temperature", // measure name
            23.0, // value
            "℃", // value unit
    };
}

void readRoom() {
    measurement.measures[1] = (measure) {
            "dth_11_0", // id
            "dth_11_app_55", // sensor name
            "room", //place
            "humidity", // measure name
            45, // value
            "%", // value unit
    };
}
