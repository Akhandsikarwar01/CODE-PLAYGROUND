#include <stdio.h>

int main() {
    int product = 1;
    int n = 0;
    for (int i = 1; i <= n; i++) {
        product *= i;
    }
    printf("The product of the first 10 natural numbers is: %d\n", product);    
    return 0;
}