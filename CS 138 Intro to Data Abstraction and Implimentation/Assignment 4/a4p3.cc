#include <iostream>
#include <string>
#include <cassert>
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

void printQ(Queue& q){
	if(isEmptyQ(q)){
		return;
	}
	Qnode* tmp = q.first;
	while(tmp != nullptr){
		cout << tmp->val << endl;
		tmp = tmp->next;
	}
	return;
}

//BST operations
struct BST_Node {
    string key;
    BST_Node* left; 
    BST_Node* right;
};

typedef BST_Node* BST;

void BST_init (BST& root) {
    root = nullptr;
}

bool BST_isMT (const BST& root) {
    return nullptr == root;
}

bool BST_has (const BST& root, string key) {
    if (nullptr == root) {
		return false;
    } else if (root->key == key) {
		return true;
    } else if (root->key > key) {
		return BST_has (root->left, key);
    } else {
		return BST_has (root->right, key);
    }
}

void BST_insert (BST& root, string key) {
    if (nullptr == root) {
		root = new BST_Node; 
		root->key = key;  
		root->left = nullptr; 
		root->right = nullptr;
    }
	else if (key < root->key) {
		BST_insert(root->left, key);
    }
	else {
		BST_insert(root->right, key);
    } 
}

void BST_print (const BST& root) {
    if (nullptr != root) {
		BST_print (root->left);
		cout << root->key << endl;
		BST_print (root->right);
    }
}

BST BST_find (const BST& root, string key) { // one of the helper functions for BST_remove(...);
    if (nullptr == root) {
		return nullptr;
    }
	else if (root->key == key) {
		return root;
    }
	else if (root->key > key) {
		return BST_find (root->left, key);
    }
	else {
		return BST_find (root->right, key);
    }
}

BST BST_find_parent (const BST& root, string key) { // another helper function for BST_remove(...);
	BST parent = nullptr;
	BST cur = root;
    if (nullptr == root || key == root->key) {
		return nullptr;
    }
    
    while(cur != nullptr){
    	if(cur->key < key){
    		parent = cur;
    		cur = cur->right;
		}
		else if(cur->key > key){
			parent = cur;
    		cur = cur->left;
		}
		else{
			return parent;
		}
	}
	return nullptr;
}

BST BST_remove_helper1(BST& root){
	if(nullptr == root){
		return nullptr; // no result;
	}
	
	if(nullptr == root->right){
		return root;
	}
	else{
		return BST_remove_helper1(root->right);
	}
}

BST BST_remove_helper2(BST& root){
	if(nullptr == root){
		return nullptr; // no result;
	}
	
	if(nullptr == root->left){
		return root;
	}
	else{
		return BST_remove_helper2(root->left);
	}
}

void BST_remove (BST& root, string key){
	BST position = BST_find(root, key);
	if(nullptr == position){
		cerr << "Error, couldn't find \"" << key << "\" in the BST\n"; 
    	assert(false);
	}
	BST parent = BST_find_parent(root, key);
	if(nullptr == position->left && nullptr == position->right){
		//case 1: removing a leaf.
		if(root == position){
		    root = nullptr;
		}
		else{
		    if(parent->left == position){
    			parent->left = nullptr;
    		}
    		else{
    			parent->right = nullptr;
    		}
		}
		delete position;
	}
	else if((nullptr == position->left && nullptr != position->right) || (nullptr != position->left && nullptr == position->right))
	{
		//case 2: removing a node with only one child.
		if(parent == nullptr){
			if(position->left != nullptr){
				root = position->left;
			}
			else{
				root = position->right;
			}
		}
		else{
			if(parent->left == position){
				if(position->left != nullptr){
					parent->left = position->left;
				}
				else{
					parent->left = position->right;
				}
			}
			else{
				if(position->left != nullptr){
					parent->right = position->left;
				}
				else{
					parent->right = position->right;
				}
			}
		}
		delete position;
	}
	else{
		//case 3: removing a node with two children.
		if(nullptr != position->left){
			//find replacement node from LHS;
			BST replace = BST_remove_helper1(position->left);
			BST replace_parent = BST_find_parent(root, replace->key);
			if(replace->left != nullptr){
				if(replace_parent->left == replace){
					replace_parent->left = replace->left;
				}
				else{
					replace_parent->right = replace->left;
				}
			}
			else{
				if(replace_parent->left == replace){
					replace_parent->left = nullptr;
				}
				else{
					replace_parent->right = nullptr;
				}
			}
			if(position->left != nullptr){
				replace->left = position->left;
			}
			if(position->right != nullptr){
				replace->right = position->right;
			}
			if(position == root){
				root = replace;
			}
			else{
				if(parent->left == position){
					parent->left = replace;
				}
				else{
					parent->right = replace;
				}
			}
			delete position;
		}
		else if(nullptr != position->right){
			//go instead RHS;
			BST replace = BST_remove_helper2(position->right);
			BST replace_parent = BST_find_parent(root, replace->key);
			if(replace->right != nullptr){
				if(replace_parent->left == replace){
					replace_parent->left = replace->right;
				}
				else{
					replace_parent->right = replace->right;
				}
			}
			else{
				if(replace_parent->left == replace){
					replace_parent->left = nullptr;
				}
				else{
					replace_parent->right = nullptr;
				}
			}
			if(position->left != nullptr){
				replace->left = position->left;
			}
			if(position->right != nullptr){
				replace->right = position->right;
			}
			if(position == root){
				root = replace;
			}
			else{
				if(parent->left == position){
					parent->left = replace;
				}
				else{
					parent->right = replace;
				}
			}
			delete position;
		}
	}
	return;
}

//SBL operations

struct SBLnode {
	Queue q;
	BST bst;
};

typedef SBLnode* SBL;

void SBL_init (SBL& sbl){
	sbl = new SBLnode;
	return;
}

int SBL_size (const SBL& sbl){
	return sizeQ(sbl->q);
}
	
void SBL_arrive (SBL& sbl, string name){
	BST_insert(sbl->bst, name);
	enterQ(sbl->q, name);
	return;
}

void SBL_leave (SBL& sbl){
	if(BST_isMT(sbl->bst) || isEmptyQ(sbl->q)){
		cerr << "Error, SBL is empty." << endl;
		assert(false);
	}
    BST_remove(sbl->bst, ((sbl->q).first)->val);
    leaveQ(sbl->q);
    return;
}

string SBL_first (const SBL& sbl) { 
    if(BST_isMT(sbl->bst) || isEmptyQ(sbl->q)){
		cerr << "Error, SBL is empty." << endl;
		assert(false);
	}
	return sbl->q.first->val;	
}

bool SBL_lookup (const SBL& sbl, string name){
	return BST_has(sbl->bst, name);
}

void SBL_printInArrivalOrder (const SBL& sbl){
	printQ(sbl->q);
	return;
}

void SBL_printInLexicographicalOrder (const SBL& sbl){
	BST_print(sbl->bst);
	return;
}

//int main(){
//	SBL sbl;
//	SBL_init(sbl);
//	cout << sbl << endl;
//	SBL_arrive(sbl, "john");cout << "arrive finished" << endl; 
//	SBL_leave(sbl);cout << "leave finished" << endl; 
//	SBL_arrive(sbl, "john");cout << "arrive finished" << endl; 
//	SBL_leave(sbl);cout << "leave finished" << endl; 
//	return 0;
//}
