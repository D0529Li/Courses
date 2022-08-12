#include <iostream>
#include <string>
#include <cassert>

using namespace std;

struct NodeChunk{
    string* val;
    NodeChunk* next;
};

struct Stack{
    int chunkSize;
    int topElt;
    NodeChunk* firstChunk;
};

NodeChunk* createNewNodeChunk (int chunkSize) {
    if(chunkSize <= 0)
    {
    	cerr << "Error, chunk size must be positive." << endl;
   		assert (false);
	}
	
	NodeChunk* newNode = new NodeChunk;
	newNode->val = new string[chunkSize];
	newNode->next = nullptr;
	return newNode;
}

void initStack (int chunkSize, Stack& s) {
    if(chunkSize <= 0)
    {
    	cerr << "Error, chunk size must be positive." << endl;
   		assert (false);
	}
	
	s.chunkSize = chunkSize;
	s.topElt = -1;
	s.firstChunk = nullptr;
	return; 
}

bool isEmpty (const Stack& s){
	return (nullptr == s.firstChunk);
}

int size (const Stack& s){
	if(isEmpty(s)){
		return 0;
	}
	
	NodeChunk* temp = s.firstChunk;
	
	int counter1 = 0;
	while(temp->val[counter1] != ""){
		counter1++;
	}
	
	int counter2 = 0;
	while(temp->next != nullptr){
		temp = temp->next;
		counter2++;
	}

	return s.chunkSize*counter2 + counter1;
}

void push (string val, Stack& s){
	if(isEmpty(s)){
		NodeChunk* newNode = createNewNodeChunk(s.chunkSize);
		newNode->val[0] = val;
		s.firstChunk = newNode;
		s.topElt = 0;
	}
	else{
		if(s.topElt == s.chunkSize - 1){
			NodeChunk* newNode = createNewNodeChunk(s.chunkSize);
			newNode->next = s.firstChunk;
			s.firstChunk = newNode;
			newNode->val[0] = val;
			s.topElt = 0;
		}
		else{
			s.firstChunk->val[++s.topElt] = val;
		}
	}
	return;
}

void pop (Stack& s){
	if(isEmpty(s))
    {
    	cerr << "Error, trying to pop an element from an empty stack." << endl;
   		assert (false);
	}

	s.firstChunk->val[s.topElt].clear();
	
	if(s.topElt == 0){
		NodeChunk* temp = s.firstChunk;
		
		if(size(s) == 0){
			s.firstChunk = nullptr;
			s.topElt = -1;
		}
		else{
			s.firstChunk = s.firstChunk->next;
			s.topElt = s.chunkSize-1;
		}
		
		delete[] temp->val;
		delete temp;
	}
	else{
		s.topElt--;
	}
	return;
}

string top (const Stack& s){
	if(isEmpty(s))
    {
    	cerr << "Error, trying to access an element from an empty stack." << endl;
   		assert (false);
	}
	return s.firstChunk->val[s.topElt];
}

void swap (Stack& s){
	if(size(s) < 2){
		cerr << "Error, trying to swap a list with fewer than 2 elements." << endl;
		assert(false);
	}
	
	int i = s.topElt;
	if(i == 0){
		int j = s.chunkSize-1;
		string t = s.firstChunk->next->val[j];
		s.firstChunk->next->val[j] = s.firstChunk->val[i];
		s.firstChunk->val[i] = t;
	}
	else{
		int j = i-1;
		string t = s.firstChunk->val[i];
		s.firstChunk->val[i] = s.firstChunk->val[j];
		s.firstChunk->val[j] = t;
	}
	return;
}
//
//int main(){
//	string s = "lyc";
//	Stack stk;
//	initStack(3, stk);
//	for (int i=0; i<3; i++){
//		push(s,stk);
//		s += "i";
//	}
//	
//	cout <<  "top=" << top(stk) << endl;
//	
//	pop(stk);
//	
//	cout << "pop1 finished; ready to top" << endl;
//	
//	cout <<  "top=" << top(stk) << endl;
//	
//	pop(stk);
//	
//	cout << "pop2 finished; ready to top" << endl;
//	
//	cout <<  "top=" << top(stk) << endl;
//	return 0;
//}
