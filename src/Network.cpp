#include "Network.h"
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include "Config.h"
#include "Feeder.h"
#include "Schedule.h"
#include <FeedSensor.h>
#include <Display.h>

using namespace websockets;

WebsocketsClient wsClient;
bool wsConnected = false;
extern const int *TIMEZONE;
double foodLevelThreshold = 20.0;

void addApiKeyHeader(HTTPClient &http)
{
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-api-key", API_KEY);
}

void connectWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void postFeedLevel(long feedLevel, const String &historyId)
{
  if (WiFi.status() != WL_CONNECTED)
    return;
  HTTPClient http;
  String url = String(SERVER_URL) + "/api/devices/" + DEVICE_ID + "/feed-level";
  http.begin(url);
  addApiKeyHeader(http);
  StaticJsonDocument<128> doc;
  doc["feedLevel"] = feedLevel;
  if (historyId.length() > 0)
    doc["historyId"] = historyId;
  String body;
  serializeJson(doc, body);
  http.POST(body);
  http.end();
}

void patchHistoryFeedLevel(const String &historyId, double feedLevel)
{
  if (WiFi.status() != WL_CONNECTED)
    return;
  HTTPClient http;
  String url = String(SERVER_URL) + "/api/devices/" + DEVICE_ID + "/histories/" + historyId + "/feed-level";
  http.begin(url);
  addApiKeyHeader(http);
  StaticJsonDocument<64> doc;
  doc["feedLevel"] = feedLevel;
  String body;
  serializeJson(doc, body);
  http.PATCH(body);
  http.end();
}

void handleWebSocketMessage(WebsocketsMessage message)
{
  Serial.print("WebSocket received: ");
  Serial.println(message.data());

  StaticJsonDocument<1024> doc;
  DeserializationError err = deserializeJson(doc, message.data());
  if (err)
    return;

  if (doc.containsKey("event"))
  {
    String event = doc["event"].as<String>();
    if (event == "newHistory")
    {
      Serial.println("Received newHistory event");
      String feedAction = doc["data"]["feedAction"] | "manual";
      String historyId = doc["data"]["id"] | "";
      triggerFeeding(feedAction, historyId);
    }
    else if (event == "device")
    {
      Serial.println("Received device event");
      JsonObject data = doc["data"];
      if (data.containsKey("feedingSchedule"))
      {
        updateFeedingSchedule(data["feedingSchedule"].as<JsonArray>(), *TIMEZONE);
      }
      if (data.containsKey("foodLevelThreshold"))
      {
        if (data["foodLevelThreshold"].as<double>() != foodLevelThreshold)
        {
          foodLevelThreshold = data["foodLevelThreshold"].as<double>();
          Serial.print("Updated foodLevelThreshold: ");
          Serial.println(foodLevelThreshold);

          double currentFeedLevel = getFeedLevelFromHeight();
          String status = (currentFeedLevel < foodLevelThreshold) ? "LOW" : "OK";
          showStatus((long)currentFeedLevel, status);
        }
      }
    }
  }
}

void connectWebSocket()
{
  String wsUrl = String(WS_URL);
  Serial.print("Connecting to WebSocket: ");
  Serial.println(wsUrl);

  wsClient.onMessage(handleWebSocketMessage);
  wsClient.onEvent([](websockets::WebsocketsEvent event, String data)
                   {
    Serial.print("WebSocket event: ");
    Serial.println((int)event);
    Serial.print("Event data: ");
    Serial.println(data);
    if(event == websockets::WebsocketsEvent::ConnectionOpened) {
      wsConnected = true;
      Serial.println("WebSocket Connected");
      StaticJsonDocument<128> subDoc;
      subDoc["event"] = "subscribeHistories";
      subDoc["data"] = DEVICE_ID;
      String subMsg;
      serializeJson(subDoc, subMsg);
      wsClient.send(subMsg);
    }
    if(event == websockets::WebsocketsEvent::ConnectionClosed) {
      wsConnected = false;
      Serial.println("WebSocket Disconnected");
    } });

  wsClient.addHeader("x-api-key", API_KEY);

  wsClient.connect(wsUrl);
  Serial.println("wsClient.connect() called");
}

void postListenNotification()
{
  if (WiFi.status() != WL_CONNECTED)
    return;
  HTTPClient http;
  String url = String(SERVER_URL) + "/api/devices/" + DEVICE_ID + "/listen";
  http.begin(url);
  addApiKeyHeader(http);
  http.POST("");
  http.end();
}