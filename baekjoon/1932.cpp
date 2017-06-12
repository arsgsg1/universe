#include <iostream>
#include <algorithm>
#include <string.h>
using namespace std;
/*
�� ������ ������ ���� �ٸ��� Ǯ �� ������ ��ư� �����ϸ� �ѵ� ���� ���� ����� �б⸦ ����ؾ��մϴ�.
���� ������ ���� �ܼ��ϰ� ���ٸ� �Է��� ���ڸ��� �� �յ��� ����صδ� �迭�� ���� ��, ���� ���� ��꿡 �̿��ϴ� ����Դϴ�.
*/
int main()
{
	int height, result = 0;
	int sum[501], prevSum[501];		//�������� �Է¹ް� �� ���� ����� �迭(sum)�� ������ ���ڻﰢ���� Ÿ�������鼭 sum�� ����� ������ �迭(prevSum)�Դϴ�.
	cin >> height;
	memset(sum, 0, sizeof(sum));
	memset(prevSum, 0, sizeof(prevSum));

	for (int i = 1; i <= height; i++) {
		memcpy(prevSum, sum, sizeof(sum));		//���� ���ڻﰢ���� �������鼭 ����� �յ��� �迭�� ����صӴϴ�. (���ڻﰢ���� ���������� ������ ��, ����� ���Դϴ�.)
		for (int j = 1; j <= i; j++) {
			scanf("%d", &sum[j]);				//�ﰢ���� �������� �Է¹޽��ϴ�.
			sum[j] += max(prevSum[j - 1], prevSum[j]);	//���� ���� �յ��� ����ص� �迭���� ����� ������ ���մϴ�.
			result = max(result, sum[j]);		//���� ������� ���� ū ���� ����մϴ�.
		}
	}
	cout << result << endl;
	return 0;
}