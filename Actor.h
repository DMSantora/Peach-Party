#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

/*
Class Hierarchy:

GraphObject
	Actor
		Vortex
		Player Avatar
			Peach
			Yoshi
		ActivateOnPlayer
			BankSquare
			CoinSquare
			DirectionalSquare
			DroppingSquare
			EventSquare
			StarSquare
			Enemy
				Bowser
				Boo			
*/

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, int startDirection, int depth, double size) :
		GraphObject(imageID, startX, startY, startDirection, depth, size), myWorld(world) {};

	virtual void doSomething() = 0;
	virtual bool stationary() = 0;
	StudentWorld* getWorld() { return myWorld; };
	bool canWalkInDirection(int direction);
	int getRandomValidDirection();
	int getNumValidDirections();

	virtual bool isImpactable() = 0;
	virtual bool getAlive() = 0;
	virtual void setDead() = 0;
	virtual bool isHit() = 0;
	virtual void setHit(bool status) = 0;

private:
	StudentWorld* myWorld;
};

class ActivateOnPlayer : public Actor {
public:
	ActivateOnPlayer(StudentWorld* world, int imageID, int startX, int startY, int startDirection, int depth, double size) :
	Actor(world, imageID, startX, startY, startDirection, depth, size), peachWasHere(false), yoshiWasHere(false) {};

	virtual void doSomething() = 0;
	virtual bool isImpactable() = 0;
	virtual bool isHit() = 0;
	virtual void setHit(bool status) = 0;
	
	virtual void setDead() { alive = false; };
	bool getAlive() { return alive; };
	bool getPeachWasHere() { return peachWasHere; };
	bool getYoshiWasHere() { return yoshiWasHere; };
	void setPeachWasHere(bool status) { peachWasHere = status; };
	void setYoshiWasHere(bool status) { yoshiWasHere = status; };

private:
	bool peachWasHere;
	bool yoshiWasHere;
	bool alive;
};

class CoinSquare : public ActivateOnPlayer
{
public:
	CoinSquare(StudentWorld* world, int imageID, int startX, int startY, int color, int startDirection = right, int depth = 1, double size = 1) :
		ActivateOnPlayer(world, imageID, startX, startY, startDirection, depth, size), myColor(color) {};

	virtual bool isHit() { return false; };
	virtual void setHit(bool status) { return; };
	virtual void doSomething();
	virtual bool stationary() { return true; };
	virtual bool isImpactable() { return false; };
	int getColor() { return myColor; };

private:

	int myColor; // 0 for blue, 1 for red
};

class BankSquare : public ActivateOnPlayer
{
public:
	BankSquare(StudentWorld* world, int startX, int startY) :
		ActivateOnPlayer(world, IID_BANK_SQUARE, startX, startY, right, 1, 1) {};

	virtual bool isHit() { return false; };
	virtual void setHit(bool status) { return; };
	virtual void doSomething();
	virtual bool stationary() { return true; };
	virtual bool isImpactable() { return false; };
};

class StarSquare : public ActivateOnPlayer
{
public:
	StarSquare(StudentWorld* world, int startX, int startY) :
		ActivateOnPlayer(world, IID_STAR_SQUARE, startX, startY, right, 1, 1) {};

	virtual bool isHit() { return false; };
	virtual void setHit(bool status) { return; };
	virtual void doSomething();
	virtual bool stationary() { return true; };
	virtual bool isImpactable() { return false; };
};

class EventSquare : public ActivateOnPlayer
{
public:
	EventSquare(StudentWorld* world, int startX, int startY) :
		ActivateOnPlayer(world, IID_EVENT_SQUARE, startX, startY, right, 1, 1) {};

	virtual bool isHit() { return false; };
	virtual void setHit(bool status) { return; };
	virtual void doSomething();
	virtual bool stationary() { return true; };
	virtual bool isImpactable() { return false; };

private:

};

class DirectionalSquare : public ActivateOnPlayer {
public:
	DirectionalSquare(StudentWorld* world, int imageID, int startX, int startY, int startDirection) :
		ActivateOnPlayer(world, imageID, startX, startY, startDirection, 1, 1), forcingDirection(startDirection) {};

	virtual bool isHit() { return false; };
	virtual void setHit(bool status) { return; };
	virtual void doSomething();
	virtual bool stationary() { return true; };
	int getForcingDirection() { return forcingDirection; };
	virtual bool isImpactable() { return false; };

private:
	int forcingDirection;
};

class DroppingSquare : public ActivateOnPlayer{
public:
	DroppingSquare(StudentWorld* world, int startX, int startY) : 
		ActivateOnPlayer(world, IID_DROPPING_SQUARE, startX, startY, right, 1, 1) {};

	virtual bool isHit() { return false; };
	virtual void setHit(bool status) { return; };
	virtual void doSomething();
	virtual bool stationary() { return true; };
	virtual bool isImpactable() { return false; };
};

class Enemy : public ActivateOnPlayer {
public:
	Enemy(StudentWorld* world, int imageID, int startX, int startY) :
		ActivateOnPlayer(world, imageID, startX, startY, right, 0, 1), walking(false), peachActivated(false), 
		yoshiActivated(false), initialTravelDistance(0), pauseCounter(180), hitByVortex(false) {};

