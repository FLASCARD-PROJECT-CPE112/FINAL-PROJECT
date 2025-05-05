#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max 1000
#define TABLE_SIZE 100

int idcount = 1;  

typedef struct 
{
    char username[100]; //store username
    char password[100]; //store passowrd
}User;

User currentuser;


// Flashcard structure
struct card 
{
    int ID; //flashcard id
    char question[Max];
    char answer[Max];
    char category[100];
    char subcategory[100];
    int score;
    struct card *next;
};


struct HashTableEntry 
{
    int key;
    struct card *card; //card (pointer)
};


struct card *head = NULL; //NULL = no flashcard
struct HashTableEntry hashTable[TABLE_SIZE];


int hashFunction(int key) 
{
    return key % TABLE_SIZE;
}

struct card *createCard(int id, const char *question, const char *answer, const char *category, const char *subcategory, int score) 
{
    struct card *newCard = (struct card *)malloc(sizeof(struct card));
    newCard->ID = id;  //newCard (pointer) -> new flashcard
    strcpy(newCard->question, question); //copy the question
    strcpy(newCard->answer, answer);
    strcpy(newCard->category, category);
    strcpy(newCard->subcategory, subcategory);
    newCard->score = score;
    newCard->next = NULL;
    return newCard;
}

// save information in csv file
void saveFlashcardsToCSV(const char *filename) 
{
    FILE *file = fopen(filename, "w"); //w = reset old data
    if (!file) // file (pointer)
    {
        printf("Error! Cannot open file to save flashcards.\n"); //if can't open file will display erorr and exit function
        return;
    }

   
    fprintf(file, "ID,Question,Answer,Score,Category,Subcategory\n"); // table head in csv file

    struct card *temp = head; //point to the flashcard in linklist
    while (temp != NULL) //loop run thru flashcard
    {
        fprintf(file, "%d,%s,%s,%d,%s,%s\n",
                temp->ID, temp->question, temp->answer, temp->score,
                temp->category, temp->subcategory); 
        temp = temp->next; //temp point to next flashcard
    }

    fclose(file);
}


//add flashcard function
void addFlashcard() 
{
    int score; //collect data form user first 
    char question[Max], answer[Max], category[100], subcategory[100];


    printf("Enter question: "); //user input question
    fgets(question, Max, stdin);
    question[strcspn(question, "\n")] = '\0';

    printf("Enter answer: "); //user input answer
    fgets(answer, Max, stdin);
    answer[strcspn(answer, "\n")] = '\0';

    printf("Enter score (1-10): "); //user input score
    scanf("%d", &score);
    getchar();

    if (score < 1 || score > 10) // score can select only 1-10
    {
    printf("Invalid score. Please enter a number between 1 and 10.\n");
    return;  
    }

    printf("Enter category: ");  //user input category
    fgets(category, 100, stdin);
    category[strcspn(category, "\n")] = '\0';

    printf("Enter subcategory: "); //user input subcategory
    fgets(subcategory, 100, stdin);
    subcategory[strcspn(subcategory, "\n")] = '\0';


    // use id count and increase id
    int id = idcount++;


    struct card *newCard = createCard(id, question, answer, category, subcategory, score);

    if (head == NULL)
    {
        head = newCard;
    }
        
    else  //add new node in linked list (in collect flashcard)
    {
        struct card *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
            
        temp->next = newCard;
    }

    int index = hashFunction(id); //collect flashcard in hash table and use id to find index(flashcard)
    hashTable[index].key = id;
    hashTable[index].card = newCard;

    printf("Flashcard added successfully with ID %d.\n", id); 

    //save for each user 
    char filename[200];
    sprintf(filename, "%s_flashcards.csv", currentuser.username); //flashcard will save in separate file for each user
    saveFlashcardsToCSV(filename);
}



