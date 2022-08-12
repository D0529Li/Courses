#include<iostream>
#include<string>

using namespace std;

int main()
{
	string s1;
	int n;
	
	cin >> n;
	if(n <= 0)
	{
		cerr << "Error, line length must be positive." << endl;
		exit(1);
	}
	
	getline(cin, s1);
	
	while(getline(cin, s1))
	{
		if(s1.length() > n)
		{
			for(int i=0; i<n; i++)
			{
				cout << s1[i];
			}
			cout << endl;
		}
		else
		{
			cout << s1 << endl;
		}
	}

	return 0;
}
