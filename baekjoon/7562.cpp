#include <iostream>
#include <string.h>
#include <queue>
#include <utility>
using namespace std;
/*
�� ������ ����Ʈ�� �� �� �ִ� ������ �� ����Ͽ� BFS�� ������ ���Դϴ�.
�׻� 8�������� ���� �� Ž���� �ؾ߸� �ϸ�, �湮����� ���� ��湮�� ���� �ʵ��� �ϴ� ��,
�� �̻� ����Ʈ�� �� �� ���� ���(����Ʈ�� �� �� �ִ� ĭ�� ���� �湮�� �� ĭ�̿��� �� �湮�� �� ���� ���)
-1�� ����ϴ� ���� �ٽ��Դϴ�.
*/
//����Ʈ�� ������� �� ���ʺ��� �ð�������� ���ʴ�� �̵� ���� ��ǥ�� �����մϴ�.
int dy[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
int dx[8] = {1, 2, 2, 1, -1, -2, -2, -1};
int arr[300][300];	//ü����
int length;			//ü������ �� ���� ����
int main()
{
	queue<pair<int, int> > q;
	int test_t, cnt;		//�׽�Ʈ ���̽��� ����, ����Ʈ�� �̵� Ƚ��
	cin >> test_t;
	for (int i = 0; i < test_t; i++) {
		memset(arr, 0, sizeof(arr));	//�湮�ϸ� 1, �湮���� �ʾ����� 0���� �ʱ�ȭ�մϴ�.
		cin >> length;
		int startX, startY, endX, endY;	//����Ʈ�� ���� (x, y) ���� (x, y)��ǥ

		cin >> startY;
		cin >> startX;
		cin >> endY;
		cin >> endX;

		//���������� �����Ͽ� ��ŷ�� �������� ���ϸ� ����Ʈ�� �� �� ���� ���̽��Դϴ�.
		//why?) BFS�� �������� ����Ǿ� �ִٸ� ������ ��� ��(vertex)���� Ž���� �� �ִ� �˰��� �Դϴ�.
		//����, BFS�� Ž������ ���Ѵٸ� ������ ����Ǿ� ���� ����,
		//(���Ǿ��ִ�) ���̹Ƿ� Ž���� ���� ���� �˴ϴ�.
		arr[startY][startX] = 1;	//������������ ��湮 ������ ���� ��ŷ�� �մϴ�.
		q.push(make_pair(startY, startX));
		while (!q.empty()) {
			pair<int, int> cur = q.front();
			q.pop();
			if (cur.first == endY && cur.second == endX) {	//����Ʈ�� ���������� �����߽��ϴ�!
				while (!q.empty()) q.pop();
				break;
			}

			int nextX, nextY;		//����Ʈ�� �� �� �ִ� ��ǥ�� �����մϴ�.
			for (int i = 0; i < 8; i++) {
				//����Ʈ�� �̵��� �� �ִ� ��ǥ�� 8������ Ž���մϴ�.
				nextY = cur.first + dy[i];
				nextX = cur.second + dx[i];
				if (0 > nextY || 0 > nextX || length <= nextY || length <= nextX) continue;	//������ ��� Ž���Ϸ��� ��츦 �����մϴ�. 0 ~ N-1�� ������ N�� �Դϴ�. 
				//length�� X�Ǵ� Y��ǥ�� ���ٴ°� ������ ����ٴ� ���Դϴ�.
				if (arr[nextY][nextX] <= 0) {
					arr[nextY][nextX] = arr[cur.first][cur.second] + 1;	//����Ʈ�� �湮�� ĭ�� �湮ó�� �մϴ�.
																		//���� �̵�Ƚ���� ĭ�� ���������ν� ���� �̵�Ƚ���� ���մϴ�.
					q.push(make_pair(nextY, nextX));//������ ������ ������ ť�� �ֽ��ϴ�.
				}
			}
		}
										//����Ʈ�� �� �� ���� ���
		cout << arr[endY][endX]-1 << endl;//�̵�Ƚ���� ����մϴ�.
										//���������� ��湮 ������ ���� 1�� ��ŷ�� �����Ƿ� ����� �Ҷ��� -1�� �������ݴϴ�. (���������� �̵�Ƚ���� 0�̱� ����)
	}
	return 0;
}