// view flashcard store in linked list
void viewFlashcards() 
{
    if (head == NULL) //if no flashcard or user dont add flashcard yet it will exit the function
    {
        printf("No flashcards available.\n");
        return;
    }

    struct card *temp = head;
    printf("\n--- Flashcards ---\n");
    while (temp != NULL) 
    {   //show all detail of flashcard
        printf("ID: %d\nCategory: %s\nSubcategory: %s\nQuestion: %s\nAnswer: %s\nScore: %d\n\n",
               temp->ID, temp->category, temp->subcategory, temp->question, temp->answer, temp->score);
        temp = temp->next;
    }
}

//serach for a flashcard by ID > use hash table
struct card *searchByID(int id) 
{
    int index = hashFunction(id);
    if (hashTable[index].key == id)
        return hashTable[index].card;
    return NULL;
}


//edit flashcard funcion
void editFlashcard() 
{
    if (head == NULL) //no flashcard
    {
        printf("No flashcards available.\n");
        return;
    }

    printf("\n--- Available Flashcards ---\n"); //show all flashcard detail with ID
    struct card* temp = head;
    while (temp != NULL) 
    {
        printf("ID: %d | Category: %s | Subcategory: %s | Question: %s\n",
               temp->ID, temp->category, temp->subcategory, temp->question);
        temp = temp->next;
    }

    int id;
    char input[Max];

    printf("\nEnter the ID of the flashcard to edit: "); //user input ID to edit flashcard
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == 0) //check ID is enter or not if not go back to the menu
    {
        printf("No ID entered. Go back to the menu.\n");
        return;
    }

    if (sscanf(input, "%d", &id) != 1)
    {
        printf("Invalid input. Go back to the menu.\n");
        return;
    }
    

    struct card *cardToEdit = searchByID(id); //flashcard not found if user enter ID does not exist
    if (cardToEdit == NULL) 
    {
        printf("Flashcard not found.\n");
        return;
    }

    printf("\nFlashcard found:\n");
    printf("ID: %d\nCategory: %s\nSubcategory: %s\nQuestion: %s\nAnswer: %s\nScore: %d\n\n",
           cardToEdit->ID,
           cardToEdit->category,
           cardToEdit->subcategory,
           cardToEdit->question,
           cardToEdit->answer,
           cardToEdit->score);

    printf("You can edit any field, or just press Enter to keep it the same.\n");

    // Edit Question
    printf("New Question (or press Enter to keep): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) strcpy(cardToEdit->question, input);

    // Edit Answer
    printf("New Answer (or press Enter to keep): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) strcpy(cardToEdit->answer, input);

    // Edit Score
    printf("New score (or press Enter to keep): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) > 0) //check user input score correct
    {
        int newScore;
        if (sscanf(input, "%d", &newScore) == 1 && newScore >= 1 && newScore <= 10 )
        {
            cardToEdit->score = newScore;
        }
            
        else
        {
            printf("Invalid score. Keeping current score.\n");
        }
            
    }
    // if user edit flashcard and logout > flashcard will be save automatically
    char filename[200];
    sprintf(filename, "%s_flashcards.csv", currentuser.username); 
    saveFlashcardsToCSV(filename);

    printf("Flashcard updated successfully.\n");
}


//delete flashcard
void deleteFlashcard() 
{
    if (head == NULL) // check have flashcard or not
    {
        printf("No flashcards available.\n");
        return;
    }

    // show flashcards 
    printf("\n--- Available Flashcards ---\n");
    struct card* temp = head;

    while (temp != NULL) 
    {
        printf("ID: %d | Category: %s | Subcategory: %s | Question: %s\n", 
        temp->ID, temp->category, temp->subcategory, temp->question);
        temp = temp->next;
    }

    int id;
    char input[100];

    printf("\nEnter the ID of the flashcard to delete: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; 

    if(strlen(input) == 0 || sscanf(input, "%d", &id) != 1)
    {
        printf("Invalid input. Returning to menu.\n");
        return;
    }
    //use hash table for search flashcard by ID
    struct card *cardToDelete = searchByID(id);
    if (cardToDelete == NULL) 
    {
        printf("Flashcard not found.\n"); //flashcard not found if enter does not exist
        return;
    }

    // confirm before delete
    printf("\nFlashcard found:\n");
    printf("ID: %d\nCategory: %s\nSubcategory: %s\nQuestion: %s\nAnswer: %s\nScore: %d\n",
           cardToDelete->ID, cardToDelete->category, cardToDelete->subcategory,
           cardToDelete->question, cardToDelete->answer, cardToDelete->score);

    while (1) 
    {
    printf("Are you sure you want to delete this flashcard? (y/n): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  

    if (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0) //user can select y if they want to delete flashcard
    {
        break;  
    } 
    else if (strcmp(input, "n") == 0 || strcmp(input, "N") == 0) 
    {
        printf("Cancelled.\n");
        return;
    } 
    else 
    {
        printf("Invalid input. Please enter 'y' or 'n'.\n");
    }
}

    // delete linked list
    struct card *temp2 = head, *prev = NULL;
    while (temp2 != NULL && temp2->ID != id) //search ID that user want to remove in linked list
    {
        prev = temp2;
        temp2 = temp2->next;
    }

    if (temp2 == NULL) 
    {
        printf("Flashcard with ID %d not found. Nothing was deleted.\n", id);
        return;
    }

    if (prev == NULL)
    {
        head = temp2->next;
    }  
    else
    {
        prev->next = temp2->next;
    }
        
    free(temp2);

    //remove flashcard from hash table
    int index = hashFunction(id);
    if (hashTable[index].key == id)
    {
        hashTable[index].key = -1;
        hashTable[index].card = NULL;
    }
    


    char filename[200];
    sprintf(filename, "%s_flashcards.csv", currentuser.username); 
    saveFlashcardsToCSV(filename);

    printf("Flashcard deleted successfully.\n");
}


int countFlashcards() //count flascard in linked list
{
    int count = 0;
    struct card *temp = head;
    while (temp) 
    {
        count++;
        temp = temp->next;
    }
    return count;
}

int totalScore() //count flashcard score in linked list
{
    int sum = 0;
    struct card *temp = head;
    while (temp) 
    {
        sum += temp->score;
        temp = temp->next;
    }
    return sum;
}

//display unique cate/subcategory without duplicate
void listCategories() 
{
    struct card *temp = head;
    char shown[100][200];  //store category and subcategory that already show
    int count = 0;

    printf("\nAvailable Categories and Subcategories:\n");

    while (temp != NULL) 
    {
        char currentCombo[200];
        sprintf(currentCombo, "%s/%s", temp->category, temp->subcategory); //combine cate/subcate into one string

        int duplicate = 0; 
        for (int i = 0; i < count; i++) 
        {
            if (strcmp(shown[i], currentCombo) == 0) 
            {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) 
        {
            strcpy(shown[count], currentCombo);
            count++;
            printf("- %s / %s\n", temp->category, temp->subcategory);
        }

        temp = temp->next;
    }
}



//practice flashcard function
void practiceFlashcards() 
{
    char category[100], subcategory[100], sortOrder[Max];
    listCategories();

    printf("Enter category: "); //user need to input category and subcategory before practice flashcard
    fgets(category, 100, stdin);
    category[strcspn(category, "\n")] = '\0';

    printf("Enter subcategory: ");
    fgets(subcategory, 100, stdin);
    subcategory[strcspn(subcategory, "\n")] = '\0';

    int total = countFlashcards();
    if (total == 0) 
    {
        printf("No flashcards available.\n");
        return;
    }

    struct card *temp = head;
    struct card **arr = malloc(total * sizeof(struct card *));
    int count = 0;

    while (temp)  //check if cate/subcate match then select the flashcard
    {
    if (strcmp(temp->category, category) == 0 && strcmp(temp->subcategory, subcategory) == 0) 
    {
        arr[count++] = temp;
    }
    temp = temp->next;
}


    if (count == 0) // no flascard match with cate/subcate
    {
        printf("No flashcards found for this category and subcategory.\n");
        free(arr);
        return;
    }
    
    int sortchoice;  //user can select the practice mode that sort by score
    printf("\nSort by score?\n");
    printf("1. Lowest to Highest\n");
    printf("2. Highest to Lowest\n");
    printf("3. None\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &sortchoice);
    getchar();


//bubble sort > sort the score that user input in flashcard
//asc
    if (sortchoice == 1) 
    {
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - i - 1; j++)
                if (arr[j]->score > arr[j + 1]->score) 
                {
                    struct card *t = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = t;
                }
        }
            
    } 
//desc
    else if (sortchoice == 2) 
    {
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - i - 1; j++)  
                if (arr[j]->score < arr[j + 1]->score) 
                {
                    struct card *t = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = t;
                }
        }
            
    }

    int totalAnswered = 0;
    int correctAnswers = 0;
    char choice[10];

    for (int i = 0; i < count; i++) 
    {
        printf("\nQ: %s\nYour answer: ", arr[i]->question);
        char userAns[Max];
        fgets(userAns, Max, stdin);
        userAns[strcspn(userAns, "\n")] = '\0';

        if (strlen(userAns) == 0) //if user press enter without answer. program will ask question again
        {
            printf("You did not enter an answer. Skipping this question.\n");
            i--;
            continue;
        }
        
        totalAnswered++; //count total correct answer

        if (strcmp(userAns, arr[i]->answer) == 0) 
        {
            printf("Correct!\n");
            correctAnswers++;
        } 
        else 
        {
            printf("Incorrect. Correct answer: %s\n", arr[i]->answer);
        }

        while (1) 
        {
            printf("Do you want to continue? (y/n): "); //if user want to continue practice can press y if not press n
            fgets(choice, sizeof(choice), stdin);
            choice[strcspn(choice, "\n")] = '\0';
            
            if (strcmp(choice, "y") == 0 || strcmp(choice, "Y") == 0) 
            {
                break;  
            } 
            else if (strcmp(choice, "n") == 0 || strcmp(choice, "N") == 0) 
            {
                i = count;  
                break;
            } 
            else 
            {
                printf("Invalid input. Please enter 'y' or 'n'.\n");
            }
        }

    }
    //show the summary score 
    printf("\nPractice Summary: %d correct out of %d questions.\n", correctAnswers, totalAnswered);
    printf("Total Score: %d\n", totalScore());
    free(arr);
}

//load flashcard information in csv file back to the program
void loadFlashcardsFromCSV(const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        printf("No existing data found. Creating new data now.\n");
        return;
    }

    char line[Max];
    fgets(line, sizeof(line), file); //skip head row

    while (fgets(line, sizeof(line), file)) 
    {
        int id, score;
        char question[Max], answer[Max], category[100], subcategory[100];

        
        sscanf(line, "%d,%[^,],%[^,],%d,%[^,],%[^\n]", &id, question, answer, &score, category, subcategory); //file column

        struct card *newCard = createCard(id, question, answer, category, subcategory, score); //create new flashcard

        
        if (head == NULL)
        {
            head = newCard;
        }
            
        else 
        {
            struct card *temp = head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
                
            temp->next = newCard;
        }

        //add flashcard to hash table > can find flashcard faster
        int index = hashFunction(id);
        hashTable[index].key = id;
        hashTable[index].card = newCard;

        
        if (id >= idcount)
        {
            idcount = id + 1;
        }
            
    }

    fclose(file);
    printf("Flashcards loaded from %s\n", filename);
}

//check the username alreadty exist or not
int checkUser(const char *username)
{
    FILE *file = fopen("users.csv", "r");
    if (!file)
    {
        return 0;
    }
    char line[200], fileuser[100], filepass[100];

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,],%[^\n]", fileuser, filepass);

        if (strcmp(fileuser, username) == 0)
        {
            fclose(file);
            return 1;
        }
        
    }

    fclose(file);
    return 0;

}

