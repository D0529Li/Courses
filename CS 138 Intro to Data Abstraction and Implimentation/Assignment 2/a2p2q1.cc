#include<iostream>

using namespace std;

struct Node{
	string val;
	Node* next;
};

Node* makeList(){
	string s;
	Node* first = nullptr;
	Node* last = nullptr;
	while(cin >> s){
		Node* newNode = new Node;
		newNode->val = s;
		newNode->next = nullptr;
		if(first == nullptr){
			first = newNode;
			last = newNode;
		}
		else{
			last->next = newNode;
			last = newNode;
		}
	}
	return first;
}
