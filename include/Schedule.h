#pragma once
#include <vector>
#include "RTClib.h"
#include <ArduinoJson.h>

extern std::vector<DateTime> feedingSchedule;
void updateFeedingSchedule(const JsonArray &arr, int timezone);