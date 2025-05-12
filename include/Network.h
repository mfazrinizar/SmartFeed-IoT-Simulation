#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoWebsockets.h>

extern websockets::WebsocketsClient wsClient;

void connectWiFi();
void connectWebSocket();
void postFeedLevel(double feedLevel, const String &historyId = "");
void postHistory(double feedLevel, const String &feedAction);
void patchHistoryFeedLevel(const String &historyId, double feedLevel);
void handleWebSocketMessage(websockets::WebsocketsMessage message);
void postListenNotification();