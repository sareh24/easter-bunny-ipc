#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>


typedef struct Bunny {
    char name[50];
    char poem[200];
    int egg_count;
} Bunny;

void menu();
void gettingInput();
void register_Bunnies(char* name, char* poem);
void listBunnies();
void modifyBunny();
void deleteBunny();
void findBunnyKing();
void signal_handler(int sig);
void startWatering();

int main() {
    signal(SIGUSR1, signal_handler);
    menu();
    
    return 0;
}
void signal_handler(int sig) {
    printf("Chief Bunny received a signal from a bunny.\n");
}

void register_Bunnies(char *name, char *poem) {
    Bunny new_bunny;

    strncpy(new_bunny.name, name, sizeof(new_bunny.name) - 1);
    new_bunny.name[sizeof(new_bunny.name) - 1] = '\0';

    strncpy(new_bunny.poem, poem, sizeof(new_bunny.poem) - 1);
    new_bunny.poem[sizeof(new_bunny.poem) - 1] = '\0';

    new_bunny.egg_count = 0;
    FILE *file = fopen("bunnies.txt", "a");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%s|%s|%d\n", new_bunny.name, new_bunny.poem, new_bunny.egg_count);

    fclose(file);

    printf("Bunny registered and saved!\n");
    printf("Registered Bunny:\n");
    printf("Name: %s\n", new_bunny.name);
    printf("Poem: %s\n", new_bunny.poem);
    printf("Eggs: %d\n", new_bunny.egg_count);
}

void gettingInput() {
    char name[50];
    char poem[200];

    printf("Enter the name of the bunny: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    printf("Enter the poem: ");
    fgets(poem, sizeof(poem), stdin);
    poem[strcspn(poem, "\n")] = '\0';

    register_Bunnies(name, poem);
}

void listBunnies() {
    FILE *file = fopen("bunnies.txt", "r");
    if (file == NULL) {
        printf("No bunnies registered yet or unable to open file.\n");
        return;
    }

    char line[512];
    printf("\n--- List of Registered Bunnies ---\n");
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        char *name = strtok(line, "|");
        char *poem = strtok(NULL, "|");
        char *eggs = strtok(NULL, "|");

        if (name && poem && eggs) {
            printf("Name: %s\n", name);
            printf("Poem: %s\n", poem);
            printf("Eggs: %s\n", eggs);
            printf("-----------------------------\n");
        }
    }
    fclose(file);
}

