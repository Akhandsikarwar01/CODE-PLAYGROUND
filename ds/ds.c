#include <stdio.h>

int main() {
    int A[10][10], B[10][10], C[10][10];
    int r1,c1,r2,c2;
    int i,j,k,choice;

    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Transpose\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);

    if(choice==1 || choice==2) {

        printf("Enter rows and columns: ");
        scanf("%d %d",&r1,&c1);

        printf("Enter Matrix A:\n");
        for(i=0;i<r1;i++)
            for(j=0;j<c1;j++)
                scanf("%d",&A[i][j]);

        printf("Enter Matrix B:\n");
        for(i=0;i<r1;i++)
            for(j=0;j<c1;j++)
                scanf("%d",&B[i][j]);

        for(i=0;i<r1;i++){
            for(j=0;j<c1;j++){
                if(choice==1)
                    C[i][j]=A[i][j]+B[i][j];
                else
                    C[i][j]=A[i][j]-B[i][j];
            }
        }

        printf("Result:\n");
        for(i=0;i<r1;i++){
            for(j=0;j<c1;j++)
                printf("%d ",C[i][j]);
            printf("\n");
        }
    }

    else if(choice==3) {

        printf("Enter rows and columns of Matrix A: ");
        scanf("%d %d",&r1,&c1);

        printf("Enter rows and columns of Matrix B: ");
        scanf("%d %d",&r2,&c2);

        if(c1!=r2){
            printf("Multiplication not possible!");
            return 0;
        }

        printf("Enter Matrix A:\n");
        for(i=0;i<r1;i++)
            for(j=0;j<c1;j++)
                scanf("%d",&A[i][j]);

        printf("Enter Matrix B:\n");
        for(i=0;i<r2;i++)
            for(j=0;j<c2;j++)
                scanf("%d",&B[i][j]);

        for(i=0;i<r1;i++){
            for(j=0;j<c2;j++){
                C[i][j]=0;
                for(k=0;k<c1;k++)
                    C[i][j]+=A[i][k]*B[k][j];
            }
        }

        printf("Result:\n");
        for(i=0;i<r1;i++){
            for(j=0;j<c2;j++)
                printf("%d ",C[i][j]);
            printf("\n");
        }
    }

    else if(choice==4) {

        printf("Enter rows and columns: ");
        scanf("%d %d",&r1,&c1);

        printf("Enter Matrix:\n");
        for(i=0;i<r1;i++)
            for(j=0;j<c1;j++)
                scanf("%d",&A[i][j]);

        printf("Transpose:\n");
        for(i=0;i<c1;i++){
            for(j=0;j<r1;j++)
                printf("%d ",A[j][i]);
            printf("\n");
        }
    }

    else {
        printf("Invalid Choice");
    }

    return 0;
}