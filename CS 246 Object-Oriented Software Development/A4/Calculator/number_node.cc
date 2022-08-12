#include "number_node.h"
#include "node_visitor.h"

NumberNode::NumberNode(int number) : Node(), number{ number } {}

int NumberNode::getNumber()
{
	return number;
}

void NumberNode::accept(NodeVisitor& v)
{
	v.visit(*this);
}
