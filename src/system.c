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

void updateAccountInfo(struct User u)
{
    struct Record r;
    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("temp.txt", "w");
    char lines[250];
    int accNum, found = 0;
    char newCountry[50];
    int newPhone, changeCountry = 0, changePhone = 0;

    if (!fp || !temp)
    {
        printf("Error opening file.\n");
        return;
    }

    printf("\nEnter your account number to update: ");
    scanf("%d", &accNum);
    getchar();

    while (fgets(lines, sizeof(lines), fp))
    {
        if (lines[0] == '\n')
        {
            continue;
        }

        if (sscanf(lines, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                   &r.id, &r.userId, r.name, &r.accountNbr,
                   &r.deposit.month, &r.deposit.day, &r.deposit.year,
                   r.country, &r.phone, &r.amount, r.accountType) != 11)
        {
            printf("Error reading line, skipping...\n");
            continue;
        }

        if (r.accountNbr == accNum && strcmp(r.name, u.name) == 0)
        {
            found = 1;

            printf("\nEnter new country (or press Enter to keep [%s]): ", r.country);
            fgets(newCountry, sizeof(newCountry), stdin);
            if (newCountry[0] != '\n')
            {
                newCountry[strcspn(newCountry, "\n")] = '\0';
                strcpy(r.country, newCountry);
                changeCountry = 1;
            }

            printf("\nEnter new phone number (or press 0 to keep [%d]): ", r.phone);
            if (scanf("%d", &newPhone) == 1 && newPhone > 0)
            {
                r.phone = newPhone;
                changePhone = 1;
            }
            getchar();

            if (changeCountry || changePhone)
            {
                printf("\n✔ Account updated successfully!\n");
            }
            else
            {
                printf("\n✔ No changes made.\n");
            }
        }

        fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                r.id, r.userId, r.name, r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        if (remove(RECORDS) != 0)
        {
            printf("Error deleting original file!\n");
            return;
        }
        if (rename("temp.txt", RECORDS) != 0)
        {
            return;
        }
        success(u);
    }
    else
    {
        printf("\n✖ Account not found!\n");
        remove("temp.txt");
    }
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

// Function to check if input is a valid number
int isValidNumber(const char *str) {
    int dotCount = 0;
    if (*str == '\0') return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            dotCount++;
            if (dotCount > 1) return 0;
        } else if (!isdigit(str[i])) {
            return 0; 
        }
    }
    return 1;
}

void makeTransaction(struct User u) {
    struct Record r;
    int accNum, found = 0;
    double amount;
    char choice;
    char line[250];
    char amountStr[50];

    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nEnter account number to make a transaction: ");
    scanf("%d", &accNum);
    getchar();

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n') continue;

        if (sscanf(line, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                   &r.id, &r.userId, r.name, &r.accountNbr,
                   &r.deposit.month, &r.deposit.day, &r.deposit.year,
                   r.country, &r.phone, &r.amount, r.accountType) != 11) {
            printf("Error reading line, skipping...\n");
            continue;
        }

        if (r.accountNbr == accNum && strcmp(u.name, r.name) == 0 ){
            
            found = 1;
            printf("\nSelect type of Transaction:");
            printf("\n1: Deposit");
            printf("\n2: Withdraw");
            printf("\n3: Check Balance");
            printf("\nEnter choice: ");
            scanf(" %c", &choice);
            getchar();

            if (choice == '1' || choice == '2') {
                if (strcmp(r.accountType, "fixed01") != 0 && strcmp(r.accountType, "fixed02") != 0 && strcmp(r.accountType, "fixed03") != 0) {
                
                do {
                    printf("\nEnter amount: ");
                    fgets(amountStr, sizeof(amountStr), stdin);
                    amountStr[strcspn(amountStr, "\n")] = 0;

                    if (!isValidNumber(amountStr)) {
                        printf("\n✖ Invalid input! Please enter a valid number.\n");
                        continue;
                    }

                    amount = strtod(amountStr, NULL);

                    if (amount < 0 || amount > 1000000) {
                        printf("\n✖ Invalid amount! Please enter a positive value and value not exceeding 1000000.\n");
                    }

                } while (amount < 0 || amount > 1000000 || !isValidNumber(amountStr));

                if (choice == '1') {
                    r.amount += amount;
                    printf("\n✔ Deposit successful! New balance: %.2lf\n", r.amount);
                } else if (choice == '2') {
                    if (amount > r.amount) {
                        printf("\n✖ Insufficient balance! Your account balance is %.2lf\n", r.amount);
                    } else {
                        r.amount -= amount;
                        printf("\n✔ Withdrawal successful! New balance: %.2lf\n", r.amount);
                    }
                }
            } else {
            printf("\n✖ You can not transact in a fixed account. Try another account\n\n");
            
            success(u);

        }
            } else if (choice == '3') {
                printf("\n✔ Current balance: %.2lf\n", r.amount);
            } else {
                printf("\n✖ Invalid choice!\n");
            }
        }

        fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                r.id, r.userId, r.name, r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        if (remove(RECORDS) != 0) {
            printf("Error deleting original file!\n");
            return;
        }
        if (rename("temp.txt", RECORDS) != 0) {
            return;
        }
        success(u);
    } else {
        printf("\n✖ Account not found!\n\n");
        remove("temp.txt");
        success(u);
    }
}

void removeExistingAccount(struct User u) {

    struct Record r;
    int accountNbr, found = 0;
    char line[256];

    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp)
    {
        perror("Error opening file!");
        return;
    }

    printf("\nEnter the account number to remove: ");
    scanf("%d", &accountNbr);

    while (fgets(line, sizeof(line), fp))
    {
        if (line[0] == '\n' || line[0] == '\r')
        {
            continue;
        }
        if (sscanf(line, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                   &r.id, &r.userId, r.name, &r.accountNbr,
                   &r.deposit.month, &r.deposit.day, &r.deposit.year,
                   r.country, &r.phone, &r.amount, r.accountType) != 11 &&
            sscanf(line, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                   &r.id, &r.userId, r.name, &r.accountNbr,
                   &r.deposit.month, &r.deposit.day, &r.deposit.year,
                   r.country, &r.phone, &r.amount, r.accountType) != '\n')
        {
            continue;
        }
        if (r.accountNbr == accountNbr && strcmp(r.name, u.name) == 0)
        {
            found = 1;
            printf("\n✔ Account successfully removed!\n");
            continue;
        }

        fprintf(temp, "%s\n", line);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(RECORDS);
        rename("temp.txt", RECORDS);
        success(u);
    }
    else
    {
        remove("temp.txt");
        printf("\n✖ Account not found!\n");
        success(u);
    }

}
