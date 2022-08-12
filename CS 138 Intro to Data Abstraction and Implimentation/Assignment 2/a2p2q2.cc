#include<iostream>

using namespace std;

struct Node{
	string val;
	Node* next;
};

void printList(Node* p){
	while(p != nullptr){
		cout << p->val << endl;
		p = p->next;
	}
}
