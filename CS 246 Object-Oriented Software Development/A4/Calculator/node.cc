#include "node.h"

Node::Node() : left{ nullptr }, right{ nullptr } {}

Node::Node(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : left{ left }, right{ right } {}

std::shared_ptr<Node> Node::getLeftChild()
{
	return left;
}

std::shared_ptr<Node> Node::getRightChild()
{
	return right;
}

Node::~Node() {}
