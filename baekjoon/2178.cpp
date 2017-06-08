#include <iostream>
#include <string.h>
#include <queue>
using namespace std;
int arr[101][101];
//0번째부터 시작하는 것을 고려하여 101번째로 하였다.
//n, m <= 100
int x[4] = {1, 0, -1, 0};
int y[4] = {0, 1, 0, -1};
//좌표를 표시하기 위한 배열
int main()
{
	memset(arr, 0, sizeof(arr));
	int row, col;
	cin >> row;
	cin >> col;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			scanf("%1d", &arr[i][j]);	
			//10101..과 같은 패턴의 입력이 들어올 때, 버퍼에서
			//정수 하나만 입력받기 위해 %1d를 사용함
		}
	}
	queue<pair<int, int> > q;	//pair<int, int> == pair<x, y>
	q.push(make_pair(0, 0));
	while (!q.empty()) {	//큐가 비지 않을때 까지
		pair<int, int> cur = q.front();
		q.pop(); 
		for (int i = 0; i < 4; i++) {
			//범위를 벗어나면 다시 continue한다. 오류가 발생하기 때문
			if (row <= cur.first + x[i]
				|| col <= cur.second + y[i]) continue;

			if (arr[cur.first + x[i]][cur.second + y[i]] == 1) {
				//현재의 좌표에서 아래, 오른, 위, 왼쪽 순서로 갈 수 있는 곳을 탐색
				arr[cur.first + x[i]][cur.second + y[i]] += arr[cur.first][cur.second];
				//이동한 거리수 카운트해서 다시 큐에 넣는다.
				q.push(make_pair(cur.first+x[i], cur.second+y[i]));
			}
		}
	}
 	cout << arr[row-1][col-1] << endl;
	return 0;
}