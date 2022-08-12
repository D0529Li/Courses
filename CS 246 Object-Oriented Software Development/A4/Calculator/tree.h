#ifndef _TREE_H_
#define _TREE_H_

#include <memory>
#include <vector>
#include <string>
#include "node.h"

// exceptions
class invalidArgumentException{};
class invalidStackOutputException{};
class invalidStackUsageException{};

class Tree {
	std::shared_ptr<Node> root;
public:
	Tree(const std::vector<std::string>& vec);
	std::shared_ptr<Node> getRoot();
};

#endif
