#include <iostream>
#include <algorithm>
#include <string.h>
using namespace std;
/*
이 문제는 관점에 따라 다르게 풀 수 있지만 어렵게 생각하면 한도 끝도 없이 어려운 분기를 사용해야합니다.
따라서 문제를 조금 단순하게 본다면 입력을 받자마자 그 합들을 기록해두는 배열을 만든 후, 다음 층의 계산에 이용하는 방식입니다.
*/
int main()
{
	int height, result = 0;
	int sum[501], prevSum[501];		//다음층을 입력받고 그 합을 계산할 배열(sum)과 이전의 숫자삼각형을 타고내려오면서 sum의 계산을 저장할 배열(prevSum)입니다.
	cin >> height;
	memset(sum, 0, sizeof(sum));
	memset(prevSum, 0, sizeof(prevSum));

	for (int i = 1; i <= height; i++) {
		memcpy(prevSum, sum, sizeof(sum));		//현재 숫자삼각형을 내려오면서 계산한 합들을 배열에 기록해둡니다. (숫자삼각형의 다음층으로 내려갈 때, 사용할 것입니다.)
		for (int j = 1; j <= i; j++) {
			scanf("%d", &sum[j]);				//삼각형의 다음층을 입력받습니다.
			sum[j] += max(prevSum[j - 1], prevSum[j]);	//이전 층의 합들을 기록해둔 배열에서 결과를 꺼내와 더합니다.
			result = max(result, sum[j]);		//더한 결과에서 가장 큰 값을 기록합니다.
		}
	}
	cout << result << endl;
	return 0;
}