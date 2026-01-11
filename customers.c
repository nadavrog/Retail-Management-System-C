// Name: Adam Korsunski, ID: 207803933
// Name: Nadav Rogers, ID: 326301520
#include "customers.h"
#include "log.h"
#include <time.h>

#define CUSTOMER_FILE "customers.txt"
// משווה תאריכים
int compareDates(const char* date1, const char* date2) {
    return strcmp(date1, date2);
}
// הכנסה לפי שיטת המיון בקובץ הלקוחות
Customer* insertCustomerSorted(Customer* head, Customer* newCustomer) {
    if (!head || compareDates(newCustomer->joinDate, head->joinDate) < 0) {
        newCustomer->next = head;
        return newCustomer;
    }

    Customer* current = head;
    while (current->next && compareDates(newCustomer->joinDate, current->next->joinDate) > 0) {
        current = current->next;
    }

    newCustomer->next = current->next;
    current->next = newCustomer;
    return head;
}
// טוען את רשימת הלקוחות מקובץ
Customer* loadCustomers() {
    FILE* file = fopen(CUSTOMER_FILE, "r");
    if (!file) return NULL;

    Customer* head = NULL;
    while (!feof(file)) {
        Customer* newCustomer = (Customer*)malloc(sizeof(Customer));
        if (fscanf(file, "%s %s %s %s", newCustomer->id, newCustomer->name, newCustomer->phone, newCustomer->joinDate) == 4) {
            newCustomer->next = NULL;
            head = insertCustomerSorted(head, newCustomer);
        }
        else {
            free(newCustomer);
        }
    }
    fclose(file);
    return head;
}
// שומר את רשימת הלקוחות לקובץ
void saveCustomers(Customer* customers) {
    FILE* file = fopen(CUSTOMER_FILE, "w");
    if (!file) return;

    Customer* current = customers;
    while (current) {
        fprintf(file, "%s %s %s %s\n",
            current->id, current->name, current->phone, current->joinDate);
        current = current->next;
    }

    fclose(file);
}
// מחפש לקוח לפי מזהה או שם
Customer* searchCustomer(Customer* head, const char* username, const char* searchTerm) {
    Customer* temp = head;

    while (temp) {
        if (strcmp(temp->id, searchTerm) == 0 || strcmp(temp->name, searchTerm) == 0) {
            logCustomerAction(username, temp->id, "Searched for a customer - Found");
            return temp;
        }
        temp = temp->next;
    }

    logCustomerAction(username, searchTerm, "Searched for a customer - Not Found");
    return NULL;
}
// פונקציה שמחזירה את התאריך היום
void getCurrentDate(char* dateBuffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(dateBuffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}
// מוסיף לקוח חדש
Customer* addCustomer(Customer* head, const char* username, const char* id, const char* name, const char* phone) {
    Customer* newCustomer = (Customer*)malloc(sizeof(Customer));
    if (!newCustomer) {
        printf("Memory allocation failed!\n");
        logCustomerAction(username, id, "Failed to add customer - Memory allocation failed");
        return head;
    }

    strcpy(newCustomer->id, id);
    strcpy(newCustomer->name, name);
    strcpy(newCustomer->phone, phone);
    getCurrentDate(newCustomer->joinDate); // ✅ מילוי תאריך הצטרפות אוטומטי

    newCustomer->next = head;
    head = newCustomer;

    // רישום פעולה בלוג עם שם המשתמש של העובד שמבצע את הפעולה
    logCustomerAction(username, id, "Added a new customer");

    // שמירת הלקוחות המעודכנים
    saveCustomers(head);

    printf("Customer added successfully! (Join Date: %s)\n", newCustomer->joinDate);
    return head;
}
// מסיר לקוח לפי מזהה או שם
Customer* removeCustomer(Customer* head, const char* username, const char* searchTerm) {
    Customer* temp = head, * prev = NULL;

    // בדיקה אם הלקוח הראשון הוא זה שצריך להימחק
    while (temp && (strcmp(temp->id, searchTerm) == 0 || strcmp(temp->name, searchTerm) == 0)) {
        logCustomerAction(username, temp->id, "Removed a customer"); // רושם ללוג לפני המחיקה
        head = temp->next;
        free(temp);
        temp = head;
    }

    // חיפוש הלקוח ברשימה
    while (temp) {
        while (temp && strcmp(temp->id, searchTerm) != 0 && strcmp(temp->name, searchTerm) != 0) {
            prev = temp;
            temp = temp->next;
        }
        if (!temp) {
            logCustomerAction(username, searchTerm, "Tried to remove a customer - Not Found");
            break;
        }

        logCustomerAction(username, temp->id, "Removed a customer"); // רושם ללוג לפני המחיקה
        prev->next = temp->next;
        free(temp);
        temp = prev->next;
    }

    // שמירת רשימת הלקוחות המעודכנת
    saveCustomers(head);

    printf("Customer removed successfully!\n");
    return head;
}
// משחרר זיכרון של רשימת הלקוחות
void freeCustomerList(Customer* head) {
    Customer* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
// מציג את רשימת הלקוחות
void displayCustomers(Customer* customers) {
    printf("\n=== Customers List ===\n");
    Customer* current = customers;
    while (current) {
        printf("ID: %s | Name: %s | Phone: %s | Join Date: %s\n",
            current->id, current->name, current->phone, current->joinDate);
        current = current->next;
    }
}
// מציג את היסטוריית הקנייה וההחזרה של הלקוח
void displayCustomerPurchaseHistory(const char* customerID)
{
    FILE* logFile = fopen("log.txt", "r");
    if (!logFile) {
        printf("Error opening log file!\n");
        return;
    }

    printf("\n=== Purchase History for Customer %s ===\n", customerID);
    char line[256];
    int found = 0;
    char searchPattern[50];
    sprintf(searchPattern, "Customer ID %s", customerID);

    while (fgets(line, sizeof(line), logFile)) {
        if (strstr(line, searchPattern) && (strstr(line, "SALE:") || strstr(line, "RETURN:"))) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf("No transactions found for this customer.\n");
    }

    fclose(logFile);
}