#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct books {
    int id;
    char bookName[50];
    char authorName[50];
    char date[12];
};

struct student {
    int id;
    char sName[50];
    char sClass[50];
    int sRoll;
    char bookName[50];
    char date[12];
};

FILE *fp;

void addBook();
void booksList();
void del();
void issueBook();
void issueList();

int main() {
    int ch;

    while (1) {
        system("cls");
        printf("<== Library Management System ==>\n");
        printf("1. Add Book\n");
        printf("2. Books List\n");
        printf("3. Remove Book\n");
        printf("4. Issue Book\n");
        printf("5. Issued Book List\n");
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 0:
                exit(0);
            case 1:
                addBook();
                break;
            case 2:
                booksList();
                break;
            case 3:
                del();
                break;
            case 4:
                issueBook();
                break;
            case 5:
                issueList();
                break;
            default:
                printf("Invalid Choice...\n\n");
                break;
        }
        printf("Press Enter To Continue...");
        getchar(); // Clear newline character from buffer
        getchar(); // Wait for user to press a key
    }

    return 0;
}

void addBook() {
    struct books b;
    char myDate[12];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
    strcpy(b.date, myDate);

    fp = fopen("books.txt", "ab");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter book id: ");
    scanf("%d", &b.id);

    printf("Enter book name: ");
    getchar(); // Clear newline character from buffer
    fgets(b.bookName, sizeof(b.bookName), stdin);
    b.bookName[strcspn(b.bookName, "\n")] = '\0'; // Remove newline from fgets input

    printf("Enter author name: ");
    fgets(b.authorName, sizeof(b.authorName), stdin);
    b.authorName[strcspn(b.authorName, "\n")] = '\0'; // Remove newline from fgets input

    printf("Book Added Successfully\n");

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);
}

void booksList() {
    struct books b;

    system("cls");
    printf("<== Available Books ==>\n\n");
    printf("%-10s %-30s %-20s %s\n\n", "Book id", "Book Name", "Author", "Date");

    fp = fopen("books.txt", "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        printf("%-10d %-30s %-20s %s\n", b.id, b.bookName, b.authorName, b.date);
    }

    fclose(fp);
}

void del() {
    int id, found = 0;
    struct books b;
    FILE *ft;

    system("cls");
    printf("<== Remove Books ==>\n\n");
    printf("Enter Book id to remove: ");
    scanf("%d", &id);

    fp = fopen("books.txt", "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    ft = fopen("temp.txt", "wb");
    if (ft == NULL) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (id == b.id) {
            found = 1;
        } else {
            fwrite(&b, sizeof(b), 1, ft);
        }
    }

    if (found) {
        printf("\n\nDeleted Successfully.\n");
    } else {
        printf("\n\nRecord Not Found !\n");
    }

    fclose(fp);
    fclose(ft);

    remove("books.txt");
    rename("temp.txt", "books.txt");
}

void issueBook() {
    struct books b;
    struct student s;
    char myDate[12];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
    strcpy(s.date, myDate);

    int found = 0;

    system("cls");
    printf("<== Issue Books ==>\n\n");

    printf("Enter Book id to issue: ");
    scanf("%d", &s.id);

    fp = fopen("books.txt", "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == s.id) {
            strcpy(s.bookName, b.bookName);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("No book found with this id\n");
        printf("Please try again...\n\n");
        return;
    }

    fp = fopen("issue.txt", "ab");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Student Name: ");
    getchar(); // Clear newline character from buffer
    fgets(s.sName, sizeof(s.sName), stdin);
    s.sName[strcspn(s.sName, "\n")] = '\0'; // Remove newline from fgets input

    printf("Enter Student Class: ");
    fgets(s.sClass, sizeof(s.sClass), stdin);
    s.sClass[strcspn(s.sClass, "\n")] = '\0'; // Remove newline from fgets input

    printf("Enter Student Roll: ");
    scanf("%d", &s.sRoll);

    printf("Book Issued Successfully\n\n");

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);
}

void issueList() {
    struct student s;

    system("cls");
    printf("<== Book Issue List ==>\n\n");

    printf("%-10s %-30s %-20s %-10s %-30s %s\n\n", "S.id", "Name", "Class", "Roll", "Book Name", "Date");

    fp = fopen("issue.txt", "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        printf("%-10d %-30s %-20s %-10d %-30s %s\n", s.id, s.sName, s.sClass, s.sRoll, s.bookName, s.date);
    }

    fclose(fp);
}
