#include "header.h"

void mainMenu(struct User u)
{
    int options;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf("%d", &options);

    switch (options) {
    case 1:
        createNewAcc(u);
        break;
    case 2: 
        updateAccountInfo(u);
        break;
    case 3:
        checkAccountDetails(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
        removeExistingAccount(u);
        break;
    case 7: 
        transferOwnership(u);
        break;
    case 8:
        printf("\nThank you for visiting our ATM system. Goodbye!\n");
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
    }
};

void initMenu(struct User *u)
{
    int r = 0;
    int option;
    char input[50];
    int valid_input = 0;

    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        printf("\nEnter your choice (1-3): ");
        fgets(input, sizeof(input), stdin);
        valid_input = sscanf(input, "%d", &option);

        if (valid_input != 1 || option < 1 || option > 3) {
            printf("\n✖ Invalid option! Please choose option 1 for login, 2 for register, or 3 to exit.\n");
            continue;
        }

        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            if (strcmp(u->password, getPassword(*u)) == 0)
            {
                printf("\n\nPassword Match!");
            }
            else
            {
                printf("\nWrong password!! or User Name\n");
                exit(1);
            }
            r = 1;
            break;
        case 2:
            registerMenu(u->name, u->password);
            break;
        case 3:
            printf("\nThank you for visiting our ATM system. Goodbye!\n");
            exit(1);
            break;
        default:
            printf("\n✖ Invalid option! Please choose option 1 for login, 2 for register, or 3 to exit.\n");
        }
    }
};

int main()
{
    struct User u;
    
    initMenu(&u);
    mainMenu(u);
    return 0;
}
