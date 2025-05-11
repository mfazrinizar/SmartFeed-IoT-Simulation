#include "Schedule.h"

std::vector<DateTime> feedingSchedule;

void updateFeedingSchedule(const JsonArray &arr, int timezone) {
  feedingSchedule.clear();
  Serial.println("Updating feeding schedule:");
  for (JsonVariant v : arr) {
    DateTime dt((uint32_t)0);
    if (v.is<const char *>()) {
      dt = DateTime(v.as<const char *>());
    } else if (v.is<long>()) {
      dt = DateTime((uint32_t)v.as<long>());
    } else if (v.is<JsonObject>()) {
      JsonObject obj = v.as<JsonObject>();
      if (obj.containsKey("_seconds")) {
        dt = DateTime((uint32_t)obj["_seconds"]);
      }
    }
    dt = dt + TimeSpan(timezone * 3600);
    feedingSchedule.push_back(dt);
    Serial.print("  - ");
    Serial.print(dt.hour());
    Serial.print(":");
    Serial.print(dt.minute());
    Serial.print(":");
    Serial.println(dt.second());
  }
}