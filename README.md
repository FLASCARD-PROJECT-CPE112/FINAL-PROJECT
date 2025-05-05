# A Digital Flashcard Learning System

## Table of Contents
- [Project Description](#project-description)
- [How to Run the Code](#how-to-run-the-code)
- [Sample Input and Expected Output](#sample-input-and-expected-output)
- [Features](#features)
- [Technologies Used](#technologies-used)
- [File Structure](#file-structure)


## Project Description

This is a **console-based flashcard learning system written in C**.
- Sign up and log in using unique usernames
- Add, View, Edit, and Delete flashcards
- Organize flashcards into categories and subcategories
- Practice flashcards with feedback and sorting by score (1-10)
- Save and load flashcards from CSV file specific to each user
- Search flashcards by ID using a hash table



## How to Run the Code

### Prerequisites
- GCC or any standard C compiler
- A terminal or command prompt

### Compile the program
```bash
gcc flashcard.c -o flashcard
```




## Sample Input and Expected Output
### 1. Sign Up

--- Welcome to Flashcard Learning System ---
1. Sign Up
2. Log in
3. Exit
   
Enter your choice: 1\
Enter new username: anton\
Enter new password: 1234\
Registration successful! You can log in to your account.

### 2. Login
Enter your choice: 2\
Enter username: anton\
Enter password: 1234\
Login successful! Welcome <anton>.



### 3. Add Flashcard
1. Add Flashcard
2. View Flashcards
3. Edit Flashcard
4. Delete Flashcard
5. Practice Flashcards
6. Logout
   
Enter your choice: 1\
Enter question: 2 + 9\
Enter answer: 11\
Enter score (1-10): 2\
Enter category: Math\
Enter subcategory: M1

### 4. View Flashcards
Enter your choice: 2

--- Flashcards ---\
ID: 1\
Category: Math\
Subcategory: M1\
Question: 2 + 9\
Answer: 11\
Score: 2

### 6. Edit Flashcards
Enter your choice: 3

--- Available Flashcards ---\
ID: 1 | Category: Math | Subcategory: M1 | Question: 2 + 9

Enter the ID of the flashcard to edit: 1

Flashcard found:\
ID: 1\
Category: Math\
Subcategory: M1\
Question: 2 + 9\
Answer: 11\
Score: 2

You can edit any field, or just press Enter to keep it the same.

New Question (or press Enter to keep): 5 + 2\
New Answer (or press Enter to keep): 7\
New Score (or press Enter to keep):\
Flashcard updated successfully.

### 7. Delete Flashcard
Enter your choice: 4

--- Available Flashcards ---\
ID: 1 | Category: Math | Subcategory: M1 | Question: 5 + 2

Enter the ID of the flashcard to delete: 1

Flashcard found:\
ID: 1\
Category: Math\
Subcategory: M1\
Question: 5 + 2\
Answer: 11\
Score: 2

Are you sure you want to delete this flashcard? (y/n): y\
Flashcard deleted successfully.


### 8. Practice Flashcards
Enter your choice: 5

Sort by score?
1. Lowest to Highest
2. Highest to Lowest
3. None

Enter your choice (1-3): 2

Q: 2 + 9\
Your answer: 9\
Correct!\
Do you want to continue? (y/n): n

Practice Summary: 1 correct out of 1 questions.\
Total Score: 2

### 9. Logout
Enter your choice: 6

--- Welcome to Flashcard Learning System ---
1. Sign Up
2. Log in
3. Exit

---


## Features

- Console-based interface
- User registration and login with CSV
- Add / View / Edit / Delete flashcards
- Categorized flashcard system
- Practice mode with sorting by score
- Score tracking and feedback
- Linked list + hash table implementation
- Save/load data to CSV file per user


## Technologies Used

- C Programming Language
- Linked List (for flashcard storage)
- Hash Table (for searching by ID)
- File I/O using CSV
- Bubble Sort (for sorting flashcards)


## File Structure

- `flashcard.c` – Main program file
- `users.csv` – Stores usernames and passwords
- `<username>_flashcards.csv` – Flashcards for each user










