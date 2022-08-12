#ifndef _NODE_H_
#define _NODE_H_

#include <memory>
#include <string>

class NodeVisitor;
class invalidOperatorException {};

class Node {
	std::shared_ptr<Node> left;
	std::shared_ptr<Node> right;
public:
	Node();
	Node(std::shared_ptr<Node> left, std::shared_ptr<Node> right);
	std::shared_ptr<Node> getLeftChild();
	std::shared_ptr<Node> getRightChild();
	virtual void accept(NodeVisitor& v) = 0;
	virtual ~Node();
};

#endif
