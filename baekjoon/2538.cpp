#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
using namespace std;
/*
�� ������ Flood Fill�˰����� ����� ������ ������ ���簢���� ��ŷ�Ͽ� ������ ������ Flood Fill�˰����� �����Ű�� �����Դϴ�.
�и��� ������ ���������� BFS�� ������� ����� ĭ���� ���� ��ŷ�� ��, �װ��� ī��Ʈ�Ͽ� ����� ����մϴ�.
�̶�, ���簢���� ���̸� �߸� �������� �ʵ��� �����մϴ�!
*/
int width, height, cnt = 0;		//�� ������ ������ �ϴ� �������� ��ǥ�� ���簢���� ���̸� ī��Ʈ�� ����
int arr[100][100];
typedef struct RECTANGLE rectangle;
struct RECTANGLE { //���簢���� ���� �� ��ǥ(x, y)�� ������ �Ʒ� ��ǥ(x, y)
	int startX;
	int startY;
	int endX;
	int endY;
};
vector<rectangle> v;	//���簢���� ������ vector

void fill(int index) {	//�����̿� ���簢���� �׸��� ����(�迭�� ���簢���� ��ŷ�ϴ°�)
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (v[index].startX <= j && v[index].endX > j		//���簢���� ��ŷ�ϴµ��� �����ؾ��մϴ�. 0, 1, 2..�� �ε����� �����ϱ� ������ endX >= j�� ���� �Ѵٸ� ���̸� ���µ���
																//���� ������ +1��ŭ ������ �Ͽ� (width+1 x height+1)�� ���� �߸��� ������ �ϰ� �˴ϴ�. ���� >, <�� ���ݴϴ�.
				&& v[index].startY <= i && v[index].endY > i) {
				arr[i][j] = 1;
				//���簢���� ������ 1�� ǥ�õ˴ϴ�.
			}
		}
	}
}
//��������� �и��Ǿ� ���������� ������ BFS�������� Ž���մϴ�.
//������ ���̸� ���� ������ cnt�� main���� ���̸� ��� ������ �ٽ� �ݺ��� ��, �ʱ�ȭ�� ���־�� �մϴ�.	task1
//���簢���� ������ 0���� ǥ���ϰ�, ���簢������ ���� �и��� ������ 1���� ǥ���մϴ�. task2
void flood_fill(int y, int x) {
	if (0 > x || 0 > y || width <= x || height <= y) return;	//������ ��� �湮�Ϸ��� ��츦 �����մϴ�.

	arr[y][x] = 1;		//���簢������ ���� �и��� ������ 1���� ��ŷ�ϸ鼭 ���̸� ī��Ʈ�մϴ�. task2(2)
	cnt++;
	if (arr[y - 1][x] == 0) {		//����
		flood_fill(y - 1, x);
	}
	if (arr[y + 1][x] == 0) {		//�Ʒ���
		flood_fill(y + 1, x);
	}
	if (arr[y][x - 1] == 0) {		//����
		flood_fill(y, x - 1);
	}
	if (arr[y][x + 1] == 0) {		//������
		flood_fill(y, x + 1);
	}
}
int main()
{
	int num;		//���簢���� ����
	vector<int> result;	//���簢���� ���̸� ������ vector�Դϴ�.
	memset(arr, 0, sizeof(arr));		//�������� ũ�⸦ �Է¹޴� ����, task2(1)
	cin >> height;
	cin >> width;
	cin >> num;
	
	for (int i = 0; i < num; i++) {		//���簢������ ��ǥ�� �Է¹޴� ����
		rectangle r;
		cin >> r.startX;
		cin >> r.startY;
		cin >> r.endX;
		cin >> r.endY;
		v.push_back(r);
	}

	for (int i = 0; i < v.size(); i++) {	//�����̿� ���簢���� ǥ���ϴ� ����
		fill(i);
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (arr[i][j] == 0) {	//���̸� ����� ���簢���� �ִ��� �˻��մϴ�.
				flood_fill(i, j);
				result.push_back(cnt);
				cnt = 0;			//task1 �Ϸ�
			}
		}
	}

	sort(result.begin(), result.end());	//���簢���� ���̸� ������������ ����ϱ� ���� �����մϴ�.
	cout << result.size() << endl;				//���̸� ������ ������ ������ �� ���簢���� ������ ���� �ǹ��Դϴ�.
	for (int i = 0; i < result.size(); i++) {
		cout << result[i] << " ";
	}
	cout << endl;
	return 0;
}