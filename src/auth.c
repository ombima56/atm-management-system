#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

char *USERS = "./data/users.txt";

struct User {
    int id;
    char name[50];
    char password[50];
};

void registerMenu(char a[50], char pass[50]) {
    struct User u;
    FILE *fp = fopen(USERS, "a+");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    u.id = getNextID();

    system("clear");
    printf("\n\n\t\t======= Register =======\n");

    do {
        printf("\nEnter username: ");
        fgets(a, 50, stdin);
        a[strcspn(a, "\n")] = 0;

        if (strchr(a, ' ') != NULL) {
            printf("Username must not contain spaces.\n");
        } else if (strlen(a) == 0) {
            printf("Username cannot be empty.\n");
        }
    } while (strlen(a) == 0 || strchr(a, ' ') != NULL);

    while (fscanf(fp, "%d %49s %49s", &u.id, u.name, u.password) == 3) {
        if (strcmp(u.name, a) == 0) {
            printf("Username already exists.\n");
            fclose(fp);
            return;
        }
    }

    do {
        printf("\nEnter password: ");
        fgets(pass, 50, stdin);
        pass[strcspn(pass, "\n")] = 0;

        if (strlen(pass) < 2) {
            printf("\nPassword must be at least 2 characters long.\n");
        }
        else if (strchr(pass, ' ') != NULL) {
            printf("Password must not contain spaces.\n");
        }
    } while (strlen(pass) < 2 || strchr(pass, ' ') != NULL);

    strcpy(u.name, a);
    strcpy(u.password, pass);

    fprintf(fp, "%d %s %s\n", u.id, u.name, u.password);
    fclose(fp);

    int options;
    printf("\n✔ Registration Successful!\n");

    printf("\n\nEnter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &options);
    while (getchar() != '\n'); // Clear input buffer

    if (options == 1) {
        mainMenu(u);
        return;
    } else if (options == 0) {
        exit(1);
    } else {
        printf("Invalid operation!\n");
        exit(1);
    }
    
}

int getNextID() {
    FILE *file = fopen(USERS, "r");
    if (file == NULL) {
        return 0;
    }

    int lastID = 0, id;
    char name[50], password[50];

    while (fscanf(file, "%d %s %s", &id, name, password) == 3) {
        if (id > lastID) {
            lastID = id;
        }
    }

    fclose(file);
    return lastID + 1;
};

void loginMenu(char a[50], char pass[50]) {
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:\n");

    while (getchar() != '\n'); // Clear input buffer

    do {
        printf("\nEnter username: ");
        fgets(a, 50, stdin);
        a[strcspn(a, "\n")] = 0;

        if (strchr(a, ' ') != NULL) {
            printf("Username must not contain spaces.\n");
        } else if (strlen(a) == 0) {
            printf("Username cannot be empty.\n");
        }
    } while (strlen(a) == 0 || strchr(a, ' ') != NULL);

    // Disable echo for password
    if (tcgetattr(fileno(stdin), &oflags) != 0) {
        perror("tcgetattr");
        exit(1);
    }

    nflags = oflags;
    nflags.c_lflag &= ~ECHO; 
    nflags.c_lflag |= ECHONL; 

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }

    printf("\nEnter password: ");
    fgets(pass, 50, stdin);
    pass[strcspn(pass, "\n")] = 0;

    // Restore terminal settings
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }

    printf("\nLogin successful for user: %s\n", a);
}

// Function to retrieve the password from the file
const char *getPassword(struct User u) {
    FILE *fp;
    struct User userChecker;
    char buffer[200];

    if ((fp = fopen(USERS, "r")) == NULL) {
        perror("Error opening file");
        exit(1);
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (sscanf(buffer, "%d %49s %49s", &userChecker.id, userChecker.name, userChecker.password) == 3) {
            if (strcmp(u.name, userChecker.name) == 0) {
                fclose(fp);
                return strdup(userChecker.password);
            }
        }
    }

    fclose(fp);
    return "no user found";
}
