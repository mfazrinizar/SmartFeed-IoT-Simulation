#pragma once
#include <Arduino.h>

void initFeeder(int servoPin);
void triggerFeeding(const String &action = "auto", const String &historyId = "");