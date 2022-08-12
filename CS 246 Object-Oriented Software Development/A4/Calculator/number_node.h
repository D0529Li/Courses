#ifndef _NUMBER_NODE_H
#define _NUMBER_NODE_H

#include "node.h"

class NumberNode : public Node {
	int number;
public:
	NumberNode(int number);
	int getNumber();
	virtual void accept(NodeVisitor& v) override;
};

#endif
