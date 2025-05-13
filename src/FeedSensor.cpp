#include <Arduino.h>
#include "FeedSensor.h"

#define TRIG_PIN 5
#define ECHO_PIN 18

static double feedHeight = 0.0;
static double lastUltrasonicDistance = 2.0;

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
  lastUltrasonicDistance = (double)distance;
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
  lastUltrasonicDistance = (double)distance;
  if (distance < 0.0) distance = 0.0;
  if (distance > 31) distance = 31;
  double level = 100.0 * (31 - distance) / 31;
  level = constrain(level, 0.0, 100.0);
  return level;
}

double readFeedLevelPreciseCustomized() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  double distance = duration * 0.034 / 2.0;
  lastUltrasonicDistance = (double)distance;

  const double D_min = 2.0;   
  const double D_empty = 31;  

  if (distance < D_min) distance = D_min;
  if (distance > D_empty) distance = D_empty;

  double level = (1.0 - (distance - D_min) / (D_empty - D_min)) * 100.0;
  level = constrain(level, 0.0, 100.0);
  return level;
}

void decreaseFeedHeight() {
  double decrease = random(300, 501) / 100.0;
  feedHeight += decrease;
  if (feedHeight > 31)
    feedHeight = 31;
}

void decreaseFeedHeightPrecise() {
  const double D_min = 2.0;
  const double D_empty = 31;
  if (feedHeight >= D_empty) return; 

  double decrease = random(10, 21) / 100.0;
  feedHeight += decrease;
  if (feedHeight > D_empty)
    feedHeight = D_empty;
  if (feedHeight < D_min)
    feedHeight = D_min;
}

double getFeedLevelFromHeight() {
  double level = 100.0 * (31 - feedHeight) / 31;
  level = constrain(level, 0.0, 100.0);
  return level;
}

double getFeedLevelFromHeightPrecise() {
  const double D_min = 2.0;
  const double D_empty = 31;

  double distance = feedHeight;
  if (distance < D_min) distance = D_min;
  if (distance > D_empty) distance = D_empty;

  double level = (1.0 - (distance - D_min) / (D_empty - D_min)) * 100.0;
  level = constrain(level, 0.0, 100.0);
  return level;
}

double getLastUltrasonicDistance() {
  return lastUltrasonicDistance;
}

void setFeedHeight(double h) {
  feedHeight = h;
}