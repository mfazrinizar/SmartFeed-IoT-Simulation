#pragma once

long readFeedLevel();
double readFeedLevelPrecise();
double readFeedLevelPreciseCustomized();
double getFeedLevelFromHeight();
double getFeedLevelFromHeightPrecise();
void decreaseFeedHeight();
void decreaseFeedHeightPrecise();
void initFeedSensor();
double getLastUltrasonicDistance();
void setFeedHeight(double h);