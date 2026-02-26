#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


struct Account {
    char accNo[20];
    char name[50];
    int pin;
    float balance;
    int locked;
    int frozen;
    char createdOn[40];
};

/* FUNCTION DECLARATIONS */
void createAccount();
void login();
void deposit(char accNo[]);
void withdraw(char accNo[]);
void miniStatement(char accNo[]);
void changePIN(char accNo[]);
void interestCalculator();
void fundTransfer(char senderAcc[]);
void exportToExcel();

int inputPIN() {
    char pinStr[10];
    int valid = 0;

    while (!valid) {
        printf("Enter 4-digit PIN: ");
        scanf("%s", pinStr);

        valid = 1;

        for (int i = 0; i < 4; i++) {
            if (pinStr[i] < '0' || pinStr[i] > '9') {
                valid = 0;
                break;
            }
        }

        if (strlen(pinStr) != 4) valid = 0;

        if (!valid)
            printf("Invalid PIN! Only 4 digits allowed.\n");
    }

    return atoi(pinStr);
}


/* ADMIN */
void adminLogin();
void viewAllAccounts();
void searchAccount();
void deleteAccount();
void updateAccount();
void freezeAccount();
void unlockAccount();
void showStatistics();
void backupDatabase();
void exportToExcel();

/* MAIN */
int main() {
    int choice;

    while (1) {
        printf("\n===== BANK MANAGEMENT SYSTEM =====\n");
        printf("1. Customer Login\n");
        printf("2. Create Customer Account\n");
        printf("3. Admin Login\n");
        printf("4. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: login(); break;
            case 2: createAccount(); break;
            case 3: adminLogin(); break;
            case 4: return 0;
            default: printf("\nInvalid choice!\n");
        }
    }
}

/* ---------------- CREATE ACCOUNT ---------------- */
void createAccount() {
    struct Account acc;

    printf("\n--- CREATE ACCOUNT ---\n");
    printf("Enter Name: ");
    scanf(" %[^\n]", acc.name);

    acc.pin = inputPIN();

    printf("Enter Opening Balance: ");
    scanf("%f", &acc.balance);

    acc.locked = 0;
    acc.frozen = 0;

    /* CREATED ON DATE */
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(acc.createdOn, "%02d-%02d-%04d_%02d:%02d:%02d",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
            t->tm_hour, t->tm_min, t->tm_sec);

    /* ACCOUNT NUMBER */
    int id;
    FILE *fp_id = fopen("id.txt", "r");
    if (fp_id == NULL) id = 1;
    else {
        fscanf(fp_id, "%d", &id);
        fclose(fp_id);
    }

    sprintf(acc.accNo, "11012025%06d01", id);

    fp_id = fopen("id.txt", "w");
    fprintf(fp_id, "%d", id + 1);
    fclose(fp_id);

    FILE *fp = fopen("accounts.txt", "a");
    fprintf(fp, "%s|%s|%d|%.2f|%d|%d|%s\n",
            acc.accNo, acc.name, acc.pin,
            acc.balance, acc.locked, acc.frozen, acc.createdOn);

    fclose(fp);

    printf("\nAccount Created Successfully!\n");
    printf("Your 16-digit Account Number: %s\n", acc.accNo);
}

/* ---------------- LOGIN ---------------- */
void login() {
    char accNo[20];
    int pin;
    struct Account acc;
    int found = 0;

    printf("\n--- LOGIN ---\n");
    printf("Enter Account Number: ");
    scanf("%s", accNo);

    printf("Enter PIN: ");
    scanf("%d", &pin);

    FILE *fp = fopen("accounts.txt", "r");

    if (fp == NULL) {
        printf("\nNo accounts found!\n");
        return;
    }

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin, &acc.balance,
                  &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        if (strcmp(acc.accNo, accNo) == 0) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) { printf("\nAccount Not Found!\n"); return; }
    if (acc.locked == 1) { printf("\nAccount is LOCKED!\n"); return; }
    if (acc.frozen == 1) { printf("\nAccount is FROZEN!\n"); return; }
    if (acc.pin != pin) { printf("\nIncorrect PIN!\n"); return; }

    printf("\nLogin Successful!\nWelcome, %s\n", acc.name);

    int cchoice;

    while (1) {
        printf("\n----- CUSTOMER MENU -----\n");
        printf("1. Deposit Money\n");
        printf("2. Withdraw Money\n");
        printf("3. Check Balance\n");
        printf("4. Mini Statement\n");
        printf("5. Logout\n");
        printf("6. Change PIN\n");
        printf("7. Calculate Interest\n");
        printf("8. Fund Transfer\n");

        printf("Enter your choice: ");
        scanf("%d", &cchoice);

        if (cchoice == 1) deposit(acc.accNo);
        else if (cchoice == 2) withdraw(acc.accNo);
        else if (cchoice == 3) {
            FILE *fp2 = fopen("accounts.txt", "r");
            struct Account acc2;
            while (fscanf(fp2, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                          acc2.accNo, acc2.name, &acc2.pin, &acc2.balance,
                          &acc2.locked, &acc2.frozen, acc2.createdOn) != EOF) {
                if (strcmp(acc2.accNo, acc.accNo) == 0) {
                    printf("\nYour Balance: %.2f\n", acc2.balance);
                    break;
                }
            }
            fclose(fp2);
        }
        else if (cchoice == 4) miniStatement(acc.accNo);
        else if (cchoice == 5) { printf("\nLogged out.\n"); break; }
        else if (cchoice == 6) changePIN(acc.accNo);
        else if (cchoice == 7) interestCalculator();
        else if (cchoice == 8) fundTransfer(acc.accNo);
        else printf("Invalid option!\n");
    }
}

