#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <unordered_map>

using namespace std;

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

void skipUseless()
{
	int n;
	cin >> n;
	getchar();
	for (int i = 0; i < n; i++)
	{
		string s;
		getline(cin, s);
	}
	cin >> n;
	getchar();
	for (int i = 0; i < n; i++)
	{
		string s;
		getline(cin, s);
	}
}

int main()
{
	skipUseless();
	string startSymbol;
	getline(cin, startSymbol);
	int n;
	cin >> n;
	getchar();
	string rules[1000];
	for (int i = 0; i < n; i++)
	{
		getline(cin, rules[i]);
	}
	int s, t;
	cin >> s >> t;
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
		cin >> state1 >> terminalOrSymbol >> reduceOrShift;
		if (reduceOrShift == "reduce")
		{
			cin >> rule;
			states[state1].reduces.addReduce(terminalOrSymbol, rule);
		}
		else if (reduceOrShift == "shift")
		{
			cin >> state2;
			states[state1].shifts.addShift(terminalOrSymbol, state2);
			if (terminalOrSymbol == "EOF")
			{
				acceptStates.push_back(state2);
			}
		}
	}
	string nextToken;
	stack<int> stateStack;
	stack<string> symbolStack;
	stateStack.push(0);
	int k = 0;
	bool eof = false;
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
					cout << rules[0] << endl;
					found = true;
				}
			}
			if (found)
				break;
		}
		if (nextToken == "")
		{
			cin >> nextToken;
			if (nextToken == "EOF")
				eof = true;
			k++;
		}
		int currState = stateStack.top();
		int rule = states[currState].findReduce(nextToken);
		int state2 = states[currState].findShift(nextToken);
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
			for (int i = 0; i < numOfSymbols; i++)
			{
				symbolStack.pop();
				stateStack.pop();
			}
			symbolStack.push(lhs);
			currState = stateStack.top();
			stateStack.push(states[currState].findShift(lhs));
			cout << rules[rule] << endl;
		}
		else if (state2 != -1)
		{
			symbolStack.push(nextToken);
			stateStack.push(state2);
			nextToken.clear();
		}
		else
		{
			cerr << "ERROR at " << k << endl;
			return 0;
		}
	}
	return 0;
}