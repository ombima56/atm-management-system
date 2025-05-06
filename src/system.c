#include "header.h"
#include <ctype.h>
#include <time.h>

const char *RECORDS = "./data/records.txt";

// Function declaration for generateNewID
int generateNewID();

// Function to get the user ID from the username
int getUserIdByUsername(const char *username)
{
    FILE *userFile = fopen(USERS, "r");
    struct User tempUser;
    if (!userFile)
    {
        printf("\nError opening users file!\n");
        return -1;
    }

    while (fscanf(userFile, "%d %s %s", &tempUser.id, tempUser.name, tempUser.password) != EOF)
    {
        if (strcmp(tempUser.name, username) == 0)
        {
            fclose(userFile);
            return tempUser.id;
        }
    }

    fclose(userFile);
    return -1;
}

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    char line[256];
    
    while (fgets(line, sizeof(line), ptr)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) {
            continue;
        }
        
        int result = sscanf(line, "%d %d %s %d %d/%d/%d %s %s %lf %s",
                      &r->id,
                      &r->userId,
                      name,
                      &r->accountNbr,
                      &r->deposit.month,
                      &r->deposit.day,
                      &r->deposit.year,
                      r->country,
                      r->phoneStr,
                      &r->amount,
                      r->accountType);
        
        if (result == 11) {
            r->phone = atoi(r->phoneStr);
            return 1;
        }
        
        result = sscanf(line, "%d %d %s %d %d/%d/%d %s %d %lf %s",
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
                      r->accountType);
        
        if (result == 11) {
            sprintf(r->phoneStr, "%d", r->phone);
            return 1;
        }
        
        char phoneStr1[15], phoneStr2[15];
        result = sscanf(line, "%d %d %s %d %d/%d/%d %s %s %s %lf %s",
                      &r->id,
                      &r->userId,
                      name,
                      &r->accountNbr,
                      &r->deposit.month,
                      &r->deposit.day,
                      &r->deposit.year,
                      r->country,
                      phoneStr1,
                      phoneStr2,
                      &r->amount,
                      r->accountType);
        
        if (result == 12) {
            strcpy(r->phoneStr, phoneStr1);
            r->phone = atoi(phoneStr2);
            return 1;
        }
        
        printf("Warning: Could not parse line: %s\n", line);
        return 0;
    }
    
    return 0;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    if (ptr == NULL) {
        printf("\nError: File pointer is NULL\n");
        return;
    }

    u.id = getUserIdByUsername(u.name);
    if (u.id == -1) {
        printf("\nError: Could not find user ID\n");
        return;
    }

    if (r.phoneStr[0] == '0') {
        r.phone = atoi(r.phoneStr + 1);
    } else if (r.phoneStr[0] == '+') {
        r.phone = atoi(r.phoneStr + 1);
    } else {
        r.phone = atoi(r.phoneStr);
    }

    if (fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
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
        r.accountType) < 0) {
        printf("\nError writing to file\n");
    }
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
    printf("Enter 1 to go to the main menu and 0 to exit: ");
    scanf("%d", &option);
    
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        exit(0);
    }
}

// Function to validate date (prevent future dates)
int isValidDate(int month, int day, int year)
{
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    
    if (month < 1 || month > 12)
        return 0;
    
    // Check days per month
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
        daysInMonth[2] = 29;
    
    if (day < 1 || day > daysInMonth[month])
        return 0;
    
    if (year > current_time->tm_year + 1900)
        return 0;
    if (year == current_time->tm_year + 1900 && month > current_time->tm_mon + 1)
        return 0;
    if (year == current_time->tm_year + 1900 && month == current_time->tm_mon + 1 && day > current_time->tm_mday)
        return 0;
    
    return 1;
}

// Function to validate phone number
int isValidPhoneNumber(const char *phone) {
    int len = strlen(phone);
    
    if (phone[0] == '+') {
        if (len != 13) {
            return 0;
        }
        
        for (int i = 1; i < len; i++) {
            if (!isdigit(phone[i])) {
                return 0;
            }
        }
    } else {
        if (len != 10 || phone[0] != '0') {
            return 0;
        }
        
        for (int i = 0; i < len; i++) {
            if (!isdigit(phone[i])) {
                return 0;
            }
        }
    }
    
    return 1;
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    char phoneStr[15];
    FILE *pf = fopen(RECORDS, "a+");
    
    if (!pf) {
        printf("\nError opening records file!\n");
        return;
    }

    r.id = generateNewID();

    system("clear");
    printf("\t\t\t===== New record =====\n");

    // Date entry with validation
    dateEntry:
    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    
    if (!isValidDate(r.deposit.month, r.deposit.day, r.deposit.year)) {
        printf("\n✖ Invalid date! Please enter a valid date (not in the future).\n");
        goto dateEntry;
    }
    
    // Account number entry with validation
    accountEntry:
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);
    
    if (r.accountNbr <= 0) {
        printf("\n✖ Invalid account number! Please enter a positive number.\n");
        goto accountEntry;
    }

    // Check if account number already exists for ANY user
    rewind(pf);
    int accountExists = 0;
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (cr.accountNbr == r.accountNbr)
        {
            accountExists = 1;
            break;
        }
    }
    
    if (accountExists) {
        printf("\n✖ Account number %d already exists. Please choose a different number.\n", 
               r.accountNbr);
        goto accountEntry;
    }
    
    // Country entry with space handling
    printf("\nEnter the country:");
    char countryInput[100];
    scanf(" %[^\n]", countryInput);  // Read the entire line including spaces

    // Replace spaces with hyphens
    for (int i = 0; i < strlen(countryInput); i++) {
        if (countryInput[i] == ' ') {
            countryInput[i] = '-';
        }
    }

    // Copy to record
    strcpy(r.country, countryInput);
    
    // Phone number entry with validation
    phoneEntry:
    printf("\nEnter the phone number (format: 0XXXXXXXXX or +XXXXXXXXXXXX):");
    scanf("%s", phoneStr);
    
    if (!isValidPhoneNumber(phoneStr)) {
        printf("\n✖ Invalid phone number! Please use format 0XXXXXXXXX (10 digits) or +XXXXXXXXXXXX (13 digits).\n");
        goto phoneEntry;
    }
    
    // Store phone number as string
    strcpy(r.phoneStr, phoneStr);
    
    // Amount entry with validation
    amountEntry:
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    
    if (r.amount < 0) {
        printf("\n✖ Invalid amount! Please enter a positive value.\n");
        goto amountEntry;
    }
    
    // Account type entry with validation
    accountTypeEntry:
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);
    
    if (strcmp(r.accountType, "saving") != 0 && 
        strcmp(r.accountType, "current") != 0 && 
        strcmp(r.accountType, "fixed01") != 0 && 
        strcmp(r.accountType, "fixed02") != 0 && 
        strcmp(r.accountType, "fixed03") != 0) {
        printf("\n✖ Invalid account type! Please choose from the options provided.\n");
        goto accountTypeEntry;
    }

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

