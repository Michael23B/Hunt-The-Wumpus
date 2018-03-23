#include "WumpusGame.h"

int main() {
	Level lvl = Level();
	GameLoop(lvl);
	system("pause");	//stops console from closing on windows
}