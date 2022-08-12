#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;

ifstream in("in.txt");

class ReduceSet
{
public:
	void addReduce(string terminal, int rule)
	{
		terminals.push_back(terminal);
		rules.push_back(rule);
	}
	int findReduce(string terminal)
	{
		int i;
		for (i = 0; i < terminals.size(); i++)
		{
			if (terminals[i] == terminal)
				break;
		}
		if (i != terminals.size())
			return rules[i];
		else
			return -1;
	}

private:
	vector<string> terminals;
	vector<int> rules;
};

class ShiftSet
{
public:
	void addShift(string symbol, int state2)
	{
		symbols.push_back(symbol);
		state2s.push_back(state2);
	}
	int findShift(string symbol)
	{
		int i;
		for (i = 0; i < symbols.size(); i++)
		{
			if (symbols[i] == symbol)
				break;
		}
		if (i != symbols.size())
			return state2s[i];
		else
			return -1;
	}

private:
	vector<string> symbols;
	vector<int> state2s;
};

class State
{
public:
	int name;
	ReduceSet reduces;
	ShiftSet shifts;
	int findReduce(string terminal)
	{
		return reduces.findReduce(terminal);
	}
	int findShift(string symbol)
	{
		return shifts.findShift(symbol);
	}
};

class TreeNode
{
public:
	string val;
	string lexeme = "";
	vector<TreeNode *> children;
	int ruleNum;
	TreeNode(string newVal)
	{
		val = newVal;
	}
	TreeNode(string newVal, string newLexeme)
	{
		val = newVal;
		lexeme = newLexeme;
	}
	void addChildren(vector<TreeNode *> vec, int rule)
	{
		for (TreeNode *node : vec)
		{
			children.push_back(node);
		}
		ruleNum = rule;
		reverse(children.begin(), children.end());
	}
	bool isTerminal()
	{
		return lexeme != "";
	}
};

void skipUseless()
{
	int n;
	in >> n;
	string nl;
	getline(in, nl);
	for (int i = 0; i < n; i++)
	{
		string s;
		getline(in, s);
	}
	in >> n;
	getline(in, nl);
	for (int i = 0; i < n; i++)
	{
		string s;
		getline(in, s);
	}
}

void print(vector<TreeNode *> vec, string rules[])
{
	for (TreeNode *node : vec)
	{
		if (!node->isTerminal())
		{
			cout << rules[node->ruleNum] << endl;
			print(node->children, rules);
		}
		else
		{
			cout << node->val << " " << node->lexeme << endl;
		}
	}
}

void clearMemory(vector<TreeNode *> &vec)
{
	for (TreeNode *node : vec)
	{
		if (!node->isTerminal())
		{
			clearMemory(node->children);
		}
		delete node;
	}
}

int main()
{
	skipUseless();
	string startSymbol;
	getline(in, startSymbol);
	int n;
	in >> n;
	string nl;
	getline(in, nl);
	string rules[1000];
	for (int i = 0; i < n; i++)
	{
		getline(in, rules[i]);
	}
	int s, t;
	in >> s >> t;
	State states[1000];
	vector<int> acceptStates;
	for (int i = 0; i < s; i++)
	{
		states[i].name = i;
	}
	for (int i = 0; i < t; i++)
	{
		int state1, state2, rule;
		string terminalOrSymbol, reduceOrShift;
		in >> state1 >> terminalOrSymbol >> reduceOrShift;
		if (reduceOrShift == "reduce")
		{
			in >> rule;
			states[state1].reduces.addReduce(terminalOrSymbol, rule);
		}
		else if (reduceOrShift == "shift")
		{
			in >> state2;
			states[state1].shifts.addShift(terminalOrSymbol, state2);
			if (terminalOrSymbol == "EOF")
			{
				acceptStates.push_back(state2);
			}
		}
	}
	string nextToken, nextTokenLexeme;
	stack<int> stateStack;
	stack<string> symbolStack;
	stack<TreeNode *> treeStack;
	stateStack.push(0);
	int k = 0;
	bool bof = false;
	bool eof = false;
	bool outputTerminal = false;
	nextToken = "BOF";
	nextTokenLexeme = "BOF";
	while (true)
	{
		if (symbolStack.size() == 1 && symbolStack.top() == startSymbol)
		{
			break;
		}
		if (eof)
		{
			int target = stateStack.top();
			bool found = false;
			for (int a : acceptStates)
			{
				if (target == a)
				{
					found = true;
				}
			}
			if (found)
				break;
		}
		if (outputTerminal && bof)
		{
			nextToken.clear();
			nextTokenLexeme.clear();
			cin >> nextToken >> nextTokenLexeme;
			if (nextToken == "")
			{
				nextToken = "EOF";
				nextTokenLexeme = "EOF";
				bof = false;
				eof = true;
			}
			k++;
			outputTerminal = false;
		}
		int currState = stateStack.top();
		int rule = states[currState].findReduce(nextToken);
		int state2 = states[currState].findShift(nextToken);
		if (!bof)
		{
			state2 = states[currState].findShift("BOF");
			bof = true;
		}
		// reduce
		if (rule != -1)
		{
			int numOfSymbols = 0;
			string lhs = "";
			for (char ch : rules[rule])
			{
				if (ch == ' ')
					numOfSymbols++;
				if (numOfSymbols == 0)
					lhs += ch;
			}
			vector<TreeNode *> vec;
			for (int i = 0; i < numOfSymbols; i++)
			{
				vec.push_back(treeStack.top());
				treeStack.pop();
				symbolStack.pop();
				stateStack.pop();
			}
			symbolStack.push(lhs);
			TreeNode *newNode = new TreeNode(lhs);
			newNode->addChildren(vec, rule);
			treeStack.push(newNode);
			currState = stateStack.top();
			stateStack.push(states[currState].findShift(lhs));
		}
		else if (state2 != -1) // shift
		{
			TreeNode *newNode = new TreeNode(nextToken, nextTokenLexeme);
			treeStack.push(newNode);
			symbolStack.push(nextToken);
			stateStack.push(state2);
			outputTerminal = true;
		}
		else
		{
			cerr << "ERROR at " << k << endl;
			vector<TreeNode *> treeVector;
			while (!treeStack.empty())
			{
				treeVector.push_back(treeStack.top());
				treeStack.pop();
			}
			reverse(treeVector.begin(), treeVector.end());
			print(treeVector, rules);
			clearMemory(treeVector);
			return 0;
		}
	}
	cout << rules[0] << endl;
	vector<TreeNode *> treeVector;
	while (!treeStack.empty())
	{
		treeVector.push_back(treeStack.top());
		treeStack.pop();
	}
	reverse(treeVector.begin(), treeVector.end());
	print(treeVector, rules);
	clearMemory(treeVector);
	return 0;
}