#include <iostream>
#include <map>
#include <queue>
#include <utility>
#include <string.h>
#define INF 999988887
/*
이 문제는 그리디 알고리즘의 일종인 다익스트라 알고리즘의 전반적인 기초 문제입니다. (종이, 펜 필수!)
현재 내가 알고 있는 값이 가장 최선이라고 생각하고, 그 다음 탐색할 값이 현재의 최선보다 낫다면 값을 갱신합니다.
또한 이 문제는 인접리스트, 배열 두 가지 방식으로 풀 수 있는데 배열의 경우 간선의 연결이 촘촘하고 크기가 큰 그래프에서 사용하기 좋고, 인접리스트의 경우 그렇지 않은 경우에 쓰기 좋습니다.
why?) vertex의 개수가 10만개라면 배열의 경우 그 관계를 표현하는데 10만 x 10만 = 100만, int형 기준 400만 byte를 기본으로 소모하게 됩니다.
그러나 인접리스트는 혹시 고립되어 있는 vertex는 간선이 없기 때문에 입력루틴에서 배제하고 시작하여 메모리적인 관점에서 이득을 볼 수 있습니다.
*/
using namespace std;
void path(int *arr, int index) {
	//재귀함수로 이전 vertex를 거쳐가면서 경로를 출력합니다. 문제에 정답과 관련된 것은 아니지만 참고용입니다.
	if (arr[index] == 0 || arr[index] == -1) {
		cout << index;
		return;
	}
	else
		path(arr, arr[index]);
	cout << " -> " << index;
}
int main()
{
	vector<pair<int, int> > *adj = 0;
	int numofV, numofE, startV;
	cin >> numofV;
	cin >> numofE;
	cin >> startV;
	
	adj = new vector<pair<int, int> >[numofV + 1];
	for (int i = 0; i < numofE; i++) {
		int start, end, cost;
		cin >> start;
		cin >> end;
		cin >> cost;
		adj[start].push_back(make_pair(end, cost));
	}
	int *visited = new int[numofV + 1];				//문제의 인덱스가 1번부터 시작하므로 +1로 범위를 늘려줍니다.
	int *dist = new int[numofV + 1];
	for (int i = 0; i <= numofV; i++) visited[i] = -1;	//-1이면 한번도 방문하지 않은 vertex, 0이면 start vertex입니다.
	for (int i = 0; i <= numofV; i++) dist[i] = INF;

	dist[startV] = 0;
	visited[startV] = 0;

	priority_queue<pair<int, int> > pq;			//priority queue로 cost에 따라 edge들을 정렬합니다.
												//why?) priority queue로 정렬하는 이유는 배열방식은 모든 n-1개의 vertex를 탐색하려 하지만 priority queue로 모든 edge들을 정렬로 하여금 필요한 vertex만을 방문하여  불필요한 vertex 방문을 줄이고자 함입니다.
	pq.push(make_pair(0, startV));

	while (!pq.empty()) {
		int cur = pq.top().second;
		int cost = -pq.top().first;		//priority queue는 default로 max_heap으로 동작하기 때문에 -로 min_heap처럼 동작하도록 합니다.
		pq.pop();

		if (dist[cur] < cost) continue;					//지금 발견한 경로보다 이미 더 짧은 경로를 알고 있다면 무시합니다.

		for (int i = 0; i < adj[cur].size(); i++) {		//vertex의 모든 연결된 간선들을 탐색합니다.
			int next = adj[cur][i].first;
			int nextDist = cost + adj[cur][i].second;
			if (dist[next] > nextDist) {				//지금 발견한 경로가 이전보다 짧다면
				dist[next] = nextDist;					//더 짧은 경로의 cost합을 기록합니다.
				pq.push(make_pair(-nextDist, next));	//priority queue는 default로 max_heap으로 동작하기 때문에 -로 min_heap처럼 동작하도록 합니다.
				visited[next] = cur;		//경로를 적어가면서 알고리즘을 수행합니다. 문제의 정답과 관련된 것은 아니지만 참고용입니다.
			}
		}
	}

	for (int i = 1; i <= numofV; i++)
	{
		if (dist[i] == INF)
			cout << "INF" << '\n';
		else
			cout << dist[i] << '\n';
	}

	/*for (int i = 1; i <= numofV; i++) {		//경로를 출력하는 부분입니다. 정답과 연관된 것은 아니지만 참고용입니다. 주석을 해제하면 경로를 볼 수 있습니다.
	cout << i << " : ";
	if (visited[i] != -1)
	path(visited, i);
	else
	cout << "x";
	cout << endl;
	}*/
	delete[] adj;
	delete[] dist;
	delete[] visited;
	return 0;
}