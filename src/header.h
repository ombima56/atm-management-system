#ifndef HEADER_H 
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Add time.h for time functions

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;           // Keep for backward compatibility
    char phoneStr[15];   // New field to store phone as string with country code
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

// authentication functions
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);

// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
void checkAccountDetails(struct User u);
void makeTransaction(struct User u);
void removeExistingAccount(struct User u);
void updateAccountInfo(struct User u);
void transferOwnership(struct User u);
void success(struct User u);
int generateNewID();

extern const char *USERS;
#endif
