#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

//Skip the grammar part of the input.
void skipLine(istream &in)
{
	string s;
	getline(in, s);
}

void skipGrammar(istream &in)
{
	int i, numTerm, numNonTerm, numRules;

	// read the number of terminals and move to the next line
	in >> numTerm;
	skipLine(in);

	// skip the lines containing the terminals
	for (i = 0; i < numTerm; i++)
	{
		skipLine(in);
	}

	// read the number of non-terminals and move to the next line
	in >> numNonTerm;
	skipLine(in);

	// skip the lines containing the non-terminals
	for (i = 0; i < numNonTerm; i++)
	{
		skipLine(in);
	}

	// skip the line containing the start symbol
	skipLine(in);

	// read the number of rules and move to the next line
	in >> numRules;
	skipLine(in);

	// skip the lines containing the production rules
	for (i = 0; i < numRules; i++)
	{
		skipLine(in);
	}
}

string trim(const string &str)
{
	size_t begin = str.find_first_not_of(" \t\n");
	if (begin == string::npos)
		return "";

	size_t end = str.find_last_not_of(" \t\n");

	return str.substr(begin, end - begin + 1);
}

// Prints the derivation with whitespace trimmed.
void printDerivation(istream &in, vector<string> &inputs)
{
	string line;
	char ch;
	in >> ch;
	while (getline(in, line))
	{
		inputs.push_back(trim(line));
	}
}

vector<string>::iterator iter;
int outputRes(vector<string> &res)
{
	int r = 0;
	bool minus = false;
	while (iter != res.end())
	{
		if (*iter == "(")
		{
			++iter;
			if (minus)
				r -= outputRes(res);
			else
				r += outputRes(res);
		}
		else if (*iter == ")")
		{
			iter++;
			return r;
		}
		else if (*iter == "id")
		{
			if (minus)
				r -= 42;
			else
				r += 42;
			minus = false;
		}
		else if (*iter == "-")
		{
			minus = true;
		}
		iter++;
	}
	return r;
}
// Reads a .cfg file and prints the left-canonical
// derivation without leading or trailing spaces.
int main()
{
	vector<string> inputs;
	skipGrammar(cin);
	printDerivation(cin, inputs);
	vector<string> res;

	// dealing with the first line
	string firstInput = inputs[0];
	string currString = "";
	int i = 0;
	for (i = 0; i < firstInput.length(); i++)
	{
		char ch = firstInput[i];
		if (ch == ' ')
		{
			res.push_back(currString);
			currString.clear();
		}
		else
		{
			currString += ch;
		}
	}
	res.assign(++res.begin(), res.end());

	// the rest of inputs
	bool skip = true;
	for (string input : inputs)
	{
		if (skip)
		{
			skip = false;
			continue;
		}
		string currs;
		int i;
		for (i = 0; input[i] != ' '; i++)
		{
			currs += input[i];
		}
		for (vector<string>::iterator it = res.begin(); it != res.end(); it++)
		{
			if (*it == currs)
			{
				vector<string> newRes;
				newRes.assign(res.begin(), it);
				currs.clear();
				for (i++; i < input.length(); i++)
				{
					char ch = input[i];
					if (ch == ' ')
					{
						newRes.push_back(currs);
						currs.clear();
					}
					else
					{
						currs += ch;
					}
				}
				newRes.push_back(currs);
				while (++it != res.end())
				{
					newRes.push_back(*it);
				}
				res = newRes;
				break;
			}
		}
	}
	iter = res.begin();
	cout << outputRes(res) << endl;
	return 0;
}