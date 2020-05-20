#include <stdio.h>
#include <stdbool.h>

int main() {
    int n;
    printf("Enter Number: ");
    scanf("%d",&n);
    if(n%2 == 0 || n == 1)
        printf("%d is not a prime number",n);
    else{
        int i;
        bool isPrime = true;
        for(i = 3; i < n/2; i += 2){
            if(n%i == 0){
                isPrime = false;
                break;
            }
        }
        if(isPrime)
            printf("%d is prime number",n);
        else
            printf("%d is not a prime number",n);
    }
for(i=0;i<n;i++){

}
return 0;
kjkjbboklb}
}
