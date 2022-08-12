#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

void constructing(vector<string> ori, vector<string> &results, const string mode, const int n)
{
	string res;
	res.clear();
	
	if(mode == "rr")
	{
		for(int i=0; i<ori.size(); i++)
		{
			if(res.length() == 0)
			{
				res.insert(res.length(), ori.at(i));
			}
			else if(res.length() + ori.at(i).length() >= n)
			{
				results.push_back(res);
				res.clear();
				res.insert(0, ori.at(i));
			}
			else
			{
				res.insert(res.length(), 1, ' ');
				res.insert(res.length(), ori.at(i));
			}
		}
		
		results.push_back(res);
	}
	else if(mode == "rl")
	{
		for(int i=0; i<ori.size(); i++)
		{			
			if(res.length() == 0)
			{
				res.insert(0, ori.at(i));
			}
			else if(res.length() + ori.at(i).length() >= n)
			{
				while(res.length() < n)
				{
					res.insert(0, 1, ' ');
				}
				results.push_back(res);
				res.clear();
				res.insert(0, ori.at(i));
			}
			else
			{
				res.insert(res.length(), 1, ' ');
				res.insert(res.length(), ori.at(i));
			}
		}
		
		while(res.length() < n)
		{
			res.insert(0, 1, ' ');
		}
		results.push_back(res);
	}
	else if(mode == "j") 
	{
		int tot = 0;
		
		for(int i=0; i<ori.size(); i++)
		{			
			if(res.length() == 0)
			{
				res.insert(0, ori.at(i));
				tot++;
			}
			else
			if(res.length() + ori.at(i).length() >= n) // this line is full; dealing with the WS issue,
												// push_back-ing the current line and adding s1 to the next line.
			{
				if(res.length() != n)	// dealing with the WS issue,
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
				results.push_back(res);
				res.clear();
				res.insert(0, ori.at(i));
				tot = 1;
			}
			else // sufficient space in the current line; adding s1 to it. 
			{
				if(res.length() != 0)
				{
					res.insert(res.length(), 1, ' ');
				}
				res.insert(res.length(), ori.at(i));
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
		results.push_back(res);
	}
	else if(mode == "c")
	{
		for(int i=0; i<ori.size(); i++)
		{			
			if(res.length() == 0)
			{
				res.insert(res.length(), ori.at(i));
			}
			else if(res.length() + ori.at(i).length() >= n)
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
				results.push_back(res);
				res.clear();
				res.insert(0, ori.at(i));
			}
			else
			{
				res.insert(res.length(), 1, ' ');
				res.insert(res.length(), ori.at(i));
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
		results.push_back(res);
	}
}
int main()
{
	string s1;
	vector<string> s;
	
	string cmd;
	string justn = "rr";
	string direction = "f";
	
	string fileName;
	
	int tot = 0; // number of strings in the current res;
	int n;
	
	cin >> n >> fileName;
	if(n < 1)
	{
		cerr << "Error, line length must be positive." << endl;
		exit(1);
	}
	
	ifstream myin;
	myin.open(fileName.c_str());
	if(!myin)
	{
		cerr << "Error, cannot open specified text file." << endl;
		exit(1); 
	}
	
	while(myin >> s1) //reading strings.
	{
		if(s1.length() > n) // pre-processing long strings.
		{
			s1 = s1.substr(0, n);
		}
		s.push_back(s1);
	}
	myin.close();
	
	while(cin >> cmd)//reading commands.
	{
		if(cmd == "rr" || cmd == "rl" || cmd == "c" || cmd == "j") //switching the justification mode.
		{
			justn = cmd;
		}
		else if(cmd == "f" || cmd == "r") //switching the print direction.
		{
			direction = cmd;
		}
		else if(cmd == "p" || cmd == "k" || cmd == "s") //printing.
		{
			//constructing the resultant strings based on the current justification mode.
			vector<string> toPrint;
			constructing(s, toPrint, justn, n);
			if(cmd == "p")
			{
				if(direction == "f")
				{
					for(int i=0; i<toPrint.size(); i++)
					{
						cout << s.at(i) << endl;
					}
				}
				else
				{
					for(int i=toPrint.size()-1; i>=0; i--)
					{
						cout << s.at(i) << endl;
					}
				}
			}
			else if(cmd == "k")
			{
				int k;
				cin >> k;
				cout << toPrint.at(k) << endl;
			}
			else if(cmd == "s")
			{
				string dest;
				cin >> dest;
				if(direction == "f")
				{
					for(int j=0; j<toPrint.size(); j++)
					{
						if(toPrint.at(j).find(dest, 0) != -1)
						{
							cout << toPrint.at(j) << endl;
						}
					}
				}
				else
				{
					for(int j=toPrint.size()-1; j>=0; j--)
					{
						if(toPrint.at(j).find(dest, 0) != -1)
						{
							cout << toPrint.at(j) << endl;
						}
					}
				}
			}
		}
		else if(cmd == "q") //quitting
		{
			exit(0);
		}
		else // error. 
		{
			cerr << "Error, command is illegal." << endl;
			exit(1);
		}
	}
	
	return 0;
}
