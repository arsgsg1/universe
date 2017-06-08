#include <stdio.h>

int arr[50][50] = { 0 }, n, m;
//참고 : flood_fill알고리즘
//각각의 배추 묶음에 대해 카운트 한 후, 방문 표시를 한다.
void flood_fill(int x, int y)
{
	arr[x][y] = 0;
	if (x - 1 >= 0 && arr[x - 1][y] == 1)
		flood_fill(x - 1, y);
	if (x + 1<n && arr[x + 1][y] == 1)
		flood_fill(x + 1, y);
	if (y - 1 >= 0 && arr[x][y - 1] == 1)
		flood_fill(x, y - 1);
	if (y + 1<m&&arr[x][y + 1] == 1)
		flood_fill(x, y + 1);
}

int main()
{
	int t, num, x, y, count;
	scanf("%d", &t);
	for (int z = 0; z < t; ++z)
	{
		count = 0;
		scanf("%d %d %d", &m, &n, &num);
		for (int i = 0; i < num; ++i)
		{
			scanf("%d %d", &x, &y);
			arr[y][x] = 1;
		}

		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j)
				if (arr[i][j] == 1)
				{
					flood_fill(i, j);
					++count;
				}

		printf("%d\n", count);
	}
	return 0;
}

