#include "header.h"
#include <ctype.h>

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
        r.id,
	    u.id,
        u.name,
        r.accountNbr,
        r.deposit.month,
        r.deposit.day,
        r.deposit.year,
        r.country,
        r.phone,
        r.amount,
        r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void checkAccountDetails(struct User u)
{
    struct Record r;
    FILE *fp = fopen(RECORDS, "r");
    int accNum, found = 0;
    float interest;
    float monthlyInterest;

    if (!fp)
    {
        printf("Error opening file.\n");
        return;
    }

    printf("\nEnter account number to check: ");
    scanf("%d", &accNum);

    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) != EOF)
    {
        if (r.accountNbr == accNum && strcmp(r.name, u.name) == 0)
        {
            found = 1;

            printf("\n===== Account Details =====\n");
            printf("Account Number: %d\n", r.accountNbr);
            printf("Name: %s\n", r.name);
            printf("Country: %s\n", r.country);
            printf("Phone: %d\n", r.phone);
            printf("Balance: $%.2lf\n", r.amount);
            printf("Account Type: %s\n", r.accountType);
            printf("Date Opened: %02d/%02d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);

            if (strcmp(r.accountType, "saving") == 0)
            {
                interest = 7 * r.amount / 100;
                monthlyInterest = interest / 12;
                printf("\n✔ You will receive %.2f as interest on day %d of every month.\n", monthlyInterest, r.deposit.day);
            }
            else if (strcmp(r.accountType, "fixed01") == 0)
            {
                interest = 4 * r.amount / 100;
                printf("\n✔ You will receive %.2f as interest on day %d/%d/%d.\n", interest, r.deposit.day, r.deposit.month, r.deposit.year+1);
            }
            else if (strcmp(r.accountType, "fixed02") == 0)
            {
                interest = (5 * r.amount / 100)*2;
                printf("\n✔ You will receive %.2f as interest on day %d/%d/%d.\n", interest, r.deposit.day, r.deposit.month, r.deposit.year+2);
            }
            else if (strcmp(r.accountType, "fixed03") == 0)
            {
                interest = (8 * r.amount / 100)*3;
                printf("\n✔ You will receive %.2f as interest on day %d/%d/%d.\n", interest, r.deposit.day, r.deposit.month, r.deposit.year+3);
            }
            else if (strcmp(r.accountType, "current") == 0)
            {
                printf("\n✖ No interest is applied as this is a current account.\n");
            }

            success(u);
            break;
        }
    }

    if (!found)
    {
        printf("\n✖ Account not found!\n");
        success(u);
    }

    fclose(fp);
};
