// Name: Adam Korsunski, ID: 207803933
// Name: Nadav Rogers, ID: 326301520
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Employee {
    char username[30];
    char firstname[30];
    char password[30];
    unsigned int level; // 1 - Admin, 2 - Employee, 3 - Practitioner
    struct Employee* next;
} Employee;

// פונקציות לניהול רשימת העובדים
Employee* loadEmployees();
void saveEmployees(Employee* head);
Employee* searchEmployee(Employee* head, const char* adminUsername, const char* username);
Employee* addEmployee(Employee* head, const char* adminUsername, const char* username, const char* firstname, const char* password, unsigned int level);
Employee* removeEmployee(Employee* head, const char* adminUsername, const char* username);
void freeEmployeeList(Employee* head);
void displayEmployees(Employee* head);

// פונקציות התחברות ואימות עובדים
int checkIfEmployeeFileExists(FILE* f_employee);
void createDefaultAdmin(FILE* f_employee);
Employee login(FILE* f_employee);
