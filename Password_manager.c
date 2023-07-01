#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PASSWORDS 100
#define MAX_LENGTH 50

typedef struct
{
    char website[MAX_LENGTH];
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
} PasswordEntry;

typedef struct
{
    PasswordEntry entries[MAX_PASSWORDS];
    int count;
} PasswordManager;

void addPassword(PasswordManager *manager, const char *website, const char *username, const char *password)
{
    if (manager->count >= MAX_PASSWORDS)
    {
        printf("Password manager is full. Cannot add more passwords.\n");
        return;
    }

    PasswordEntry newEntry;
    strncpy(newEntry.website, website, MAX_LENGTH);
    strncpy(newEntry.username, username, MAX_LENGTH);
    strncpy(newEntry.password, password, MAX_LENGTH);

    manager->entries[manager->count] = newEntry;
    manager->count++;
}

void savePasswords(const PasswordManager *manager, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < manager->count; i++)
    {
        fprintf(file, "Website: %s\n", manager->entries[i].website);
        fprintf(file, "Username: %s\n", manager->entries[i].username);
        fprintf(file, "Password: %s\n", manager->entries[i].password);
        fprintf(file, "------------------------\n");
    }

    fclose(file);
    printf("Passwords saved to file: %s\n", filename);
}

void loadPasswords(PasswordManager *manager, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
        return;
    }

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n' || line[0] == '\r')
            continue;

        char key[MAX_LENGTH];
        char value[MAX_LENGTH];

        if (sscanf(line, "%[^:]: %[^\n]", key, value) != 2)
            continue;

        if (strcmp(key, "Website") == 0)
            strncpy(manager->entries[manager->count].website, value, MAX_LENGTH);
        else if (strcmp(key, "Username") == 0)
            strncpy(manager->entries[manager->count].username, value, MAX_LENGTH);
        else if (strcmp(key, "Password") == 0)
            strncpy(manager->entries[manager->count].password, value, MAX_LENGTH);
        else if (strcmp(key, "------------------------") == 0)
            manager->count++;
    }

    fclose(file);
    printf("Passwords loaded from file: %s\n", filename);
}

void printPasswords(const PasswordManager *manager)
{
    printf("Stored Passwords:\n");
    for (int i = 0; i < manager->count; i++)
    {
        printf("Website: %s\n", manager->entries[i].website);
        printf("Username: %s\n", manager->entries[i].username);
        printf("Password: %s\n", manager->entries[i].password);
        printf("------------------------\n");
    }
}

int main()
{
    PasswordManager manager;
    manager.count = 0;

    int numPasswords;
    printf("How many passwords do you want to add? ");
    scanf("%d", &numPasswords);

    for (int i = 0; i < numPasswords; i++)
    {
        char website[MAX_LENGTH];
        char username[MAX_LENGTH];
        char password[MAX_LENGTH];

        printf("\nEnter details for Password %d:\n", i + 1);

        printf("Website: ");
        scanf("%s", website);

        printf("Username: ");
        scanf("%s", username);

        printf("Password: ");
        scanf("%s", password);

        addPassword(&manager, website, username, password);
    }

    printPasswords(&manager);

    char filename[MAX_LENGTH];
    printf("\nEnter the filename to save the passwords: ");
    scanf("%s", filename);

    savePasswords(&manager, filename);

    // Reset the manager
    manager.count = 0;

    loadPasswords(&manager, filename);

    printPasswords(&manager);

    return 0;
}
