#include <iostream>
#include <string>
#include <vector>

#include "tree.h"
#include "node_value_visitor.h"
#include "node_expression_visitor.h"

using namespace std;

vector<string> splitInput(const string& s) {
	vector<string> res;
	string curr;
	for (char c : s) {
		if (c != ' ') {
			curr += c;
		}
		else {
			if (curr != "") res.push_back(curr);
			curr.clear();
		}
	}
	if (curr != "") res.push_back(curr);
	return res;
}

int main() {
	string s;
	vector<string> input;
	while (getline(cin, s))
	{
		input = splitInput(s);
		try {
			auto tree = make_shared<Tree>(input);
			NodeExpressionVisitor expressionVisitor;
			NodeValueVisitor valueVisitor;
			tree->getRoot()->accept(expressionVisitor);
			tree->getRoot()->accept(valueVisitor);
			cout << expressionVisitor.expression << " = " << valueVisitor.value << endl;
		}
		catch (invalidArgumentException) {
			cerr << "Invalid argument." << endl;
		}
		catch (invalidOperatorException) {
			cerr << "Invalid operator." << endl;
		}
		catch (invalidStackUsageException) {
			cerr << "Invalid stack usage. You are trying to use an operation with only one element on the stack." << endl;
		}
		catch (invalidStackOutputException) {
			cerr << "Invalid stack output. The expression does not end with exactly one element on the stack." << endl;
		}
	}
	return 0;
}
