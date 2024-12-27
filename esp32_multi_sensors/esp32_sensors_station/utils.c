//
// Created by artem on 26.12.2024.
//

#include "utils.h"

int is_temperature_valid(float temperature)
{
    return temperature > TEMPERATURE_MIN_LIMIT && temperature < TEMPERATURE_MAX_LIMIT;
}

int is_humidity_valid(float humidity)
{
    return humidity > HUMIDITY_MIN_LIMIT && humidity < HUMIDITY_MAX_LIMIT;
}

int is_atm_pressure_valid(float pressure)
{
    return pressure > ATM_PRESSURE_MIN_LIMIT && pressure < ATM_PRESSURE_MAX_LIMIT;
}

float round_two_sings_after_point(float value)
{
    return round(value * 100) /100;
}