#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>

#define MAX_USERS 20
#define MAX_LIQUOR 20
#define MAX_USERNAME 20
#define MAX_PASSWORD 20
#define MAX_LIQUOR_NAME 50

// Global variables:
struct User {
	char username[MAX_USERNAME];
	char password[MAX_PASSWORD];
};

struct Liquor {
	char name[MAX_LIQUOR_NAME];
	int price;
	int ml;
	int stock;
};

struct User users[MAX_USERS];
struct Liquor liquors[MAX_LIQUOR];
int num_users = 0;
int num_liquors = 0;
int current_user_index = -1; // Declaration of current_user_index

// Function prototypes
void load_data(); // Function to load data from text files
int getMenuChoice(int choice); // Function to display main menu and get user choice
void create_account(); // Function to create a new user account
int is_username_valid(char *username); // Function to check if username is valid
int is_password_valid(char *password); // Function to check if password is valid
void login(); // Function to login a user
int find_user_index(char *username); // Function to find the index of a user in the users array
void display_liquors(); // Function to display list of liquors
void order_liquor(); // Function to order liquor
int find_liquor_index(char *liquor_name); // Function to find the index of a liquor in the liquors array
int strcmp_nocase(const char *str1, const char *str2); // Function to compare strings case-insensitively
void export_receipt(char *username, char *liquor_name, int quantity, float total_amount); // Function to export a receipt
void save_data(); // Function to save data to text files

int main() {
	load_data();  // Load data on startup

	int choice, i, valid_input;
	do {
		choice = getMenuChoice(choice);

		switch (choice) {
			case 1:
				system("cls");
				create_account();
				break;
			case 2:
				system("cls");
				printf("LIQUOR ORDERING SYSTEM\n\n");
				printf("LOGIN\n\n");
				login();
				break;
			case 3:
				save_data();  // Save data before exiting
				exit(0);
		}
	} while (!valid_input || choice !=3);

	return 0;
}

// Function to load data from text files
void load_data() {
	FILE *user_file = fopen("users.txt", "r");
	FILE *liquor_file = fopen("liquors.txt", "r");

	if (user_file == NULL || liquor_file == NULL) {
		printf("Error opening data files!\n");
		return;
	}

	char line[MAX_LIQUOR_NAME + 20]; // Add buffer for entire line including price, ml, and stock
	while (fgets(line, sizeof(line), user_file) != NULL && num_users < MAX_USERS) {
		sscanf(line, "%s %s", users[num_users].username, users[num_users].password);
		num_users++;
	}

	while (fgets(line, sizeof(line), liquor_file) != NULL && num_liquors < MAX_LIQUOR) {
		char name[MAX_LIQUOR_NAME];
		int price, ml, stock;
		// Use sscanf to parse the line manually, taking into account spaces in the name
		if (sscanf(line, "%[^,],%d %d %d", name, &ml, &price, &stock) == 4) {
			strcpy(liquors[num_liquors].name, name);
			liquors[num_liquors].price = price;
			liquors[num_liquors].ml = ml;
			liquors[num_liquors].stock = stock;
			num_liquors++;
		}
	}

	fclose(user_file);
	fclose(liquor_file);
}

// Function to display main menu and get user choice
int getMenuChoice(int choice) {
	do {
		printf("LIQUOR ORDERING SYSTEM\n\n");
		printf("MAIN MENU\n\n");
		printf("[1] - CREATE ACCOUNT\n");
		printf("[2] - LOGIN\n");
		printf("[3] - EXIT\n\n");
		printf("Enter your choice: ");

		if (scanf("%d", &choice) != 1 || choice <= 0 || choice >= 4) {
			printf("\nInvalid Input! Press any key to continue . . . ");
			getch();
			fflush(stdin);
			system("cls");
		}
	} while (choice <= 0 || choice >= 4);

	return choice;
}

