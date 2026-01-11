// Name: Adam Korsunski, ID: 207803933
// Name: Nadav Rogers, ID: 326301520
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Item {
    char id[10];
    char name[50];
    char category[25];
    int IsKosher;
    char EntryDate[11];
    float price;
    int quantity;
    struct Item* next;
} Item;

// פונקציות לניהול קובץ הפריטים הבינארי
Item* loadItems(int* count);
void saveItems(Item* items, int count);
Item* searchItem(Item* items, int count, const char* searchTerm);
Item* addItem(Item* items, int* count, const char* username, const char* id, const char* name, const char* category, int IsKosher, float price, int quantity);
Item* removeItem(Item* items, int* count, const char* username, const char* searchTerm);
Item* addSearchResult(Item* results, Item* foundItem, int* foundCount);
void updateItemAttributes(Item* items, int count, const char* username, const char* id, float newPrice, int newQuantity);
void freeItemList(Item* items);
void displayItems(Item* items, int count);