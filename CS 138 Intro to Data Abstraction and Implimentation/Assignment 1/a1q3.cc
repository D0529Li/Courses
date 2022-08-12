#include<iostream>
#include<string>
#include<vector>

using namespace std;

int main()
{
	vector<string> s;
	string s1;
	string res = "";
	int n;
	
	cin >> n;
	if(n <= 0)
	{
		cerr << "Error, line length must be positive." << endl;
		exit(1);
	}
	
	//reading strings.
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
	
	//printing.
	const int sz = s.size();
	for(int i = 0; i < sz; i++)
	{
		cout << s.at(i) << endl;
	}

	return 0;
}
