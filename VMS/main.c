/*
 * Voting Management System in C
 * 
 * This program simulates a real-life voting management system with two roles: Admin and Voter.
 * 
 * Overall Program Logic:
 * - Data is stored permanently using file handling (voters.txt, candidates.txt, votes.txt).
 * - Admin logs in with a password, manages voters and candidates, and views results.
 * - Voters are registered by Admin with auto-generated unique IDs, can vote once anonymously.
 * - Voting prevents duplicates via votes.txt; results show only candidate vote counts.
 * - Unique IDs: Voter IDs like VOTER001, Candidate IDs sequential.
 * - Deletes use temp files for safe file operations.
 * - Menus guide navigation with input validation.
 * - Suitable for college projects: modular, commented, console-based.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define constants
#define MAX_VOTERS 100
#define MAX_CANDIDATES 100
#define ADMIN_PASSWORD "admin123"
#define VOTERS_FILE "voters.txt"
#define CANDIDATES_FILE "candidates.txt"
#define VOTES_FILE "votes.txt"
#define TEMP_FILE "temp.txt"

// Structure for Voter
struct Voter {
    char id[20];
    char name[50];
    int age;
};

// Structure for Candidate
struct Candidate {
    int id;
    char name[50];
    char party[50];
    char symbol[50];
    int votes;
};

// Global variables
struct Voter voters[MAX_VOTERS];
int num_voters = 0;
struct Candidate candidates[MAX_CANDIDATES];
int num_candidates = 0;
int voter_counter = 1; // For generating unique voter IDs

// Function prototypes
void load_voters();
void save_voters();
void load_candidates();
void save_candidates();
void save_votes(char id[]);
void main_menu();
void admin_login();
void admin_menu();
void add_voter();
void delete_voter();
void view_voters();
void add_candidate();
void delete_candidate();
void view_candidates();
void vote();
void show_results();
void generate_voter_id(char id[]);
int is_registered(char id[]);
int is_voted(char id[]);
void add_voted(char id[]);

int main() {
    load_voters();
    load_candidates();
    main_menu();
    return 0;
}

// Load voters from file
void load_voters() {
    FILE *file = fopen(VOTERS_FILE, "r");
    if (file == NULL) return;
    while (fscanf(file, "%s %s %d", voters[num_voters].id, voters[num_voters].name, &voters[num_voters].age) == 3) {
        num_voters++;
        if (num_voters >= MAX_VOTERS) break;
    }
    fclose(file);
    // Update voter_counter based on loaded IDs
    if (num_voters > 0) {
        sscanf(voters[num_voters - 1].id + 5, "%d", &voter_counter);
        voter_counter++;
    }
}

// Save voters to file (not used directly, as deletes use temp)
void save_voters() {
    FILE *file = fopen(VOTERS_FILE, "w");
    for (int i = 0; i < num_voters; i++) {
        fprintf(file, "%s %s %d\n", voters[i].id, voters[i].name, voters[i].age);
    }
    fclose(file);
}

// Load candidates from file
void load_candidates() {
    FILE *file = fopen(CANDIDATES_FILE, "r");
    if (file == NULL) return;
    while (fscanf(file, "%d %s %s %s %d", &candidates[num_candidates].id, candidates[num_candidates].name,
                  candidates[num_candidates].party, candidates[num_candidates].symbol, &candidates[num_candidates].votes) == 5) {
        num_candidates++;
        if (num_candidates >= MAX_CANDIDATES) break;
    }
    fclose(file);
}

// Save candidates to file (not used directly, as deletes use temp)
void save_candidates() {
    FILE *file = fopen(CANDIDATES_FILE, "w");
    for (int i = 0; i < num_candidates; i++) {
        fprintf(file, "%d %s %s %s %d\n", candidates[i].id, candidates[i].name, candidates[i].party, candidates[i].symbol, candidates[i].votes);
    }
    fclose(file);
}

// Save a voted voter ID to file
void save_votes(char id[]) {
    FILE *file = fopen(VOTES_FILE, "a");
    fprintf(file, "%s\n", id);
    fclose(file);
}

// Main menu
void main_menu() {
    int choice;
    while (1) {
        printf("\n=== Voting Management System ===\n");
        printf("1. Admin Login\n");
        printf("2. Vote\n");
        printf("3. Show Results\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                admin_login();
                break;
            case 2:
                vote();
                break;
            case 3:
                show_results();
                break;
            case 4:
                printf("Exiting the system. Thank you!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Admin login
void admin_login() {
    char password[20];
    printf("Enter admin password: ");
    scanf("%s", password);
    
    if (strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("Login successful!\n");
        admin_menu();
    } else {
        printf("Incorrect password. Access denied.\n");
    }
}

// Admin menu
void admin_menu() {
    int choice;
    while (1) {
        printf("\n=== Admin Menu ===\n");
        printf("1. Add Voter\n");
        printf("2. Delete Voter\n");
        printf("3. View Voters\n");
        printf("4. Add Candidate\n");
        printf("5. Delete Candidate\n");
        printf("6. View Candidates\n");
        printf("7. View Results\n");
        printf("8. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                add_voter();
                break;
            case 2:
                delete_voter();
                break;
            case 3:
                view_voters();
                break;
            case 4:
                add_candidate();
                break;
            case 5:
                delete_candidate();
                break;
            case 6:
                view_candidates();
                break;
            case 7:
                show_results();
                break;
            case 8:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Add voter (Admin only)
void add_voter() {
    if (num_voters >= MAX_VOTERS) {
        printf("Maximum voters reached. Cannot add more.\n");
        return;
    }
    
    char name[50];
    int age;
    printf("Enter voter name: ");
    scanf("%s", name);
    printf("Enter voter age: ");
    scanf("%d", &age);
    
    if (age < 18) {
        printf("Voter must be 18 or older. Registration failed.\n");
        return;
    }
    
    generate_voter_id(voters[num_voters].id);
    strcpy(voters[num_voters].name, name);
    voters[num_voters].age = age;
    num_voters++;
    save_voters();
    printf("Voter registered successfully! Voter ID: %s\n", voters[num_voters - 1].id);
}

// Delete voter (Admin only, using temp file)
void delete_voter() {
    if (num_voters == 0) {
        printf("No voters to delete.\n");
        return;
    }
    
    view_voters();
    
    char id[20];
    printf("Enter the Voter ID to delete: ");
    scanf("%s", id);
    
    FILE *file = fopen(VOTERS_FILE, "r");
    FILE *temp = fopen(TEMP_FILE, "w");
    if (file == NULL || temp == NULL) {
        printf("Error opening files.\n");
        return;
    }
    
    char file_id[20], name[50];
    int age, found = 0;
    while (fscanf(file, "%s %s %d", file_id, name, &age) == 3) {
        if (strcmp(file_id, id) != 0) {
            fprintf(temp, "%s %s %d\n", file_id, name, age);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(temp);
    
    if (found) {
        remove(VOTERS_FILE);
        rename(TEMP_FILE, VOTERS_FILE);
        load_voters(); // Reload voters
        printf("Voter deleted successfully!\n");
    } else {
        printf("Voter ID not found.\n");
        remove(TEMP_FILE);
    }
}

// View voters (Admin only)
void view_voters() {
    printf("\n=== Registered Voters ===\n");
    if (num_voters == 0) {
        printf("No voters registered.\n");
        return;
    }
    
    printf("%-15s %-20s %-5s\n", "Voter ID", "Name", "Age");
    printf("-------------------------------------\n");
    for (int i = 0; i < num_voters; i++) {
        printf("%-15s %-20s %-5d\n", voters[i].id, voters[i].name, voters[i].age);
    }
}

// Add candidate (Admin only)
void add_candidate() {
    if (num_candidates >= MAX_CANDIDATES) {
        printf("Maximum candidates reached. Cannot add more.\n");
        return;
    }
    
    candidates[num_candidates].id = num_candidates + 1;
    printf("Enter candidate name: ");
    scanf("%s", candidates[num_candidates].name);
    printf("Enter candidate party: ");
    scanf("%s", candidates[num_candidates].party);
    printf("Enter candidate symbol: ");
    scanf("%s", candidates[num_candidates].symbol);
    candidates[num_candidates].votes = 0;
    num_candidates++;
    save_candidates();
    printf("Candidate added successfully! Candidate ID: %d\n", candidates[num_candidates - 1].id);
}

// Delete candidate (Admin only, using temp file)
void delete_candidate() {
    if (num_candidates == 0) {
        printf("No candidates to delete.\n");
        return;
    }
    
    view_candidates();
    
    int id;
    printf("Enter the Candidate ID to delete: ");
    scanf("%d", &id);
    
    FILE *file = fopen(CANDIDATES_FILE, "r");
    FILE *temp = fopen(TEMP_FILE, "w");
    if (file == NULL || temp == NULL) {
        printf("Error opening files.\n");
        return;
    }
    
    int file_id, votes, found = 0;
    char name[50], party[50], symbol[50];
    while (fscanf(file, "%d %s %s %s %d", &file_id, name, party, symbol, &votes) == 5) {
        if (file_id != id) {
            fprintf(temp, "%d %s %s %s %d\n", file_id, name, party, symbol, votes);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(temp);
    
    if (found) {
        remove(CANDIDATES_FILE);
        rename(TEMP_FILE, CANDIDATES_FILE);
        load_candidates(); // Reload candidates
        printf("Candidate deleted successfully!\n");
    } else {
        printf("Candidate ID not found.\n");
        remove(TEMP_FILE);
    }
}

// View candidates (Admin only)
void view_candidates() {
    printf("\n=== Candidate List ===\n");
    if (num_candidates == 0) {
        printf("No candidates.\n");
        return;
    }
    
    printf("%-5s %-20s %-20s %-20s %-10s\n", "ID", "Name", "Party", "Symbol", "Votes");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < num_candidates; i++) {
        printf("%-5d %-20s %-20s %-20s %-10d\n", candidates[i].id, candidates[i].name,
               candidates[i].party, candidates[i].symbol, candidates[i].votes);
    }
}

// Vote (Voter)
void vote() {
    char voter_id[20];
    printf("Enter your Voter ID: ");
    scanf("%s", voter_id);
    
    if (!is_registered(voter_id)) {
        printf("Voter ID not registered. Please contact Admin to register.\n");
        return;
    }
    
    if (is_voted(voter_id)) {
        printf("You have already voted. Duplicate voting is not allowed.\n");
        return;
    }
    
    if (num_candidates == 0) {
        printf("No candidates available to vote for.\n");
        return;
    }
    
    view_candidates();
    
    int choice;
    printf("Enter the Candidate ID you want to vote for: ");
    scanf("%d", &choice);
    
    int found = 0;
    for (int i = 0; i < num_candidates; i++) {
        if (candidates[i].id == choice) {
            candidates[i].votes++;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Invalid Candidate ID.\n");
        return;
    }
    
    add_voted(voter_id);
    printf("Vote cast successfully!\n");
}

// Show results (Accessible to all)
void show_results() {
    printf("\n=== Election Results ===\n");
    if (num_candidates == 0) {
        printf("No candidates.\n");
        return;
    }
    
    printf("%-5s %-20s %-20s %-20s %-10s\n", "ID", "Name", "Party", "Symbol", "Votes");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < num_candidates; i++) {
        printf("%-5d %-20s %-20s %-20s %-10d\n", candidates[i].id, candidates[i].name,
               candidates[i].party, candidates[i].symbol, candidates[i].votes);
    }
}

// Generate unique voter ID
void generate_voter_id(char id[]) {
    sprintf(id, "VOTER%03d", voter_counter++);
}

// Check if voter ID is registered
int is_registered(char id[]) {
    for (int i = 0; i < num_voters; i++) {
        if (strcmp(voters[i].id, id) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if voter has already voted
int is_voted(char id[]) {
    FILE *file = fopen(VOTES_FILE, "r");
    if (file == NULL) return 0;
    
    char line[20];
    while (fscanf(file, "%s", line) == 1) {
        if (strcmp(line, id) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Add voted voter ID
void add_voted(char id[]) {
    save_votes(id);
}