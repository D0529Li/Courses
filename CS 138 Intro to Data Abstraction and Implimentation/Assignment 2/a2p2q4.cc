#include<iostream>
#include<assert.h>

using namespace std;

struct Node{
	string val;
	Node* next;
};

Node* sortPair(Node* p1, Node* p2){
	assert(p1 != nullptr);
	assert(p2 != nullptr);
	string s1 = p1->val;
	string s2 = p2->val;
	
	if(s1 < s2){
		p1->next = p2;
		p2->next = nullptr;
		return p1;
	}
	else{
		p2->next = p1;
		p1->next = nullptr;
		return p2;
	}
}
