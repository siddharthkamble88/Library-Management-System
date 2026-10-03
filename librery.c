#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* -------------------- CONSTANTS -------------------- */

#define RETURN_DAYS 30
#define PENALTY_PER_DAY 10


/* -------------------- BOOK STRUCTURE -------------------- */

struct Book
{
    int bookID;
    char title[100];
    char author[100];

    int available;
    char issuedTo[100];

    time_t issueDate;       // Date and time when book was issued

    struct Book *next;
};


/* -------------------- QUEUE STRUCTURE -------------------- */

struct Student
{
    char studentName[100];
    int bookID;

    struct Student *next;
};


/* -------------------- GLOBAL VARIABLES -------------------- */

struct Student *front = NULL;
struct Student *rear = NULL;

struct Book *head = NULL;


/* =========================================================
   FUNCTION 1: CREATE A NEW BOOK
   ========================================================= */

struct Book* createBook()
{
    struct Book *newBook;

    newBook = (struct Book*)malloc(sizeof(struct Book));

    if (newBook == NULL)
    {
        printf("\nMemory allocation failed!\n");
        exit(1);
    }

    printf("\nEnter Book ID: ");
    scanf("%d", &newBook->bookID);

    printf("Enter Book Title: ");
    scanf(" %[^\n]", newBook->title);

    printf("Enter Author Name: ");
    scanf(" %[^\n]", newBook->author);

    newBook->available = 1;

    strcpy(newBook->issuedTo, "None");

    newBook->issueDate = 0;

    newBook->next = NULL;

    return newBook;
}


/* =========================================================
   FUNCTION 2: ADD BOOK TO LINKED LIST
   ========================================================= */

void addBook()
{
    struct Book *newBook;
    struct Book *temp;

    newBook = createBook();

    if (head == NULL)
    {
        head = newBook;
    }
    else
    {
        temp = head;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newBook;
    }

    printf("\nBook added successfully!\n");
}


/* =========================================================
   FUNCTION 3: DISPLAY ALL BOOKS
   ========================================================= */

void displayBooks()
{
    struct Book *temp;

    if (head == NULL)
    {
        printf("\nNo books available in library.\n");
        return;
    }

    temp = head;

    printf("\n================ LIBRARY BOOKS ================\n");

    while (temp != NULL)
    {
        printf("\nBook ID      : %d", temp->bookID);
        printf("\nTitle        : %s", temp->title);
        printf("\nAuthor       : %s", temp->author);

        if (temp->available == 1)
        {
            printf("\nStatus       : Available");
        }
        else
        {
            printf("\nStatus       : Issued");
            printf("\nIssued To    : %s", temp->issuedTo);

            /* Display issue date */
            printf("\nIssue Date   : %s", ctime(&temp->issueDate));
            printf("Return Limit : 30 days");
        }

        printf("-----------------------------------------------");

        temp = temp->next;
    }
}


/* =========================================================
   FUNCTION 4: SEARCH BOOK
   ========================================================= */

struct Book* searchBook(int bookID)
{
    struct Book *temp = head;

    while (temp != NULL)
    {
        if (temp->bookID == bookID)
        {
            return temp;
        }

        temp = temp->next;
    }

    return NULL;
}


/* =========================================================
   FUNCTION 5: ADD STUDENT TO WAITING QUEUE
   ========================================================= */

void enqueue()
{
    struct Student *newStudent;

    newStudent = (struct Student*)malloc(sizeof(struct Student));

    if (newStudent == NULL)
    {
        printf("\nMemory allocation failed!\n");
        return;
    }

    printf("\nEnter Student Name: ");
    scanf(" %[^\n]", newStudent->studentName);

    printf("Enter Book ID for which student is waiting: ");
    scanf("%d", &newStudent->bookID);

    newStudent->next = NULL;

    if (rear == NULL)
    {
        front = rear = newStudent;
    }
    else
    {
        rear->next = newStudent;
        rear = newStudent;
    }

    printf("\nStudent added to waiting queue successfully!\n");
}


/* =========================================================
   FUNCTION 6: DISPLAY WAITING QUEUE
   ========================================================= */

void displayQueue()
{
    struct Student *temp;

    if (front == NULL)
    {
        printf("\nWaiting queue is empty.\n");
        return;
    }

    temp = front;

    printf("\n================ WAITING QUEUE ================\n");

    while (temp != NULL)
    {
        printf("\nStudent Name : %s", temp->studentName);
        printf("\nBook ID      : %d", temp->bookID);
        printf("\n-----------------------------------------------");

        temp = temp->next;
    }
}


