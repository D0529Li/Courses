#include "operator_node.h"
#include "node_visitor.h"

OperatorNode::OperatorNode(std::string s, std::shared_ptr<Node> left, std::shared_ptr<Node> right) : Node(left, right) {
	if (s.length() != 1 || (s != "+" && s != "-" && s != "*" && s != "/")) throw invalidOperatorException();
	oper = s;
}

std::string OperatorNode::getOperator()
{
	return oper;
}

void OperatorNode::accept(NodeVisitor& v)
{
	v.visit(*this);
}

