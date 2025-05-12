#include <Arduino.h>
#include "Config.h"
#include "FeedSensor.h"
#include "Feeder.h"
#include "Schedule.h"
#include "Display.h"
#include "Network.h"
#include "RTClib.h"

extern double foodLevelThreshold;

RTC_DS1307 rtc;
extern std::vector<DateTime> feedingSchedule;
extern const int *TIMEZONE;

void syncRTCWithNTP()
{
  configTime(0, 0, "pool.ntp.org");
  struct tm timeinfo;
  if (getLocalTime(&timeinfo))
  {
    rtc.adjust(DateTime(
        timeinfo.tm_year + 1900,
        timeinfo.tm_mon + 1,
        timeinfo.tm_mday,
        timeinfo.tm_hour,
        timeinfo.tm_min,
        timeinfo.tm_sec));
    Serial.println("RTC set from NTP (UTC)");
  }
  else
  {
    Serial.println("Failed to get time from NTP");
  }
}

void setup()
{
  Serial.begin(115200);
  loadConfig();
  connectWiFi();
  initFeedSensor();
  initFeeder(19);
  Wire.begin(21, 22);
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
  }
  syncRTCWithNTP();
  DateTime now = rtc.now();
  Serial.print("RTC time (UTC): ");
  Serial.print(now.year());
  Serial.print("-");
  Serial.print(now.month());
  Serial.print("-");
  Serial.print(now.day());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());
  initDisplay();
  connectWebSocket();
  postListenNotification();
}

void loop()
{
  wsClient.poll();

  static unsigned long lastFeedCheck = 0;
  const long feedInterval = 5000;
  static double lastUltrasonicReading = -100.0;
  static double lastReportedFeedLevel = -1.0;

  if (millis() - lastFeedCheck > feedInterval)
  {
    lastFeedCheck = millis();
    double ultrasonicReading = readFeedLevelPreciseCustomized();

    double newDistance = getLastUltrasonicDistance();

    if (fabs(newDistance - lastUltrasonicReading) > 0.1)
    {
      lastUltrasonicReading = newDistance;
      setFeedHeight(newDistance);
    }
  }

  double newFeedLevel = getFeedLevelFromHeightPrecise();
  String status = (newFeedLevel < foodLevelThreshold) ? "LOW" : "OK";
  showStatus((long)newFeedLevel, status);

  if (fabs(newFeedLevel - lastReportedFeedLevel) > 0.5)
  {
    postFeedLevel(newFeedLevel);
    lastReportedFeedLevel = newFeedLevel;
  }

  static uint32_t lastScheduleCheck = 0;
  static int lastTriggeredDay = -1;
  static int lastTriggeredHour = -1;
  static int lastTriggeredMinute = -1;

  if (millis() - lastScheduleCheck > 8000)
  {
    lastScheduleCheck = millis();
    DateTime now = rtc.now();
    Serial.print("RTC time (UTC): ");
    Serial.print(now.year());
    Serial.print("-");
    Serial.print(now.month());
    Serial.print("-");
    Serial.print(now.day());
    Serial.print(" ");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.println(now.second());

    now = now + TimeSpan((*TIMEZONE) * 3600);

    Serial.print("Current time: ");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.println(now.second());

    if (now.day() != lastTriggeredDay)
    {
      lastTriggeredHour = -1;
      lastTriggeredMinute = -1;
      lastTriggeredDay = now.day();
    }

    bool triggered = false;
    for (const auto &sched : feedingSchedule)
    {
      Serial.print("  Schedule: ");
      Serial.print(sched.hour());
      Serial.print(":");
      Serial.print(sched.minute());
      Serial.print(":");
      Serial.println(sched.second());

      if (now.hour() == sched.hour() && now.minute() == sched.minute())
      {
        if (lastTriggeredHour != now.hour() || lastTriggeredMinute != now.minute())
        {
          Serial.println("triggerFeeding(\"auto\") called!");
          triggerFeeding("auto");
          lastTriggeredHour = now.hour();
          lastTriggeredMinute = now.minute();
        }
        triggered = true;
        break;
      }
    }
    if (!triggered)
    {
      Serial.println("No schedule matched, triggerFeeding auto not called.");
    }
  }
}