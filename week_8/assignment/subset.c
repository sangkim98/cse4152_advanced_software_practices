#include <stdio.h>
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(){
    int n, s;

    scanf("%d %d", &n, &s);

    int numList[n];

    for(int i = 0; i < n; ++i){
        scanf("%d", numList+i);
    }

    int i, j, ans, sum;

    i = j = 0;
    ans = n+1;
    sum = numList[i];

    while(i <= j && j < n){
        if(sum < s){
            sum += numList[++j];
        }
        else{
            ans = MIN(ans, j - i + 1);
            sum -= numList[i++];
        }
    }

    if(ans == n+1) ans = 0;
    
    printf("%d\n", ans);

    return 0;
}