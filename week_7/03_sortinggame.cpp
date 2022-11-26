#include<iostream>
#include<vector>
#include<map>
#include<queue>
#include<algorithm>
using namespace std;
// toSort[v] = v�� ���Ľ�Ű�µ� �ʿ��� ������ Ƚ��
map<vector<int>, int> toSort;

// ũ�Ⱑ n�� �迭�� ���� ��� ����� ������ ��� �� ������ ���(= ������ Ƚ��) ���
void precalc(int n) {
	// ��� �ڿ��� ���Ұ� ���ĵ� n ũ���� vector�� ���� perm ����
	vector<int> perm(n);
	for (int i = 0; i < n; i++)
		perm[i] = i;
	queue<vector<int>> q;	// bfs�� ���� queue �ڷ��� ����
	q.push(perm);
	toSort[perm] = 0;
	while (!q.empty()) {
		vector<int> here = q.front();
		q.pop();
		int cost = toSort[here];	// ���� �迭(here)�� ���� ������ Ƚ��
		// ��� ������ ��Ȳ�� ���� ���� for��
		for (int i = 0; i < n; i++)
			for (int j = i + 2; j <= n; j++) {
				reverse(here.begin()+i, here.begin()+j);	// here�� i���� j��°������ ���� ������(reverse)
					if (toSort.count(here) == 0) {	// �迭�� ���������� ���� ����� �ȵ� �迭�̸�
						// ���� �迭 ��뿡 +1
						toSort[here] = cost + 1;
						q.push(here);
					}
				// �迭 ���󺹱�
				reverse(here.begin()+i, here.begin()+j);	// here�� i���� j��°������ ���� ������(reverse), 26��° ���� �ڵ�� ����
			}
	}
}
// �Է� �迭�� ����� ũ�⸸ ����ϸ� �ǹǷ� ����ȭ ��Ų �� toSort���� ������ ��� ã��
int solve(const vector<int>& perm) {
	int n = perm.size();
	vector<int> fixed(n);	// �Է� �迭�� ����ȭ�� ����� ������ vector�� ���� fixed

	// �Է� �迭�� 1~8 ������ ������ ����ȭ�ϴ� �ݺ��� �ۼ�
	// ex) [1, 6, 2, 19, 33] => [1, 3, 2, 4, 5]
	int norm_value;
	for(int i = 0; i < n; i++){
		norm_value = 0;
		for(int j = 0; j < n; j++){
			if(perm[i] > perm[j]) norm_value++;
		}
		fixed[i] = norm_value;
	}

	return toSort[fixed];	// fixed �迭�� �ش��ϴ� ������ ��� ���� (Hint : toSort�� fixed �̿�)
}

int main() {
	// 1<=n<=8 ��� ��쿡 ���� ������ ��� �� ��� ���
	for(int i = 1; i <= 8; i++){
		precalc(i);
	}

	int testcase;
	cin >> testcase;	// test case ���� �Է�
	while (testcase--) {	// ��� test case�� ���Ͽ� �ݺ�
		int n;
		cin >> n;	// �Է��� �迭�� ����
		vector<int> v;
		int tmp;
		// int�� ������ n�� ��ŭ �Է¹޾� vector ���� v�� ���ʴ�� ����
		for (int i = 0; i < n; i++) {
			cin >> tmp;
			v.push_back(tmp);
		}
		cout << solve(v) << endl;
	}
}