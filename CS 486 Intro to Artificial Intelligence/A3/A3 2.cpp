#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <cmath>

using namespace std;

bool compare(pair<int, double> a, pair<int, double> b)
{
	return (a.second > b.second);
}

int main()
{
	int max_word_number = 1;
	ifstream in;
	in.open("trainLabel.txt");
	vector<int> trainLabels, testLabels;
	int a, b;
	trainLabels.push_back(-1);
	testLabels.push_back(-1); // skip index 0
	while (in >> a)
		trainLabels.push_back(a);
	in.close();

	in.open("testLabel.txt");
	while (in >> a)
		testLabels.push_back(a);
	in.close();

	in.open("trainData.txt");
	map<int, int> trainDocs[1501], testDocs[1501];
	while (in >> a)
	{
		in >> b;
		trainDocs[a][b] = 1; // doc number a contains word number b
		if (b > max_word_number)
			max_word_number = b;
	}
	in.close();
	in.open("testData.txt");
	while (in >> a)
	{
		in >> b;
		testDocs[a][b] = 1; // doc number a contains word number b
	}
	in.close();

	double theta[7000][2];
	for (int i = 1; i <= max_word_number; i++) // laplace correction
	{
		theta[i][0] = 1;
		theta[i][1] = 1;
	}
	double theta_c;
	int ones = 2, twos = 2; // laplace correction
	for (int i = 1; i <= 1500; i++)
	{
		if (trainLabels[i] == 1)
			ones++;
		else
			twos++;
	}
	for (int i = 1; i <= 1500; i++)
	{
		for (int j = 1; j <= max_word_number; j++)
		{
			if (trainDocs[i][j] == 1)
			{
				if (trainLabels[i] == 1)
					theta[j][0]++;
				else
					theta[j][1]++;
			}
		}
	}
	for (int j = 1; j <= max_word_number; j++)
	{
		theta[j][0] /= ones;
		theta[j][1] /= twos;
	}
	theta_c = (double)ones / (double)(ones + twos);

	// ----------------------------

	vector<pair<int, double>> vec;
	for (int j = 1; j <= max_word_number; j++)
	{
		vec.push_back(pair(j, abs(log(theta[j][1]) - log(theta[j][0]))));
	}
	sort(vec.begin(), vec.end(), compare);
	for (int i = 0; i < 10; i++)
	{
		cout << vec[i].first << " " << vec[i].second << endl;
	}

	int correct = 0, wrong = 0;
	for (int j = 1; j <= 1500; j++)
	{
		double p1 = theta_c, p2 = 1 - theta_c;
		for (int i = 1; i <= max_word_number; i++)
		{
			int a_i = testDocs[j][i];
			p1 *= pow(theta[i][0], a_i);
			p1 *= pow(1 - theta[i][0], 1 - a_i);
			p2 *= pow(theta[i][1], a_i);
			p2 *= pow(1 - theta[i][1], 1 - a_i);
		}
		if ((p1 > p2 && testLabels[j] == 1) || (p1 <= p2 && testLabels[j] == 2))
			correct++;
		else
			wrong++;
	}
	cout << (double)correct / (double)(correct + wrong) << endl;
	return 0;
}
