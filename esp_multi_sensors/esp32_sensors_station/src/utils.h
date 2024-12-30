//
// Created by artem on 26.12.2024.
//

#ifndef UTILS_H
#define UTILS_H

const float TEMPERATURE_MAX_LIMIT = 50.0f;
const float TEMPERATURE_MIN_LIMIT = -50.0f;

const float HUMIDITY_MAX_LIMIT = 99.0f;
const float HUMIDITY_MIN_LIMIT = 3.0f;

const float ATM_PRESSURE_MAX_LIMIT = 830.0f;
const float ATM_PRESSURE_MIN_LIMIT = 670.0f;

int is_temperature_valid(float temperature);

int is_humidity_valid(float humidity);

int is_atm_pressure_valid(float pressure);

float round_two_sings_after_point(float value);

#endif //UTILS_H