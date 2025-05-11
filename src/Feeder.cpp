#include "Feeder.h"
#include <ESP32Servo.h>
#include "FeedSensor.h"
#include "Display.h"
#include "Network.h"

static Servo feederServo;

void initFeeder(int servoPin) {
  feederServo.attach(servoPin);
}

void triggerFeeding(const String &action, const String &historyId) {
  feederServo.write(90);
  delay(1000);
  feederServo.write(0);

  decreaseFeedHeight();
  double newFeedLevel = getFeedLevelFromHeight();

  if (action == "manual" && historyId.length() > 0) {
    patchHistoryFeedLevel(historyId, newFeedLevel);
  }

  postFeedLevel(newFeedLevel, historyId);
  showStatus((long)newFeedLevel, "FED (" + action + ")");
}