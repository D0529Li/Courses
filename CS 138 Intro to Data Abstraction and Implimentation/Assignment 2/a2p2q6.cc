#include<iostream>

using namespace std;

struct Node{
	string val;
	Node* next;
};

Node* append(Node* p1, Node* p2){
	if(p1 == nullptr){
		return p2;
	}
	if(p2 == nullptr){
		return p1;
	}
	
	Node* tmp = p1;
	
	while(tmp->next != nullptr){
		tmp = tmp->next;
	}
	tmp->next = p2;
	return p1;
}
