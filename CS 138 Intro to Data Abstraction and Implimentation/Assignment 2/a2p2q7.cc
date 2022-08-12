#include<iostream>

using namespace std;

struct Node{
	string val;
	Node* next;
};

void printReverseRecursive(){
	string s = "";
	while(cin >> s){
		printReverseRecursive();
	}
	if(s != ""){
		cout << s << endl;
	}
}
