#include<stdio.h>

int main() {
    int p,r,t;
    float si;
    printf("Enter principal, rate and time: ");
    scanf("%d %d %d", &p, &r, &t);
    si = (p*r*t)/100.0;
    printf("Simple Interest = %.2f\n", si);
    return 0;
    
}