#include <iostream>
#include "player.h"
#include "hydra_basic.h"

using namespace std;

int main(int argc, char** argv) {
	bool testingMode = false;
	for (int i = 1; i < argc; i++) {
		if (static_cast<string>(argv[i]) == "-testing") {
			testingMode = true;
		}
	}
	while (true) {
		try {
			auto game = make_shared<HydraBasic>(testingMode);
			game->play();
		}
		catch (QuitGameException) {}
		cout << endl << "Your previous game has finished. Type \"q\" to quit the program or anything else to start the next game. " << endl;
		string s;
		getline(cin, s);
		if (s == "q") break;
	}
	return 0;
}
