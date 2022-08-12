#include<iostream>
#include<string>
#include<cassert>
using namespace std;

//Queue declarations.
struct Qnode{
	string val;
	Qnode* next;
};

struct Queue{
	Qnode* first;
	Qnode* last;
};

//Queue operations.
void initQ (Queue& q){
	q.first = nullptr;
	q.last = nullptr;
}

bool isEmptyQ (const Queue& q){
	return nullptr == q.first;
}

int sizeQ (const Queue& q){
	int i=0;
	Qnode* cur = q.first;
	
	while(nullptr != cur){
		cur = cur->next;
		++i;
	}
	return i;
}

void enterQ (Queue& q, string val){
	Qnode* newQNode = new Qnode;
	newQNode->val = val;
	newQNode->next = nullptr;
	if(isEmptyQ(q)){
		q.first = newQNode;
		q.last = newQNode;
	}
	else{
		q.last->next = newQNode;
		q.last = newQNode;
	}
	return;
}

string firstQ (const Queue& q) { 
    if(isEmptyQ(q)){
    	cerr << "Error, queue is empty." << endl;
    	assert(false);
	}
	return q.first->val;
}

void leaveQ (Queue& q) {
    if(isEmptyQ(q)){
    	cerr << "Error, queue is empty." << endl;
    	assert(false);
	}
	Qnode* tmp = q.first;
	q.first = q.first->next;
	delete tmp;
	return;
}

//Priority queue declarations.
struct PQnode {
	int priority;
    Queue q;
	PQnode* next;
};

typedef PQnode* PQ; 

//Priority queue operations.
void initPQ (PQ& pq) {
    pq = nullptr;
}

bool isEmptyPQ (const PQ& pq){
	return nullptr == pq;
}

void enterPQ (PQ& pq, string val, int priority){
	PQ cur = pq;
	if(isEmptyPQ(pq)){
		//add the node to an empty pq
		PQ newPQ = new PQnode;
		newPQ->priority = priority;
		newPQ->next = nullptr;
		pq = newPQ;
		enterQ(newPQ->q, val);
	}
	else if(priority == cur->priority){
		//add the node to the first queue.
		enterQ(cur->q, val); 
	}
	else if(priority < cur->priority){
		//add the node to a new queue before the first queue.
		PQ newPQ = new PQnode;
		newPQ->priority = priority;
		newPQ->next = pq;
		pq = newPQ;
		enterQ(newPQ->q, val);
	}
	else{
		// add the node to somewhere after the first node.
		while(nullptr != cur->next && priority > cur->next->priority){
			cur = cur->next;
		}
		
		if(nullptr != cur->next){
			// add the node to somewhere before the last node or to the last node.
			if(priority == cur->next->priority){
				// add the node to an existing queue.
				enterQ(cur->next->q, val);
			}
			else{
				// add the node to a new queue.
				PQ newPQ = new PQnode;
				newPQ->priority = priority;
				newPQ->next = cur->next;
				cur->next = newPQ;
				enterQ(newPQ->q, val);
			}
		}
		else{
			// i.e. nullptr == cur->next ; i.e. cur == "the last pq".
			// add the node to a new queue after the last node.
			PQ newPQ = new PQnode;
			newPQ->priority = priority;
			newPQ->next = nullptr;
			cur->next = newPQ;
			enterQ(newPQ->q, val);
		}
	}
	return;
}

string firstPQ (const PQ& pq) {
	if(isEmptyPQ(pq)){
		cerr << "Error, priority queue is empty." << endl;
		assert(false);
	}
	return pq->q.first->val;
}

void leavePQ (PQ& pq) {
    if(isEmptyPQ(pq)){
		cerr << "Error, priority queue is empty." << endl;
		assert(false);
	}
	leaveQ(pq->q);
	if(isEmptyQ(pq->q)){
		PQ tmp = pq;
		pq = pq->next;
		delete tmp;
	}
	return;
}

int sizePQ (const PQ& pq){
	PQ cur = pq;
	int i = 0;
	while(nullptr != cur){
		i += sizeQ(cur->q);
		cur = cur->next;
	}
	return i;
}

int sizeByPriority (const PQ& pq, int priority){
	PQ cur = pq;
	while(nullptr != cur){
		if(priority == cur->priority){
			return sizeQ(cur->q);
		}
		cur = cur->next;
	}
	return 0;
}

int numPriorities (const PQ& pq){
	if(isEmptyPQ(pq)){
		return 0;
	}
	
	PQ cur = pq;
	int i=0;
	while(nullptr != cur){
		++i;
		cur = cur->next;
	}
	return i;
}

//printPQ(const PQ& pq) just for debugging
//void printPQ(const PQ& pq){
//	PQ tmp = pq;
//	while(nullptr != tmp){
//		cout << "priority = " << tmp->priority << ":  ";
//		Qnode* qtmp = tmp->q.first;
//		while(qtmp != tmp->q.last){
//			cout << qtmp->val << "  ";
//			qtmp = qtmp->next;
//		}
//		cout << qtmp->val << endl;
//		tmp = tmp->next;
//	}
//	return;
//}

//int main(){
//	PQ pq;
//	string s="hi";
//	initPQ(pq);
//	enterPQ(pq, s+"1", 3); cout << "success enter 1" << endl;
//	enterPQ(pq, s+"2", 1); cout << "success enter 2" << endl;
//	enterPQ(pq, s+"3", 3); cout << "success enter 3" << endl;
//	enterPQ(pq, s+"4", 2); cout << "success enter 4" << endl;
//	
//	cout << "printing the resulting pq: " << endl;
//	printPQ(pq);
//	
//	cout << "sizePQ(pq) = " << sizePQ(pq) << endl;
//	cout << "firstPQ(pq) = " << firstPQ(pq) << endl;
//	leavePQ(pq); cout << "success leave 1" << endl;
//	cout << "firstPQ(pq) = " << firstPQ(pq) << endl;
//	leavePQ(pq); cout << "success leave 2" << endl;
//	cout << "firstPQ(pq) = " << firstPQ(pq) << endl;
//	leavePQ(pq); cout << "success leave 3" << endl;
//	cout << "firstPQ(pq) = " << firstPQ(pq) << endl;
//	leavePQ(pq); cout << "success leave 4" << endl;
//	cout << "isEmptyPQ(pq) = " << (int)isEmptyPQ(pq) << endl;
//	cout << "sizePQ(pq) = " << sizePQ(pq) << endl;
//	return 0;
//}
