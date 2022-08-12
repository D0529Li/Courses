#include<iostream>
#include<string>
#include<vector>

using namespace std;

int main()
{
	vector<string> s;
	int n;
	char m;
	string s1;
	const string dest = "fnord";
	
	cin >> n;
	if(n <= 0) // read in n.
	{
		cerr << "Error, line length must be positive." << endl;
		exit(1);
	}
	
	cin >> m; //read in the command.
	
	getline(cin, s1); //skip to the new line.
	
	while(getline(cin, s1)) //read in strings.
	{
		s.push_back(s1);
	}
	
	if(m == 'f')
	{
		for(int j=0; j < s.size(); j++)
		{
			if((s.at(j)).length() >= n)
			{
				cout << (s.at(j)).substr(0, n) << endl;
			}
			else
			{
				cout << s.at(j) << endl;
			}
		}
	}
	else if(m == 'r')
	{
		int sz = s.size();
		for(int i=0; i < sz; i++)
		{
			if((s.back()).length() >= n)
			{
				cout << (s.back()).substr(0, n) << endl;
			}
			else
			{
				cout << s.back() << endl;
			}
			s.pop_back();
		}
	}
	else if(m == 'g')
	{
		for(int j=0; j < s.size(); j++)
		{
			if(s[j].find(dest, 0) != -1)
			{
				if((s.at(j)).length() >= n)
				{
					cout << (s.at(j)).substr(0, n) << endl;
				}
				else
				{
					cout << s.at(j) << endl;
				}
			}
		}
	}
	else
	{
		cerr << "Error, command is illegal." << endl;
		exit(1);
	}
	return 0;
}
