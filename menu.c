// Name: Adam Korsunski, ID: 207803933
// Name: Nadav Rogers, ID: 326301520
#include "menu.h"
#include "log.h"
#include <time.h>

void showMenu(Employee* employees, Employee loggedInUser) {
    int mainChoice;
    do {
        printf("\n===== Pizza Management System =====\n");
        printf("1. Items\n");
        printf("2. Customers\n");
        if (loggedInUser.level == 1) {
            printf("3. Employees\n");
        }
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &mainChoice);

        switch (mainChoice) {
        case 1:
            showItemsMenu(loggedInUser);
            break;
        case 2:
            showCustomersMenu(loggedInUser);
            break;
        case 3:
            if (loggedInUser.level == 1 || loggedInUser.level == 2) {
                showEmployeesMenu(employees, loggedInUser);
            }
            else {
                printf("Invalid choice. Try again.\n");
            }
            break;
        case 4:
            printf("Logging out...\n");
            break;
        default:
            printf("Invalid choice. Try again.\n");
        }
    } while (mainChoice != 4);
}
// מציג את תפריט ניהול הלקוחות
void showCustomersMenu(Employee loggedInUser) {
    int subChoice;
    do {
        printf("\n=== Customers Menu ===\n");
        printf("1. View Customers\n");
        printf("2. View Purchase History\n");

        if (loggedInUser.level <= 3) {
            printf("3. Add Customer\n");
        }
        if (loggedInUser.level <= 2) {
            printf("4. Remove Customer\n");
        }
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &subChoice);

        Customer* customers = loadCustomers();

        switch (subChoice) {
        case 1:
            displayCustomers(customers);
            break;
        case 2:
            {
                char customerID[15];
                printf("Enter Customer ID: ");
                scanf("%14s", customerID);
                displayCustomerPurchaseHistory(customerID);
            }
            break;
        case 3:
            if (loggedInUser.level <= 3) {
                char id[15], name[50], phone[15];
                printf("Enter customer ID: ");
                scanf("%s", id);
                printf("Enter customer name: ");
                scanf("%s", name);
                printf("Enter phone number: ");
                scanf("%s", phone);
                customers = addCustomer(customers, loggedInUser.username, id, name, phone);
            }
            else {
                printf("Invalid choice. Try again.\n");
            }
            break;
        case 4:
            if (loggedInUser.level <= 2) {
                char id[15];
                printf("Enter customer ID to remove: ");
                scanf("%s", id);
                customers = removeCustomer(customers, loggedInUser.username, id);
            }
            else {
                printf("Invalid choice. Try again.\n");
            }
            break;
        case 5:
            break;
        default:
            printf("Invalid choice. Try again.\n");
        }
    } while (subChoice != 5);
}
// מציג את תפריט ניהול העובדים
void showEmployeesMenu(Employee* employees, Employee loggedInUser) {
    if (loggedInUser.level != 1) {
        printf("Access denied! Only Admins can manage employees.\n"); // במידה ועובד או מתלמד הכניסו 3 בתפריט למרות שאסור להם, נוודא שאכן אין לו גישה
        return;
    }

    int subChoice;
    do {
        printf("\n=== Employees Menu ===\n");
        printf("1. View Employees\n");
        printf("2. Add Employee\n");
        printf("3. Remove Employee\n");
        printf("4. Search Employee\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &subChoice);

        switch (subChoice) {
        case 1:
            displayEmployees(employees);
            break;
        case 2: {
            char username[30], firstname[30], password[30];
            unsigned int level;
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter first name: ");
            scanf("%s", firstname);
            printf("Enter password: ");
            scanf("%s", password);
            printf("Enter level (1 = Admin, 2 = Employee, 3 = Practitioner): ");
            scanf("%u", &level);
            employees = addEmployee(employees, loggedInUser.username, username, firstname, password, level);
            break;
        }
        case 3: {
            char username[30];
            printf("Enter username to remove: ");
            scanf("%s", username);
            employees = removeEmployee(employees, loggedInUser.username, username);
            break;
        }
        case 4: { // חיפוש עובד לפי שם משתמש עם לוג
            char username[30];
            printf("Enter username to search: ");
            scanf("%s", username);

            Employee* foundEmployee = searchEmployee(employees, loggedInUser.username, username);
            if (foundEmployee) {
                printf("\n=== Employee Found ===\n");
                printf("Username: %s\n", foundEmployee->username);
                printf("First Name: %s\n", foundEmployee->firstname);
                printf("Access Level: %u\n", foundEmployee->level);
                logEmployeeAction(loggedInUser.username, "Searched for an employee - Found");
            }
            else {
                printf("Employee not found!\n");
                logEmployeeAction(loggedInUser.username, "Searched for an employee - Not Found");
            }
            break;
        }
        case 5:
            break;
        default:
            printf("Invalid choice. Try again.\n");
        }
    } while (subChoice != 5);
}
// מציג את תפריט ניהול הפריטים כולל מכירת מוצרים
void showItemsMenu(Employee loggedInUser) {
    int subChoice;
    Item* items = NULL;
    int itemCount = 0;
    items = loadItems(&itemCount);

    do {
        printf("\n=== Items Menu ===\n");
        printf("1. View Items\n");

        if (loggedInUser.level <= 3) {
            printf("2. Add Item\n");
        }
        if (loggedInUser.level <= 2) {
            printf("3. Remove Item\n");
            printf("4. Update Item (Advanced Search)\n");
            printf("5. Sell Item\n");
            printf("6. Return Item\n");
        }   
        printf("7. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &subChoice);

        switch (subChoice) {
        case 1:
            displayItems(items, itemCount);
            break;
        case 2:
            if (loggedInUser.level <= 3) {
                char id[10], name[50], category[25];
                int isKosher, quantity;
                float price;

                printf("Enter item ID: ");
                scanf("%s", id);
                printf("Enter item name: ");
                scanf("%s", name);
                printf("Enter category: ");
                scanf("%s", category);
                printf("Is Kosher (1 = Yes, 0 = No): ");
                scanf("%d", &isKosher);
                printf("Enter item price: ");
                scanf("%f", &price);
                printf("Enter quantity: ");
                scanf("%d", &quantity);

                items = addItem(items, &itemCount, loggedInUser.username, id, name, category, isKosher, price, quantity);
                freeItemList(items);
                items = loadItems(&itemCount);

                // מציאת הפריט האחרון ברשימה כדי להדפיס אותו
                Item* lastItem = items;
                while (lastItem && lastItem->next) {
                    lastItem = lastItem->next;
                }

                if (lastItem) {
                    printf("Item added: %s (Entry Date: %s)\n", lastItem->name, lastItem->EntryDate);
                }
            }
            break;
        case 3:
            if (loggedInUser.level <= 2) {
                char id[10];
                printf("Enter item ID to remove: ");
                scanf("%s", id);
                items = removeItem(items, &itemCount, loggedInUser.username, id);
                freeItemList(items);
                items = loadItems(&itemCount);
            }
            break;
        case 4:
            if (loggedInUser.level <= 2) {
                updateItemMenu(items, itemCount, loggedInUser);
                freeItemList(items);
                items = loadItems(&itemCount);
            }
            break;
        case 5:
            if (loggedInUser.level <= 2) {
                sellItem(loggedInUser);
                freeItemList(items);
                items = loadItems(&itemCount);
            }
            break;
        case 6:
            if (loggedInUser.level <= 2) {
                returnItem(loggedInUser);
                freeItemList(items);
                items = loadItems(&itemCount);
            }
            break;
        case 7:
            break;
        default:
            printf("Invalid choice. Try again.\n");
        }
    } while (subChoice != 7);
    freeItemList(items);
}
// פונקציה לחיפוש מתקדם על פי קריטריון שנבחר
Item* searchItems(Item* items, int* foundCount) {
    int searchChoice;
    *foundCount = 0;
    Item* results = NULL;
    Item* current = items;

    printf("\n=== Choose Search Method ===\n");
    printf("1. Search by Name or Category\n");
    printf("2. Search by Price (Greater / Smaller)\n");
    printf("3. Search by Kosher Status\n");
    printf("4. Search by Entry Date\n");
    printf("Enter your choice: ");
    scanf("%d", &searchChoice);

    switch (searchChoice) {
    case 1: { // חיפוש לפי שם או קטגוריה
        char searchTerm[50];
        printf("Enter Name or Category: ");
        scanf("%s", searchTerm);
        while (current) {
            if (strcmp(current->name, searchTerm) == 0 || strcmp(current->category, searchTerm) == 0) {
                results = addSearchResult(results, current, foundCount);
            }
            current = current->next;
        }
        break;
    }
    case 2: { // חיפוש לפי מחיר
        float priceLimit;
        int comparison;
        printf("1. Greater than price\n2. Less than price\n");
        scanf("%d", &comparison);
        printf("Enter Price: ");
        scanf("%f", &priceLimit);
        while (current) {
            if ((comparison == 1 && current->price > priceLimit) ||
                (comparison == 2 && current->price < priceLimit)) {
                results = addSearchResult(results, current, foundCount);
            }
            current = current->next;
        }
        break;
    }
    case 3: { // חיפוש לפי כשרות
        int kosherChoice;
        printf("1. Only Kosher\n2. Only Non-Kosher\n");
        scanf("%d", &kosherChoice);
        while (current) {
            if ((kosherChoice == 1 && current->IsKosher == 1) ||
                (kosherChoice == 2 && current->IsKosher == 0)) {
                results = addSearchResult(results, current, foundCount);
            }
            current = current->next;
        }
        break;
    }
    case 4: { // חיפוש לפי תאריך
        char date[11];
        int dateOption;
        printf("1. Exact Date\n2. Before Date\n3. After Date\n");
        scanf("%d", &dateOption);
        printf("Enter Date (YYYY-MM-DD): ");
        scanf("%s", date);
        while (current) {
            if ((dateOption == 1 && strcmp(current->EntryDate, date) == 0) ||
                (dateOption == 2 && strcmp(current->EntryDate, date) < 0) ||
                (dateOption == 3 && strcmp(current->EntryDate, date) > 0)) {
                results = addSearchResult(results, current, foundCount);
            }
            current = current->next;
        }
        break;
    }
    default:
        printf("Invalid choice. Returning...\n");
        return NULL;
    }

    if (*foundCount == 0) {
        printf("No items found.\n");
        return NULL;
    }

    printf("\n=== Found Items ===\n");
    current = results;
    while (current) {
        printf("ID: %s | Name: %s | Category: %s | Price: %.2f | Kosher: %s | Date: %s | Quantity: %d\n",
            current->id, current->name, current->category,
            current->price, current->IsKosher ? "Yes" : "No",
            current->EntryDate, current->quantity);
        current = current->next;
    }

    return results;
}
// פונקציה שמציגה את תוצאת החיפוש המתקדם
Item* addSearchResult(Item* results, Item* foundItem, int* foundCount) {
    Item* newItem = (Item*)malloc(sizeof(Item));
    if (!newItem) return results; // אם אין זיכרון, לא מוסיפים את התוצאה

    // העתקת נתוני הפריט שנמצא
    strcpy(newItem->id, foundItem->id);
    strcpy(newItem->name, foundItem->name);
    strcpy(newItem->category, foundItem->category);
    newItem->IsKosher = foundItem->IsKosher;
    strcpy(newItem->EntryDate, foundItem->EntryDate);
    newItem->price = foundItem->price;
    newItem->quantity = foundItem->quantity;
    newItem->next = results;

    (*foundCount)++;
    return newItem;
}
// פונקציה לעדכון פריט
void updateItemMenu(Item* items, int count, Employee loggedInUser) {
    int foundCount;
    Item* searchResults = searchItems(items, &foundCount);
    if (!searchResults) return;

    char id[10];
    float newPrice;
    int newQuantity;
    printf("Enter item ID to update from found list: ");
    scanf("%s", id);

    Item* current = searchResults; // מעבר דרך רשימת התוצאות
    while (current) {
        if (strcmp(current->id, id) == 0) {
            printf("Enter new price: ");
            scanf("%f", &newPrice);
            printf("Enter new quantity: ");
            scanf("%d", &newQuantity);
            updateItemAttributes(items, count, loggedInUser.username, id, newPrice, newQuantity);
            printf("Item updated successfully!\n");
            break;
        }
        current = current->next;
    }

    freeItemList(searchResults); // משחרר את כל הרשימה
}
// פונקציה למכירת מוצר ללקוח
void sellItem(Employee loggedInUser) {
    if (loggedInUser.level > 2) {
        printf("Access denied! Only Admins and Employees can sell items.\n"); // במידה ומתלמד הכניס 5 בתפריט למרות שאסור להם, נוודא שאכן אין לו גישה
        return;
    }

    char customerID[10], itemID[10];
    int quantity, itemCount = 0, customerCount = 0;

    printf("\n=== Sell Item ===\n");

    // טעינת הלקוחות מהקובץ
    Customer* customers = loadCustomers(&customerCount);
    if (!customers) {
        printf("Error: Could not load customers.\n");
        return;
    }

    printf("Enter Customer ID: ");
    scanf("%s", customerID);

    // חיפוש לקוח
    Customer* customer = searchCustomer(customers, loggedInUser.username, customerID);
    if (!customer) {
        printf("Customer not found!\n");
        freeCustomerList(customers);
        return;
    }

    // טעינת הפריטים מהקובץ
    Item* items = loadItems(&itemCount);
    if (!items) {
        printf("Error: Could not load items.\n");
        freeCustomerList(customers);
        return;
    }

    printf("Enter Item ID to sell: ");
    scanf("%s", itemID);

    // חיפוש פריט
    Item* item = searchItem(items, loggedInUser.username, itemID);
    if (!item) {
        printf("Item not found!\n");
        freeItemList(items);
        freeCustomerList(customers);
        return;
    }

    printf("Enter quantity to sell: ");
    scanf("%d", &quantity);

    // בדיקה שהכמות חוקית
    if (quantity <= 0) {
        printf("Error: Invalid quantity!\n");
        freeItemList(items);
        freeCustomerList(customers);
        return;
    }

    // אם הכמות גדולה מ-3, לתת אפשרות לשנות או לבטל
    if (quantity > 3) {
        printf("Warning: You can only sell up to 3 items per transaction.\n");
        printf("Would you like to sell 3 items instead? (1 = Yes, 0 = No): ");
        int choice;
        scanf("%d", &choice);
        if (choice == 0) {
            printf("Transaction cancelled.\n");
            freeItemList(items);
            freeCustomerList(customers);
            return;
        }
        quantity = 3;
    }

    // בדיקה האם יש מלאי מספק
    if (item->quantity < quantity) {
        printf("Not enough stock available!\n");
        freeItemList(items);
        freeCustomerList(customers);
        return;
    }

    // עדכון מלאי
    item->quantity -= quantity;
    saveItems(items, itemCount); // שמירת השינויים

    // רישום המכירה בקובץ הלוג
    logSale(loggedInUser.username, customer->id, item->id, quantity);
    printf("Transaction successful!\n");

    // שחרור זיכרון
    freeItemList(items);
    freeCustomerList(customers);
}
// מחזיר מוצר ללקוח
void returnItem(Employee loggedInUser) {
    if (loggedInUser.level > 2) {
        printf("Access denied! Only Admins and Employees can process returns.\n"); // במידה ומתלמד הכניס 6 בתפריט למרות שאסור להם, נוודא שאכן אין לו גישה
        return;
    }

    char customerID[15], itemID[10], purchaseDate[11];
    int quantity, found = 0, purchasedQuantity = 0, returnedQuantity = 0;

    printf("\n=== Return Item ===\n");
    printf("Enter Customer ID: ");
    scanf("%14s", customerID);

    char customerSearch[30];
    sprintf(customerSearch, "Customer ID %s", customerID);

    // קריאת היסטוריה
    FILE* logFile = fopen("log.txt", "r");
    if (!logFile) {
        printf("Error: Unable to open log file.\n");
        return;
    }

    printf("\n=== Purchase History for Customer %s ===\n", customerID);
    char line[256];
    while (fgets(line, sizeof(line), logFile)) {
        if (strstr(line, customerSearch) && (strstr(line, "SALE:") || strstr(line, "RETURN:"))) {
            printf("%s", line);
            found = 1;
        }
    }
    fclose(logFile);

    if (!found) {
        printf("No purchases found for this customer.\n");
        return;
    }

    // קבלת פרטי החזרה
    printf("Enter Item ID to return: ");
    scanf("%9s", itemID);
    printf("Enter Purchase Date (YYYY-MM-DD): ");
    scanf("%10s", purchaseDate);

    // חיפוש רכישה תואמת
    logFile = fopen("log.txt", "r");
    if (logFile) {
        char itemSearch[30];
        sprintf(itemSearch, "Item ID %s", itemID);

        while (fgets(line, sizeof(line), logFile)) {
            if (strstr(line, customerSearch) && strstr(line, "SALE:") && strstr(line, itemSearch)) {
                char logDate[11];
                // חילוץ תאריך מדויק
                if (sscanf(line, "[%10[^]]", logDate) == 1 && strcmp(logDate, purchaseDate) == 0) {
                    char* restOfLine = strchr(line, ']');
                    if (restOfLine) {
                        int qty;
                        // חילוף כמות מהחלק השני של השורה
                        if (sscanf(restOfLine + 1, "%*s %*s %*s %d", &qty) == 1) {
                            purchasedQuantity += qty;
                        }
                    }
                }
            }
        }
        fclose(logFile);
    }

    if (purchasedQuantity == 0) {
        printf("Error: No matching purchase found for Item ID %s on date %s!\n", itemID, purchaseDate);
        return;
    }

    // בדיקת תוקף תאריך
    char currentDate[11];
    getCurrentTime(currentDate, sizeof(currentDate));
    if (daysBetweenDates(purchaseDate, currentDate) > 14) {
        printf("Return period expired! (14 days)\n");
        return;
    }

    // חישוב החזרות קודמות
    logFile = fopen("log.txt", "r");
    if (logFile) {
        char itemSearch[30];
        sprintf(itemSearch, "Item ID %s", itemID);

        while (fgets(line, sizeof(line), logFile)) {
            if (strstr(line, customerSearch) && strstr(line, "RETURN:") && strstr(line, itemSearch)) {
                char logDate[11];
                if (sscanf(line, "[%10[^]]", logDate) == 1 && strcmp(logDate, purchaseDate) == 0) {
                    char* restOfLine = strchr(line, ']');
                    if (restOfLine) {
                        int qty;
                        // חילוף כמות מהחלק השני של השורה
                        if (sscanf(restOfLine + 1, "%*s %*s %*s %*s %d", &qty) == 1) {
                            returnedQuantity += qty;
                        }
                    }
                }
            }
        }
        fclose(logFile);
    }

    // בדיקת כמות חוקית
    printf("Enter quantity to return: ");
    if (scanf("%d", &quantity) != 1 || quantity <= 0) {
        printf("Error: Invalid quantity!\n");
        return;
    }

    if (quantity + returnedQuantity > purchasedQuantity) {
        printf("Error: Maximum allowed return is %d\n", purchasedQuantity - returnedQuantity);
        return;
    }

    // עדכון המלאי
    int itemCount = 0;
    Item* items = loadItems(&itemCount);
    Item* targetItem = searchItem(items, loggedInUser.username, itemID);
    if (targetItem) {
        targetItem->quantity += quantity;
        saveItems(items, itemCount);
        logReturn(loggedInUser.username, customerID, itemID, quantity);
        printf("Successfully returned %d of %s!\n", quantity, targetItem->name);
    }
    else {
        printf("Error: Item not found in stock!\n");
    }
    freeItemList(items);
}
// מחשב את ההפרש בימים בין שני תאריכים
int daysBetweenDates(const char* date1, const char* date2) {
    struct tm tm1 = { 0 }, tm2 = { 0 };

    // המרת תאריך ראשון
    sscanf(date1, "%d-%d-%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);
    tm1.tm_year -= 1900;  // שנה בפורמט struct tm
    tm1.tm_mon -= 1;      // חודשים בפורמט struct tm

    // המרת תאריך שני
    sscanf(date2, "%d-%d-%d", &tm2.tm_year, &tm2.tm_mon, &tm2.tm_mday);
    tm2.tm_year -= 1900;
    tm2.tm_mon -= 1;

    // חישוב הפרש הימים
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    return (int)difftime(t2, t1) / (60 * 60 * 24);
}