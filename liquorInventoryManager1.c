#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIQUOR 100
#define MAX_NAME 50
#define MAX_FILE_NAME "liquor.dat"

// Structure to store liquor information
typedef struct {
    char name[MAX_NAME];
    int quantity;
    float price;
} Liquor;

// Global variable to store liquor data (replace with file-based storage later)
Liquor liquor_data[MAX_LIQUOR];
int num_liquor = 0;  // Number of liquors currently stored

// Function prototypes
void display_menu();
void add_liquor();
void view_liquors();
void edit_liquor();
void delete_liquor();
void order_liquor(Liquor* liquor, int* quantity);
int find_liquor(const char* name);
void load_data();
void save_data();

int main() {
    load_data();  // Load data from file on startup (if implemented)

    int choice;
    do {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_liquor();
                break;
            case 2:
                view_liquors();
                break;
            case 3:
                edit_liquor();
                break;
            case 4:
                delete_liquor();
                break;
            case 5:
                printf("Exiting...\n");
                save_data();  // Save data to file before exiting (if implemented)
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

// Function to display the menu
void dispay_menu() {
    printf("\nLiquor Inventory Manager\n");
    printf("1. Add Liquor\n");
    printf("2. View Liquors\n");
    printf("3. Edit Liquor\n");
    printf("4. Delete Liquor\n");
    printf("5. Responsible Ordering Reminder\n");
    printf("6. Exit\n");
}

// Function to add a liquor
void add_liquor() {
    if (num_liquor >= MAX_LIQUOR) {
        printf("Maximum number of liquors reached.\n");
        return;
    }

    printf("Enter liquor name: ");
    fgets(liquor_data[num_liquor].name, MAX_NAME, stdin);

    // Ensure the name is null-terminated by checking its length using strlen
    int name_length = strlen(liquor_data[num_liquor].name);
    if (name_length == MAX_NAME - 1 && liquor_data[num_liquor].name[name_length - 1] == '\n') {
        liquor_data[num_liquor].name[name_length - 1] = '\0';  // Replace newline with null terminator
    }

    printf("Enter quantity: ");
    scanf("%d", &liquor_data[num_liquor].quantity);
    while (getchar() != '\n');  // Clear input buffer

    printf("Enter price (e.g., 19.99): ");
    scanf("%f", &liquor_data[num_liquor].price);
    while (getchar() != '\n');  // Clear input buffer

    num_liquor++;
    printf("Liquor added successfully.\n");
}

// Function to view all liquors
void view_liquors() {
    if (num_liquor == 0) {
        printf("No liquors available.\n");
        return;
    }

    printf("\nLiquor List:\n");
    printf("  Name\t\tQuantity\tPrice\n");
    printf("-------\t\t--------\t------\n");
    for (int i = 0; i < num_liquor; i++) {
        printf("  %-20s%d\t\t%.2f\n", liquor_data[i].name, liquor_data[i].quantity, liquor_data[i].price);
    }
}

// Function to edit a liquor
void edit_liquor() {
    if (num_liquor == 0) {
        printf("No liquors available to edit.\n");
        return;
    }

    char name[MAX_NAME];
    int index;

    printf("Enter the name of the liquor to edit: ");
    fgets(name, MAX_NAME, stdin);
    strtok(name, "\n");  // Remove newline character

    index = find_liquor(name);
    if (index == -1) {
        printf("Liquor not found.\n");
        return;
    }

    int choice;
    printf("Edit liquor details:\n");
    printf("1. Edit name\n");
    printf("2. Edit quantity\n");
    printf("3. Edit price\n");
    printf("4. Cancel\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    while (getchar() != '\n');  // Clear input buffer

    switch (choice) {
        case 1:
            printf("Enter new name: ");
            fgets(liquor_data[index].name, MAX_NAME, stdin);
            strtok(liquor_data[index].name, "\n");  // Remove newline character
            printf("Name updated successfully.\n");
            break;
        case 2:
            printf("Enter new quantity: ");
            scanf("%d", &liquor_data[index].quantity);
            while (getchar() != '\n');  // Clear input buffer
            printf("Quantity updated successfully.\n");
            break;
        case 3:
            printf("Enter new price (e.g., 19.99): ");
            scanf("%f", &liquor_data[index].price);
            while (getchar() != '\n');  // Clear input buffer
            printf("Price updated successfully.\n");
            break;
        case 4:
            printf("Edit cancelled.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

void load_data() {
    FILE* fp = fopen(MAX_FILE_NAME, "rb");  // Open file in binary read mode

    if (fp == NULL) {
        printf("Error opening file for loading data.\n");
        return;
    }

    // Read the number of liquors stored in the file (if available)
    int num_liquor_in_file;
    if (fread(&num_liquor_in_file, sizeof(int), 1, fp) != 1) {
        // Handle potential errors (e.g., empty file, corrupted data)
        printf("Error reading number of liquors from file.\n");
        fclose(fp);
        return;
    }

    // Limit loading based on available space and file data
    num_liquor = (num_liquor_in_file < MAX_LIQUOR) ? num_liquor_in_file : MAX_LIQUOR;

    // Read liquor data from the file
    if (fread(liquor_data, sizeof(Liquor), num_liquor, fp) != num_liquor) {
        // Handle potential errors (e.g., incomplete data)
        printf("Error reading liquor data from file.\n");
        fclose(fp);
        return;
    }

    fclose(fp);
    printf("Data loaded successfully (if any).\n");
}

void display_menu() {
    printf("\nLiquor Inventory Manager\n");
    printf("1. Add Liquor\n");
    printf("2. View Liquors\n");
    printf("3. Edit Liquor\n");
    printf("4. Delete Liquor\n");
    printf("5. Exit\n");
}

void delete_liquor() {
    if (num_liquor == 0) {
        printf("No liquors available to delete.\n");
        return;
    }

    char name[MAX_NAME];
    int index;

    printf("Enter the name of the liquor to delete: ");
    fgets(name, MAX_NAME, stdin);
    strtok(name, "\n");  // Remove newline character

    index = find_liquor(name);
    if (index == -1) {
        printf("Liquor not found.\n");
        return;
    }

    printf("Are you sure you want to delete '%s'? (y/n): ", name);
    char confirmation;
    scanf(" %c", &confirmation);  // Read a single character with space before

    while (getchar() != '\n');  // Clear input buffer

    if (confirmation == 'y' || confirmation == 'Y') {
        // Shift elements to remove the deleted liquor
        for (int i = index; i < num_liquor - 1; i++) {
            liquor_data[i] = liquor_data[i + 1];
        }
        num_liquor--;
        printf("Liquor deleted successfully.\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}

void save_data() {
    FILE* fp = fopen(MAX_FILE_NAME, "wb");  // Open file in binary write mode

    if (fp == NULL) {
        printf("Error opening file for saving data.\n");
        return;
    }

    // Write the number of liquors currently stored
    int num_liquors_to_save = num_liquor;  // Save actual number of liquors
    if (fwrite(&num_liquors_to_save, sizeof(int), 1, fp) != 1) {
        printf("Error writing number of liquors to file.\n");
        fclose(fp);
        return;
    }

    // Write the liquor data to the file
    if (fwrite(liquor_data, sizeof(Liquor), num_liquor, fp) != num_liquor) {
        printf("Error writing liquor data to file.\n");
        fclose(fp);
        return;
    }

    fclose(fp);
    printf("Data saved successfully.\n");
}

int find_liquor(const char* name) {
    for (int i = 0; i < num_liquor; i++) {
        if (strcmp(liquor_data[i].name, name) == 0) {
            return i;  // Return index if found
        }
    }
    return -1;  // Return -1 if not found
}