#include <iostream>
#include <string.h>
#include <queue>
using namespace std;
int arr[101][101];
//0��°���� �����ϴ� ���� ����Ͽ� 101��°�� �Ͽ���.
//n, m <= 100
int x[4] = {1, 0, -1, 0};
int y[4] = {0, 1, 0, -1};
//��ǥ�� ǥ���ϱ� ���� �迭
int main()
{
	memset(arr, 0, sizeof(arr));
	int row, col;
	cin >> row;
	cin >> col;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			scanf("%1d", &arr[i][j]);	
			//10101..�� ���� ������ �Է��� ���� ��, ���ۿ���
			//���� �ϳ��� �Է¹ޱ� ���� %1d�� �����
		}
	}
	queue<pair<int, int> > q;	//pair<int, int> == pair<x, y>
	q.push(make_pair(0, 0));
	while (!q.empty()) {	//ť�� ���� ������ ����
		pair<int, int> cur = q.front();
		q.pop(); 
		for (int i = 0; i < 4; i++) {
			//������ ����� �ٽ� continue�Ѵ�. ������ �߻��ϱ� ����
			if (row <= cur.first + x[i]
				|| col <= cur.second + y[i]) continue;

			if (arr[cur.first + x[i]][cur.second + y[i]] == 1) {
				//������ ��ǥ���� �Ʒ�, ����, ��, ���� ������ �� �� �ִ� ���� Ž��
				arr[cur.first + x[i]][cur.second + y[i]] += arr[cur.first][cur.second];
				//�̵��� �Ÿ��� ī��Ʈ�ؼ� �ٽ� ť�� �ִ´�.
				q.push(make_pair(cur.first+x[i], cur.second+y[i]));
			}
		}
	}
 	cout << arr[row-1][col-1] << endl;
	return 0;
}