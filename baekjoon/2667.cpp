#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
/*
이 문제는 Flood Fill 알고리즘이 사용되는 대표적인 문제입니다.
BFS의 한 종류로 배열을 전체 돌면서 BFS를 적용시켜 채워나가는 방식으로 문제에선 BFS가 적용된
단지내 집의 수(vertex 개수)를 저장하여 오름차순으로 출력하는 것입니다.
*/
int arr[25][25];	//입력받을 정사각형
int length, cnt;			//입력받을 정사각형의 길이, 단지 내 집의 수를 카운트하기 위한 변수(vertex의 개수)
void flood_fill(int x, int y) {
	if (0 > x || 0 > y || length <= x || length <= y) return;	//정사각형의 범위를 벗어나 탐색하려는 경우
	cnt++;
	arr[x][y] = 0;
	if (arr[x][y - 1] == 1) {		//왼쪽
		flood_fill(x, y - 1);
	}
	if (arr[x][y + 1] == 1) {	//오른쪽
		flood_fill(x, y + 1);
	}
	if (arr[x - 1][y] == 1) {	//위쪽
		flood_fill(x - 1, y);
	}
	if (arr[x + 1][y] == 1) {	//아래
		flood_fill(x + 1, y);
	}
	//왜 else if를 사용하면 안되나요? ) else if를 사용하면 else if 구문을 한 번 만족하여 실행하게 된다면 다른 나머지 분기를 실행하지 않고
	//바로 건너뛰어 버리기 때문에 네 방향을 전부 다 탐색할 수 없게 됩니다. 따라서 네 방향을 탐색하기 위해 if를 사용합니다.
}
using namespace std;
int main()
{
	cnt = 0;
	vector<int> v;	//단지 내 집의 개수(vertex 개수)를 저장하기 위해 사용합니다.
	memset(arr, 0, sizeof(arr));
	cin >> length;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			scanf("%1d", &arr[i][j]);	//1010...과 같은 패턴의 입력에서 정수 하나만 입력받기 위해 %1d를 사용합니다.
		}
	}

	for (int x = 0; x < length; x++) {
		for (int y = 0; y < length; y++) {
			if (arr[x][y] == 1) {	//세지 않은 단지(vertex 묶음)를 발견했을 때
				flood_fill(x, y);	//flood fill 알고리즘 적용
				v.push_back(cnt);		//flood fill을 적용시켰으므로 cnt엔 한 단지 내의 집의 개수가 저장되어 있습니다.
			}
			cnt = 0;				//다시 초기화합니다.		
		}
	}
	sort(v.begin(), v.end());		//저장된 단지 내 집의 개수들을 오름차 순으로 정렬합니다.

	cout << v.size() << endl;		//단지의 개수 출력
	for (int i = 0; i < v.size(); i++) {
		cout << v[i] << endl;		//단지내 집들의 개수 출력
	}
	return 0;
}