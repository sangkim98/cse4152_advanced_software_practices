#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <memory>
#include <queue>
#include <utility>
#include <cmath>

// SegmentTree1D 구현하세요. 직접 스크래치로 전부 구현하셔도 무관합니다.
// 원소 합을 return 해주는 sum 함수와 원소 수정에 대한 modify 함수 구현

class Node
{
public:
    Node(int left, int right)
    {

        this->left = left;
        this->right = right;
        mid = (left + right) >> 1;
        sum = 0;
        leftNode = NULL;
        rightNode = NULL;
    }
    int left, right, mid, sum;
    Node *leftNode, *rightNode;
};

class SegmentTree1D
{

public:
    SegmentTree1D(int n)
    {
        // n개 사이즈 만큼 할당
        this->n = n;

        array = new int[n];
        root = new Node(0, n - 1);
    }

    // 초기화
    void initialize(Node *nodePointer)
    {
        if (nodePointer->left == nodePointer->right)
        {
            nodePointer->sum = array[nodePointer->mid];
        }
        else
        {
            nodePointer->leftNode = new Node(nodePointer->left, nodePointer->mid);
            nodePointer->rightNode = new Node((nodePointer->mid) + 1, nodePointer->right);

            initialize(nodePointer->leftNode);
            initialize(nodePointer->rightNode);

            nodePointer->sum = nodePointer->leftNode->sum + nodePointer->rightNode->sum;
        }
    }

    int sum(Node *node, int x, int y)
    {
        /*
            x에서 y 까지의 합
        */
        if (node->left >= x && node->right <= y)
        {
            return node->sum;
        }
        else if (node->left > y || node->right < x)
        {
            return 0;
        }
        else
        {
            return sum(node->leftNode, x, y) + sum(node->rightNode, x, y);
        }
    }

    void modify(int idx, int num)
    {
        /*
            idx에 위치한 원소를 num으로 교체
        */
        int delta = num - array[idx];
        array[idx] = num;

        Node *nodePointer = root;

        while (nodePointer)
        {
            nodePointer->sum += delta;

            if (idx <= nodePointer->mid)
            {
                nodePointer = nodePointer->leftNode;
            }
            else
            {
                nodePointer = nodePointer->rightNode;
            }
        }
    }

    int n;
    int *array;
    Node *root;
};

int main()
{

    int n, m;
    FILE *in = fopen("input_assignment1.txt", "r");

    fscanf(in, "%d", &n);
    SegmentTree1D A(n);

    int temp;
    for (int i = 0; i < n; i++)
    {
        fscanf(in, "%d", &temp);
        printf("%d ", temp);
        A.array[i] = temp;
    }
    printf("\n");

    A.initialize(A.root);

    fscanf(in, "%d", &m);
    for (int i = 0; i < m; i++)
    {

        fscanf(in, "%d\n", &temp);

        if (temp == 0)
        {
            int st, ed;
            fscanf(in, "%d %d", &st, &ed);
            printf("sum (%d,%d): %d\n", st, ed, A.sum(A.root, st, ed));
        }
        else
        {
            int idx, num;
            fscanf(in, "%d %d", &idx, &num);
            printf("change %dth elem with %d\n", idx + 1, num);
            A.modify(idx, num);
        }
    }

    return 0;
}