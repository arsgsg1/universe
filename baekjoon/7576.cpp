#include <iostream>
#include <string.h>
#include <queue>
#include <utility>
using namespace std;
int arr[1001][1001];
//BFS�� �����ų ������ ��
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { 1, 0, -1, 0 };
/*
�� ������ BFS�� ��ǥ���� ������, ������ �丶�丶�� BFS�� ������Ѽ� ť�� �� �� ������ ������ ������ �մϴ�.
������ ���������κ����� BFS�� �ƴ� ������ �ٸ� ���������κ��� BFS�� ������� �������� �ؾ��մϴ�.
�� ��, �ߺ��湮�� ���� �ʵ��� �����ؾ� �մϴ�.
*/
int main()
{
	memset(arr, 0, sizeof(arr));
	int row, col, day = 0;	//day = �ּ� �ϼ�
	queue <pair<int, int> > q;
	cin >> col;
	cin >> row;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			scanf("%d", &arr[i][j]);
			if (arr[i][j] == 1) {
				q.push(make_pair(i, j));	//pair<x, y> == pair<row, col>
				//������ ���������κ���(���ھȿ� ����ִ� ������ �丶��) BFS�� �����Ű�� ���� ó�� �Է¹��� ��, ���� �丶��
				//(������)�� ť�� �ֽ��ϴ�.
			}
		}
	}
	int nextX, nextY;
	while (!q.empty()) {
		pair<int, int> cur = q.front();
		q.pop();
			for (int i = 0; i < 4; i++) {
				nextX = cur.first + dx[i];
				nextY = cur.second + dy[i];
				if (0 > nextX || row <= nextX ||
					0 > nextY ||col <= nextY) continue;	//������ ������ ��� Ž���Ϸ��� ���
				if (arr[nextX][nextY] == 0) {
					//���� ���� �丶��(�� �� �ִ� ����)�� �Ʒ��ʺ��� �ݽð���⿡ ���缭 ������� Ž���մϴ�.
					arr[nextX][nextY] = arr[cur.first][cur.second] + 1;	//�湮ó��, �丶�䰡 ����
					//�������� 0������ 1�� �������� ���������Ƿ� ���߿� -1�� �ؾ��մϴ�.
					//�ּ� �ϼ��� ǥ���ϸ鼭 �湮ó���� �ϴ� ����߿� �ϳ� �Դϴ�. BFS���� �ַ� ���� ����ϹǷ� ����صθ� �����ϰ� ����� �� �ֽ��ϴ�.

					q.push(make_pair(nextX, nextY)); //������ �� ������ ť�� ����
				}
			}
	}

	//���
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			day = max(day, arr[i][j]);	//�迭�� ����ص� �ϼ��� �߿� ���� ū �ϼ��� �����մϴ�.
			if (arr[i][j] == 0) {	//���� ���� �丶�䰡 ���� ��, �丶�䰡 ��� ���� ���ϴ� ��Ȳ�Դϴ�.
				cout << -1 << endl;
				return 0;
			}
		}
	}
	day -= 1;	//�������� 0������ 1�� �������� ���������Ƿ� ���߿� - 1�� �ؾ��մϴ�.
	cout << day << endl;

	return 0;
}