/* ---------------- DEPOSIT ---------------- */
void deposit(char accNo[]) {
    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Account acc;
    float amount;

    printf("\nEnter amount to deposit: ");
    scanf("%f", &amount);

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        if (strcmp(acc.accNo, accNo) == 0) {
            acc.balance += amount;

            /* Time Stamp */
            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            char date[20], timeX[20];
            sprintf(date, "%02d-%02d-%04d", t->tm_mday, t->tm_mon+1, t->tm_year+1900);
            sprintf(timeX, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);

            FILE *fpt = fopen("transactions.txt", "a");
            fprintf(fpt, "%s Deposit %.2f %s %s\n",
                    accNo, amount, date, timeX);
            fclose(fpt);

            printf("\nDeposit Successful!\nNew Balance: %.2f\n", acc.balance);
        }

        fprintf(temp, "%s|%s|%d|%.2f|%d|%d|%s\n",
                acc.accNo, acc.name, acc.pin,
                acc.balance, acc.locked, acc.frozen, acc.createdOn);
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}

/* ---------------- WITHDRAW ---------------- */
void withdraw(char accNo[]) {
    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Account acc;
    float amount;

    printf("\nEnter amount to withdraw: ");
    scanf("%f", &amount);

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        if (strcmp(acc.accNo, accNo) == 0) {
            if (acc.balance >= amount) {
                acc.balance -= amount;

                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                char date[20], timeX[20];
                sprintf(date, "%02d-%02d-%04d", t->tm_mday, t->tm_mon+1, t->tm_year+1900);
                sprintf(timeX, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);

                FILE *fpt = fopen("transactions.txt", "a");
                fprintf(fpt, "%s Withdraw %.2f %s %s\n",
                        accNo, amount, date, timeX);
                fclose(fpt);

                printf("\nWithdrawal Successful!\nRemaining Balance: %.2f\n", acc.balance);
            } else {
                printf("\nInsufficient Balance!\n");
            }
        }

        fprintf(temp, "%s|%s|%d|%.2f|%d|%d|%s\n",
                acc.accNo, acc.name, acc.pin,
                acc.balance, acc.locked, acc.frozen, acc.createdOn);
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}

/* ---------------- MINI STATEMENT ---------------- */
void miniStatement(char accNo[]) {
    FILE *fp = fopen("transactions.txt", "r");
    char account[20], type[20], date[20], timeX[20];
    float amount;

    if (fp == NULL) {
        printf("\nNo transactions found!\n");
        return;
    }

    printf("\n----- MINI STATEMENT -----\n");

    while (fscanf(fp, "%s %s %f %s %s",
                  account, type, &amount, date, timeX) != EOF) {

        if (strcmp(account, accNo) == 0) {
            printf("%-12s ₹%.2f %s %s\n", type, amount, date, timeX);
        }
    }

    fclose(fp);
}

