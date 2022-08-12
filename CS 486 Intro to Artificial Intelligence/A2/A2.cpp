#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <cmath>

using namespace std;

struct compare
{
	bool operator()(const pair<int, double> &a, const pair<int, double> &b)
	{
		return (a.second < b.second);
	}
};

class Node
{
public:
	int val = -1; // word number to split on
	int pe = -1;  // point estimate
	Node *lchild = NULL;
	Node *rchild = NULL;
	Node *parent = NULL;
	vector<int> usedWords;
	vector<int> docs;
	Node() {}
};

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

	// --------------------------
	// beginning of tree building
	Node *root = new Node();
	for (int i = 1; i <= 1500; i++)
		root->docs.push_back(i);
	queue<Node *> nodeQueue;
	nodeQueue.push(root);
	int counter = 0;
	while (counter < 100) // compute 100 nodes
	{
		Node *node = nodeQueue.front();
		Node *temp = node;

		// compute initial information content
		int ones = 0, twos = 0;
		double i_e0, p1, p2;
		for (int i : node->docs)
		{
			if (trainLabels[i] == 1)
				ones++;
			else
				twos++;
		}
		p1 = (double)ones / (double)(ones + twos);
		p2 = (double)twos / (double)(ones + twos);
		if (p1 == 0 || p2 == 0)
		{
			// cout << " leaf parent val = " << node->parent->val;
			// if (node->parent->lchild == node)
			// 	cout << " left child ";
			// else
			// 	cout << " right child ";
			// cout << ones << " " << twos << endl;
			nodeQueue.pop();
			// this should be a leaf
			continue;
		}
		i_e0 = -p1 * log(p1) / log(2) - p2 * log(p2) / log(2);

		// compute current node's feature selection
		priority_queue<pair<int, double>, vector<pair<int, double>>, compare> pq;
		for (int i = 1; i <= max_word_number; i++) // split on words
		{
			if (find(node->usedWords.begin(), node->usedWords.end(), i) != node->usedWords.end())
				continue;
			int plus_ones = 0, plus_twos = 0, minus_ones = 0, minus_twos = 0;
			for (int j : node->docs) // look into each remaining document to see whether it contains word i
			{
				if (trainDocs[j][i])
				{
					if (trainLabels[j] == 1)
						plus_ones++;
					else
						plus_twos++;
				}
				else
				{
					if (trainLabels[j] == 1)
						minus_ones++;
					else
						minus_twos++;
				}
			}
			p1 = (double)plus_ones / (double)(plus_ones + plus_twos);
			p2 = (double)plus_twos / (double)(plus_ones + plus_twos);
			double i_e1 = 0;

			if (p1 != 0 && !isnan(p1))
				i_e1 -= p1 * log(p1) / log(2);
			if (p2 != 0 && !isnan(p2))
				i_e1 -= p2 * log(p2) / log(2);

			p1 = (double)minus_ones / (double)(minus_ones + minus_twos);
			p2 = (double)minus_twos / (double)(minus_ones + minus_twos);
			double i_e2 = 0;
			if (p1 != 0 && !isnan(p1))
				i_e2 -= p1 * log(p1) / log(2);
			if (p2 != 0 && !isnan(p2))
				i_e2 -= p2 * log(p2) / log(2);

			// double i_esplit = (double)0.5 * i_e1 + (double)0.5 * i_e2; // method 1
			//  method 2 below
			double i_esplit = (double)(plus_ones + plus_twos) / 1500 * i_e1 + (double)(minus_ones + minus_twos) / 1500 * i_e2;
			double ig = i_e0 - i_esplit; // information gain
			pq.push(pair<int, double>(i, ig));
		}
		node->val = pq.top().first;
		// if (counter < 10)
		// 	cout << counter + 1 << " " << node->val << " " << pq.top().second << endl;

		// create child nodes and add them to nodeQueue
		Node *left = new Node();
		left->parent = node;
		node->lchild = left;
		Node *right = new Node();
		node->rchild = right;
		right->parent = node;
		nodeQueue.push(left);
		nodeQueue.push(right);

		// initilize both children's doc set and usedWord list
		left->docs = node->docs;
		right->docs = node->docs;
		for (int i = left->docs.size() - 1; i >= 0; i--)
		{
			if (!trainDocs[left->docs[i]][node->val])
				left->docs.erase(left->docs.begin() + i);
		}
		for (int i = right->docs.size() - 1; i >= 0; i--)
		{
			if (trainDocs[right->docs[i]][node->val])
				right->docs.erase(right->docs.begin() + i);
		}
		left->usedWords = node->usedWords;
		right->usedWords = node->usedWords;
		left->usedWords.push_back(node->val);
		right->usedWords.push_back(node->val);

		// ------------------
		// training set accuracy
		int correct = 0;
		for (int k = 1; k <= 1500; k++)
		{
			// find the target node
			Node *targetNode = root;
			while (targetNode->lchild != NULL && targetNode->rchild != NULL)
			{
				if (trainDocs[k][targetNode->val])
					targetNode = targetNode->lchild;
				else
					targetNode = targetNode->rchild;
			}
			// compute point estimate
			int ones = 0, twos = 0;
			for (int i : targetNode->docs)
			{
				if (trainLabels[i] == 1)
					ones++;
				else
					twos++;
			}
			if (ones > twos && trainLabels[k] == 1)
				correct++;
			else if (twos >= ones && trainLabels[k] == 2)
				correct++;
		}
		// cout << (double)correct / 1500 << endl;

		// test set accuracy
		correct = 0;
		for (int k = 1; k <= 1500; k++)
		{
			// find the target node
			Node *targetNode = root;
			while (targetNode->lchild != NULL && targetNode->rchild != NULL)
			{
				if (testDocs[k][targetNode->val])
					targetNode = targetNode->lchild;
				else
					targetNode = targetNode->rchild;
			}
			// compute point estimate
			int ones = 0, twos = 0;
			for (int i : targetNode->docs)
			{
				if (testLabels[i] == 1)
					ones++;
				else
					twos++;
			}
			if (ones > twos && testLabels[k] == 1)
				correct++;
			else if (twos >= ones && testLabels[k] == 2)
				correct++;
		}
		cout << (double)correct / 1500 << endl;

		nodeQueue.pop();
		counter++;
		// if (counter == 10)
		// {
		// 	cout << "output leaves" << endl;
		// 	while (!nodeQueue.empty())
		// 	{
		// 		Node *n = nodeQueue.front();
		// 		int ones = 0, twos = 0;
		// 		for (int i : n->docs)
		// 		{
		// 			if (trainLabels[i] == 1)
		// 				ones++;
		// 			else
		// 				twos++;
		// 		}
		// 		if (ones > twos)
		// 			cout << n->parent->val << " " << ones << " " << twos << " "
		// 				 << "1" << endl;
		// 		else
		// 			cout << n->parent->val << " " << ones << " " << twos << " "
		// 				 << "2" << endl;
		// 		nodeQueue.pop();
		// 	}
		// 	return 0;
		// }
	}
	return 0;
}
