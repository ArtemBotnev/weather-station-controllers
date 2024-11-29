//
// Created by artem on 25.11.2024.
//

#ifndef DEVICE_DATA_H
#define DEVICE_DATA_H

#include "inttypes.h"
#define MEASURE_COUNT 4

#define BASE_JSON_CHAR_CAPACITY 1024 // could be extends for long strings
#define MEASURE_JSON_CHAR_CAPACITY 2048 // could be extends for long strings

struct device {
    uint16_t id;
    const char *type;
    const char *name;
    const char *location;
};

struct measure {
    const char *sensorId;
    const char *sensorName;
    const char *sensorPlace;
    const char *measureName;
    float measureValue;
    const char *measureUnit;
};

struct measurement {
    uint8_t timeZoneHours;
    struct device device;
    measure measures[MEASURE_COUNT];
};

#endif //DEVICE_DATA_H
