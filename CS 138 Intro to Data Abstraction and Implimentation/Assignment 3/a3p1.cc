#include <iostream>
#include <string>
#include <cassert>
using namespace std;

struct Node {
	string val;
	Node* next;
	Node* prev;
};

struct Stew {
	Node* first;
	Node* last;
};

void initStew (Stew& q) {
    q.first = nullptr;
    q.last = nullptr;
}

bool isEmpty (const Stew& q) {
    return nullptr == q.first;
}

// a3p1q1

void addFront (Stew& q, string val) {
	Node* newstew = new Node;
	newstew->val = val;
	if(isEmpty(q)){
		q.first = newstew;
		q.last = newstew;
		newstew->next = nullptr;
		newstew->prev = nullptr;
	}
	else{
		newstew->next = q.first;
		q.first->prev = newstew;
		q.first = newstew;
	}
	return;
}

void leaveFront (Stew& q) {
	assert(!isEmpty(q));
	if(q.first == q.last){
		delete q.first;
		q.first = nullptr;
		q.last = nullptr;
	}
	else{
		Node* fst = q.first;
		q.first = fst->next;
		q.first->prev = nullptr;
		delete fst;
	}
	return;
}

string peekFront (const Stew& q) {
	assert(!isEmpty(q));
	return q.first->val;
}

// a3p1q2

void addBack (Stew& q, string val) {
	Node* newstew = new Node;
	newstew->val = val;
	if(isEmpty(q)){
		q.first = newstew;
		newstew->next = nullptr;
		newstew->prev = nullptr;
		q.last = newstew;
	}
	else{
		q.last->next = newstew;
		newstew->prev = q.last;
		q.last = newstew;
		newstew->next = nullptr;
	}
	return;
}

void leaveBack (Stew& q) {
	assert(!isEmpty(q));
	Node* lst = q.last;
	
	if(q.first == q.last){
		q.first = nullptr;
		q.last = nullptr;
	}
	else{
		q.last = lst->prev;
		q.last->next = nullptr;
	}
	delete lst;
	return;
}

string peekBack (const Stew& q) {
	assert(!isEmpty(q));
	return q.last->val;
}

// a3p1q3

void print (const Stew& q, char direction) { 
    if(direction == 'f'){
    	Node* tem = q.first;
    	while(nullptr != tem){
    		cout << tem->val << endl;
    		tem = tem->next;
		}
	}
	else if(direction == 'r'){
		Node* tem = q.last;
    	while(nullptr != tem){
    		cout << tem->val << endl;
    		tem = tem->prev;
		}
	}
	else{
		cerr << "Error, illegal direction: " << direction << endl;
	}
	return;
}

//a3p1q4

void nuke (Stew& q) {
	Node* tem = q.first;
	while(nullptr != tem){
		delete tem;
		tem = tem->next;
	}
	q.first = nullptr;
	q.last = nullptr;
	return;
}

int main (int argc, char* argv[]) {
    Stew s1;
    initStew (s1);
    addFront (s1, "alpha");
    addFront (s1, "beta");
    addFront (s1, "gamma");
    addBack (s1, "delta");
    cout << "This prints \"gamma beta alpha delta\" across four lines\n";
    print (s1, 'f');
    cout << "This prints \"delta alpha beta gamma\" across four lines\n";
    print (s1, 'r');
    leaveFront (s1);
    leaveBack (s1);
    cout << "This prints \"beta alpha\" in one line\n";
    cout << peekFront (s1) << " " << peekBack (s1) << endl;
    cout << "This nuke has no output, but is good form to call when done\n";
    nuke (s1);
    cout << "This assertion should succeed\n";
    assert (isEmpty (s1));
    cout << "Illegal direction should cause error mesg\n";
    print (s1, 'k');
}
