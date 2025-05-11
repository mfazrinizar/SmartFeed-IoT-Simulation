#include <Arduino.h>
#include "FeedSensor.h"

#define TRIG_PIN 5
#define ECHO_PIN 18

static double feedHeight = 0.0;

void initFeedSensor() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

long readFeedLevel() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  long distance = duration * 0.034 / 2;
  long level = map(distance, 2, 30, 100, 0);
  level = constrain(level, 0, 100);
  return level;
}

double readFeedLevelPrecise() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  double distance = duration * 0.034 / 2.0;
  if (distance < 0.0) distance = 0.0;
  if (distance > 8.5) distance = 8.5;
  double level = 100.0 * (8.5 - distance) / 8.5;
  level = constrain(level, 0.0, 100.0);
  return level;
}

void decreaseFeedHeight() {
  double decrease = random(10, 21) / 100.0;
  feedHeight += decrease;
  if (feedHeight > 8.5)
    feedHeight = 8.5;
}

double getFeedLevelFromHeight() {
  double level = 100.0 * (8.5 - feedHeight) / 8.5;
  level = constrain(level, 0.0, 100.0);
  return level;
}