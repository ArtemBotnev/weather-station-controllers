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

int isZero(float value)
{
    return value == 0.0f;
}

float round_two_sings_after_point(float value)
{
    return roundTo(value, 2);
}

float roundTo(float value, uint8_t sings_after_point)
{
    int multiplier = powTo(10, sings_after_point);

    return round(value * multiplier) / multiplier;
}

uint32_t powTo(uint16_t value, uint8_t exp)
{
    if (exp < 1) return 1;

    unsigned int result = value;
    for (int i = 1; i < exp; i++) {
        result *= value;
    }

    return result;
}