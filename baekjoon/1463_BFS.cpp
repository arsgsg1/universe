#include <iostream>
#include <string.h>
#include <queue>
using namespace std;
/*
이 문제는 다양한 방법으로도 풀 수 있지만 한 가지의 방법으로 BFS 방법으로 풀었습니다. 어떻게 보면 DP나 BFS나 거의 비슷한 맥락으로 이루어지지만 보는 관점에서 미묘하게 다를 수 있습니다.
문제 풀이에는 다양한 방법이 있으므로 나중에 DP로도 풀어 올릴 계획 입니다.
Tip: 최단거리를 찾는 알고리즘엔 DFS, BFS 등등이 있지만 몇 가지 분류를 해보면 DFS의 경우 모든 상황을 다 알아야 한다는 등등의 상황에 사용하기 적절하고
BFS의 경우엔 모든 상황을 다 알지 않아도 답을 찾을 수 있거나 가까운 곳에 답이 있는 경우에 사용하면 적절합니다.
*/
int arr[1000001];	//입력은 1보다 크거나 같고, 10^6 == 100000와 같으므로 배열의 길이도 맞춰서 적용시켜 줍니다.

int main()
{
	int num;
	queue<int> q;
	cin >> num;

	memset(arr, 0, sizeof(arr));

	q.push(num);
	arr[num] = 1;	//시작 지점을 재방문하지 않도록 하기 위해 1을 표시해줍니다.
					//따라서 나중에 결과창에서 출력을 할 때, -1을 해준 값을 표시해주어야 합니다.(task1)
	while (!q.empty()) {
		int cur = q.front();
		q.pop();
		if (cur == 1) break;//도착지점에 도착했습니다! (정수 N을 1로 만들었습니다!)

		if (cur % 3 == 0) {				//X가 3으로 나누어 떨어진다면
			if (0 < cur / 3 && arr[cur / 3] == 0) {	//범위를 벗어나지 않으며, 방문하지 않은 항이라면
				q.push(cur / 3);		//3으로 나누는 연산을 실행합니다.(연산 1을 통해 vertex를 이동합니다.)
				arr[cur / 3] = arr[cur] + 1;
			}
		}
		if (cur % 2 == 0) {
			if (0 < cur / 2 && arr[cur / 2] == 0) {	//범위를 벗어나지 않으며, 방문하지 않은 항이라면
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