#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

// ���ĺ��� �� ���ڿ� ���� ���� ��� ǥ��
vector<vector<int>> adj;
vector<int> seen, order;    // seen : ������ �湮�� ���� �ִ����� �����ϴ� vector, order : ���ĵ� ���ĺ��� �����ϴ� vector

// �־��� �ܾ��κ��� ���ĺ� ���� ���İ��� �׷����� �����Ѵ�.
void makeGraph(const vector<string>& words) {
    adj = vector<vector<int> >(26, vector<int>(26, 0));
    // ��� ���Ұ� 0(int��)�� 26 x 26 ũ���� 2���� vector ���� adj ����
    //  26�� ���� = ���ĺ� ������ 26���̱� ����

    // j-1��° �ܾ�� j��° �ܾ��� ������ ���� ��
    for (int j = 1; j < words.size(); ++j) {
        int i = j - 1, len = min(words[i].size(), words[j].size());
        for (int k = 0; k < len; ++k)
            if (words[i][k] != words[j][k]) {   // �� �ܾ� words[i]�� words[j]�� k��° ���ĺ��� ���� �ٸ� ��
                                                // ex) lotte�� lg�� ù ��° ���ĺ��� l�� ���� ������ �� ���� ���ĺ��� o�� g�� ���ؾ���
                int a = words[i][k] - 'a';  // �ƽ�Ű�ڵ带 �̿��Ͽ� ���ĺ��� int�� ������ ����    ex) 'a' = 97, 'b' = 98, 'c' = 99 .........
                int b = words[j][k] - 'a';
                adj[a][b] = 1;  // ���� ��� adj�� ���ĺ��� ���� ���� ����. ex) adj[a][b] => ���ĺ� a ���� ������ b�� ����
                break;  // ���� word �� ��
            }
    }
}

void dfs(int here) {
    seen[here] = 1; // ���� ����(here)�� �湮�Ͽ����� 0 => 1�� state ����
    for (int there = 0; there < adj.size(); there++)
        if (adj[here][there] == 1 && seen[there] != 1)   // if (���� ������ ���� ���� ������ �ְ�, �ش� ���� ������ �湮�� ���� ������)
                                                         // Hint : ���� ������ ���� ���� ���� ������ adj��, �湮 ����� seen vector�� �̿�)
            dfs(there);     // �� ������ �����ϸ� �ش� ���� �������� ���(recursive)
    order.push_back(here);  // ���� ������ order�� �� �ڿ� �߰�
}

//adj�� �־��� �׷����� ���������� ����� ��ȯ
vector<int> topologicalSort() {
    int m = adj.size();     // ��������� ũ�� = 26 (= ���ĺ� ����)
    seen = vector<int>(m, 0);
    order.clear();
    for (int i = 0; i < m; i++)
        if (!seen[i])
            dfs(i);
    reverse(order.begin(), order.end());    // vector�� ���� order�� ����(reverse)��Ŵ
    // �׷����� ���� ���� �� ����Ŭ�� �����ϸ�(= Directed Acycle Graph�� �ƴ϶��) �� ���� ��ȯ
        for (int i = 0; i < m; i++)
            for (int j = i + 1; j < m; j++)
                if (adj[order[j]][order[i]])
                    return vector<int>();
    //���� ����� ���� �켱 Ž������ ���� ����(order)�� ��ȯ
    return order;
}
int main(void) {
    int cases;
    cin >> cases;   // test case �Է�
    while (cases--) {   // ��� test case�� ���Ͽ� �ݺ�
        int n;
        cin >> n;   // �Է��� �ܾ��� ����
        vector<string> words;   // string�� ���Ҹ� ������ vector�� ���� words ����
        for (int i = 0; i < n; i++) {
            string s;
            cin >> s;
            words.push_back(s); // �Էµ� �ܾ ���ʴ�� words�� ����
        }
        makeGraph(words);   // �Էµ� �ܾ�鿡 ���� ���� ��� ����
        vector<int> solve = topologicalSort();  // ���� ��� words�� �̿��Ͽ� ��ȣȭ�� ���ĺ� ������ vector�� ���� solve�� ����
        if (solve.empty())  // �Է� �ܾ���� ���ĺ� ������ �� �� ���� ���
            cout << "INVALID HYPOTHESIS";
        else  // �Է� �ܾ���� ���ĺ� ������ �� �� �ִ� ���
            for (int i = 0; i < solve.size(); i++)
                cout << char(solve[i] + 'a');   // ���ĺ� ������ ���
        cout << "\n";
    }
    return 0;
}