#include<iostream>
#include<string>

using namespace std;

int main()
{
	string s1;
	string res;
	int tot = 0; // number of strings in the current res;
	int n;
	
	cin >> n;
	if(n <= 0)
	{
		cerr << "Error, line length must be positive." << endl;
		exit(1);
	}
	
	res.clear();
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
		if(res.length() + s1.length() >= n) // this line is full; dealing with the WS issue, printing the current line and adding s1 to the next line.
		{
			if(res.length() != n) // dealing with the WS issue, if(res.length() == n), then we dont need to consider WS. 
			{
				if(tot == 1) // only 1 token in the current line; adding WS at the end.
				{
					res.insert(res.end(), n-res.length(), ' ');
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
			// printing and adding s1 to the next line. 
			cout << res << endl;
			res.clear();
			res.insert(0, s1);
			tot = 1;
		}
		else // enough space in the current line; adding s1 to it. 
		{
			if(res.length() != 0)
			{
				res.insert(res.length(), 1, ' ');
			}
			res.insert(res.length(), s1);
			tot++;
		}
	}
	
	if(res.length() != n) // editting and printing the last line. 
	{
		if(tot == 1)
		{
			res.insert(res.length(), n-res.length(), ' ');
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
	cout << res << endl;
	return 0;
}
