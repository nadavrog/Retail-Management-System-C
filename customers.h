// Name: Adam Korsunski, ID: 207803933
// Name: Nadav Rogers, ID: 326301520
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Customer {
    char id[15];
    char name[50];
    char phone[15];
    char joinDate[11]; // תאריך הצטרפות בפורמט YYYY-MM-DD
    struct Customer* next;
} Customer;

// פונקציות לניהול לקוחות
Customer* loadCustomers();
void saveCustomers(Customer* head); 
Customer* searchCustomer(Customer* head, const char* username, const char* searchTerm);
Customer* addCustomer(Customer* head, const char* username, const char* id, const char* name, const char* phone);
Customer* removeCustomer(Customer* head, const char* username, const char* searchTerm);
void freeCustomerList(Customer* head);
void displayCustomers(Customer* head);
void displayCustomerPurchaseHistory(const char* customerID);