#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 10
#define MAX_LIQUOR 20
#define MAX_USERNAME 20
#define MAX_PASSWORD 20
#define MAX_LIQUOR_NAME 50

// Global variables
struct User {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
};

struct Liquor {
    char name[MAX_LIQUOR_NAME];
    int price;
    int stock;
};

struct User users[MAX_USERS];
struct Liquor liquors[MAX_LIQUOR];
int num_users = 0;
int num_liquors = 0;

// Function prototypes
int is_username_valid(char *username);
int find_user_index(char *username);
int is_password_valid(char *password);
void create_account();
void login();
void display_liquors();
void order_liquor();
void load_data();
void save_data();
int strcmp_nocase(const char *str1, const char *str2);

int main() {
    load_data();  // Load data from files on startup

    int choice;
    while (1) {
        printf("\nLiquor Ordering System\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_account();
                break;
            case 2:
                login();
                break;
            case 3:
                save_data();  // Save data before exiting
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}

// Function to check if username is valid (alphanumeric)
int is_username_valid(char *username) {
    int len = strlen(username);
    for (int i = 0; i < len; i++) {
        if (!isalnum(username[i])) {
            return 0;
        }
    }
    return 1;
}

// Function to find the index of a user in the users array
int find_user_index(char *username) {
    for (int i = 0; i < num_users; i++) {
        if (strcmp_nocase(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to check if password is valid (at least 8 characters)
int is_password_valid(char *password) {
    return strlen(password) >= 8;
}

// Function to create a new user account
void create_account() {
    if (num_users == MAX_USERS) {
        printf("Maximum number of users reached!\n");
        return;
    }

    char username[MAX_USERNAME], password[MAX_PASSWORD];

    printf("Enter username: ");
    scanf("%s", username);

    // Validate username
    while (!is_username_valid(username)) {
        printf("Username must be alphanumeric. Please try again: ");
        scanf("%s", username);
    }

    if (find_user_index(username) != -1) {
        printf("Username already exists!\n");
        return;
    }

    printf("Enter password (minimum 8 characters): ");
    scanf("%s", password);

    // Validate password
    while (!is_password_valid(password)) {
        printf("Password must be at least 8 characters long. Please try again: ");
        scanf("%s", password);
    }

    strcpy(users[num_users].username, username);
    strcpy(users[num_users].password, password);
    num_users++;

    printf("Account created successfully!\n");
}

// Function to login a user
void login() {
    char username[MAX_USERNAME], password[MAX_PASSWORD];

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    int user_index = find_user_index(username);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 10
#define MAX_LIQUOR 20
#define MAX_USERNAME 20
#define MAX_PASSWORD 20
#define MAX_LIQUOR_NAME 50

// Global variables
struct User {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
};

struct Liquor {
    char name[MAX_LIQUOR_NAME];
    int price;
    int stock;
};

struct User users[MAX_USERS];
struct Liquor liquors[MAX_LIQUOR];
int num_users = 0;
int num_liquors = 0;

// Function prototypes
int is_username_valid(char *username);
int find_user_index(char *username);
int is_password_valid(char *password);
void create_account();
void login();
void display_liquors();
void order_liquor();
void load_data();
void save_data();
int strcmp_nocase(const char *str1, const char *str2);

// Function to compare strings ignoring case (custom)
int strcmp_nocase(const char *str1, const char *str2) {
    int i = 0;
    while (tolower(str1[i]) == tolower(str2[i]) && str1[i] != '\0') {
        i++;
    }
    return tolower(str1[i]) - tolower(str2[i]);
}

// Function to load data from files (replace with actual file operations)
void load_data() {
    // You'll need to implement file I/O functions here to read user and liquor data from files
    printf("** Load data functionality not implemented (replace with file I/O) **\n");
}

// Function to save data to files (replace with actual file operations)
void save_data() {
    // You'll need to implement file I/O functions here to write user and liquor data to files
    printf("** Save data functionality not implemented (replace with file I/O) **\n");
}

// Function to display available liquors
void display_liquors() {
    if (num_liquors == 0) {
        printf("No liquors available!\n");
        return;
    }

    printf("\nLiquors:\n");
    for (int i = 0; i < num_liquors; i++) {
        printf("%s (₱%d - %d in stock)\n", liquors[i].name, liquors[i].price, liquors[i].stock);
    }
}

// Function to order liquor
void order_liquor() {
    if (num_liquors == 0) {
        printf("No liquors available to order!\n");
        return;
    }

    char liquor_name[MAX_LIQUOR_NAME];
    int quantity, index;

    display_liquors();

    printf("Enter liquor name: ");
    scanf("%s", liquor_name);

    index = -1;
    for (int i = 0; i < num_liquors; i++) {
        if (strcmp_nocase(liquors[i].name, liquor_name) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Liquor not found!\n");
        return;
    }

    printf("Enter quantity: ");
    scanf("%d", &quantity);

    if (quantity > liquors[index].stock) {
        printf("Insufficient stock!\n");
        return;
    }

    printf("You ordered %d %s for a total of ₱%d\n", quantity, liquors[index].name, quantity * liquors[index].price);
    // Simulate reducing stock (actual order processing not implemented)
    liquors[index].stock -= quantity;
}

int main() {
    load_data();  // Load data from files on startup

    int choice;
    while (1) {
        printf("\nLiquor Ordering System\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Display Liquors\n");
        printf("4. Order Liquor\n");
        printf("