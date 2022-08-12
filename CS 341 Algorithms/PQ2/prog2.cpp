#include <iostream>
#include <vector>
#include <stack>
using namespace std;

const int max_weight = 10000001, max_edge_count = 1000001;
int n, m, s;
int x[max_edge_count], y[max_edge_count], w[max_edge_count];
int d[max_edge_count], parent[max_edge_count], minw[max_edge_count], use_alt[max_edge_count];
vector<int> in_v[max_edge_count], in_w[max_edge_count];

int main()
{
	// input from std::in
	cin >> n >> m >> s;
	for (int i = 0; i < m; i++)
	{
		cin >> x[i] >> y[i] >> w[i];
		in_v[y[i]].push_back(x[i]);
		in_w[y[i]].push_back(w[i]);
	}

	// initialize
	for (int i = 0; i < n; i++)
	{
		d[i] = max_weight;
		parent[i] = -1;
		minw[i] = -max_weight;
		use_alt[i] = -1;
	}
	d[s] = 0;

	// main algo
	for (int i = 1; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (d[x[j]] + w[j] < d[y[j]])
			{
				// check if strictly increasing
				if (w[j] > minw[x[j]])
				{
					d[y[j]] = d[x[j]] + w[j];
					parent[y[j]] = x[j];
					minw[y[j]] = w[j];
					use_alt[y[j]] = -1;
				}
				else
				{
					// if not increasing, see if we can attach the current edge to another (sub-optimal) path going to x[j]
					for (int k = 0; k < in_v[x[j]].size(); k++)
					{
						if (w[j] > in_w[x[j]][k] && in_w[x[j]][k] > minw[in_v[x[j]][k]] && d[in_v[x[j]][k]] + in_w[x[j]][k] + w[j] < d[y[j]])
						{
							d[y[j]] = d[in_v[x[j]][k]] + in_w[x[j]][k] + w[j];
							parent[y[j]] = x[j];
							minw[y[j]] = w[j];
							use_alt[y[j]] = in_v[x[j]][k];
						}
					}
				}
			}
		}
	}

	// output the paths with their lengths
	for (int i = 0; i < n; i++)
	{
		if (i == s)
			cout << "src" << endl;
		else if (d[i] == max_weight)
			cout << "inf" << endl;
		else
		{
			cout << d[i] << ":";
			int j = i;
			stack<int> q;
			while (parent[j] != -1)
			{
				q.push(j);
				if (use_alt[j] != -1)
				{
					q.push(parent[j]);
					j = use_alt[j];
				}
				else
				{
					j = parent[j];
				}
			}
			q.push(s);
			while (!q.empty())
			{
				cout << q.top() << " ";
				q.pop();
			}
			cout << endl;
		}
	}

	return 0;
}
