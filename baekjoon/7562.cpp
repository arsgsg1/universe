#include <iostream>
#include <string.h>
#include <queue>
#include <utility>
using namespace std;
/*
이 문제는 나이트의 갈 수 있는 방향을 잘 고려하여 BFS를 돌리는 것입니다.
항상 8방향으로 전부 다 탐색을 해야만 하며, 방문기록을 남겨 재방문을 하지 않도록 하는 것,
더 이상 나이트가 갈 수 없는 경우(나이트의 갈 수 있는 칸이 전부 방문을 한 칸이여서 더 방문할 수 없는 경우)
-1을 출력하는 것이 핵심입니다.
*/
//나이트의 진행방향 중 위쪽부터 시계방향으로 차례대로 이동 가능 좌표를 설정합니다.
int dy[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
int dx[8] = {1, 2, 2, 1, -1, -2, -2, -1};
int arr[300][300];	//체스판
int length;			//체스판의 한 변의 길이
int main()
{
	queue<pair<int, int> > q;
	int test_t, cnt;		//테스트 케이스의 개수, 나이트의 이동 횟수
	cin >> test_t;
	for (int i = 0; i < test_t; i++) {
		memset(arr, 0, sizeof(arr));	//방문하면 1, 방문하지 않았으면 0으로 초기화합니다.
		cin >> length;
		int startX, startY, endX, endY;	//나이트의 시작 (x, y) 도착 (x, y)좌표

		cin >> startY;
		cin >> startX;
		cin >> endY;
		cin >> endX;

		//도착지점에 도착하여 마킹을 수정하지 못하면 나이트가 갈 수 없는 케이스입니다.
		//why?) BFS는 간선으로 연결되어 있다면 무조건 모든 항(vertex)들을 탐색할 수 있는 알고리즘 입니다.
		//따라서, BFS로 탐색하지 못한다면 간선이 연결되어 있지 않은,
		//(고립되어있는) 항이므로 탐색할 수가 없게 됩니다.
		arr[startY][startX] = 1;	//시작지점부터 재방문 방지를 위한 마킹을 합니다.
		q.push(make_pair(startY, startX));
		while (!q.empty()) {
			pair<int, int> cur = q.front();
			q.pop();
			if (cur.first == endY && cur.second == endX) {	//나이트가 도착지점에 도착했습니다!
				while (!q.empty()) q.pop();
				break;
			}

			int nextX, nextY;		//나이트가 갈 수 있는 좌표를 저장합니다.
			for (int i = 0; i < 8; i++) {
				//나이트가 이동할 수 있는 좌표인 8방향을 탐색합니다.
				nextY = cur.first + dy[i];
				nextX = cur.second + dx[i];
				if (0 > nextY || 0 > nextX || length <= nextY || length <= nextX) continue;	//범위를 벗어나 탐색하려는 경우를 무시합니다. 0 ~ N-1의 개수는 N개 입니다. 
				//length와 X또는 Y좌표가 같다는건 범위를 벗어났다는 뜻입니다.
				if (arr[nextY][nextX] <= 0) {
					arr[nextY][nextX] = arr[cur.first][cur.second] + 1;	//나이트가 방문한 칸을 방문처리 합니다.
																		//또한 이동횟수를 칸에 저장함으로써 최종 이동횟수를 구합니다.
					q.push(make_pair(nextY, nextX));//다음에 진행할 방향을 큐에 넣습니다.
				}
			}
		}
										//나이트가 갈 수 없는 경우
		cout << arr[endY][endX]-1 << endl;//이동횟수를 출력합니다.
										//시작지점에 재방문 방지를 위해 1로 마킹을 했으므로 출력을 할때는 -1로 조절해줍니다. (시작지점은 이동횟수가 0이기 때문)
	}
	return 0;
}