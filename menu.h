// Name: Adam Korsunski, ID: 207803933
// Name: Nadav Rogers, ID: 326301520
#pragma once

#include "employees.h"
#include "customers.h"
#include "items.h"

// פונקציות ניהול תפריט
void showMenu(Employee* employees, Employee loggedInUser);
void showItemsMenu(Employee loggedInUser);
void showCustomersMenu(Employee loggedInUser);
void showEmployeesMenu(Employee* employees, Employee loggedInUser);
void sellItem(Employee loggedInUser);
void returnItem(Employee loggedInUser);
void updateItemMenu(Item* items, int count, Employee loggedInUser);