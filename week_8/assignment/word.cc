#include <iostream>
#include <string>
#define MIN(a, b) ((a) < (b) ? (a) : (b))

using namespace std;

int main(){
    string ab_str;

    cin >> ab_str;

    int n = ab_str.length();
    int a_count, b_count, ans;
    
    a_count = 0;
    ans = n;

    for(int i = 0; i < n; ++i){
        if(ab_str[i] == 'a') a_count++;
    }

    int i, j;

    for(i = 0; i < n; ++i){
        b_count = 0;
        for(j = i; j < i + a_count; ++j){
            if(ab_str[(j%n)] == 'b') b_count++;
        }
        ans = MIN(ans, b_count);
    }

    printf("%d", ans);

    return 0;
}