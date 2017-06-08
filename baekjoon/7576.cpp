#include <iostream>
#include <string.h>
#include <queue>
#include <utility>
using namespace std;
int arr[1001][1001];
//BFS로 적용시킬 방향의 길
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { 1, 0, -1, 0 };
/*
이 문제는 BFS의 대표적인 문제로, 각각의 토마토마다 BFS를 적용시켜서 큐가 다 빌 때까지 루프를 돌도록 합니다.
기존의 시작점으로부터의 BFS가 아닌 각각의 다른 시작점으로부터 BFS를 적용시켜 퍼지도록 해야합니다.
이 때, 중복방문을 하지 않도록 조심해야 합니다.
*/
int main()
{
	memset(arr, 0, sizeof(arr));
	int row, col, day = 0;	//day = 최소 일수
	queue <pair<int, int> > q;
	cin >> col;
	cin >> row;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			scanf("%d", &arr[i][j]);
			if (arr[i][j] == 1) {
				q.push(make_pair(i, j));	//pair<x, y> == pair<row, col>
				//각각의 시작점으로부터(상자안에 들어있는 각각의 토마토) BFS를 적용시키기 위해 처음 입력받을 때, 익은 토마토
				//(시작점)을 큐에 넣습니다.
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
					0 > nextY ||col <= nextY) continue;	//상자의 범위를 벗어나 탐색하려는 경우
				if (arr[nextX][nextY] == 0) {
					//익지 않은 토마토(갈 수 있는 방향)를 아래쪽부터 반시계방향에 맞춰서 순서대로 탐색합니다.
					arr[nextX][nextY] = arr[cur.first][cur.second] + 1;	//방문처리, 토마토가 익음
					//시작일은 0이지만 1을 기준으로 시작했으므로 나중에 -1을 해야합니다.
					//최소 일수를 표시하면서 방문처리를 하는 방법중에 하나 입니다. BFS에서 주로 많이 사용하므로 기억해두면 유용하게 사용할 수 있습니다.

					q.push(make_pair(nextX, nextY)); //다음에 갈 방향을 큐에 넣음
				}
			}
	}

	//결과
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			day = max(day, arr[i][j]);	//배열에 기록해둔 일수들 중에 가장 큰 일수를 저장합니다.
			if (arr[i][j] == 0) {	//익지 않은 토마토가 있을 때, 토마토가 모두 익지 못하는 상황입니다.
				cout << -1 << endl;
				return 0;
			}
		}
	}
	day -= 1;	//시작일은 0이지만 1을 기준으로 시작했으므로 나중에 - 1을 해야합니다.
	cout << day << endl;

	return 0;
}