#ifndef _NODE_EXPRESSION_VISITOR_H_
#define _NODE_EXPRESSION_VISITOR_H_

#include <string>
#include "node_visitor.h"

class NodeExpressionVisitor : public NodeVisitor {
public:
	std::string expression;
	virtual void visit(OperatorNode& node) override;
	virtual void visit(NumberNode& node) override;
};

#endif