	virtual void doSomething() { return; };
	virtual bool stationary() { return false; };
	virtual bool isImpactable() { return true; };
	virtual bool isHit() { return hitByVortex; };
	virtual void setHit(bool status) {hitByVortex = status; };
	int getTicksToMove() { return ticks_to_move; };
	void setTicksToMove(int ticks) { ticks_to_move = ticks; };
	int getSquaresToMove() { return squares_to_move; };
	void setSquaresToMove(int squares) { squares_to_move = squares; };
	int getWalkDirection() { return walkDirection; };
	void setWalkDirection(int direction) { walkDirection = direction; };
	void setNewWalkDirection();
	void walk();
	bool isWalking() { return walking; };
	void setWalking(bool status) { walking = status; };
	bool isPeachActivated() { return peachActivated; };
	void setPeachActivated(bool status) { peachActivated = status; };
	bool isYoshiActivated() { return yoshiActivated; };
	void setYoshiActivated(bool status) { yoshiActivated = status; };
	int getPauseCounter() { return pauseCounter; };
	void setPauseCounter(int counter) { pauseCounter = counter; };
	bool getHitByVortex() { return hitByVortex; };
	void setHitByVortex(bool status) { hitByVortex = status; };

private:
	int ticks_to_move;
	int squares_to_move;
	int walkDirection;
	bool walking; // true for walking, false for paused
	bool peachActivated;
	bool yoshiActivated;
	int initialTravelDistance;
	int pauseCounter;
	bool hitByVortex;
};

class Boo : public Enemy {
public:
	Boo(StudentWorld* world, int startX, int startY) :
		Enemy(world, IID_BOO, startX, startY) {};

	virtual void doSomething();

private:
};

class Bowser : public Enemy {
public:
	Bowser(StudentWorld* world, int startX, int startY) :
		Enemy(world, IID_BOWSER, startX, startY) {};

	virtual void doSomething();
};

class PlayerAvatar : public Actor
{
public:
	PlayerAvatar(StudentWorld* world, int imageID, int startX, int startY, int playerNumber) :
		Actor(world, imageID, startX, startY, right, 0, 1), walking(false), myCoins(0), myStars(0), walkDirection(right),
		die_roll(0), ticks_to_move(0), myPlayerNumber(playerNumber), justTeleported(false), onDirectional(false), 
		hasVor(false) {};

	virtual void doSomething();
	virtual bool stationary() { return false; };
	virtual bool isImpactable() { return false; };
	virtual bool getAlive() { return false; }
	virtual void setDead() { return; };
	virtual bool isHit() { return false; };
	virtual void setHit(bool status) { return; };
	int getPlayerNumber() { return myPlayerNumber; };
	virtual void setWalking() { walking = true; };
	virtual void setStanding() { walking = false; };
	bool isWalking() { return walking; };
	void setWalking(bool status) { walking = status; };
	

	void setWalkDirection(int direction) { walkDirection = direction; };
	void setNewWalkDirection();
	int getWalkDirection() { return walkDirection; };

	void walk();

	int getDieRoll() { return die_roll; };
	int getTicksToMove() { return ticks_to_move; };
	void setDieRoll(int newRoll) { die_roll = newRoll; };
	void setTicksToMove(int ticks) { ticks_to_move = ticks; };
	int getCoins() { return myCoins; };
	void setCoins(int coins) {
		if (coins > 0)
			myCoins = coins;
		else
			myCoins = 0;
	};
	int getStars() { return myStars; };
	void setStars(int stars) {
		myStars = stars;
	};
	bool getJustTeleported() { return justTeleported; };
	void setJustTeleported(bool status) { justTeleported = status; };
	bool getOnDirectional() { return onDirectional; };
	void setOnDirectional(bool status) { onDirectional = status; };
	bool hasVortex() { return hasVor; };
	void setHasVortex(bool status) { hasVor = status; };

private:
	int die_roll;
	int ticks_to_move;
	int myPlayerNumber;
	bool walking; // true for walking, false for waiting to roll
	int walkDirection;
	int myCoins;
	int myStars;
	bool justTeleported;
	bool onDirectional;
	bool hasVor;
};

class Peach : public PlayerAvatar
{
public:
	Peach(StudentWorld* world, int startX, int startY) :
		PlayerAvatar(world, IID_PEACH, startX, startY, 1) {};

private:
	//int FILLTHISLATER;
};

class Yoshi : public PlayerAvatar
{
public:
	Yoshi(StudentWorld* world, int startX, int startY) :
		PlayerAvatar(world, IID_YOSHI, startX, startY, 2) {};

private:
	//int FILLTHISLATER;
};

class Vortex : public Actor {
public:
	Vortex(StudentWorld* world, int startX, int startY, int startDirection) : 
		Actor(world, IID_VORTEX, startX, startY, startDirection, 0, 1), alive(true) {};

	virtual bool isHit() { return false; };
	virtual void setHit(bool status) { return; };

	virtual void doSomething();
	//virtual void walk();
	virtual bool stationary() { return false; };
	virtual bool isImpactable() { return false; };

	virtual void setDead() { alive = false; };
	bool getAlive() { return alive; };

private:
	bool alive;
};

#endif // ACTOR_H_