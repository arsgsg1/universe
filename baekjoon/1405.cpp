#include <iostream>
#include <string.h>
using namespace std;
/*
이 문제는 DFS의 한 종류로 상태 공간 트리* 를 사용해서 풀면 도움이 됩니다.
상태 공간 트리란 문제의 가능성(확률)들을 모두 나타낸 트리로 이 트리를 전부 탐색하면 문제의 해가 존재하거나, 
존재하지 않는다는 결론을 얻을 수 있습니다. 따라서 상태 공간 트리를 얼마나 효과적으로 탐색할 수 있는지가 핵심입니다.
*/

double prob[4];		//로봇의 진행방향에 따른 확률을 저장할 배열입니다.
int dx[4] = { 1, -1, 0, 0 };//로봇이 동서남북으로 이동하는 것을 나타내기 위해 선언한 배열입니다. (동서)
int dy[4] = { 0, 0, 1, -1 };//로봇이 동서남북으로 이동하는 것을 나타내기 위해 선언한 배열입니다. (남북)
int **location;	//2차원 좌표평면
double result;		//결과확률을 저장할 변수

double dfs(int y, int x, int move) {
	if (location[y][x] == 1) //이미 지나쳐온 칸을 다시 탐색하는 경우라면 더 이상 탐색할 필요가 없으므로 종료해버립니다.
		return 0;

	if (move == 0)			//1을 반환하여 *를 통한 확률계산이 가능하도록 합니다. (0을 반환하면 같은 부분을 다시 탐색했다는 뜻입니다, 또한 move가 0이라는건 상태공간트리의 말단노드라는 뜻입니다.)
		return 1;

	location[y][x] = 1;	//방문하지 않은 칸이라면 마킹을 합니다.

	double ret = 0;

	for (int i = 0; i < 4; i++) {
		ret += dfs(y + dy[i], x + dx[i], move-1) * prob[i];	//DFS 알고리즘의 핵심입니다. 재귀적인 함수부분에서는 분할 정복 방식으로 생각하는 것이 좋습니다.
		//즉, 당장엔 동, 서, 남, 북으로 한칸씩 이동하는 모든 확률들을 더하는 것이지만 분할정복으로 생각해본다면 다시 그것을 쪼개어 
		//동(동, 서, 남, 북), 서(동, 서, 남, 북), 남(동, 서, 남, 북), 북(동, 서, 남, 북)... 이런식으로 다시 작은 단위의 모든 확률들을 더하는 것으로 생각하여 결국엔
		//최소 단위(상태공간트리의 말단노드)부터 타고올라가 답을 구하게 되는 방식입니다. 
	}
	location[y][x] = 0;		//탐색이 끝났으므로 마킹을 지웁니다. (상태공간트리에서 현재 노드의 모든 자식노드들을 탐색하여 ret에 확률을 계산, 저장해둔 상태)

	return ret;
}
//문제를 풀어가는 방식은 2차원 배열로 로봇을 좌표평면의 중간지점에 세워둔 후 이동방향을 체크해 나가는 것입니다.
int main()
{
	int move;	//로봇의 이동횟수
	int size;	//2차원 배열의 크기
	cin >> move;
	for (int i = 0; i < 4; i++) {
		double temp;
		scanf("%lf", &temp);
		prob[i] = temp / 100;	//확률을 퍼센트로 변환하여 저장합니다.
	}
	
	size = move * 3;

	location = new int *[size];
	for (int i = 0; i < size; i++) {
		location[i] = new int[size];
		for (int j = 0; j < size; j++) {
			location[i][j] = 0;	//방문하지 않았으면 0, 방문했으면 1
		}
	}

	/*로직시작*/
	cout << fixed;
	cout.precision(9);	//가장 큰 자리수부터 9자리를 출력합니다.
	cout << dfs(size / 2, size / 2, move) << endl;
	/*로직끝*/

	for (int i = 0; i < size; i++) {
		delete[] location[i];
	}
	delete[] location;
	return 0;
}