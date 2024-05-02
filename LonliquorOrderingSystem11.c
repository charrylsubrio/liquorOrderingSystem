#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
int find_liquor_index(char *liquor_name);
void export_receipt(char *liquor_name, int quantity, float total_amount);

int main() {
	load_data();  // Load data from files on startup

	int choice, i, valid_input;
	do {
		printf("\nLiquor Ordering System\n");
		printf("1. Create Account\n");
		printf("2. Login\n");
		printf("3. Exit\n");
		printf("Enter your choice: ");
//        scanf("%d", &choice);

		if (scanf("%d", &choice) != 1) {
			system("cls");
			printf("INVALID INPUT. PLEASE ENTER A NUMBER.\n");
			while (getchar() != '\n');
		} else if (choice <=0 || choice >= 4) {
			system("cls");
			printf("INVALID CHOICE. PLEASE TRY AGAIN.\n");
			while (getchar() != '\n');
		} else {
			valid_input = 1;
		} //LOMARDA

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
//            default:
//                printf("Invalid choice!\n");
		}
	} while (!valid_input || choice !=3);

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
	
	system("cls"); //LOMARDA
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
	
	system("cls"); //LOMARDA
	printf("Account created successfully!\n");
}

// Function to login a user
void login() {
	char username[MAX_USERNAME], password[MAX_PASSWORD];
	int attempts = 3; // Optional: Set number of allowed login attempts

	printf("Enter username: ");
	scanf("%s", username);

	printf("Enter password: ");
	scanf("%s", password);

	int user_index = find_user_index(username);

	while (user_index == -1 && attempts > 0) {
		printf("Invalid username or password. Please try again.\n");
		attempts--; // Decrement attempts remaining (optional)

		printf("Enter username: ");
		scanf("%s", username);

		printf("Enter password: ");
		scanf("%s", password); //LOMARDA (password must be case-sensitive)

		user_index = find_user_index(username);
	}

	if (user_index != -1) {
		// Successful login
		printf("Login successful!\n");

		// New menu for logged-in user (**inner loop**)
		int logged_in_choice;
		while (1) {
			printf("\nLogged-in Menu\n");
			printf("1. Display Liquors\n");
			printf("2. Order Liquor\n");
			printf("3. Exit Logged-in Menu\n");
			printf("Enter your choice: ");
			scanf("%d", &logged_in_choice);

			switch (logged_in_choice) {
				case 1:
					display_liquors(); // Call function to display liquors
					break;
				case 2:
					order_liquor();   // Call function to order liquor
					break;
				case 3:
					break; // Exit the inner loop (logged-in menu)
				default:
					printf("Invalid choice!\n");
			}

			// Exit the inner loop if user chooses option 3
			if (logged_in_choice == 3) {
				break;
			}
		}
	} else {
		// All attempts exhausted (optional)
		if (attempts == 0) {
			printf("Maximum login attempts reached. Exiting...\n");
		} else {
			printf("Login failed.\n");
		}
	}
}

void load_data() {
	FILE *user_file = fopen("users.txt", "r"); // Open user data file for reading
	FILE *liquor_file = fopen("liquors.txt", "r"); // Open liquor data file for reading

	// Check if files opened successfully
	if (user_file == NULL || liquor_file == NULL) {
		printf("Error opening data files!\n");
		return; // Handle error or exit
	}

	// Read user data (example format: username,password)
	while (fscanf(user_file, "%s %s", users[num_users].username, users[num_users].password) != EOF) {
		num_users++; // Increment user count after successful read
		if (num_users >= MAX_USERS) {
			break; // Reached maximum number of users
		}
	}

	// Read liquor data (example format: name,price,stock)
	while (fscanf(liquor_file, "%s %d %d", liquors[num_liquors].name, &liquors[num_liquors].price, &liquors[num_liquors].stock) != EOF) {
		num_liquors++; // Increment liquor count after successful read
		if (num_liquors >= MAX_LIQUOR) {
			break; // Reached maximum number of liquors
		}
	}

	// Close data files
	fclose(user_file);
	fclose(liquor_file);
}