//sign up function > user need to sign up before login
void SignupUser()
{
    char username[100], password[100];
    FILE *file = fopen("users.csv", "a");  //append mode

    if (!file)
    {
        printf("Cannot access file for user registration.\n");
        return;
    }

    printf("Enter new username: "); //enter new username
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter new password: "); //enter new password
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (strlen(username) == 0 || strlen(password) == 0)  //cant press enter without username and password
    {
        printf("Username or password cannot be empty. Please try again.\n");
        fclose(file);
        return;
    }

    if (strchr(username, ',')  || strchr(password, ',')) //cant have comma in user name > program use comma for spilt data in csv file
    {
        printf("Username or password cannot contain a comma. Please try again.\n");
        return;
    }
    

    if (checkUser(username)) //check username is already exist or not
    {
        printf("Username already exists. Please choose another one.\n");
        return;
    }
    
    

    fprintf(file, "%s,%s\n", username, password);
    fclose(file);
    printf("Registration successful! You can log in to your account.\n");

}

//login function > user need to enter username , password and then can go to the flashcard menu
int loginUser()
{
    char username[100], password[100], line[100];
    char fileuser[100], filepass[100];
    int found = 0;

    FILE *file = fopen("users.csv", "r"); //read mode
    if (!file)
    {
        printf("Cannot open file for login.\n");
        return 0;
    }

    printf("Enter username: "); //enter user name
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter password: "); //enter password
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,],%[^\n]", fileuser, filepass);

        if(strcmp(username, fileuser) == 0)
        {
            found = 1;

            if (strcmp(password, filepass) == 0)
            {
                strcpy(currentuser.username, username);
                fclose(file);
                return 1;
            }
            else
            {
                printf("Incorrect password.\n");
                fclose(file);
                return 0;
            }
            
        }

    }

    fclose(file); //check username , password in cav file

    if (!found)
    {
        printf("Username not found. Please sign up first.\n");
    }
    
    return 0; 

}
    

