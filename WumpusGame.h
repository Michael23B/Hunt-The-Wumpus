#ifndef WUMPUSGAME_H_INCLUDED
#define WUMPUSGAME_H_INCLUDED

#include <string>
#include <time.h>
#include <iostream>
#include <vector>
#include <random>

enum hazard { Empty, Wumpus, Bat, Pit };

class Room {
public:
	Room()
		:ID(0), roomHazard(hazard::Empty) {};
	Room(int _ID, hazard _hazard, std::vector<int> _pathways)
		:ID(_ID), roomHazard(_hazard), paths(_pathways) {};

	int getID() { return ID; };
	//private:
	int ID;
	hazard roomHazard;
	std::vector<int> paths;
};

class Level {
public:
	Level();
	void PrintRoomIDs();
	//private:
	Room rooms[20];

	std::vector<int> defaultRoomPaths = { { 5,8,2,1,10,3,2,12,4,3,14,5,4,6,1,5,7,15,6,17,8,7,1,9,8,18,10,9,2,11,10,12,19,3,13,11,20,14,12,15,13,4,6,14,16,15,17,20,7,18,16,17,9,19,18,11,20,16,19,13 } };
};

Level::Level() {
	srand(time(nullptr));
	for (int i = 0; i < 20; ++i) {
		std::vector<int> paths;
		for (int j = 0; j < 3; ++j) {
			paths.push_back(defaultRoomPaths[i * 3 + j]);
		}
		rooms[i] = Room(i + 1, hazard::Empty, paths);
	}
	//Generate random rooms to place the hazards
	std::vector<int> randValues;
	bool repeatFlag = false;
	while (randValues.size() < 4) {
		int randInt = std::rand() % 20;

		for (int i : randValues) if (i == randInt || randInt == 0) repeatFlag = true;

		if (!repeatFlag) randValues.push_back(randInt);
		else repeatFlag = false;
	}
	rooms[randValues[0]].roomHazard = hazard::Wumpus;
	rooms[randValues[1]].roomHazard = hazard::Bat;
	rooms[randValues[2]].roomHazard = hazard::Bat;
	rooms[randValues[3]].roomHazard = hazard::Pit;
};

void Level::PrintRoomIDs() {
	for (int i = 0; i < 20; ++i) {
		std::cout << rooms[i].getID() << ": " << rooms[i].paths[0] << rooms[i].paths[1] << rooms[i].paths[2] << '\n';
	}
}

void CheckRoom(Level& lvl, int& roomNo, bool& batFlag, bool& gameOverFlag) {
	Room room = lvl.rooms[roomNo - 1];
	if (room.roomHazard == Wumpus) {
		std::cout << "The Wumpus ate you! Game over.\n";
		gameOverFlag = true;
		return;
	}
	if (room.roomHazard == Bat) {
		std::cout << "A bat carries you away!\n";
		batFlag = true;
		roomNo = (std::rand() % 20) + 1;
		return;
	}
	if (room.roomHazard == Pit) {
		std::cout << "You fall in a pit and die! Game over.\n";
		gameOverFlag = true;
	}

	std::cout << "Current room: " << roomNo << " Current paths: " << room.paths[0] << ", " << room.paths[1] << ", " << room.paths[2] << '\n';

	for (int i = 0; i < room.paths.size(); ++i) {
		if (lvl.rooms[room.paths[i] - 1].roomHazard == Wumpus) std::cout << "You smell the Wumpus nearby!\n";
		if (lvl.rooms[room.paths[i] - 1].roomHazard == Bat) std::cout << "You hear flapping wings.\n";
		if (lvl.rooms[room.paths[i] - 1].roomHazard == Pit) std::cout << "You hear rushing wind.\n";
	}
	std::cout << '>';
}

void GameLoop(Level& lvl) {
	int arrows = 1;
	int roomNo = 1;
	char command;
	int cmdInt = 0;

	bool batFlag = false;
	bool arrowFlag = false;
	bool gameOverFlag = false;

	std::cout << "Welcome to Hunt the Wumpus!\nEnter (m) and a number to move to that room\nEnter (a) and a number to shoot into that room\n\n";

	while (true) {
		if (!arrowFlag && !gameOverFlag) CheckRoom(lvl, roomNo, batFlag, gameOverFlag);
		if (gameOverFlag) return;

		if (batFlag) {      //if theres a bat, the room has changed and needs to be checked again
			batFlag = false;
		}
		else {              //get commands for the current room
			std::cin >> command;

			if (command == 'm') {
				std::cout << "Move into which room?\n";
			}
			else if (command == 'a') {
				std::cout << "Fire into which room?\n";
				arrowFlag = true;
			}

			std::cin >> cmdInt;

			for (int i = 0; i < lvl.rooms[roomNo - 1].paths.size(); ++i) {    //check each connected room to see if it matches command
				if (cmdInt == lvl.rooms[roomNo - 1].paths[i]) {
					if (arrowFlag) {
						if (lvl.rooms[cmdInt - 1].roomHazard == Wumpus) {
							std::cout << "You killed the Wumpus! You win!\n";
							gameOverFlag = true;
							return;
						}
						else {
							std::cout << "You are out of arrows. You lose!\n";
							gameOverFlag = true;
							return;
						}
					}
					roomNo = lvl.rooms[roomNo - 1].paths[i];
					break;
				}
			}

			if (arrowFlag) {
				std::cout << "Not a valid target!\n";
				arrowFlag = false;
			}
			std::cin.clear();
			std::cin.ignore(10000, '\n');
		}
	}
}

#endif // WUMPUSGAME_H_INCLUDED