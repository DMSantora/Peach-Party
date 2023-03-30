#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"

// my changes
#include "Actor.h"

#include <string>

// my changes
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	~StudentWorld();
	Peach* getPeach() { return myPeach; };
	Yoshi* getYoshi() { return myYoshi; };
	Board getBoard() { return myBoard; };

	bool isValidPos(int x, int y);
	int getBankBalance() { return bankBalance; };
	void setBankBalance(int balance) { bankBalance = balance; };
	int* getRandomTeleportationSquare();
	void teleportToRandomSquare(Actor* actor);
	void swapPlayers();
	void swapStars();
	void swapCoins();
	void dropSquare(int x, int y);
	bool intersecting(Actor* a, Actor* b);
	bool intersecting(int x1, int y1, int x2, int y2);
	bool findImpactable(Actor* myVortex);
	void newVortex(int x, int y, int direction);
	bool touchingPeach(Actor* myActor);
	bool touchingYoshi(Actor* myActor);

private:
	Board myBoard;
	Peach* myPeach;
	Yoshi* myYoshi;
	std::vector<Actor*> actors;
	int bankBalance;
};

#endif // STUDENTWORLD_H_
