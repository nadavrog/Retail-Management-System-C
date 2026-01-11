// Name: Adam Korsunski, ID: 207803933
// Name: Nadav Rogers, ID: 326301520
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// פונקציות לניהול לוגים
void logAction(const char* category, const char* username, const char* action);
void logEmployeeAction(const char* username, const char* action);
void logCustomerAction(const char* username, const char* customerID, const char* action);
void logItemAction(const char* username, const char* itemID, const char* action);
void logSale(const char* username, const char* customerID, const char* itemID, int quantity);
void logReturn(const char* username, const char* customerID, const char* itemID, int quantity);
void logError(const char* errorMessage);
void getCurrentTime(char* buffer, int bufferSize);