/* ---------------- CHANGE PIN ---------------- */
void changePIN(char accNo[]) {
    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Account acc;
    int oldPin, newPin, confirmPin;

    printf("\n--- CHANGE PIN ---\n");
    printf("Enter Old PIN: ");
    scanf("%d", &oldPin);

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        if (strcmp(acc.accNo, accNo) == 0) {

            if (acc.pin != oldPin) {
                printf("\nIncorrect Old PIN!\n");
                fclose(fp);
                fclose(temp);
                remove("temp.txt");
                return;
            }

            newPin = inputPIN();

            confirmPin = inputPIN();

            if (newPin != confirmPin) {
                printf("\nPINs do not match!\n");
                fclose(fp);
                fclose(temp);
                remove("temp.txt");
                return;
            }

            acc.pin = newPin;
            printf("\nPIN Updated Successfully!\n");
        }

        fprintf(temp, "%s|%s|%d|%.2f|%d|%d|%s\n",
                acc.accNo, acc.name, acc.pin,
                acc.balance, acc.locked, acc.frozen, acc.createdOn);
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}

/* ---------------- INTEREST CALCULATOR ---------------- */
void interestCalculator() {
    float amount, rate, time, interest;

    printf("\n--- INTEREST CALCULATOR ---\n");

    printf("Enter Amount: ");
    scanf("%f", &amount);

    printf("Enter Rate (in %%): ");
    scanf("%f", &rate);

    printf("Enter Time (in years): ");
    scanf("%f", &time);

    interest = (amount * rate * time) / 100;

    printf("\nInterest for %.2f at %.2f%% for %.2f years = %.2f\n",
           amount, rate, time, interest);
}

/* ---------------- FUND TRANSFER ---------------- */
void fundTransfer(char senderAcc[]) {
    char receiverAcc[20];
    float amount;

    struct Account acc, sender, receiver;
    int receiverFound = 0, senderFound = 0;

    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("\n--- FUND TRANSFER ---\n");
    printf("Enter Receiver Account Number: ");
    scanf("%s", receiverAcc);

    /* BLOCK SAME ACCOUNT TRANSFER */
    if (strcmp(senderAcc, receiverAcc) == 0) {
        printf("\n❌ You cannot transfer funds to the same account!\n");
        fclose(fp);
        fclose(temp);
        remove("temp.txt");
        return;
    }

    printf("Enter Amount to Transfer: ");
    scanf("%f", &amount);

    /* Find sender & receiver */
    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        if (strcmp(acc.accNo, senderAcc) == 0) {
            sender = acc;
            senderFound = 1;
        }

        if (strcmp(acc.accNo, receiverAcc) == 0) {
            receiver = acc;
            receiverFound = 1;
        }
    }
    fclose(fp);

    if (!senderFound) {
        printf("\n❌ Sender account missing (unexpected error)\n");
        remove("temp.txt");
        return;
    }

    if (!receiverFound) {
        printf("\n❌ Receiver account does not exist!\n");
        remove("temp.txt");
        return;
    }

    if (sender.balance < amount) {
        printf("\n❌ Insufficient balance!\n");
        remove("temp.txt");
        return;
    }

    /* Update balances */
    sender.balance -= amount;
    receiver.balance += amount;

    fp = fopen("accounts.txt", "r");

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        if (strcmp(acc.accNo, sender.accNo) == 0)
            acc = sender;

        else if (strcmp(acc.accNo, receiver.accNo) == 0)
            acc = receiver;

        fprintf(temp, "%s|%s|%d|%.2f|%d|%d|%s\n",
                acc.accNo, acc.name, acc.pin,
                acc.balance, acc.locked, acc.frozen, acc.createdOn);
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");

    /* Save Transaction Log */
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char dateX[20], timeX[20];
    sprintf(dateX, "%02d-%02d-%04d", t->tm_mday, t->tm_mon+1, t->tm_year+1900);
    sprintf(timeX, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);

    FILE *tlog = fopen("transactions.txt", "a");
    fprintf(tlog, "%s TransferOut %.2f %s %s\n", senderAcc, amount, dateX, timeX);
    fprintf(tlog, "%s TransferIn %.2f %s %s\n", receiverAcc, amount, dateX, timeX);
    fclose(tlog);

    printf("\n✅ Transfer Successful!\n₹%.2f sent to %s\n", amount, receiverAcc);
}


