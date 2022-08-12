#ifndef _NODE_VALUE_VISITOR_H_
#define _NODE_VALUE_VISITOR_H_

#include "node_visitor.h"

class NodeValueVisitor : public NodeVisitor {
public:
	int value;
	virtual void visit(OperatorNode& node) override;
	virtual void visit(NumberNode& node) override;
};

#endif
