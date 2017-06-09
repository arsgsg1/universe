#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
/*
�� ������ Flood Fill �˰����� ���Ǵ� ��ǥ���� �����Դϴ�.
BFS�� �� ������ �迭�� ��ü ���鼭 BFS�� ������� ä�������� ������� �������� BFS�� �����
������ ���� ��(vertex ����)�� �����Ͽ� ������������ ����ϴ� ���Դϴ�.
*/
int arr[25][25];	//�Է¹��� ���簢��
int length, cnt;			//�Է¹��� ���簢���� ����, ���� �� ���� ���� ī��Ʈ�ϱ� ���� ����(vertex�� ����)
void flood_fill(int x, int y) {
	if (0 > x || 0 > y || length <= x || length <= y) return;	//���簢���� ������ ��� Ž���Ϸ��� ���
	cnt++;
	arr[x][y] = 0;
	if (arr[x][y - 1] == 1) {		//����
		flood_fill(x, y - 1);
	}
	if (arr[x][y + 1] == 1) {	//������
		flood_fill(x, y + 1);
	}
	if (arr[x - 1][y] == 1) {	//����
		flood_fill(x - 1, y);
	}
	if (arr[x + 1][y] == 1) {	//�Ʒ�
		flood_fill(x + 1, y);
	}
	//�� else if�� ����ϸ� �ȵǳ���? ) else if�� ����ϸ� else if ������ �� �� �����Ͽ� �����ϰ� �ȴٸ� �ٸ� ������ �б⸦ �������� �ʰ�
	//�ٷ� �ǳʶپ� ������ ������ �� ������ ���� �� Ž���� �� ���� �˴ϴ�. ���� �� ������ Ž���ϱ� ���� if�� ����մϴ�.
}
using namespace std;
int main()
{
	cnt = 0;
	vector<int> v;	//���� �� ���� ����(vertex ����)�� �����ϱ� ���� ����մϴ�.
	memset(arr, 0, sizeof(arr));
	cin >> length;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			scanf("%1d", &arr[i][j]);	//1010...�� ���� ������ �Է¿��� ���� �ϳ��� �Է¹ޱ� ���� %1d�� ����մϴ�.
		}
	}

	for (int x = 0; x < length; x++) {
		for (int y = 0; y < length; y++) {
			if (arr[x][y] == 1) {	//���� ���� ����(vertex ����)�� �߰����� ��
				flood_fill(x, y);	//flood fill �˰��� ����
				v.push_back(cnt);		//flood fill�� ����������Ƿ� cnt�� �� ���� ���� ���� ������ ����Ǿ� �ֽ��ϴ�.
			}
			cnt = 0;				//�ٽ� �ʱ�ȭ�մϴ�.		
		}
	}
	sort(v.begin(), v.end());		//����� ���� �� ���� �������� ������ ������ �����մϴ�.

	cout << v.size() << endl;		//������ ���� ���
	for (int i = 0; i < v.size(); i++) {
		cout << v[i] << endl;		//������ ������ ���� ���
	}
	return 0;
}