#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

int A[1000][1000];
int cum[1000][1000];

int main()
{

    FILE *in = fopen("input_assignment2.txt", "r");

    int n, m;
    fscanf(in, "%d %d", &m, &n);

    // 배열에 0값 삽입
    for (int i = 0; i < n; i++)
        A[i + 1][0] = 0;
    for (int i = 0; i < m; i++)
        A[0][i + 1] = 0;
    A[0][0] = 0;

    //
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int tmp;
            fscanf(in, "%d ", &tmp);
            A[i + 1][j + 1] = tmp;
        }
    }

    // start code
    /*
    A 배열에 넣은 값을 누적 합 계산해서 cum 배열에 넣기
    수업시간에 점화식 설명하신 부분 생각하면 쉽게 접근 가능합니다.
    */

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cum[i + 1][j + 1] = cum[i + 1][j] + A[i + 1][j + 1];
        }
    }

    // end code

    // 누적합 배열 확인용 코드
    // for (int i = 0; i <= n; i++)
    // {
    //     for (int j = 0; j <= m; j++)
    //     {
    //         printf("%d ", cum[i][j]);
    //     }
    //     printf("\n");
    // }

    // Q queries
    fscanf(in, "%d", &m);

    for (int i = 0; i < m; i++)
    {

        // left up, right down (x, y)
        int lux, luy, rdx, rdy;
        fscanf(in, "%d %d %d %d", &lux, &luy, &rdx, &rdy);

        lux++;
        luy++;
        rdx++;
        rdy++;

        int value = 0; // value 작성하세요.

        for(int i = luy; i <= rdy; ++i){
            value += cum[i][rdx]-cum[i][lux-1];
        }

        printf("%d\n", value);
    }

    return 0;
}