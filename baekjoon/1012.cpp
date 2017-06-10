#include <iostream>
#include <string.h>
#include <queue>
using namespace std;

int arr[100001];	//�Է��� 1���� ũ�ų� ����, 10^6 == 100000�� �����Ƿ� �迭�� ���̵� ���缭 ������� �ݴϴ�.

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
			if (1 < num / 3 && arr[num / 3] == 0) {	//������ ����� ������, �湮���� ���� ���̶��
				q.push(num / 3);		//3���� ������ ������ �����մϴ�.(���� 1�� ���� vertex�� �̵��մϴ�.)
				arr[num / 3] = arr[cur] + 1;
			}
		}
		if (cur % 2 == 0) {
			if (1 < num/2 && arr[num / 2] == 0) {	//������ ����� ������, �湮���� ���� ���̶��
				q.push(num / 2);
				arr[num / 2] = arr[cur] + 1;
			}
		}
		if (0 <= num-1 && arr[num - 1] == 0) {		
			q.push(num - 1);
			arr[num - 1] = arr[cur] + 1;
		}
	}
	cout << arr[1]-1 << endl;		//task1
	return 0;
}