// Function to create a new user account
void create_account() {
	if (num_users == MAX_USERS) {
		printf("Maximum number of users reached!\n");
		return;
	}

	char username[MAX_USERNAME], password[MAX_PASSWORD];

	do {
		printf("LIQUOR ORDERING SYSTEM\n\n");
		printf("CREATE ACCOUNT\n\n");
		printf("Enter username: ");
		scanf("%s", username);

		// Validate username
		if(!is_username_valid(username)) {
			printf("\nUsername must be minimum of 7 characters. Press any key to continue . . . ");
			getch();
			fflush(stdin);
			system("cls");
		} else if (find_user_index(username) != -1) {
			printf("\nUsername already exists!. Press any key to continue . . . ");
			getch();
			fflush(stdin);
			system("cls");
		}
	} while(!is_username_valid(username) || find_user_index(username) != -1);


	if (find_user_index(username) != -1) {
		printf("\nUsername already exists!\n");
		return;
	}
	printf("Enter password: ");
	scanf("%s", password);

	// Validate password
	while (!is_password_valid(password)) {
		printf("\nPassword must be at least 8 characters long.\n\n");
		printf("Enter password: ");
		scanf("%s", password);
	}

	strcpy(users[num_users].username, username);
	strcpy(users[num_users].password, password);
	num_users++;

	printf("\nAccount created successfully! Press any key to continue . . . ");
	getch();
	system("cls");
}

// Function to check if username is valid
int is_username_valid(char *username) {
	int i, len = strlen(username);
	// Check if username length is within the specified range
	if (len < 7 || len > 30)
		return 0;
	for (i = 0; i < len; i++) {
		if (!isalnum(username[i])) {

			return 0;
		}
	}
	return 1;
}

// Function to check if password is valid
int is_password_valid(char *password) {
	return strlen(password) >= 8;
}

// Function to login a user
void login() {
	char username[MAX_USERNAME], password[MAX_PASSWORD];
	int attempts = 3;

	printf("Enter username: ");
	scanf("%s", username);

	int user_index = find_user_index(username);

	while (user_index == -1 && attempts > 0) {
		printf("\nInvalid username. Press any key to continue . . . ");
		getch();
		system("cls");
		attempts--;

		printf("LIQUOR ORDERING SYSTEM\n\n");
		printf("LOGIN\n\n");
		printf("Enter username: ");
		scanf("%s", username);

		user_index = find_user_index(username);
	}

	if (user_index != -1) {
		printf("Enter password: ");
		scanf("%s", password);

		while (strcmp(users[user_index].password, password) != 0 && attempts > 1) {
			printf("\nInvalid password. Please try again.\n\n");
			attempts--;

			printf("Enter password: ");
			scanf("%s", password);
		}

		if (strcmp(users[user_index].password, password) == 0) {
			printf("\nLogin successful!");
			system("cls");
			current_user_index = user_index; // Set the current_user_index

			int logged_in_choice;
			while (1) {
				printf("LIQUOR ORDERING SYSTEM\n\n");
				printf("LOGGED-IN MENU\n\n");
				printf("[1] - DISPLAY LIQUORS\n");
				printf("[2] - ORDER LIQUOR\n");
				printf("[3] - BACK\n\n");

				printf("Enter your choice: ");

				if (scanf("%d", &logged_in_choice) != 1) {
					printf("\nInvalid Input! Please enter a number.\n\n");
					while (getchar() != '\n');
					continue; // Prompt the user again for input
				}

				switch (logged_in_choice) {
					case 1:
						system("cls");
						display_liquors();
						break;
					case 2:
						order_liquor();
						break;
					case 3:
						system("cls");
						return; // Exit the function
					default:
						printf("\nInvalid choice!\n\n");
				}
			}
		} else {
			printf("\nMaximum password attempts reached. Press any key to continue . . . \n");
			getch();
			system("cls");
		}
	} else {
		if (attempts == 0) {
			printf("\nMaximum login attempts reached. Press any key to continue . . . \n");
			getch();
			system("cls");
		} else {
			printf("Login failed.\n");
		}
	}
}

// Function to find the index of a user in the users array
int find_user_index(char *username) {
	int i;
	for (i = 0; i < num_users; i++) {
		if (strcmp(users[i].username, username) == 0) {
			return i;
		}
	}
	return -1;
}

// Function to display available liquors
void display_liquors() {
	printf("  LIQUOR NAME\t\t\tML\t\tPRICE\t\tSTOCK\n");
	printf("  -----------\t\t\t------\t\t-------\t\t-----\n");

	for (int i = 0; i < num_liquors; i++) {
		printf("  %-26s\t%d ml\t\tPHP %d\t\t%d\n", liquors[i].name, liquors[i].ml, liquors[i].price, liquors[i].stock);
	}

	printf("\n");
}

