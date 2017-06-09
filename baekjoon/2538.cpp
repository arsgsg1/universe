#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
using namespace std;
/*
이 문제는 Flood Fill알고리즘이 적용된 문제로 각각의 직사각형을 마킹하여 나머지 영역에 Flood Fill알고리즘을 적용시키는 문제입니다.
분리된 영역의 시작지점에 BFS를 적용시켜 연결된 칸들을 전부 마킹한 뒤, 그것을 카운트하여 결과를 출력합니다.
이때, 직사각형의 넓이를 잘못 연산하지 않도록 주의합니다!
*/
int width, height, cnt = 0;		//모눈 종이의 오른쪽 하단 꼭짓점의 좌표와 직사각형의 넓이를 카운트할 변수
int arr[100][100];
typedef struct RECTANGLE rectangle;
struct RECTANGLE { //직사각형의 왼쪽 위 좌표(x, y)와 오른쪽 아래 좌표(x, y)
	int startX;
	int startY;
	int endX;
	int endY;
};
vector<rectangle> v;	//직사각형을 저장할 vector

void fill(int index) {	//모눈종이에 직사각형을 그리는 과정(배열에 직사각형을 마킹하는것)
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (v[index].startX <= j && v[index].endX > j		//직사각형을 마킹하는데에 주의해야합니다. 0, 1, 2..로 인덱스를 접근하기 때문에 endX >= j와 같이 한다면 넓이를 세는데에
																//원래 길이의 +1만큼 연산을 하여 (width+1 x height+1)과 같이 잘못된 연산을 하게 됩니다. 따라서 >, <를 해줍니다.
				&& v[index].startY <= i && v[index].endY > i) {
				arr[i][j] = 1;
				//직사각형의 영역은 1로 표시됩니다.
			}
		}
	}
}
//재귀적으로 분리되어 나누어지는 영역을 BFS형식으로 탐색합니다.
//영역의 넓이를 세는 변수인 cnt는 main에서 넓이를 재는 루프가 다시 반복될 때, 초기화를 해주어야 합니다.	task1
//직사각형의 영역은 0으로 표시하고, 직사각형으로 인해 분리된 영역은 1으로 표시합니다. task2
void flood_fill(int y, int x) {
	if (0 > x || 0 > y || width <= x || height <= y) return;	//범위를 벗어나 방문하려는 경우를 무시합니다.

	arr[y][x] = 1;		//직사각형으로 인해 분리된 영역을 1으로 마킹하면서 넓이를 카운트합니다. task2(2)
	cnt++;
	if (arr[y - 1][x] == 0) {		//위쪽
		flood_fill(y - 1, x);
	}
	if (arr[y + 1][x] == 0) {		//아래쪽
		flood_fill(y + 1, x);
	}
	if (arr[y][x - 1] == 0) {		//왼쪽
		flood_fill(y, x - 1);
	}
	if (arr[y][x + 1] == 0) {		//오른쪽
		flood_fill(y, x + 1);
	}
}
int main()
{
	int num;		//직사각형의 개수
	vector<int> result;	//직사각형의 넓이를 저장할 vector입니다.
	memset(arr, 0, sizeof(arr));		//모눈종이의 크기를 입력받는 루프, task2(1)
	cin >> height;
	cin >> width;
	cin >> num;
	
	for (int i = 0; i < num; i++) {		//직사각형들의 좌표를 입력받는 루프
		rectangle r;
		cin >> r.startX;
		cin >> r.startY;
		cin >> r.endX;
		cin >> r.endY;
		v.push_back(r);
	}

	for (int i = 0; i < v.size(); i++) {	//모눈종이에 직사각형을 표시하는 루프
		fill(i);
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (arr[i][j] == 0) {	//넓이를 재야할 직사각형이 있는지 검사합니다.
				flood_fill(i, j);
				result.push_back(cnt);
				cnt = 0;			//task1 완료
			}
		}
	}

	sort(result.begin(), result.end());	//직사각형의 넓이를 오름차순으로 출력하기 위해 정렬합니다.
	cout << result.size() << endl;				//넓이를 저장한 각각의 수들은 곧 직사각형의 개수와 같은 의미입니다.
	for (int i = 0; i < result.size(); i++) {
		cout << result[i] << " ";
	}
	cout << endl;
	return 0;
}