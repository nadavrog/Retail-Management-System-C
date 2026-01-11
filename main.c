// Name: Adam Korsunski, ID: 207803933
// Name: Nadav Rogers, ID: 326301520
#include "employees.h"
#include "menu.h"
#include "log.h"

int main() {
    FILE* f_employee = NULL;

    // בדיקה אם קובץ העובדים קיים, ואם לא - יצירת מנהל ברירת מחדל
    if (!checkIfEmployeeFileExists(f_employee)) {
        createDefaultAdmin(f_employee);
        logEmployeeAction("SYSTEM", "Default admin created.");
        printf("No employee file found. Default admin has been created.\n");
    }

    // טעינת נתונים מהקבצים
    int itemCount;
    Employee* employees = loadEmployees();
    Customer* customers = loadCustomers();
    Item* items = loadItems(&itemCount);

    // תהליך התחברות עם 3 ניסיונות
    Employee loggedInUser = login(f_employee);
    if (loggedInUser.level == 0) {
        logError("User failed to log in after 3 attempts.");
        return 1;
    }

    // תיעוד התחברות בלוג
    logEmployeeAction(loggedInUser.username, "Logged in successfully");

    // הצגת תפריט מתאים למשתמש
    showMenu(employees, loggedInUser);

    // תיעוד יציאה מהמערכת
    logEmployeeAction(loggedInUser.username, "Logged out");

    // ניקוי זיכרון
    freeEmployeeList(employees);
    freeCustomerList(customers);
    freeItemList(items);

    printf("Thank you for using the system. Goodbye!\n");
    return 0;
}