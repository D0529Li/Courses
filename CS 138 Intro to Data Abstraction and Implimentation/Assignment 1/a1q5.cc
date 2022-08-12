#include<iostream>
#include<string>
#include<vector>

using namespace std;

int main()
{
	string s1;
	string c1;
	string c2; 
	string res;
	const string dest = "fnord";
	vector<string> s;
	int tot = 0; // number of strings in the current res;
	int n;
	
	cin >> n >> c1 >> c2;
	if(n <= 0)
	{
		cerr << "Error, line length must be positive." << endl;
		exit(1);
	}
	
	//reading strings.
	if(c1 == "rr") // do as Q3.
	{
		while(cin >> s1)
		{
			if(s1.length() > n) // pre-processing long strings.
			{
				s1 = s1.substr(0, n);
			}
			
			// inserting s1.
			if(res.length() == 0)
			{
				res.insert(res.length(), s1);
			}
			else if(res.length() + s1.length() >= n)
			{
				s.push_back(res);
				res.clear();
				res.insert(0, s1);
			}
			else
			{
				res.insert(res.length(), 1, ' ');
				res.insert(res.length(), s1);
			}
		}
		
		s.push_back(res);
		res.clear();
	}
	else if(c1 == "j") // do as Q4. 
	{
		while(cin >> s1)
		{
			if(s1.length() > n) // checking and editing long strings.
			{
				s1 = s1.substr(0, n);
			}
			
			if(res.length() == 0)
			{
				res.insert(0, s1);
				tot++;
			}
			else
			if(res.length() + s1.length() >= n) // this line is full; dealing with the WS issue,
												// push_back-ing the current line and adding s1 to the next line.
			{
				if(res.length() != n) 	// dealing with the WS issue,
										// if(res.length() == n), then we dont need to consider WS. 
				{
					if(tot == 1) // only 1 token in the current line; adding WS at the end.
					{
						while(res.length() != n)
						{
							res.insert(res.length(), 1, ' ');
						}
					}
					else // more than 1 token in the current line.
					{
						int ws = (n - res.length()) / (tot-1);
						int counter = (n - res.length()) % (tot-1);
						int pos = res.find(' ', 0);
						while(-1 != pos)
						{
							if(counter > 0)
							{
								counter--;
								res.insert(pos, ws+1, ' ');
								pos += ws+2;
							}
							else
							{
								res.insert(pos, ws, ' ');
								pos += ws+1;
							}
							pos = res.find(' ', pos);
						}
					}
				}
				// push_back-ing and adding s1 to the next line. 
				s.push_back(res);
				res.clear();
				res.insert(0, s1);
				tot = 1;
			}
			else // sufficient space in the current line; adding s1 to it. 
			{
				if(res.length() != 0)
				{
					res.insert(res.length(), 1, ' ');
				}
				res.insert(res.length(), s1);
				tot++;
			}
		}
		
		if(res.length() != n) // editting and push_back-ing the last line. 
		{
			if(tot == 1)
			{
				while(res.length() != n)
				{
					res.insert(res.length(), 1, ' ');
				}
			}
			else
			{
				int ws = (n - res.length()) / (tot-1);
				int counter = (n - res.length()) % (tot-1);
				int pos = res.find(' ', 0);
				while(-1 != pos)
				{
					if(counter > 0)
					{
						counter--;
						res.insert(pos, ws+1, ' ');
						pos += ws+2;
					}
					else
					{
						res.insert(pos, ws, ' ');
						pos += ws+1;
					}
					pos = res.find(' ', pos);
				}
			}
		}
		s.push_back(res);
		res.clear();
	}
	else if(c1 == "rl") // ragged left
	{
		while(cin >> s1)
		{
			if(s1.length() > n)
			{
				s1 = s1.substr(0, n);
			}
			
			if(res.length() == 0)
			{
				res.insert(0, s1);
			}
			else if(res.length() + s1.length() >= n)
			{
				while(res.length() < n)
				{
					res.insert(0, 1, ' ');
				}
				s.push_back(res);
				res.clear();
				res.insert(0, s1);
			}
			else
			{
				res.insert(res.length(), 1, ' ');
				res.insert(res.length(), s1);
			}
		}
		
		while(res.length() < n) //dealing with the last string.
		{
			res.insert(0, 1, ' ');
		}
		s.push_back(res);
		res.clear();
	}
	else if(c1 == "c") // center the lines
	{
		while(cin >> s1)
		{
			if(s1.length() > n)
			{
				s1 = s1.substr(0, n);
			}
			
			if(res.length() == 0)
			{
				res.insert(res.length(), s1);
			}
			else if(res.length() + s1.length() >= n)
			{
				bool switcher = true;
				while(res.length() < n)
				{
					if(switcher)
					{
						res.insert(0, 1, ' ');
						switcher = false;
					}
					else
					{
						res.insert(res.length(), 1, ' ');
						switcher = true;
					}
				}
				s.push_back(res);
				res.clear();
				res.insert(0, s1);
			}
			else
			{
				res.insert(res.length(), 1, ' ');
				res.insert(res.length(), s1);
			}
		}
		
		//dealing with the last string.
		bool switcher = true;
		while(res.length() < n)
		{
			if(switcher)
			{
				res.insert(0, 1, ' ');
				switcher = false;
			}
			else
			{
				res.insert(res.length(), 1, ' ');
				switcher = true;
			}
		}
		s.push_back(res);
		res.clear();
	}
	else // error. 
	{
		cerr << "Error, command is illegal." << endl;
		exit(1);
	}
	
	//printing.
	if(c2 == "f") // normal printing. 
	{
		const int sz = s.size();
		for(int i = 0; i < sz; i++)
		{
			cout << s.at(i) << endl;
		}

	}
	else if(c2 == "r") // reverse printing.
	{
		while(!s.empty())
		{
			cout << s.back() << endl;
			s.pop_back();
		}
	}
	else if(c2 == "g") // "fnord" only.
	{
		const int sz = s.size();
		for(int j = 0; j < sz; j++)
		{
			if(s[j].find(dest, 0) != -1)
			{
				cout << s.at(j) << endl;
			}
		}
	}
	else // error. 
	{
		cerr << "Error, command is illegal." << endl;
		exit(1);
	}
	return 0;
}
