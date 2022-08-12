#include <iostream>
#include <vector>

using namespace std;

int n, m, h;
int v[1001], f[1001];
vector<vector<vector<int>>> M(1000, vector<vector<int>>(1000, vector<int>(1000, 0)));

vector<int> backtrack(int i, int j, int k)
{
	if (i == 0 || j == 0 || k == 0)
	{
		vector<int> res;
		return res;
	}

	if (k < f[i])
		return backtrack(i - 1, j, k);
	if (M[i][j][k] == M[i - 1][j][k])
		return backtrack(i - 1, j, k);
	if (M[i][j][k] == M[i][j - 1][k])
		return backtrack(i, j - 1, k);
	if (M[i][j][k] == M[i][j][k - 1])
		return backtrack(i, j, k - 1);

	M[i - 1][j - 1][f[i]] + v[i];
	vector<int> res = backtrack(i - 1, j - 1, f[i]);
	res.push_back(i);
	return res;
}

int main()
{
	cin >> n >> m >> h;

	for (int i = 1; i <= n; i++)
	{
		cin >> v[i] >> f[i];
	}

	for (int i = 0; i <= n; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			M[0][i][j] = 0;
			M[i][0][j] = 0;
			M[i][j][0] = 0;
		}
	}

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= i; j++)
		{
			for (int k = 1; k < f[i]; k++)
			{
				M[i][j][k] = M[i - 1][j][k];
			}
			for (int k = f[i]; k <= h; k++)
			{
				M[i][j][k] = max(M[i][j][k - 1], max(M[i][j - 1][k], max(M[i - 1][j][k], M[i - 1][j - 1][f[i]] + v[i])));
			}
		}
	}

	cout << M[n][m][h] << endl;

	auto sol = backtrack(n, m, h);
	for (auto it : sol)
	{
		cout << it << " ";
	}
	cout << endl;

	return 0;
}