void save_data() {
	FILE *user_file = fopen("users.txt", "w"); // Open user data file for writing (overwrite)
	FILE *liquor_file = fopen("liquors.txt", "w"); // Open liquor data file for writing (overwrite)

	// Check if files opened successfully
	if (user_file == NULL || liquor_file == NULL) {
		printf("Error opening data files for saving!\n");
		return; // Handle error
	}

	// Write user data (example format: username,password)
	for (int i = 0; i < num_users; i++) {
		fprintf(user_file, "%s %s\n", users[i].username, users[i].password);
	}

	// Write liquor data (example format: name,price,stock)
	for (int j = 0; j < num_liquors; j++) {
		fprintf(liquor_file, "%s %d %d\n", liquors[j].name, liquors[j].price, liquors[j].stock);
	}

	// Close data files
	fclose(user_file);
	fclose(liquor_file);

	printf("Data saved successfully!\n");
}

int strcmp_nocase(const char *str1, const char *str2) {
	while (*str1 && *str2) {
		char c1 = toupper((unsigned char)*str1++);
		char c2 = toupper((unsigned char)*str2++);
		if (c1 != c2) {
			return c1 - c2;
		}
	}
	return *str1 - *str2;
}

void display_liquors() {
	printf("\nLiquor List:\n");
	printf("  Name\t\tPrice\tStock\n");
	printf("-------\t\t-------\t-------\n");

	// Loop through all liquors
	for (int i = 0; i < num_liquors; i++) {
		printf("  %-20s\t%d\t%d\n", liquors[i].name, liquors[i].price, liquors[i].stock);
	}

	printf("\n");
}

void order_liquor() {
	char liquor_name[MAX_LIQUOR_NAME];
	int quantity;
	float total_amount;

	printf("\nEnter the name of the liquor you want to order: ");
	scanf("%s", liquor_name); // Read liquor name into a string variable

	// Find the liquor index based on the name entered by the user
	int liquor_index = find_liquor_index(liquor_name);

	if (liquor_index == -1) {
		printf("Liquor not found!\n");
		return;
	}

	printf("Enter the quantity you want to order: ");
	scanf("%d", &quantity);

	// Check if sufficient stock is available
	if (liquors[liquor_index].stock < quantity) {
		printf("Insufficient stock. Only %d %s available.\n", liquors[liquor_index].stock, liquors[liquor_index].name);
		return;
	}

	// Calculate total amount
	total_amount = (float)liquors[liquor_index].price * quantity;

	// Simulate order processing (update stock, display order confirmation)
	liquors[liquor_index].stock -= quantity;
	printf("Order successful! You have ordered %d %s for a total of $%.2f.\n", quantity, liquors[liquor_index].name, total_amount);

	// Export order details to a receipt file
	export_receipt(liquor_name, quantity, total_amount);
}

void export_receipt(char *liquor_name, int quantity, float total_amount) {
	FILE *receipt_file;

	receipt_file = fopen("receipt.txt", "w"); // Open receipt file for writing

	if (receipt_file == NULL) {
		printf("Error creating receipt file!\n");
		return;
	}

	// Write order details to the receipt file
	fprintf(receipt_file, "Liquor Ordering System Receipt\n");
	fprintf(receipt_file, "--------------------------------\n");
	fprintf(receipt_file, "Liquor Name: %s\n", liquor_name);
	fprintf(receipt_file, "Quantity: %d\n", quantity);
	fprintf(receipt_file, "Total Amount: $%.2f\n", total_amount);
	fprintf(receipt_file, "--------------------------------\n");
	fprintf(receipt_file, "Thank you for your order!\n");

	fclose(receipt_file); // Close the receipt file

	printf("Order details saved to receipt.txt\n");
}

// Function to find the index of a liquor in the liquors array based on name (implementation needed)
int find_liquor_index(char *liquor_name) {
	// Implement logic to search the liquors array by name and return the index
	// You can use techniques similar to find_user_index
	for (int i = 0; i < num_liquors; i++) {
		if (strcmp_nocase(liquors[i].name, liquor_name) == 0) {
			return i;
		}
	}
	return -1; // Liquor not found
}