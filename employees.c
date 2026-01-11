// Name: Adam Korsunski, ID: 207803933
// Name: Nadav Rogers, ID: 326301520
#include "employees.h"
#include "log.h"


#define EMPLOYEE_FILE "f_employee.txt"

// טוען את רשימת העובדים מקובץ
Employee* loadEmployees() {
    FILE* file = fopen(EMPLOYEE_FILE, "r");
    if (!file) return NULL;

    Employee* head = NULL;
    Employee* temp = NULL;
    while (!feof(file)) {
        Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
        if (fscanf(file, "%s %s %s %u", newEmployee->username, newEmployee->firstname, newEmployee->password, &newEmployee->level) == 4) {
            newEmployee->next = NULL;
            if (!head) {
                head = newEmployee;
            }
            else {
                temp->next = newEmployee;
            }
            temp = newEmployee;
        }
        else {
            free(newEmployee);
        }
    }
    fclose(file);
    return head;
}

// שומר את רשימת העובדים לקובץ
void saveEmployees(Employee* head) {
    FILE* file = fopen(EMPLOYEE_FILE, "w");
    if (!file) return;

    Employee* temp = head;
    while (temp) {
        fprintf(file, "%s %s %s %u\n", temp->username, temp->firstname, temp->password, temp->level);
        temp = temp->next;
    }
    fclose(file);
}

// מחפש עובד לפי שם משתמש
Employee* searchEmployee(Employee* head, const char* adminUsername, const char* username) {
    Employee* temp = head;

    while (temp) {
        if (strcmp(temp->username, username) == 0) {
            logEmployeeAction(adminUsername, "Searched for an employee - Found");
            return temp;
        }
        temp = temp->next;
    }

    logEmployeeAction(adminUsername, "Searched for an employee - Not Found");
    return NULL;
}


// מוסיף עובד חדש למערכת
Employee* addEmployee(Employee* head, const char* adminUsername, const char* username, const char* firstname, const char* password, unsigned int level) {
    Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
    if (!newEmployee) {
        printf("Memory allocation failed!\n");
        return head;
    }

    strcpy(newEmployee->username, username);
    strcpy(newEmployee->firstname, firstname);
    strcpy(newEmployee->password, password);
    newEmployee->level = level;
    newEmployee->next = head;
    head = newEmployee;

    // רישום הפעולה בלוג עם שם המשתמש של המנהל שמבצע את ההוספה
    logEmployeeAction(adminUsername, "Added a new employee");

    // שמירת העובדים המעודכנים
    saveEmployees(head);

    printf("Employee added successfully!\n");
    return head;
}


// מסיר עובד לפי שם משתמש
Employee* removeEmployee(Employee* head, const char* adminUsername, const char* username) {
    Employee* temp = head, * prev = NULL;

    // בדיקה אם העובד הראשון הוא זה שצריך להימחק
    while (temp && strcmp(temp->username, username) == 0) {
        logEmployeeAction(adminUsername, "Removed an employee");  // רושם ללוג לפני מחיקה
        head = temp->next;
        free(temp);
        temp = head;
    }

    // חיפוש ומחיקת העובד מהרשימה
    while (temp) {
        while (temp && strcmp(temp->username, username) != 0) {
            prev = temp;
            temp = temp->next;
        }
        if (!temp) break;

        logEmployeeAction(adminUsername, "Removed an employee");  // רושם ללוג לפני מחיקה
        prev->next = temp->next;
        free(temp);
        temp = prev->next;
    }

    // שמירת רשימת העובדים המעודכנת
    saveEmployees(head);

    printf("Employee removed successfully!\n");
    return head;
}


// משחרר זיכרון של רשימת העובדים
void freeEmployeeList(Employee* head) {
    Employee* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// מציג את רשימת העובדים
void displayEmployees(Employee* head) {
    Employee* temp = head;
    printf("\nEmployees List:\n");
    while (temp) {
        printf("Username: %s | Name: %s | Level: %u\n",
            temp->username, temp->firstname, temp->level);
        temp = temp->next;
    }
}

// בודק אם קובץ העובדים קיים
int checkIfEmployeeFileExists(FILE* f_employee) {
    f_employee = fopen("f_employee.txt", "r");
    if (f_employee == NULL) {
        return 0;
    }
    fclose(f_employee);
    return 1;
}

// יוצר מנהל ברירת מחדל אם אין קובץ עובדים
void createDefaultAdmin(FILE* f_employee) {
    f_employee = fopen("f_employee.txt", "w");
    if (f_employee == NULL) {
        printf("Creating Failed\n");
        return;
    }
    Employee Admin = { "admin", "manager", "12345678", 1 };
    fprintf(f_employee, "%-20s %-10s %-10s %d\n",
        Admin.username, Admin.firstname, Admin.password, Admin.level);
    fclose(f_employee);
}

// מבצע התחברות עם 3 ניסיונות
Employee login(FILE* f_employee) {
    Employee currentEmployee = { 0 };
    int attempts = 0;

    while (attempts < 3) {
        printf("Enter username: ");
        scanf("%19s", currentEmployee.username);
        printf("Enter password: ");
        scanf("%9s", currentEmployee.password);

        f_employee = fopen("f_employee.txt", "r");
        if (!f_employee) {
            printf("Error opening file\n");
            return (Employee) { 0 };
        }

        Employee temp;
        while (fscanf(f_employee, "%19s %9s %9s %d", temp.username, temp.firstname, temp.password, &temp.level) != EOF)
        {
            if (strcmp(temp.username, currentEmployee.username) == 0 && strcmp(temp.password, currentEmployee.password) == 0)
            {
                fclose(f_employee);
                return temp;
            }
        }
        fclose(f_employee);
        attempts++;
        printf("Invalid login. Attempts left: %d\n", 3 - attempts);
    }
    return (Employee) { 0 }; // Failed login
}