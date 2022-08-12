#ifndef _OPERATOR_NODE_H_
#define _OPERATOR_NODE_H_

#include <string>
#include "node.h"

class OperatorNode : public Node {
	std::string oper;
public:
	OperatorNode(std::string s, std::shared_ptr<Node> left, std::shared_ptr<Node> right);
	std::string getOperator();
	virtual void accept(NodeVisitor& v) override;
};

#endif