// Function to order liquor
void order_liquor() {
	char username[MAX_USERNAME];
	char liquor_name[MAX_LIQUOR_NAME];
	int quantity;
	float total_amount;

	// Get the username from the logged-in user session
	strcpy(username, users[current_user_index].username);

// Clear input buffer
	while (getchar() != '\n');

	printf("\nEnter the name of the liquor you want to order: ");
	fgets(liquor_name, sizeof(liquor_name), stdin);
	liquor_name[strcspn(liquor_name, "\n")] = '\0'; // Remove the trailing newline character
// Remove the trailing newline character

	int liquor_index = find_liquor_index(liquor_name);

	while (liquor_index == -1) {
		printf("\nLiquor not found! Please enter a valid liquor name: ");
		fgets(liquor_name, sizeof(liquor_name), stdin);
		liquor_name[strcspn(liquor_name, "\n")] = '\0'; // Remove the trailing newline character
		liquor_index = find_liquor_index(liquor_name);
	}

	if (liquors[liquor_index].stock == 0) {
		printf("\nSorry, the liquor you want to order is out of stock. Press any key to continue . . . \n");
		getch();
		system("cls");
		return;
	}

	do {
		printf("Enter the quantity you want to order: ");
		if (scanf("%d", &quantity) != 1) {
			printf("Invalid input. Please enter a number.\n");
			while (getchar() != '\n');
			continue;
		} else if (quantity <= 0) {
			printf("\nInvalid! Zero Inputted!\n\n");
		}

	} while (quantity <= 0);

	while (liquors[liquor_index].stock < quantity) {
		printf("\nInsufficient stock. Only %d %s available.\n\nPlease enter a valid quantity: ", liquors[liquor_index].stock, liquors[liquor_index].name);
		scanf("%d", &quantity);
	}

	total_amount = (float)liquors[liquor_index].price * quantity;

	liquors[liquor_index].stock -= quantity;

	system("cls");
	printf("Order successful!\n\nYou have ordered %d %s for a total of PHP %.2f.\n\n", quantity, liquors[liquor_index].name, total_amount);

	export_receipt(username, liquor_name, quantity, total_amount);
}

// Function to find the index of a liquor in the liquors array
int find_liquor_index(char *liquor_name) {
	for (int i = 0; i < num_liquors; i++) {
		if (strcmp_nocase(liquors[i].name, liquor_name) == 0) {
			return i;
		}
	}
	return -1;
}

// Function to compare strings case-insensitively
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

// Function to export a receipt
void export_receipt(char *username, char *liquor_name, int quantity, float total_amount) {
	FILE *receipt_file;
	time_t current_time;
	char *date_string;
	char filename[20]; // Maximum length for filename

	// Get the current date and time
	current_time = time(NULL);
	date_string = ctime(&current_time);

	// Generate a unique filename
	int receipt_number = 1;
	while (1) {
		snprintf(filename, sizeof(filename), "receipt%d.txt", receipt_number);
		receipt_file = fopen(filename, "r");
		if (receipt_file == NULL) {
			// File does not exist, so this filename is available
			break;
		}
		fclose(receipt_file);
		receipt_number++;
	}

	// Open the receipt file for writing
	receipt_file = fopen(filename, "w");

	if (receipt_file == NULL) {
		printf("Error creating receipt file!\n");
		return;
	}

	fprintf(receipt_file, "Liquor Ordering System Receipt\n");
	fprintf(receipt_file, "--------------------------------\n");
	fprintf(receipt_file, "Username: %s\n", username);  // Include the username
	fprintf(receipt_file, "Date: %s", date_string);       // Include the date
	fprintf(receipt_file, "Liquor Name: %s\n", liquor_name);
	fprintf(receipt_file, "Quantity: %d\n", quantity);
	fprintf(receipt_file, "Total Amount: PHP %.2f\n", total_amount);
	fprintf(receipt_file, "--------------------------------\n");
	fprintf(receipt_file, "Thank you for your order!\n");

	fclose(receipt_file);

	printf("Order details saved to %s\n\n", filename);
	printf("Press any key to continue . . . \n");
	getch();
	system("cls");

}

// Function to save data to files
void save_data() {
	FILE *user_file = fopen("users.txt", "w");
	FILE *liquor_file = fopen("liquors.txt", "w");

	if (user_file == NULL || liquor_file == NULL) {
		printf("Error opening data files for saving!\n");
		return;
	}

	for (int i = 0; i < num_users; i++) {
		fprintf(user_file, "%s %s\n", users[i].username, users[i].password);
	}

	for (int j = 0; j < num_liquors; j++) {
		fprintf(liquor_file, "%s, %d %d %d\n", liquors[j].name, liquors[j].ml, liquors[j].price, liquors[j].stock);
	}

	fclose(user_file);
	fclose(liquor_file);

	printf("\nData saved successfully!\n");
}