//show log in menu
void showLoginmenu()
{
    int choice;
    char input[100];

    while (1) //user select the menu first > sign up, log in, exit
    {
        printf("\n--- Welcome to Flashcard Learning System ---\n");
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        fgets(input, sizeof(input), stdin);
        

        choice = -1;

        if (sscanf(input, "%d", &choice) != 1) //user can enter only 1-3, can't enter alphabet
        {
            printf("Invalid choice. Please enter 1-3.\n");
            continue; // back to the menu
        }

        switch (choice)
        {
        case 1:
            SignupUser(); // sign up 
            break;
        
        case 2:
            if (loginUser()) //if user log in successful > go to the flashcard menu
            {
                printf("Login Successful! Welcome <%s>.\n" ,currentuser.username);
                return; 
            }
            else
            {
                printf("Login failed. Please try again.\n");
            }
            break;
            

        case 3:
            printf("Goodbye!\n"); //exit program
            exit(0);
        
        default:
            printf("Invalid choice. Please enter 1-3.\n");
        }
    }
    
}

// flashcard menu
void showMenu() 
{
    printf("\n1. Add Flashcard\n");
    printf("2. View Flashcards\n");
    printf("3. Edit Flashcard\n");
    printf("4. Delete Flashcard\n");
    printf("5. Practice Flashcards\n");
    printf("6. Logout\n");
    printf("Enter your choice (1-6): ");
}

// this menu will show after user log in
void mainMenu() 
{
    int choice;
    char input[100];
    char filename[200]; //store current user flashcard name

    sprintf(filename, "%s_flashcards.csv", currentuser.username); 
    loadFlashcardsFromCSV(filename);

        while (1) 
        {
        showMenu(); //show main flashcard menu options
        fgets(input, sizeof(input), stdin);  

        if (sscanf(input, "%d", &choice) != 1) //check if input is not a number
        {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) 
        {
            case 1: addFlashcard(); break; //add a new flashcard 
            case 2: viewFlashcards(); break; //view all flashcard
            case 3: editFlashcard(); break; //edit a flashcard by ID
            case 4: deleteFlashcard(); break; //delete a flashcard by ID
            case 5: practiceFlashcards(); break; //practice flashcards
            case 6: return; //logout > return to login menu
            default: printf("Invalid choice.\n");
        }
    }
    
}

int main()
{
    while (1)
    {
        showLoginmenu(); //show log in menu
        mainMenu(); 
    }

    return 0;
    
}
