#include<iostream>

using namespace std;

struct Node{
	string val;
	Node* next;
};

Node* makePairList(string p1, string p2){
	Node* first = new Node;
	Node* last = new Node;
	
	if(p1 < p2){
		first->val = p1;
		last->val = p2;
		first->next = last;
		last->next = nullptr;
	}
	else{
		first->val = p2;
		last->val = p1;
		first->next = last;
		last->next = nullptr;
	}
	return first;
}
