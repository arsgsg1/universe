#include <iostream>
#include <map>
#include <queue>
#include <utility>
#include <string.h>
#define INF 999988887
/*
�� ������ �׸��� �˰����� ������ ���ͽ�Ʈ�� �˰����� �������� ���� �����Դϴ�. (����, �� �ʼ�!)
���� ���� �˰� �ִ� ���� ���� �ּ��̶�� �����ϰ�, �� ���� Ž���� ���� ������ �ּ����� ���ٸ� ���� �����մϴ�.
���� �� ������ ��������Ʈ, �迭 �� ���� ������� Ǯ �� �ִµ� �迭�� ��� ������ ������ �����ϰ� ũ�Ⱑ ū �׷������� ����ϱ� ����, ��������Ʈ�� ��� �׷��� ���� ��쿡 ���� �����ϴ�.
why?) vertex�� ������ 10������� �迭�� ��� �� ���踦 ǥ���ϴµ� 10�� x 10�� = 100��, int�� ���� 400�� byte�� �⺻���� �Ҹ��ϰ� �˴ϴ�.
�׷��� ��������Ʈ�� Ȥ�� ���Ǿ� �ִ� vertex�� ������ ���� ������ �Է·�ƾ���� �����ϰ� �����Ͽ� �޸����� �������� �̵��� �� �� �ֽ��ϴ�.
*/
using namespace std;
void path(int *arr, int index) {
	//����Լ��� ���� vertex�� ���İ��鼭 ��θ� ����մϴ�. ������ ����� ���õ� ���� �ƴ����� ������Դϴ�.
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
	int *visited = new int[numofV + 1];				//������ �ε����� 1������ �����ϹǷ� +1�� ������ �÷��ݴϴ�.
	int *dist = new int[numofV + 1];
	for (int i = 0; i <= numofV; i++) visited[i] = -1;	//-1�̸� �ѹ��� �湮���� ���� vertex, 0�̸� start vertex�Դϴ�.
	for (int i = 0; i <= numofV; i++) dist[i] = INF;

	dist[startV] = 0;
	visited[startV] = 0;

	priority_queue<pair<int, int> > pq;			//priority queue�� cost�� ���� edge���� �����մϴ�.
												//why?) priority queue�� �����ϴ� ������ �迭����� ��� n-1���� vertex�� Ž���Ϸ� ������ priority queue�� ��� edge���� ���ķ� �Ͽ��� �ʿ��� vertex���� �湮�Ͽ�  ���ʿ��� vertex �湮�� ���̰��� ���Դϴ�.
	pq.push(make_pair(0, startV));

	while (!pq.empty()) {
		int cur = pq.top().second;
		int cost = -pq.top().first;		//priority queue�� default�� max_heap���� �����ϱ� ������ -�� min_heapó�� �����ϵ��� �մϴ�.
		pq.pop();

		if (dist[cur] < cost) continue;					//���� �߰��� ��κ��� �̹� �� ª�� ��θ� �˰� �ִٸ� �����մϴ�.

		for (int i = 0; i < adj[cur].size(); i++) {		//vertex�� ��� ����� �������� Ž���մϴ�.
			int next = adj[cur][i].first;
			int nextDist = cost + adj[cur][i].second;
			if (dist[next] > nextDist) {				//���� �߰��� ��ΰ� �������� ª�ٸ�
				dist[next] = nextDist;					//�� ª�� ����� cost���� ����մϴ�.
				pq.push(make_pair(-nextDist, next));	//priority queue�� default�� max_heap���� �����ϱ� ������ -�� min_heapó�� �����ϵ��� �մϴ�.
				visited[next] = cur;		//��θ� ����鼭 �˰����� �����մϴ�. ������ ����� ���õ� ���� �ƴ����� ������Դϴ�.
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

	/*for (int i = 1; i <= numofV; i++) {		//��θ� ����ϴ� �κ��Դϴ�. ����� ������ ���� �ƴ����� ������Դϴ�. �ּ��� �����ϸ� ��θ� �� �� �ֽ��ϴ�.
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