/* ---------------- ADMIN LOGIN ---------------- */
void adminLogin() {
    char user[20], pass[20];

    printf("\n--- ADMIN LOGIN ---\n");
    printf("Enter Username: ");
    scanf("%s", user);
    printf("Enter Password: ");
    scanf("%s", pass);

    if (strcmp(user, "admin") == 0 && strcmp(pass, "8425") == 0) {
        printf("\nAdmin Login Successful!\n");

        int choice;

        while (1) {
            printf("\n----- ADMIN MENU -----\n");
            printf("1. View All Accounts\n");
            printf("2. Search Account\n");
            printf("3. Delete Account\n");
            printf("4. Update Account\n");
            printf("5. Freeze / Unfreeze Account\n");
            printf("6. Unlock Account\n");
            printf("7. Bank Statistics\n");
            printf("8. Backup Database\n");
            printf("9. Logout\n");
            printf("10. Export Excel File\n");

            printf("Enter choice: ");
            scanf("%d", &choice);

            if (choice == 1) viewAllAccounts();
            else if (choice == 2) searchAccount();
            else if (choice == 3) deleteAccount();
            else if (choice == 4) updateAccount();
            else if (choice == 5) freezeAccount();
            else if (choice == 6) unlockAccount();
            else if (choice == 7) showStatistics();
            else if (choice == 8) backupDatabase();
            else if (choice == 9) {
                printf("\nAdmin Logged Out.\n");
                break;
            }
            else if (choice == 10) exportToExcel();
            else printf("\nInvalid choice!\n");
        }
    } else {
        printf("\nAdmin Login Failed!\n");
    }
}

/* ---------------- VIEW ALL ACCOUNTS ---------------- */
void viewAllAccounts() {
    FILE *fp = fopen("accounts.txt", "r");
    struct Account acc;

    if (fp == NULL) {
        printf("\nNo accounts found!\n");
        return;
    }

    printf("\n--- ALL ACCOUNTS ---\n");

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        printf("%s | %-20s | ₹%.2f | Locked:%d | Frozen:%d | Created:%s\n",
               acc.accNo, acc.name, acc.balance, acc.locked, acc.frozen, acc.createdOn);
    }

    fclose(fp);
}