/* =========================================================
   FUNCTION 7: ISSUE BOOK
   ========================================================= */

void issueBook()
{
    int bookID;
    int choice;

    char studentName[100];

    struct Book *book;

    printf("\nEnter Book ID to issue: ");
    scanf("%d", &bookID);

    book = searchBook(bookID);

    if (book == NULL)
    {
        printf("\nBook not found!\n");
        return;
    }

    /* Check whether book is already issued */

    if (book->available == 0)
    {
        printf("\nBook is already issued to %s.\n",
               book->issuedTo);

        printf("\nDo you want to join the waiting queue?\n");
        printf("1. Yes\n");
        printf("2. No\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            enqueue();
        }

        return;
    }

    /* Issue book */

    printf("Enter Student Name: ");
    scanf(" %[^\n]", studentName);

    book->available = 0;

    strcpy(book->issuedTo, studentName);

    /* Store current date and time */

    book->issueDate = time(NULL);

    printf("\n===============================================\n");
    printf("          BOOK ISSUED SUCCESSFULLY\n");
    printf("===============================================\n");

    printf("\nBook ID      : %d", book->bookID);
    printf("\nBook         : %s", book->title);
    printf("\nIssued To    : %s", book->issuedTo);

    printf("\nIssue Date   : %s", ctime(&book->issueDate));

    printf("Return Limit : 30 days");
    printf("\nPenalty      : Rs.10 per day after 30 days\n");
}


/* =========================================================
   FUNCTION 8: RETURN BOOK
   ========================================================= */

void returnBook()
{
    int bookID;

    struct Book *book;

    printf("\nEnter Book ID to return: ");
    scanf("%d", &bookID);

    book = searchBook(bookID);

    if (book == NULL)
    {
        printf("\nBook not found!\n");
        return;
    }

    /* Check if book is already available */

    if (book->available == 1)
    {
        printf("\nThis book is already available.\n");
        return;
    }


    /* -----------------------------------------------------
       CALCULATE NUMBER OF DAYS
       ----------------------------------------------------- */

    time_t currentDate;

    currentDate = time(NULL);

    double seconds;

    int totalDays;
    int overdueDays;
    int penalty;

    seconds = difftime(currentDate, book->issueDate);

    totalDays = (int)(seconds / (60 * 60 * 24));


    /* -----------------------------------------------------
       CALCULATE PENALTY
       ----------------------------------------------------- */

    overdueDays = 0;
    penalty = 0;

    if (totalDays > RETURN_DAYS)
    {
        overdueDays = totalDays - RETURN_DAYS;

        penalty = overdueDays * PENALTY_PER_DAY;
    }


    /* -----------------------------------------------------
       DISPLAY RETURN INFORMATION
       ----------------------------------------------------- */

    printf("\n===============================================\n");
    printf("             BOOK RETURN DETAILS\n");
    printf("===============================================\n");

    printf("\nBook ID          : %d", book->bookID);
    printf("\nBook             : %s", book->title);
    printf("\nReturned By      : %s", book->issuedTo);

    printf("\nIssue Date       : %s", ctime(&book->issueDate));

    printf("Total Days       : %d", totalDays);

    printf("\nAllowed Days     : %d", RETURN_DAYS);


    if (overdueDays > 0)
    {
        printf("\nOverdue Days     : %d", overdueDays);

        printf("\nPenalty Per Day  : Rs.%d",
               PENALTY_PER_DAY);

        printf("\nTotal Penalty    : Rs.%d",
               penalty);
    }
    else
    {
        printf("\nOverdue Days     : 0");
        printf("\nTotal Penalty    : Rs.0");
        printf("\nStatus           : Returned on time");
    }


    /* -----------------------------------------------------
       MAKE BOOK AVAILABLE
       ----------------------------------------------------- */

    book->available = 1;

    strcpy(book->issuedTo, "None");

    book->issueDate = 0;


    /* -----------------------------------------------------
       CHECK WAITING QUEUE
       ----------------------------------------------------- */

    if (front != NULL)
    {
        struct Student *temp = front;
        struct Student *previous = NULL;

        while (temp != NULL)
        {
            if (temp->bookID == bookID)
            {
                /*
                   Automatically issue the returned book
                   to the first waiting student.
                */

                book->available = 0;

                strcpy(book->issuedTo,
                       temp->studentName);

                /* New issue date */

                book->issueDate = time(NULL);

                printf("\n\nBook was reserved by a waiting student.");

                printf("\nAutomatically issued to: %s\n",
                       temp->studentName);


                /* Remove student from queue */

                if (previous == NULL)
                {
                    front = temp->next;
                }
                else
                {
                    previous->next = temp->next;
                }


                /* Update rear */

                if (temp == rear)
                {
                    rear = previous;
                }


                free(temp);

                return;
            }

            previous = temp;

            temp = temp->next;
        }
    }
}


