#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *fp = fopen("sample_accounts.txt", "w");

    if (!fp) {
        printf("File create nahi ho payi!\n");
        return 1;
    }

    char *names[] = {
        "Utsav Shakya", "Akhand Sikarwar", "Arjun Patel", "Rohit Kumar",
        "Sneha Verma", "Priya Singh", "Aman Yadav", "Kunal Sharma",
        "Ravi Chauhan", "Vikas Dubey", "Tanya Rajput", "Ritu Agrawal"
    };
    int totalNames = 12;

    srand(time(NULL));

    for (int i = 1; i <= 500; i++) {
        char accNo[20];
        sprintf(accNo, "11012025%06d01", i);

        char *name = names[rand() % totalNames];

        int pin = rand() % 9000 + 1000;
        float balance = (rand() % 200000) + (rand() % 100) / 100.0;

        int locked = 0;
        int frozen = 0;

        // random date-time
        int day = rand() % 28 + 1;
        int month = rand() % 12 + 1;
        int year = 2025;
        int hour = rand() % 24;
        int min = rand() % 60;
        int sec = rand() % 60;

        fprintf(fp, "%s|%s|%d|%.2f|%d|%d|%02d-%02d-%04d_%02d:%02d:%02d\n",
                accNo, name, pin, balance, locked, frozen,
                day, month, year, hour, min, sec);
    }

    fclose(fp);

    printf("500 sample accounts generated in sample_accounts.txt!\n");
    return 0;
}
