#include "node_expression_visitor.h"
#include "number_node.h"
#include "operator_node.h"

void NodeExpressionVisitor::visit(NumberNode& node)
{
	expression = std::to_string(node.getNumber());
}

void NodeExpressionVisitor::visit(OperatorNode& node)
{
	NodeExpressionVisitor v1, v2;
	node.getLeftChild()->accept(v1);
	node.getRightChild()->accept(v2);
	expression = "(" + v1.expression + node.getOperator() + v2.expression + ")";
}
