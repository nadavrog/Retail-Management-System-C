// Name: Adam Korsunski, ID: 207803933
// Name: Nadav Rogers, ID: 326301520
#include "log.h"
#define LOG_FILE "log.txt"

// מקבל את התאריך והשעה הנוכחיים בפורמט YYYY-MM-DD HH:MM:SS
void getCurrentTime(char* buffer, int bufferSize) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", t);
}

// פונקציה כללית לרישום פעולה בלוג
void logAction(const char* category, const char* username, const char* action) {
    FILE* file = fopen("log.txt", "a");
    if (!file) return;

    char timestamp[20];
    getCurrentTime(timestamp, sizeof(timestamp));
    fprintf(file, "[%s] %s: %s - %s\n", timestamp, category, username, action);
    fclose(file);
}
// רישום פעולות של עובדים
void logEmployeeAction(const char* username, const char* action) {
    logAction("EMPLOYEE", username, action);
}
// רישום פעולות של לקוחות
void logCustomerAction(const char* username, const char* customerID, const char* action) {
    FILE* file = fopen("log.txt", "a");
    if (!file) return;

    char timestamp[20];
    getCurrentTime(timestamp, sizeof(timestamp));
    fprintf(file, "[%s] CUSTOMER: %s performed action '%s' on Customer ID %s\n",
        timestamp, username, action, customerID);
    fclose(file);
}
// רישום פעולות על פריטים
void logItemAction(const char* username, const char* itemID, const char* action) {
    FILE* file = fopen("log.txt", "a");
    if (!file) return;

    char timestamp[20];
    getCurrentTime(timestamp, sizeof(timestamp));
    fprintf(file, "[%s] ITEM: %s performed action '%s' on Item ID %s\n",
        timestamp, username, action, itemID);
    fclose(file);
}

// רישום שגיאות בלוג
void logError(const char* errorMessage) {
    FILE* file = fopen("log.txt", "a");
    if (!file) return;

    char timestamp[20];
    getCurrentTime(timestamp, sizeof(timestamp));
    fprintf(file, "[%s] ERROR: %s\n", timestamp, errorMessage);
    fclose(file);
}
// רישום מכירה בלוג
void logSale(const char* username, const char* customerID, const char* itemID, int quantity) {
    FILE* file = fopen("log.txt", "a");
    if (!file) return;

    char timestamp[20];
    getCurrentTime(timestamp, sizeof(timestamp));
    fprintf(file, "[%s] SALE: %s sold %d of Item ID %s to Customer ID %s\n",
        timestamp, username, quantity, itemID, customerID);
    fclose(file);
}
// רישום החזרת פריט בלוג
void logReturn(const char* username, const char* customerID, const char* itemID, int quantity) {
    FILE* file = fopen("log.txt", "a");
    if (!file) return;

    char timestamp[20];
    getCurrentTime(timestamp, sizeof(timestamp));
    fprintf(file, "[%s] RETURN: %s processed return of %d of Item ID %s for Customer ID %s\n",
        timestamp, username, quantity, itemID, customerID);
    fclose(file);
}