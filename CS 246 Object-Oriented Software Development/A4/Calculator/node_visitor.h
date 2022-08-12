#ifndef _NODE_VISITOR_H_
#define _NODE_VISITOR_H_

class OperatorNode;
class NumberNode;

class NodeVisitor {
public:
	virtual void visit(OperatorNode& node) = 0;
	virtual void visit(NumberNode& node) = 0;
	virtual ~NodeVisitor();
};

#endif
