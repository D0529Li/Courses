#include "tree.h"
#include "operator_node.h"
#include "number_node.h"

Tree::Tree(const std::vector<std::string>& vec)
{
	std::vector<std::shared_ptr<Node>> stack;
	for (auto s : vec) {
		std::shared_ptr<Node> newNode;
		if (s == "+" || s == "-" || s == "*" || s == "/") {
			if (stack.size() < 2) {
				throw invalidStackUsageException();
			}
			auto rightChild = stack[stack.size()-1];
			stack.erase(stack.end()-1);
			auto leftChild = stack[stack.size() - 1];
			stack.erase(stack.end() - 1);
			newNode = std::make_shared<OperatorNode>(s, leftChild, rightChild);
		}
		else {
			try {
				newNode = std::make_shared<NumberNode>(std::stoi(s));
			}
			catch (...) {
				throw invalidArgumentException();
			}
		}
		stack.push_back(newNode);
	}
	if (stack.size() != 1) {
		throw invalidStackOutputException();
	}
	root = stack[stack.size() - 1];
}

std::shared_ptr<Node> Tree::getRoot()
{
	return root;
}