/* =========================================================
   FUNCTION 9: DELETE BOOK
   ========================================================= */

void deleteBook()
{
    int bookID;

    struct Book *temp;
    struct Book *previous;

    printf("\nEnter Book ID to delete: ");
    scanf("%d", &bookID);

    temp = head;
    previous = NULL;

    while (temp != NULL)
    {
        if (temp->bookID == bookID)
        {
            /* Cannot delete issued book */

            if (temp->available == 0)
            {
                printf("\nCannot delete an issued book.\n");
                return;
            }


            /* Delete first book */

            if (previous == NULL)
            {
                head = temp->next;
            }

            /* Delete middle/end book */

            else
            {
                previous->next = temp->next;
            }

            free(temp);

            printf("\nBook deleted successfully!\n");

            return;
        }

        previous = temp;

        temp = temp->next;
    }

    printf("\nBook not found!\n");
}


/* =========================================================
   FUNCTION 10: MAIN FUNCTION
   ========================================================= */

int main()
{
    int choice;

    printf("\n===============================================");
    printf("\n       LIBRARY BOOK ISSUE SYSTEM");
    printf("\n===============================================\n");

    printf("\nRules:");
    printf("\n1. Book must be returned within 30 days.");
    printf("\n2. After 30 days, penalty is Rs.10 per day.");
    printf("\n");


    do
    {
        printf("\n\n--------------- MAIN MENU ----------------");

        printf("\n1. Add Book");
        printf("\n2. Display All Books");
        printf("\n3. Search Book");
        printf("\n4. Issue Book");
        printf("\n5. Return Book");
        printf("\n6. Add Student to Waiting Queue");
        printf("\n7. Display Waiting Queue");
        printf("\n8. Delete Book");
        printf("\n9. Exit");

        printf("\n-------------------------------------------");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);


        switch (choice)
        {
            /* -----------------------------------------
               ADD BOOK
               ----------------------------------------- */

            case 1:
                addBook();
                break;


            /* -----------------------------------------
               DISPLAY BOOKS
               ----------------------------------------- */

            case 2:
                displayBooks();
                break;


            /* -----------------------------------------
               SEARCH BOOK
               ----------------------------------------- */

            case 3:
            {
                int bookID;

                struct Book *book;

                printf("\nEnter Book ID to search: ");
                scanf("%d", &bookID);

                book = searchBook(bookID);

                if (book != NULL)
                {
                    printf("\nBook Found!");

                    printf("\nBook ID : %d",
                           book->bookID);

                    printf("\nTitle   : %s",
                           book->title);

                    printf("\nAuthor  : %s",
                           book->author);


                    if (book->available)
                    {
                        printf("\nStatus  : Available\n");
                    }

                    else
                    {
                        printf("\nStatus  : Issued");

                        printf("\nIssued To: %s",
                               book->issuedTo);

                        printf("\nIssue Date: %s",
                               ctime(&book->issueDate));

                        printf("Return Limit: 30 days\n");
                    }
                }

                else
                {
                    printf("\nBook not found!\n");
                }

                break;
            }


            /* -----------------------------------------
               ISSUE BOOK
               ----------------------------------------- */

            case 4:
                issueBook();
                break;


            /* -----------------------------------------
               RETURN BOOK
               ----------------------------------------- */

            case 5:
                returnBook();
                break;


            /* -----------------------------------------
               ADD STUDENT TO QUEUE
               ----------------------------------------- */

            case 6:
                enqueue();
                break;


            /* -----------------------------------------
               DISPLAY QUEUE
               ----------------------------------------- */

            case 7:
                displayQueue();
                break;


            /* -----------------------------------------
               DELETE BOOK
               ----------------------------------------- */

            case 8:
                deleteBook();
                break;


            /* -----------------------------------------
               EXIT
               ----------------------------------------- */

            case 9:
                printf("\nThank you for using Library Book Issue System!\n");
                break;


            /* -----------------------------------------
               INVALID CHOICE
               ----------------------------------------- */

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 9);


    return 0;
}