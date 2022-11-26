#include <iostream>
#include <queue>
#include <deque>
#include <stack>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

//�׷�����  ���� ��� ǥ��,adj[i][j] = i�� j������ ������ ��
vector<vector<int> > adj;
//graph[i][j]=i�� �����ؼ� j�� ������ �ܾ��� ���
vector<string> graph[26][26];
//indegree[i]=i�� �����ϴ� �ܾ��� ��
//outdegree[i]=i�� ������ �ܾ��� ��
vector<int> indegree, outdegree;

// dfs�� �̿��Ͽ� �׷������� circuit ������ ȹ��
void getEulerCircuit(int here, vector<int>& circuit) {
    for (int there = 0; there < adj[here].size(); there++) {
        while (adj[here][there] > 0) {  // ���� ����(here)���� ����� ��� ������ ���� �ݺ�
            adj[here][there]--; // ���� ������ ����
            getEulerCircuit(there, circuit);
        }
    }
    circuit.push_back(here);
}

//���� �׷����� ���Ϸ� ��Ŷ���� ���Ϸ� Ʈ�������� �Ǵ��Ͽ� �ش�Ǵ� �׷����� dfs ������ getEulerCircuit �Լ��� �̿��Ͽ� ����
vector<int> getEulerTrailOrCircuit() {
    vector<int> circuit;
    // ���Ϸ� Ʈ������ ��� : �������� �����ϹǷ� ���������� dfs ����
    for (int i = 0; i < 26; i++)
        if ((outdegree[i]-indegree[i]) == 1) {  // ���� ������ �������� ����
            getEulerCircuit(i, circuit);
            return circuit;
        }
    // ���Ϸ� ��Ŷ�� ��� : ������ ������ �ƹ� �������� dfs�� ����
    for (int i = 0; i < 26; i++)
        if ((outdegree[i] > 0) || (indegree[i] > 0)) { // ���� ������ ������ �ִ� ������ ����          
            getEulerCircuit(i, circuit);
            return circuit;
        }
    //��� ������ ��� �� �迭�� ��ȯ�Ѵ�.
    return circuit;
}
//���� �׷����� ���Ϸ� ��Ŷ/Ʈ���� ���θ� Ȯ��
bool checkEuler() {
    int plus1 = 0, minus1 = 0;  // plus1 : �׷����� ������ ������, minus1 : �׷����� ������ ����
    for (int i = 0; i < 26; i++) {
        int delta = outdegree[i] - indegree[i];
        if (delta < -1 || 1 < delta) return false;  // �������� ������ ������ ������ ������ ������ ���� ������ ������ ���� �ٸ��� false ����
        if (delta == 1) plus1++;    // ���� ������ �������� ���
        if (delta == -1) minus1++;  // ���� ������ ������ ���
    }
    // ���Ϸ� ��Ŷ : �������� ���� �� �� ����� ��
    // ���Ϸ� Ʈ���� : �������� ������ ���� �ϳ����� �־����
    // �־��� �ܾ ��� �̿��Ͽ� �����ձ⸦ �Ϸ��� �׷����� ���Ϸ� ��Ŷ�̰ų� ���Ϸ� Ʈ�����̾�� ��
    if (plus1 == 0 && minus1 == 0) return true; // �������� ������ ���� �ϳ��� �ְų� �ϳ��� ������ True �ƴϸ� False
    if (plus1 == 1 && minus1 == 1) return true;

    return false;
}

void makegraph(const vector<string>& words) {
    //���� ���� �ʱ�ȭ
    for (int i = 0; i < 26; i++)
        for (int j = 0; j < 26; j++)
            graph[i][j].clear();
    adj = vector<vector<int> >(26, vector<int>(26, 0));     // ��� ���Ұ� 0(int��)�� 26 x 26 ũ���� 2���� vector ���� adj ����
    indegree = outdegree = vector<int>(26, 0);

    //�� �ܾ �׷����� �߰��Ѵ�.
    for (int i = 0; i < words.size(); i++) {    // �Էµ� ��� �ܾ ���� �ݺ�
        int a = words[i].front() - 'a';  // (i��°�� �Էµ� �ܾ��� ù ���ĺ�) - 'a'    , int���� ������ �ƽ�Ű�ڵ�� ���ĺ��� ǥ���ϱ� ����
        int b = words[i].back() - 'a';    // (i��°�� �Էµ� �ܾ��� ������ ���ĺ�) - 'a'

        // graph, adj, outdegree, indegree ������ a�� �����ϰ� b�� ������ �ܾ� ���� �߰�
        graph[a][b].push_back(words[i]);
        adj[a][b]++;
        outdegree[a]++;
        indegree[b]++;
    }
}
string solve(const vector<string>& words) {
    makegraph(words);   // �����ձ� ���θ� �˱� ���� ���ĺ����� ���踦 �׷���(������� ����)�� �����ϴ� ����
    //������ ���������� ����!
    if (!checkEuler()) return "IMPOSSIBLE";     // �׷����� Euler circuit, Euler trail �� �� �ƴ϶�� �����ձ� ���� �Ұ���
    //���Ϸ� ��Ŷ�̳� ��θ� ã�Ƴ���.
    vector<int> circuit = getEulerTrailOrCircuit();
    //��� ������ �湮���� �������� ����!
    if (circuit.size() != words.size() + 1) return "IMPOSSIBLE";

    //�ƴ� ��� �湮������ ������ �� �������� ��� ���ڿ��� ����� ��ȯ�Ѵ�.
    reverse(circuit.begin(), circuit.end());
    string ret;
    for (int i = 1; i < circuit.size(); i++) {
        int a = circuit[i - 1], b = circuit[i];
        if (ret.size()) ret += " ";
        ret += graph[a][b].back();
        graph[a][b].pop_back();
    }
    return ret;
}
int main(void) {
    int cases;
    cin >> cases;   // test case ���� �Է�
    while (cases--) {   // ��� test case�� ���� �ݺ�
        adj.clear();    // ������� �ʱ�ȭ
        int n;
        cin >> n;   // �����ձ� ���ο� ������ �˾ƺ� �ܾ� ���� n �Է�
        vector<string> words;
        for (int i = 0; i < n; i++) {
            string s;
            cin >> s;
            words.push_back(s); // �ܾ �ϳ��� �Է¹޾� vector�� ���� words�� �ϳ��� �߰�
        }
        cout << solve(words) << endl;

    }
    return 0;
}