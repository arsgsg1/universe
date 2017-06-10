#include <iostream>
#include <string.h>
#include <queue>
using namespace std;
/*
�� ������ �پ��� ������ε� Ǯ �� ������ �� ������ ������� BFS ������� Ǯ�����ϴ�. ��� ���� DP�� BFS�� ���� ����� �ƶ����� �̷�������� ���� �������� �̹��ϰ� �ٸ� �� �ֽ��ϴ�.
���� Ǯ�̿��� �پ��� ����� �����Ƿ� ���߿� DP�ε� Ǯ�� �ø� ��ȹ �Դϴ�.
*/
int arr[1000001];	//�Է��� 1���� ũ�ų� ����, 10^6 == 100000�� �����Ƿ� �迭�� ���̵� ���缭 ������� �ݴϴ�.

int main()
{
	int num;
	queue<int> q;
	cin >> num;

	memset(arr, 0, sizeof(arr));

	q.push(num);
	arr[num] = 1;	//���� ������ ��湮���� �ʵ��� �ϱ� ���� 1�� ǥ�����ݴϴ�.
					//���� ���߿� ���â���� ����� �� ��, -1�� ���� ���� ǥ�����־�� �մϴ�.(task1)
	while (!q.empty()) {
		int cur = q.front();
		q.pop();
		if (cur == 1) break;//���������� �����߽��ϴ�! (���� N�� 1�� ��������ϴ�!)

		if (cur % 3 == 0) {				//X�� 3���� ������ �������ٸ�
			if (0 < cur / 3 && arr[cur / 3] == 0) {	//������ ����� ������, �湮���� ���� ���̶��
				q.push(cur / 3);		//3���� ������ ������ �����մϴ�.(���� 1�� ���� vertex�� �̵��մϴ�.)
				arr[cur / 3] = arr[cur] + 1;
			}
		}
		if (cur % 2 == 0) {
			if (0 < cur / 2 && arr[cur / 2] == 0) {	//������ ����� ������, �湮���� ���� ���̶��
				q.push(cur / 2);
				arr[cur / 2] = arr[cur] + 1;
			}
		}
		if (0 < cur - 1 && arr[cur - 1] == 0) {
			q.push(cur - 1);
			arr[cur - 1] = arr[cur] + 1;
		}
	}
	cout << arr[1] - 1 << endl;		//task1
	return 0;
}