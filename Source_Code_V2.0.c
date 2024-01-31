#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>

#define MASTER_PASSWORD "MayssaYassine2023*"
#define MAX_L 200
#define MAX_P 100

bool authenticate() {
    printf("Enter the master password: ");

    char password[MAX_L];
    int i = 0;
    char c;

    while ((c = _getch()) != '\r') {
        if (c == '\b') {
            if (i > 0) {
                printf("\b \b");
                i--;
            }
        } else {
            password[i++] = c;
            printf("*");
        }
    }

    password[i] = '\0';

    if (strcmp(password, MASTER_PASSWORD) == 0) {
        printf("\nAuthentication successful.\n");
        return true;
    } else {
        printf("\nAuthentication failed. Exiting Cypher.\n");
        return false;
    }
}

typedef struct {
    char Website[MAX_L];
    char Email_Address[MAX_L];
    char Password[MAX_L];
} PasswordEntry;

PasswordEntry passwords[MAX_P];
int numPasswords = 0;

void savePasswordsToFile() {
    FILE *file = fopen("passwords.txt", "w");


    for (int i = 0; i < numPasswords; i++) {
        fprintf(file, "%s %s %s\n", passwords[i].Website, passwords[i].Email_Address, passwords[i].Password);
    }

    fclose(file);
    printf("Passwords saved to file.\n");
}

void loadPasswordsFromFile() {
    FILE *file = fopen("passwords.txt", "r");
    if (file == NULL) {
        printf("No saved passwords found.\n");
        return;
    }

    numPasswords = 0;
    while (fscanf(file, "%s %s %s", passwords[numPasswords].Website, passwords[numPasswords].Email_Address, passwords[numPasswords].Password) != EOF) {
        numPasswords++;
    }

    fclose(file);
    printf("Passwords loaded from file.\n");
}

void generatePassword(char* password, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+{}[]<>?";

    srand(time(NULL));

    for (int i = 0; i < length; i++) {
        int index = rand() % strlen(charset);
        password[i] = charset[index];
    }

    password[length] = '\0';
}

void addPassword() {
    if (numPasswords >= MAX_P) {
        printf("Maximum number of passwords reached.\n");
        return;
    }

    PasswordEntry userEntry;

    printf("Enter Associated Website: ");
    scanf("%s", userEntry.Website);

    printf("Enter Your Email Address: ");
    scanf("%s", userEntry.Email_Address);

    printf("Do you want to enter the password [M]anually || [G]enerated? ");
    char choice;
    scanf(" %c", &choice);
    choice = toupper(choice);

    if (choice == 'M') {
        printf("Enter Password: ");
        scanf("%s", userEntry.Password);
    } else if (choice == 'G') {
        int passwordLength;
        printf("Enter the desired length of the password: ");
        scanf("%d", &passwordLength);

        if (passwordLength <= 0 || passwordLength > MAX_L) {
            printf("Invalid password length. Password not added.\n");
            return;
        }

        generatePassword(userEntry.Password, passwordLength);
    } else {
        printf("Invalid choice. Password not added.\n");
        return;
    }

    passwords[numPasswords++] = userEntry;

    printf("Password added successfully.\n");
    savePasswordsToFile();
}

void viewPasswords(){
if (numPasswords == 0) { // checks if the variable numPasswords is equal to 0, indicating that there are no passwords stored.
    printf("Sorry, but we couldn't find any passwords stored in our DB.\n");
    return;
}
printf("| %-30s | %-30s | %-30s\n", "Website", "Email Address", "Password");

//printf("Website\t                   Email Address\t                Password\n");
printf("----------------------------------------------------------------------------\n");
for (int i=0;i<numPasswords;i++) { //  starts a loop that iterates over the password entries stored in the passwords array.
    printf("| %-30s | %-30s | %-30s\n", passwords[i].Website, passwords[i].Email_Address,     passwords[i].Password);
    }
}

void deletePassword() {
    if (numPasswords == 0) {
        printf("No passwords to delete.\n");
        return;
    }

    printf("Select the number of the password entry you want to delete:\n");
    for (int i = 1; i < numPasswords; i++) {
        printf("%d. Website: %s, Email Address: %s\n", i , passwords[i].Website, passwords[i].Email_Address);
    }

    int choice;
    printf("Enter the number: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > numPasswords -1 ) {
        printf("Invalid input. Please enter a valid number.\n");
        while (getchar() != '\n');
        return;
    }

    choice--;
    for (int i = choice; i < numPasswords -1 ; i++) {
        strcpy(passwords[i].Website, passwords[i + 1].Website);
        strcpy(passwords[i].Email_Address, passwords[i + 1].Email_Address);
        strcpy(passwords[i].Password, passwords[i + 1].Password);
    }
    numPasswords--;

    printf("Password deleted successfully.\n");
    savePasswordsToFile();
}

void modifyPassword() {
    if (numPasswords == 0) {
        printf("No passwords to modify.\n");
        return;
    }

    printf("Select the number of the password entry you want to modify:\n");
    for (int i = 0; i < numPasswords-1; i++) {
        printf("%d. Website: %s, Email Address: %s\n", i + 1, passwords[i].Website, passwords[i].Email_Address);
    }

    int choice;
    printf("Enter the number: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > numPasswords-1) {
        printf("Invalid input. Please enter a valid number.\n");
        while (getchar() != '\n');
        return;
    }

    choice--;
    PasswordEntry *entry = &passwords[choice];

    printf("Do you want to enter the password [M]anually || [G]enerated?  ");
    char option;
    scanf(" %c", &option);
    option = toupper(option);

    if (option == 'M') {
        printf("Enter the new password: ");
        scanf("%s", entry->Password);
    } else if (option == 'G') {
        int passwordLength;
        printf("Enter the desired length of the new password: ");
        scanf("%d", &passwordLength);

        if (passwordLength <= 0 || passwordLength > MAX_L) {
            printf("Invalid password length. Password not modified.\n");
            return;
        }

        generatePassword(entry->Password, passwordLength);
    } else {
        printf("Invalid choice. Password not modified.\n");
        return;
    }

    printf("Password modified successfully.\n");
    savePasswordsToFile();
}

int main() {
    if (!authenticate()) {
        return 0;
    }

    loadPasswordsFromFile();
    int choice;

    while (1) {
        printf("\n[!] Welcome to Cypher, The Safest Password Manager Worldwide [!]\n");
        printf("1. Add Password\n");
        printf("2. View Passwords\n");
        printf("3. Delete Password\n");
        printf("4. Modify Password\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addPassword();
                break;
            case 2:
                viewPasswords();
                break;
            case 3:
                deletePassword();
                break;
            case 4:
                modifyPassword();
                break;
            case 5:
                printf("Exiting Cypher. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
                break;
        }
    }
}
