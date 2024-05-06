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
    int ml; // New field for milliliters
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
		}

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
		}
	} while (!valid_input || choice !=3);

	return 0;
}

// Function to check if username is valid (alphanumeric)
int is_username_valid(char *username) {
  int len = strlen(username), i;
  for (i = 0; i < len; i++) {
    if (!isalnum(username[i])) {
      return 0;
    }
  }
  return 1;
}

// Function to find the index of a user in the users array
int find_user_index(char *username) {
  for (int i = 0; i < num_users; i++) {
    if (strcmp(users[i].username, username) == 0) {
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
  int attempts = 3;

  printf("Enter username: ");
  scanf("%s", username);

  int user_index = find_user_index(username);

  while (user_index == -1 && attempts > 0) {
    printf("Invalid username. Please try again.\n");
    attempts--;

    printf("Enter username: ");
    scanf("%s", username);

    user_index = find_user_index(username);
  }

  if (user_index != -1) {
    printf("Enter password: ");
    scanf("%s", password);

    while (strcmp(users[user_index].password, password) != 0 && attempts > 1) {
      printf("Invalid password. Please try again.\n");
      attempts--;

      printf("Enter password: ");
      scanf("%s", password);
    }

    if (strcmp(users[user_index].password, password) == 0) {
      printf("Login successful!\n");

      int logged_in_choice;
      while (1) {
        printf("\nLogged-in Menu\n");
        printf("1. Display Liquors\n");
        printf("2. Order Liquor\n");
        printf("3. Exit Logged-in Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &logged_in_choice) != 1) {
          printf("Invalid input. Please enter a number.\n");
          while (getchar() != '\n');
          continue;
        }

        switch (logged_in_choice) {
          case 1:
            display_liquors();
            break;
          case 2:
            order_liquor();
            break;
          case 3:
            break;
          default:
            printf("Invalid choice!\n");
        }

        if (logged_in_choice == 3) {
          break;
        }
      }
    } else {
      printf("Invalid password. Exiting...\n");
    }
  } else {
    if (attempts == 0) {
      printf("Maximum login attempts reached. Exiting...\n");
    } else {
      printf("Login failed.\n");
    }
  }
}

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
        if (sscanf(line, "%[^,],%d %d %d", name, &price, &ml, &stock) == 4) {
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
    fprintf(liquor_file, "%s %d %d %d\n", liquors[j].name, liquors[j].price, liquors[j].ml, liquors[j].stock);
  }

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
  printf("\nLiquor List:\n\n");
  printf("  LIQUOR NAME\t\t\t\tPRICE\t\tML\t\tSTOCK\n");
  printf("  -----------\t\t\t\t-------\t\t---\t\t-----\n");

  for (int i = 0; i < num_liquors; i++) {
    printf("  %-50s\tPHP %d\t\t%d ml\t\t%d\n", liquors[i].name, liquors[i].price, liquors[i].ml, liquors[i].stock);
  }

  printf("\n");
}


void order_liquor() {
    char liquor_name[MAX_LIQUOR_NAME];
    int quantity;
    float total_amount;

    printf("\nEnter the name of the liquor you want to order: ");
    fgets(liquor_name, sizeof(liquor_name), stdin);
    liquor_name[strcspn(liquor_name, "\n")] = '\0'; // Remove the trailing newline character

    int liquor_index = find_liquor_index(liquor_name);

    while (liquor_index == -1) {
        printf("Liquor not found! Please enter a valid liquor name: ");
        fgets(liquor_name, sizeof(liquor_name), stdin);
        liquor_name[strcspn(liquor_name, "\n")] = '\0'; // Remove the trailing newline character
        liquor_index = find_liquor_index(liquor_name);
    }
	
	if (liquors[liquor_index].stock == 0) {
        printf("Sorry, the liquor is out of stock.\n");
        return;
  	}
        
  	do {     
    	printf("Enter the quantity you want to order: ");
    	if (scanf("%d", &quantity) != 1) {
          printf("Invalid input. Please enter a number.\n");
          while (getchar() != '\n');
          continue;
        } else if (quantity <= 0) {
			printf("Invalid! Zero Inputted!");
		}
		
	} while (quantity <= 0);
	
    while (liquors[liquor_index].stock < quantity) {
        printf("Insufficient stock. Only %d %s available. Please enter a valid quantity: ", liquors[liquor_index].stock, liquors[liquor_index].name);
        scanf("%d", &quantity);
    }

    total_amount = (float)liquors[liquor_index].price * quantity;

    liquors[liquor_index].stock -= quantity;
    printf("Order successful! You have ordered %d %s for a total of PHP %.2f.\n", quantity, liquors[liquor_index].name, total_amount);

    export_receipt(liquor_name, quantity, total_amount);
}


void export_receipt(char *liquor_name, int quantity, float total_amount) {
  FILE *receipt_file;

  receipt_file = fopen("receipt.txt", "w");

  if (receipt_file == NULL) {
    printf("Error creating receipt file!\n");
    return;
  }

  fprintf(receipt_file, "Liquor Ordering System Receipt\n");
  fprintf(receipt_file, "--------------------------------\n");
  fprintf(receipt_file, "Liquor Name: %s\n", liquor_name);
  fprintf(receipt_file, "Quantity: %d\n", quantity);
  fprintf(receipt_file, "Total Amount: PHP %.2f\n", total_amount);
  fprintf(receipt_file, "--------------------------------\n");
  fprintf(receipt_file, "Thank you for your order!\n");

  fclose(receipt_file);

  printf("Order details saved to receipt.txt\n");
}

int find_liquor_index(char *liquor_name) {
  for (int i = 0; i < num_liquors; i++) {
    if (strcmp_nocase(liquors[i].name, liquor_name) == 0) {
      return i;
    }
  }
  return -1;
}