#include<iostream>
#include<cassert>
#include<string>

using namespace std;

struct Node{
	string val;
	Node* next;
};

void printPairInOrder(Node* p1, Node* p2){
	assert(p1 != nullptr);
	assert(p2 != nullptr);
	string s1 = p1->val;
	string s2 = p2->val;
	if(s1 < s2){
		cout << s1 << endl << s2 << endl;
	}
	else{
		cout << s2 << endl << s1 << endl;
	}
	return;
}
