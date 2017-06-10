#include <iostream>
#include <string.h>
#include <queue>
using namespace std;

int arr[100001];	//입력은 1보다 크거나 같고, 10^6 == 100000와 같으므로 배열의 길이도 맞춰서 적용시켜 줍니다.

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
			if (1 < num / 3 && arr[num / 3] == 0) {	//범위를 벗어나지 않으며, 방문하지 않은 항이라면
				q.push(num / 3);		//3으로 나누는 연산을 실행합니다.(연산 1을 통해 vertex를 이동합니다.)
				arr[num / 3] = arr[cur] + 1;
			}
		}
		if (cur % 2 == 0) {
			if (1 < num/2 && arr[num / 2] == 0) {	//범위를 벗어나지 않으며, 방문하지 않은 항이라면
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