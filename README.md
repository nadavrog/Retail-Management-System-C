# Retail Management System (C)

A final project for the Advanced Programming Laboratory. This application is a console-based management system designed to handle the core operations of a retail business, including inventory tracking, employee permissions, customer relationships, and transaction logging.

## 📖 Description

This system allows a business to manage its three main pillars: **Items**, **Employees**, and **Customers**. It utilizes persistent storage to save data between sessions, ensuring that inventory levels and customer databases remain up to date.

The project demonstrates advanced C programming concepts, specifically dynamic memory management using Linked Lists and mixed file handling (Text & Binary).

## 🚀 Key Features

### 1. Employee Management (RBAC)
The system implements a Role-Based Access Control system with three permission levels:
* **Admin (Level 1):** Full access to all features, including adding/removing employees.
* **Employee (Level 2):** Can manage items/customers and process sales/returns.
* **Practitioner (Level 3):** Read-only access to search/view data and add new entries.
* **Security:** Includes a login system with a 3-attempt lockout mechanism.

### 2. Inventory Control
* **Binary Storage:** Items are stored in binary files for efficiency.
* **Smart Sorting:** The inventory is maintained as a Linked List, automatically sorted by Item ID.
* **Attributes:** Tracks Item ID, Name, Category, Price, Quantity, and specific attributes like **Kosher Status**.
* **Search:** Supports searching by ID or Name.

### 3. Customer CRM
* **Sorted Database:** Customer lists are automatically sorted by their **Join Date**.
* **Purchase History:** Tracks customer transactions and allows employees to view purchase history.
* **Management:** Efficient addition, removal, and searching of customer records.

### 4. System Logging
A comprehensive `log.txt` file automatically records all system activities with timestamps, including:
* User Logins/Logouts.
* Sales and Returns.
* Item/Customer updates.
* System Errors.

## 🛠️ Technical Implementation

* **Language:** C
* **Data Structures:** Singly Linked Lists (used for Employees, Customers, and Items) to allow dynamic memory allocation.
* **File I/O:**
    * `items.bin`: Binary file for inventory data.
    * `customers.txt`: Text file for customer data.
    * `f_employee.txt`: Text file for employee credentials.
    * `log.txt`: Text file for the audit trail.

## 📦 How to Run

1.  Clone the repository.
2.  Compile the source files using a C compiler (e.g., GCC):
    ```bash
    gcc main.c employees.c customers.c items.c log.c menu.c -o system.exe
    ```
3.  Run the executable:
    ```bash
    ./system.exe
    ```
4.  **First Run:** If no employee file exists, the system will create a default Admin account:
    * **User:** `admin`
    * **Password:** `12345678`