void modifyBunny() {
    Bunny bunnies[100];
    int bunnyCount = 0;
    char line[512];
    char searchName[50];
    FILE *file = fopen("bunnies.txt", "r");
    if (file == NULL) {
        printf("No bunnies registered yet.\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        char *name = strtok(line, "|");
        char *poem = strtok(NULL, "|");
        char *eggs = strtok(NULL, "|");

        if (name && poem && eggs) {
            strncpy(bunnies[bunnyCount].name, name, sizeof(bunnies[bunnyCount].name));
            strncpy(bunnies[bunnyCount].poem, poem, sizeof(bunnies[bunnyCount].poem));
            bunnies[bunnyCount].egg_count = atoi(eggs);
            bunnyCount++;
        }
    }
    fclose(file);

    printf("Enter the name of the bunny to modify: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    int found = 0;
    for (int i = 0; i < bunnyCount; i++) {
        if (strcmp(bunnies[i].name, searchName) == 0) {
            found = 1;
            printf("What would you like to modify?\n");
            printf("1. Poem\n");
            printf("2. Egg Count\n");
            int option;
            scanf("%d", &option);
            getchar();

            if (option == 1) {
                char newPoem[200];
                printf("Enter new poem: ");
                fgets(newPoem, sizeof(newPoem), stdin);
                newPoem[strcspn(newPoem, "\n")] = '\0';
                strncpy(bunnies[i].poem, newPoem, sizeof(bunnies[i].poem));
                printf("Poem updated!\n");
            } else if (option == 2) {
                int newEggCount;
                printf("Enter new egg count: ");
                scanf("%d", &newEggCount);
                bunnies[i].egg_count = newEggCount;
                printf("Egg count updated!\n");
            } else {
                printf("Invalid option.\n");
            }
            break;
        }
    }

    if (!found) {
        printf("Bunny with name '%s' not found.\n", searchName);
        return;
    }

    file = fopen("bunnies.txt", "w");
    if (file == NULL) {
        printf("Error writing to file.\n");
        return;
    }

    for (int i = 0; i < bunnyCount; i++) {
        fprintf(file, "%s|%s|%d\n", bunnies[i].name, bunnies[i].poem, bunnies[i].egg_count);
    }

    fclose(file);
    printf("Changes saved successfully.\n");
}

void deleteBunny() {
    Bunny bunnies[100];
    int bunnyCount = 0;
    char line[512];
    char searchName[50];

    FILE *file = fopen("bunnies.txt", "r");
    if (file == NULL) {
        printf("No bunnies registered yet.\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        char *name = strtok(line, "|");
        char *poem = strtok(NULL, "|");
        char *eggs = strtok(NULL, "|");

        if (name && poem && eggs) {
            strncpy(bunnies[bunnyCount].name, name, sizeof(bunnies[bunnyCount].name));
            strncpy(bunnies[bunnyCount].poem, poem, sizeof(bunnies[bunnyCount].poem));
            bunnies[bunnyCount].egg_count = atoi(eggs);
            bunnyCount++;
        }
    }
    fclose(file);

    printf("Enter the name of the bunny to delete: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    int found = 0;
    file = fopen("bunnies.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < bunnyCount; i++) {
        if (strcmp(bunnies[i].name, searchName) == 0) {
            found = 1;
            continue;
        }
        fprintf(file, "%s|%s|%d\n", bunnies[i].name, bunnies[i].poem, bunnies[i].egg_count);
    }

    fclose(file);

    if (found) {
        printf("Bunny '%s' deleted successfully.\n", searchName);
    } else {
        printf("Bunny with name '%s' not found.\n", searchName);
    }
}

void findBunnyKing() {
    int maxEggs = -1;
    char kingName[50];
    char kingPoem[200];
    int countEgs;

    FILE *file = fopen("bunnies.txt", "r");
    if (file == NULL) {
        printf("No bunnies registered yet or unable to open file.\n");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        char *name = strtok(line, "|");
        char *poem = strtok(NULL, "|");
        char *eggs = strtok(NULL, "|");

        if (name && poem && eggs) {
            countEgs = atoi(eggs);
            if (countEgs > maxEggs) {
                maxEggs = countEgs;
                strncpy(kingName, name, sizeof(kingName));
                strncpy(kingPoem, poem, sizeof(kingPoem));
            }
        }
    }

    fclose(file);

    if (maxEggs != -1) {
        printf("\n Easter Bunny King \n");
        printf("Name: %s\n", kingName);
        printf("Poem: %s\n", kingPoem);
        printf("Eggs: %d\n", maxEggs);
    } else {
        printf("No bunnies found in the file.\n");
    }
}


void startWatering() {
    FILE *file = fopen("bunnies.txt", "r");
    if (!file) {
        printf("Unable to open file.\n");
        return;
    }

    Bunny bunnies[100];
    int bunnyCount = 0;
    char line[512];

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        char *name = strtok(line, "|");
        char *poem = strtok(NULL, "|");
        char *eggs = strtok(NULL, "|");

        if (name && poem && eggs) {
            strncpy(bunnies[bunnyCount].name, name, sizeof(bunnies[bunnyCount].name));
            strncpy(bunnies[bunnyCount].poem, poem, sizeof(bunnies[bunnyCount].poem));
            bunnyCount++;
        }
    }
    fclose(file);

    int pipefd[2];
    pipe(pipefd);

    for (int i = 0; i < bunnyCount; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            close(pipefd[0]);
            kill(getppid(), SIGUSR1);
            printf("%s recites: \"%s\"\n", bunnies[i].name, bunnies[i].poem);
            srand(getpid());
            int eggs = rand() % 20 + 1;
            write(pipefd[1], &eggs, sizeof(int));
            exit(0);
        }
    }

    close(pipefd[1]);
    FILE *temp = fopen("temp.txt", "w");
    for (int i = 0; i < bunnyCount; i++) {
        int eggs;
        wait(NULL);
        read(pipefd[0], &eggs, sizeof(int));
        bunnies[i].egg_count = eggs;
        fprintf(temp, "%s|%s|%d\n", bunnies[i].name, bunnies[i].poem, bunnies[i].egg_count);
    }
    fclose(temp);
    remove("bunnies.txt");
    rename("temp.txt", "bunnies.txt");
    findBunnyKing();
}


void menu() {
    int choice;
    do {
        printf("Easter Bunny Menu: \n");
        printf("1. Register a new bunny\n");
        printf("2. List all bunnies\n");
        printf("3. Modify a bunny\n");
        printf("4. Delete a bunny\n");
        printf("5. Show Easter Bunny King\n");
        printf("6. Start Watering\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        char input[10];
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);

        switch (choice) {
            case 1:
                gettingInput();
                break;
            case 2:
                listBunnies();
                break;
            case 3:
                modifyBunny();
                break;
            case 4:
                deleteBunny();
                break;
            case 5:
                findBunnyKing();
                break;
            case 6:
                startWatering();
                break;
            case 7:
                printf("Goodbye!\n");
                break;
            default:
                printf("Please enter a valid option (1-7).\n");
        }
    } while (choice != 7);
}
