#include <iostream>
#include <string>
#include <cassert>
using namespace std;
#define nullptr NULL
// Definitions from class
struct BST_Node {
    string key;
    string stuff;
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

string BST_lookup (const BST& root, string key) {
    if (nullptr == root) {
	return "";
    } else if (root->key == key) {
	return root->stuff;
    } else if (root->key > key) {
	return BST_lookup (root->left, key);
    } else {
	return BST_lookup (root->right, key);
    }
}

void BST_insert (BST& root, string key, string stuff) {
    if (nullptr == root) {
	root = new BST_Node; 
	root->key = key; 
	root->stuff = stuff; 
	root->left = nullptr; 
	root->right = nullptr;
    } else if (key < root->key) {
	BST_insert(root->left, key, stuff);
    } else {
	BST_insert(root->right, key, stuff);
    } 
}

void BST_print (const BST& root) {
    if (nullptr != root) {
		BST_print (root->left);
		cout << root->key << endl;
		BST_print (root->right);
    }
}

// a4p2
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

int main(){
	BST root;
	BST_init(root);
	BST_insert(root, "1", "o");
	BST_insert(root, "2", "o");
	
	
	
	BST_remove(root, "1");BST_print(root);
	BST_remove(root, "2");
	
	cout << "end" << endl;
	BST_print(root);
	return 0;
}
