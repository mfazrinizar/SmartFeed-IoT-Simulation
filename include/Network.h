#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoWebsockets.h>

extern websockets::WebsocketsClient wsClient;

void connectWiFi();
void connectWebSocket();
void postFeedLevel(long feedLevel, const String &historyId = "");
void patchHistoryFeedLevel(const String &historyId, double feedLevel);
void handleWebSocketMessage(websockets::WebsocketsMessage message);
void postListenNotification();