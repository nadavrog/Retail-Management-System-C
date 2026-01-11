// Name: Adam Korsunski, ID: 207803933
// Name: Nadav Rogers, ID: 326301520
#include "items.h"
#include "employees.h"
#include "log.h"
#define ITEM_FILE "items.bin"

// הפונקציה מוסיפה פריט לרשימה מקושרת בסדר עולה לפי המספר הסידורי
Item* insertItemSorted(Item* head, Item* newItem) {
    if (!head || atoi(newItem->id) < atoi(head->id)) {
        newItem->next = head;
        return newItem;
    }

    Item* current = head;
    while (current->next && atoi(current->next->id) < atoi(newItem->id)) {
        current = current->next;
    }

    newItem->next = current->next;
    current->next = newItem;
    return head;
}

// טוען פריטים מקובץ בינארי
Item* loadItems(int* count) {
    FILE* file = fopen(ITEM_FILE, "rb");
    if (!file) {
        *count = 0;
        printf("No items file found, starting empty.\n");
        return NULL;
    }

    if (fread(count, sizeof(int), 1, file) != 1) {
        fclose(file);
        *count = 0;
        printf("Error reading item count from file.\n");
        return NULL;
    }

    Item* head = NULL;
    for (int i = 0; i < *count; i++) {
        Item* newItem = (Item*)malloc(sizeof(Item));
        if (!newItem) {
            fclose(file);
            freeItemList(head);
            *count = 0;
            printf("Memory allocation failed while loading items.\n");
            return NULL;
        }

        memset(newItem, 0, sizeof(Item));  // מבטיח שכל השדות מאותחלים
        if (fread(newItem, sizeof(Item), 1, file) != 1) {
            free(newItem);
            fclose(file);
            freeItemList(head);
            *count = 0;
            printf("Error reading item from file.\n");
            return NULL;
        }

        newItem->next = NULL;
        head = insertItemSorted(head, newItem);
    }

    fclose(file);
    return head;
}


// שומר את רשימת הפריטים לקובץ בינארי
void saveItems(Item* items, int count) {
    FILE* file = fopen(ITEM_FILE, "wb");
    if (!file) return;

    fwrite(&count, sizeof(int), 1, file);

    Item* current = items;
    while (current) {
        fwrite(current, sizeof(Item), 1, file);
        current = current->next;
    }

    fclose(file);
}


// מחפש פריט לפי מזהה או שם
Item* searchItem(Item* items, const char* username, const char* searchTerm) {
    Item* current = items;
    while (current) {
        if (strcmp(current->id, searchTerm) == 0 || strcmp(current->name, searchTerm) == 0) {
            logItemAction(username, current->id, "Searched for an item - Found");
            return current;
        }
        current = current->next;
    }
    logItemAction(username, searchTerm, "Searched for an item - Not Found");
    return NULL;
}


// מוסיף פריט חדש למערך ומעדכן את הקובץ
Item* addItem(Item* items, int* count, const char* username, const char* id, const char* name, const char* category, int IsKosher, float price, int quantity) {
    Item* newItem = (Item*)malloc(sizeof(Item));
    if (!newItem) {
        logItemAction(username, id, "Failed to add item - Memory allocation failed");
        return items;
    }

    strcpy(newItem->id, id);
    strcpy(newItem->name, name);
    strcpy(newItem->category, category);
    newItem->IsKosher = IsKosher;
    getCurrentDate(newItem->EntryDate);
    newItem->price = price;
    newItem->quantity = quantity;
    newItem->next = NULL;

    // הכנסת הפריט החדש לרשימה בצורה ממיונת לפי id
    items = insertItemSorted(items, newItem);
    (*count)++;

    // שמירת הפריטים המעודכנים לקובץ
    saveItems(items, *count);

    // רישום פעולה בלוג
    logItemAction(username, id, "Added a new item");

    printf("Item added successfully! (Entry Date: %s)\n", newItem->EntryDate);
    return items;
}


// מסיר פריט מהמערך ומעדכן את הקובץ
Item* removeItem(Item* items, int* count, const char* username, const char* searchTerm) {
    if (!items) {
        logItemAction(username, searchTerm, "Tried to remove an item - List is empty");
        return NULL;
    }

    Item* current = items;
    Item* prev = NULL;

    // חיפוש הפריט להסרה
    while (current) {
        if (strcmp(current->id, searchTerm) == 0 || strcmp(current->name, searchTerm) == 0) {
            // אם הפריט בראש הרשימה
            if (!prev) {
                items = current->next;
            }
            else {
                prev->next = current->next;
            }

            logItemAction(username, current->id, "Removed an item");
            free(current);
            (*count)--;

            // שמירת הרשימה המעודכנת
            saveItems(items, *count);
            return items;
        }
        prev = current;
        current = current->next;
    }

    logItemAction(username, searchTerm, "Tried to remove an item - Not Found");
    return items;
}


// מעדכן את מאפייני הפריט (מחיר וכמות) ומעדכן בקובץ
void updateItemAttributes(Item* items, int count, const char* username, const char* id, float newPrice, int newQuantity) {
    Item* item = searchItem(items, username, id);
    if (item) {
        item->price = newPrice;
        item->quantity = newQuantity;
        saveItems(items, count);
        logItemAction(username, id, "Updated item attributes");
    }
    else {
        logItemAction(username, id, "Tried to update an item - Not Found");
        printf("Error: Item not found!\n");
    }
}


// משחרר את הזיכרון של רשימת הפריטים
void freeItemList(Item* items) {
    Item* current = items;
    while (current) {
        Item* temp = current;
        current = current->next;
        free(temp);
    }
}

// מציג את רשימת הפריטים
void displayItems(Item* items) {
    printf("\n=== Items List ===\n");
    if (!items) {  // אם הרשימה ריקה, נציג הודעה ולא נעבור על NULL
        printf("No items available.\n");
        return;
    }

    Item* current = items;
    while (current) {
        printf("ID: %s | Name: %s | Category: %s | Kosher: %s | Entry Date: %s | Price: %.2f | Quantity: %d\n",
            current->id, current->name, current->category,
            current->IsKosher ? "Yes" : "No", current->EntryDate,
            current->price, current->quantity);
        current = current->next;
    }
}