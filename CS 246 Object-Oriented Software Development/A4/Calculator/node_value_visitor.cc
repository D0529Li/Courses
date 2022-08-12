#include "node_value_visitor.h"
#include "number_node.h"
#include "operator_node.h"

void NodeValueVisitor::visit(NumberNode& node)
{
	value = node.getNumber();
}

void NodeValueVisitor::visit(OperatorNode& node)
{
	NodeValueVisitor v1, v2;
	node.getLeftChild()->accept(v1);
	node.getRightChild()->accept(v2);
	if (node.getOperator() == "+") value = v1.value + v2.value;
	if (node.getOperator() == "-") value = v1.value - v2.value;
	if (node.getOperator() == "*") value = v1.value * v2.value;
	if (node.getOperator() == "/") value = v1.value / v2.value;
}