/* ---------------- SEARCH ACCOUNT ---------------- */
void searchAccount() {
    char accNo[20];
    struct Account acc;
    int found = 0;

    printf("\n--- SEARCH ACCOUNT ---\n");
    printf("Enter Account Number: ");
    scanf("%s", accNo);

    FILE *fp = fopen("accounts.txt", "r");

    if (fp == NULL) {
        printf("\nNo accounts found!\n");
        return;
    }

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        if (strcmp(acc.accNo, accNo) == 0) {
            found = 1;
            printf("\nACCOUNT FOUND!\n");
            printf("Account No : %s\n", acc.accNo);
            printf("Name       : %s\n", acc.name);
            printf("PIN        : %d\n", acc.pin);
            printf("Balance    : %.2f\n", acc.balance);
            printf("Locked     : %d\n", acc.locked);
            printf("Frozen     : %d\n", acc.frozen);
            printf("Created On : %s\n", acc.createdOn);
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("\nAccount NOT FOUND!\n");
}

/* ---------------- DELETE ACCOUNT ---------------- */
void deleteAccount() {
    char accNo[20];
    struct Account acc;
    int found = 0;

    printf("\n--- DELETE ACCOUNT ---\n");
    printf("Enter Account Number: ");
    scanf("%s", accNo);

    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        if (strcmp(acc.accNo, accNo) == 0) {
            found = 1;
            printf("\nAccount Deleted Successfully!\n");
            continue;
        }

        fprintf(temp, "%s|%s|%d|%.2f|%d|%d|%s\n",
                acc.accNo, acc.name, acc.pin,
                acc.balance, acc.locked, acc.frozen, acc.createdOn);
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");

    if (!found)
        printf("\nAccount NOT FOUND!\n");
}

/* ---------------- UPDATE ACCOUNT ---------------- */
void updateAccount() {
    char accNo[20];
    struct Account acc;
    int found = 0;
    int choice;

    printf("\n--- UPDATE ACCOUNT ---\n");
    printf("Enter Account Number: ");
    scanf("%s", accNo);

    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        if (strcmp(acc.accNo, accNo) == 0) {
            found = 1;

            printf("\n1. Update Name\n2. Update PIN\nEnter choice: ");
            scanf("%d", &choice);

            if (choice == 1) {
                printf("Enter New Name: ");
                scanf(" %[^\n]", acc.name);
                printf("\nName Updated Successfully!\n");
            } else if (choice == 2) {
                printf("Enter New PIN: ");
                scanf("%d", &acc.pin);
                printf("\nPIN Updated Successfully!\n");
            }
        }

        fprintf(temp, "%s|%s|%d|%.2f|%d|%d|%s\n",
                acc.accNo, acc.name, acc.pin,
                acc.balance, acc.locked, acc.frozen, acc.createdOn);
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");

    if (!found)
        printf("\nAccount NOT FOUND!\n");
}

/* ---------------- FREEZE / UNFREEZE ACCOUNT ---------------- */
void freezeAccount() {
    char accNo[20];
    struct Account acc;
    int found = 0;

    printf("\n--- FREEZE / UNFREEZE ---\n");
    printf("Enter Account Number: ");
    scanf("%s", accNo);

    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        if (strcmp(acc.accNo, accNo) == 0) {
            found = 1;

            acc.frozen = !acc.frozen;

            if (acc.frozen)
                printf("\nAccount Frozen!\n");
            else
                printf("\nAccount Unfrozen!\n");
        }

        fprintf(temp, "%s|%s|%d|%.2f|%d|%d|%s\n",
                acc.accNo, acc.name, acc.pin,
                acc.balance, acc.locked, acc.frozen, acc.createdOn);
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");

    if (!found)
        printf("\nAccount NOT FOUND!\n");
}

/* ---------------- UNLOCK ACCOUNT ---------------- */
void unlockAccount() {
    char accNo[20];
    struct Account acc;
    int found = 0;

    printf("\n--- UNLOCK ACCOUNT ---\n");
    printf("Enter Locked Account Number: ");
    scanf("%s", accNo);

    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        if (strcmp(acc.accNo, accNo) == 0) {
            found = 1;
            acc.locked = 0;
            printf("\nAccount Unlocked Successfully!\n");
        }

        fprintf(temp, "%s|%s|%d|%.2f|%d|%d|%s\n",
                acc.accNo, acc.name, acc.pin,
                acc.balance, acc.locked, acc.frozen, acc.createdOn);
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");

    if (!found)
        printf("\nAccount NOT FOUND!\n");
}

/* ---------------- STATISTICS ---------------- */
void showStatistics() {
    FILE *fp = fopen("accounts.txt", "r");
    struct Account acc;

    int totalAccounts = 0;
    float totalBalance = 0;
    float highest = -1, lowest = 99999999;
    char highName[50], lowName[50];

    if (fp == NULL) {
        printf("\nNo accounts found!\n");
        return;
    }

    while (fscanf(fp, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin,
                  &acc.balance, &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        totalAccounts++;
        totalBalance += acc.balance;

        if (acc.balance > highest) {
            highest = acc.balance;
            strcpy(highName, acc.name);
        }

        if (acc.balance < lowest) {
            lowest = acc.balance;
            strcpy(lowName, acc.name);
        }
    }

    fclose(fp);

    printf("\n--- BANK STATISTICS ---\n");
    printf("Total Accounts : %d\n", totalAccounts);
    printf("Total Bank Balance : %.2f\n", totalBalance);
    printf("Highest Balance : %.2f (%s)\n", highest, highName);
    printf("Lowest Balance : %.2f (%s)\n", lowest, lowName);
}

/* ---------------- BACKUP DATABASE ---------------- */
void backupDatabase() {
    FILE *src1 = fopen("accounts.txt", "r");
    FILE *dst1 = fopen("backup_accounts.txt", "w");

    FILE *src2 = fopen("transactions.txt", "r");
    FILE *dst2 = fopen("backup_transactions.txt", "w");

    char ch;

    if (src1 != NULL && dst1 != NULL)
        while ((ch = fgetc(src1)) != EOF) fputc(ch, dst1);

    if (src2 != NULL && dst2 != NULL)
        while ((ch = fgetc(src2)) != EOF) fputc(ch, dst2);

    fclose(src1);
    fclose(dst1);
    fclose(src2);
    fclose(dst2);

    printf("\nBackup Completed Successfully!\n");
}

/* ---------------- EXPORT CSV ---------------- */
void exportToExcel() {
    FILE *src = fopen("accounts.txt", "r");
    FILE *csv = fopen("accounts_export.csv", "w");

    fprintf(csv, "Account No,Name,PIN,Balance,Locked,Frozen,Created On\n");

    struct Account acc;

    while (fscanf(src, "%[^|]|%[^|]|%d|%f|%d|%d|%s\n",
                  acc.accNo, acc.name, &acc.pin, &acc.balance,
                  &acc.locked, &acc.frozen, acc.createdOn) != EOF) {

        fprintf(csv, "%s,%s,%d,%.2f,%d,%d,%s\n",
                acc.accNo, acc.name, acc.pin,
                acc.balance, acc.locked, acc.frozen, acc.createdOn);
    }

    fclose(src);
    fclose(csv);

    printf("\nExcel Export Completed! (accounts_export.csv)\n");
}