void transferOwnership(struct User u)
{
    struct Record r;
    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("temp.txt", "w");
    int accNum, found = 0, newOwnerId;
    char newOwner[50];

    if (!fp || !temp)
    {
        printf("Error opening records file.\n");
        return;
    }

    printf("\nEnter the account number to transfer ownership: ");
    scanf("%d", &accNum);

    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s\n\n",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) != EOF)
    {
        if (r.accountNbr == accNum && strcmp(r.name, u.name) == 0)
        {
            found = 1;

            printf("\nEnter the new owner's username: ");
            scanf("%s", newOwner);

            newOwnerId = getUserIdByUsername(newOwner);
            if (newOwnerId == -1)
            {
                printf("\n✖ Error: User '%s' not found!\n", newOwner);
                fclose(fp);
                fclose(temp);
                remove("temp.txt"); 
                return;
            }

            r.userId = newOwnerId;
            strcpy(r.name, newOwner);

            printf("\n✔ Ownership transferred successfully to %s!\n", newOwner);
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
        remove(RECORDS);
        rename("temp.txt", RECORDS);
         success(u);
    }
    else
    {
        printf("\n✖ Account not found or you are not the owner!\n");
        remove("temp.txt"); 
    }
}

int generateNewID()
{
    FILE *file = fopen("./data/records.txt", "r");
    if (file == NULL)
    {
        return 0;
    }

    int maxID = -1;
    char line[256];
    int tempID;
    
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%d", &tempID) == 1)
        {
            if (tempID > maxID) {
                maxID = tempID;
            }
        }
    }
    fclose(file);
    return maxID + 1;
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
    char userName[100];
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

    // Reset file pointer to beginning
    rewind(fp);
    
    while (getAccountFromFile(fp, userName, &r))
    {
        if (r.accountNbr == accNum)
        {
            // If not the owner, check if we should allow viewing
            if (strcmp(userName, u.name) != 0) {
                printf("\n✖ This account belongs to %s, not to you.\n", userName);
                fclose(fp);
                success(u);
                return;
            }
            
            found = 1;

            printf("\n===== Account Details =====\n");
            printf("Account Number: %d\n", r.accountNbr);
            printf("Name: %s\n", userName);
            char displayCountry[100];
            strcpy(displayCountry, r.country);

            // Convert hyphens back to spaces for display
            for (int i = 0; i < strlen(displayCountry); i++) {
                if (displayCountry[i] == '-') {
                    displayCountry[i] = ' ';
                }
            }

            printf("Country: %s\n", displayCountry);
            printf("Phone: %s\n", r.phoneStr);
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

            fclose(fp);
            success(u);
            return;
        }
    }

    printf("\n✖ Account not found!\n");
    fclose(fp);
    success(u);
}

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
    time_t now;
    struct tm *local_time;

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
        
        sprintf(r.phoneStr, "%d", r.phone);

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

                double oldAmount = r.amount;
                
                if (choice == '1') {
                    r.amount += amount;
                    printf("\n✔ Deposit successful! New balance: %.2lf\n", r.amount);
                    
                    now = time(NULL);
                    local_time = localtime(&now);
                    
                    FILE *logFile = fopen("./data/transactions.log", "a");
                    if (logFile) {
                        fprintf(logFile, "[%02d/%02d/%d %02d:%02d:%02d] DEPOSIT: Account %d, Amount: +%.2lf, New Balance: %.2lf\n",
                                local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_year + 1900,
                                local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
                                r.accountNbr, amount, r.amount);
                        fclose(logFile);
                    }
                    
                } else if (choice == '2') {
                    if (amount > r.amount) {
                        printf("\n✖ Insufficient balance! Your account balance is %.2lf\n", r.amount);
                    } else {
                        r.amount -= amount;
                        printf("\n✔ Withdrawal successful! New balance: %.2lf\n", r.amount);
                        
                        now = time(NULL);
                        local_time = localtime(&now);
                        
                        FILE *logFile = fopen("./data/transactions.log", "a");
                        if (logFile) {
                            fprintf(logFile, "[%02d/%02d/%d %02d:%02d:%02d] WITHDRAW: Account %d, Amount: -%.2lf, New Balance: %.2lf\n",
                                    local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_year + 1900,
                                    local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
                                    r.accountNbr, amount, r.amount);
                            fclose(logFile);
                        }
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
            printf("Error renaming temporary file!\n");
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
