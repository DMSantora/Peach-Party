#include "Actor.h"
#include "StudentWorld.h"

#include <cstdlib>

using namespace std;

void PlayerAvatar::doSomething() {
	// if PlayerAvatar cannot walk in current direction, set new valid direction.
	if (!canWalkInDirection(getWalkDirection())) {
		setNewWalkDirection();
		switch (getWalkDirection()) {
		case right:
			setDirection(right);
			break;
		case up:
			setDirection(right);
			break;
		case left:
			setDirection(left);
			break;
		case down:
			setDirection(right);
			break;
		}
	}

	// if player is not walking, let player either roll or fire
	if (!isWalking()) {
		switch (getWorld()->getAction(getPlayerNumber())) {
		case ACTION_NONE:
			break;
		case ACTION_LEFT:
			break;
		case ACTION_RIGHT:
			break;
		case ACTION_UP:
			break;
		case ACTION_DOWN:
			break;
		case ACTION_ROLL:
			setDieRoll(rand() % 10 + 1);
			setTicksToMove(getDieRoll() * 8);
			setWalking();
			break;
		case ACTION_FIRE:
			if (hasVortex()) {
				getWorld()->newVortex(getX(), getY(), getWalkDirection());
			}
			setHasVortex(false);
			getWorld()->playSound(SOUND_PLAYER_FIRE);
			break;
		}
	}

	if (isWalking()) {
		// fork code
		int possibleDirections = 0;
		if (getX() % 16 == 0 && getY() % 16 == 0 && !getOnDirectional()) {
			if (canWalkInDirection(right))
				possibleDirections++;
			if (canWalkInDirection(up))
				possibleDirections++;
			if (canWalkInDirection(left))
				possibleDirections++;
			if (canWalkInDirection(down))
				possibleDirections++;
		}

		// if player is currently on fork (but not on DirectionalSquare) let player choose valid direction
		if (possibleDirections > 2) {
			switch (getWorld()->getAction(getPlayerNumber())) {
			case ACTION_LEFT:
				if (getWalkDirection() == right || !canWalkInDirection(left)) {
					return;
				}
				setWalkDirection(left);
				setDirection(left);
				break;
			case ACTION_RIGHT:
				if (getWalkDirection() == left || !canWalkInDirection(right)) {
					return;
				}
				setWalkDirection(right);
				setDirection(right);
				break;
			case ACTION_UP:
				if (getWalkDirection() == down || !canWalkInDirection(up)) {
					return;
				}
				setWalkDirection(up);
				setDirection(up);
				break;
			case ACTION_DOWN:
				if (getWalkDirection() == up || !canWalkInDirection(down)) {
					return;
				}
				setWalkDirection(down);
				setDirection(down);
				break;
			case ACTION_ROLL:
				return;
				break;
			case ACTION_FIRE:
				return;
				break;
			case ACTION_NONE:
				return;
				break;
			}
		}

		// use PlayerAvatar walk() function
		if (canWalkInDirection(getWalkDirection())) {
			walk();
		}
		else {
			setNewWalkDirection();
			walk();
		}
		setTicksToMove(getTicksToMove() - 1);
		setDieRoll((getTicksToMove() + 3) / 8);
		if (getTicksToMove() == 0) {
			setStanding();
		}
	}
	return;
}

// returns true if the square in actor's direction is not empty
bool Actor::canWalkInDirection(int direction) {
	if (getX() % 16 == 0 && getY() % 16 == 0) {
		switch (direction) {
		case right:
			if (getWorld()->isValidPos(getX() / 16 + 1, getY() / 16)) {
				return true;
			}
			return false;
			break;
		case up:
			if (getWorld()->isValidPos(getX() / 16, getY() / 16 + 1)) {
				return true;
			}
			return false;
			break;
		case left:
			if (getWorld()->isValidPos(getX() / 16 - 1, getY() / 16)) {
				return true;
			}
			return false;
			break;
		case down:
			if (getWorld()->isValidPos(getX() / 16, getY() / 16 - 1)) {
				return true;
			}
			return false;
			break;
		}
	}
	return true;
}

// returns a random valid direction for an actor
int Actor::getRandomValidDirection() {
	vector<int> validDirections;
	if (canWalkInDirection(right)) {
		validDirections.push_back(right);
	}
	if (canWalkInDirection(up)) {
		validDirections.push_back(up);
	}
	if (canWalkInDirection(left)) {
		validDirections.push_back(left);
	}
	if (canWalkInDirection(down)) {
		validDirections.push_back(down);
	}
	int random_roll = rand() % validDirections.size();
	return validDirections.at(random_roll);
}

// returns the number of valid directions an actor has
int Actor::getNumValidDirections() {
	int possibleDirections = 0;
	if (canWalkInDirection(right))
		possibleDirections++;
	if (canWalkInDirection(up))
		possibleDirections++;
	if (canWalkInDirection(left))
		possibleDirections++;
	if (canWalkInDirection(down))
		possibleDirections++;
	return possibleDirections;
}

// sets new walk direction for playerAvatar based on previous walking direction (hierarchy of walking directions)
void PlayerAvatar::setNewWalkDirection() {
	switch (getWalkDirection()) {
	case right:
		if (canWalkInDirection(up)) {
			setWalkDirection(up);
			break;
		}
		if (canWalkInDirection(down)) {
			setWalkDirection(down);
			break;
		}
		if (canWalkInDirection(left)) {
			setWalkDirection(left);
			break;
		}
	case up:
		if (canWalkInDirection(right)) {
			setWalkDirection(right);
			break;
		}
		if (canWalkInDirection(left)) {
			setWalkDirection(left);
			break;
		}
		if (canWalkInDirection(down)) {
			setWalkDirection(down);
			break;
		}
	case left:
		if (canWalkInDirection(up)) {
			setWalkDirection(up);
			break;
		}
		if (canWalkInDirection(down)) {
			setWalkDirection(down);
			break;
		}
		if (canWalkInDirection(right)) {
			setWalkDirection(right);
			break;
		}
	case down:
		if (canWalkInDirection(right)) {
			setWalkDirection(right);
			break;
		}
		if (canWalkInDirection(left)) {
			setWalkDirection(left);
			break;
		}
		if (canWalkInDirection(up)) {
			setWalkDirection(up);
			break;
		}
	}
}

// moves playerAvatar based on walking direction and sprite direction
void PlayerAvatar::walk() {
	switch (getWalkDirection()) {
	case right:
		setDirection(right);
		moveForward(2);
		break;
	case up:
		setDirection(right);
		moveAtAngle(up, 2);
		break;
	case left:
		setDirection(left);
		moveForward(2);
		break;
	case down:
		setDirection(right);
		moveAtAngle(down, 2);
		break;
	}
	setJustTeleported(false);
	setOnDirectional(false);
}

// if a playerAvatar is on this square give them 3 coins if blue, take away 3 coins (or as many as possible) if red. 
// Uses function setPeachWasHere() and setYoshiWasHere() to keep track of player activations
void CoinSquare::doSomething() {
	if (!getPeachWasHere()) {
		if (!(getWorld()->getPeach()->isWalking()) && getWorld()->touchingPeach(this)) {
			if (getColor() == 0) {
				getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins() + 3);
				getWorld()->playSound(SOUND_GIVE_COIN);
			}
			if (getColor() == 1) {
				int deduction;
				if (getWorld()->getPeach()->getCoins() - 3 < 0) {
				deduction = getWorld()->getPeach()->getCoins();
				}
				else {
					deduction = 3;
				}
				getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins() - deduction);
				getWorld()->playSound(SOUND_TAKE_COIN);
			}
			setPeachWasHere(true);
		}
	}
	if (!getYoshiWasHere()) {
		if (!(getWorld()->getYoshi()->isWalking()) && getWorld()->touchingYoshi(this)) {
			if (getColor() == 0) {
				getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins() + 3);
				getWorld()->playSound(SOUND_GIVE_COIN);
			}
			if (getColor() == 1) {
				int deduction;
				if (getWorld()->getYoshi()->getCoins() - 3 < 0) {
					deduction = getWorld()->getYoshi()->getCoins();
				}
				else {
					deduction = 3;
				}
				getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins() - deduction);
				getWorld()->playSound(SOUND_TAKE_COIN);
			}
			setYoshiWasHere(true);
		}
	}
	if (getPeachWasHere() && (getX() != getWorld()->getPeach()->getX() || getY() != getWorld()->getPeach()->getY())) {
		setPeachWasHere(false);
	}
	if (getYoshiWasHere() && (getX() != getWorld()->getYoshi()->getX() || getY() != getWorld()->getYoshi()->getY())) {
		setYoshiWasHere(false);
	}
	return;
}

// if a playerAvatar is on this square and has 20 or more coins, take 20 coins away and give them 1 star.
// Uses function setPeachWasHere() and setYoshiWasHere() to keep track of player activations
void StarSquare::doSomething() {
	if (!getPeachWasHere()) {
		if (!(getWorld()->getPeach()->isWalking()) && getWorld()->touchingPeach(this)) {
			if (getWorld()->getPeach()->getCoins() < 20) {
				return;
			}
			else {
				getWorld()->getPeach()->setStars(getWorld()->getPeach()->getStars() + 1);
				getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins() - 20);
				getWorld()->playSound(SOUND_GIVE_STAR);
				setPeachWasHere(true);
			}
		}
	}
	if (!getYoshiWasHere()) {
		if (!(getWorld()->getYoshi()->isWalking()) && getWorld()->touchingYoshi(this)) {
			if (getWorld()->getYoshi()->getCoins() < 20) {
				return;
			}
			else {
				getWorld()->getYoshi()->setStars(getWorld()->getYoshi()->getStars() + 1);
				getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins() - 20);
				getWorld()->playSound(SOUND_GIVE_STAR);
				setPeachWasHere(true);
			}
		}
	}
	if (getPeachWasHere() && (getX() != getWorld()->getPeach()->getX() || getY() != getWorld()->getPeach()->getY())) {
		setPeachWasHere(false);
	}
	if (getYoshiWasHere() && (getX() != getWorld()->getYoshi()->getX() || getY() != getWorld()->getYoshi()->getY())) {
		setYoshiWasHere(false);
	}
	return;
}

// if a playerAvatar is on this square, set their direction to forceDirection and set that player's 
// onDirectionalSquare bool to true
void DirectionalSquare::doSomething() {
	if (getWorld()->touchingPeach(this)) {
		getWorld()->getPeach()->setOnDirectional(true);
		getWorld()->getPeach()->setWalkDirection(getForcingDirection());
	}
	if (getWorld()->touchingYoshi(this)) {
		getWorld()->getYoshi()->setOnDirectional(true);
		getWorld()->getYoshi()->setWalkDirection(getForcingDirection());
	}
}

// if a playerAvatar passes over this square take away 5 coins (or as many as possible).
// if player lands on Bank, give them the bank's balance
// Uses function setPeachWasHere() and setYoshiWasHere() to keep track of player activations
void BankSquare::doSomething() {
	if (getWorld()->getPeach()->isWalking() && getX() == getWorld()->getPeach()->getX() && getY() == getWorld()->getPeach()->getY()) {
		int deduction;
		if (getWorld()->getPeach()->getCoins() - 5 < 0) {
			deduction = getWorld()->getPeach()->getCoins();
		}
		else {
			deduction = 5;
		}
		getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins() - deduction);
		getWorld()->setBankBalance(getWorld()->getBankBalance() + deduction);
		getWorld()->playSound(SOUND_DEPOSIT_BANK);
	}
	if (!(getWorld()->getPeach()->isWalking()) && !getPeachWasHere() && getX() == getWorld()->getPeach()->getX() && getY() == getWorld()->getPeach()->getY()) {
		getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins() + getWorld()->getBankBalance());
		getWorld()->setBankBalance(0);
		setPeachWasHere(true);
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
	}
	if (getWorld()->getYoshi()->isWalking() && getX() == getWorld()->getYoshi()->getX() && getY() == getWorld()->getYoshi()->getY()) {
		int deduction;
		if (getWorld()->getYoshi()->getCoins() - 5 < 0) {
			deduction = getWorld()->getYoshi()->getCoins();
		}
		else {
			deduction = 5;
		}
		getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins() - deduction);
		getWorld()->setBankBalance(getWorld()->getBankBalance() + deduction);
		getWorld()->playSound(SOUND_DEPOSIT_BANK);
	}
	if (!(getWorld()->getYoshi()->isWalking()) && !getYoshiWasHere() && getX() == getWorld()->getYoshi()->getX() && getY() == getWorld()->getYoshi()->getY()) {
		getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins() + getWorld()->getBankBalance());
		getWorld()->setBankBalance(0);
		setYoshiWasHere(true);
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
	}
	if (getPeachWasHere() && (getX() != getWorld()->getPeach()->getX() || getY() != getWorld()->getPeach()->getY())) {
		setPeachWasHere(false);
	}
	if (getYoshiWasHere() && (getX() != getWorld()->getYoshi()->getX() || getY() != getWorld()->getYoshi()->getY())) {
		setYoshiWasHere(false);
	}
}

// if a playerAvatar is on this square, randomly choose 1 option:
// 1. teleport player to random location on board
// 2. swap player's attributes
// 3. give player vortex 
// Uses function setPeachWasHere() and setYoshiWasHere() to keep track of player activations
// Uses setJustTeleported() function to prevent swapping player locations from triggering EventSquare again.
void EventSquare::doSomething() {
	if (!(getWorld()->getPeach()->isWalking()) && getX() == getWorld()->getPeach()->getX() && getY() == getWorld()->getPeach()->getY() && !(getWorld()->getPeach()->getJustTeleported())) {
		int random_roll = rand() % 3 + 1;
		if (random_roll == 1) { // teleport to random square
			int* randomSquare = getWorld()->getRandomTeleportationSquare();
			getWorld()->getPeach()->moveTo(randomSquare[0], randomSquare[1]);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
		}
		else if (random_roll == 2) { // swap position and movement state with other player
			getWorld()->swapPlayers();
			getWorld()->getPeach()->setJustTeleported(true);
			getWorld()->getYoshi()->setJustTeleported(true);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
		}
		else if (random_roll == 3) { // give player vortex if they dont already have one
			getWorld()->getPeach()->setJustTeleported(true);
			getWorld()->getPeach()->setHasVortex(true);
			getWorld()->playSound(SOUND_GIVE_VORTEX);
		}
	}
	if (!(getWorld()->getYoshi()->isWalking()) && getX() == getWorld()->getYoshi()->getX() && getY() == getWorld()->getYoshi()->getY() && !(getWorld()->getYoshi()->getJustTeleported())) {
		int random_roll = rand() % 3 + 1;
		if (random_roll == 1) { // teleport to random square
			int* randomSquare = getWorld()->getRandomTeleportationSquare();
			getWorld()->getYoshi()->moveTo(randomSquare[0], randomSquare[1]);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
		}
		else if (random_roll == 2) { // swap position and movement state with other player
			getWorld()->swapPlayers();
			getWorld()->getPeach()->setJustTeleported(true);
			getWorld()->getYoshi()->setJustTeleported(true);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
		}
		else if (random_roll == 3) { // give player vortex if they dont already have one
			getWorld()->getYoshi()->setJustTeleported(true);
			getWorld()->getYoshi()->setHasVortex(true);
			getWorld()->playSound(SOUND_GIVE_VORTEX);
			}
		}
	return;
}

// if a playerAvatar is on randomly remove either 1 of their stars or 10 of their coins.
// if a player does not have a star, remove 10 coins (or as many as possible)
// Uses function setPeachWasHere() and setYoshiWasHere() to keep track of player activations
void DroppingSquare::doSomething() {
	if (!getPeachWasHere()) {
		if (!(getWorld()->getPeach()->isWalking()) && getX() == getWorld()->getPeach()->getX() && getY() == getWorld()->getPeach()->getY()) {
			getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			if (getWorld()->getPeach()->getStars() > 0) {
				int random_roll = rand() % 2 + 1;
				if (random_roll == 1) {
					getWorld()->getPeach()->setStars(getWorld()->getPeach()->getStars() - 1);
				}
				else if (random_roll == 2) {
					int deduction;
					if (getWorld()->getPeach()->getCoins() - 10 < 0) {
						deduction = getWorld()->getPeach()->getCoins();
					}
					else {
						deduction = 10;
					}
					getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins() - deduction);
				}
			}
			else {
				int deduction;
				if (getWorld()->getPeach()->getCoins() - 10 < 0) {
					deduction = getWorld()->getPeach()->getCoins();
				}
				else {
					deduction = 10;
				}
				getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins() - deduction);
			}
			setPeachWasHere(true);
		}
	}
	if (!getYoshiWasHere()) {
		if (!(getWorld()->getYoshi()->isWalking()) && getX() == getWorld()->getYoshi()->getX() && getY() == getWorld()->getYoshi()->getY()) {
			getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			if (getWorld()->getYoshi()->getStars() > 0) {
				int random_roll = rand() % 2 + 1;
				if (random_roll == 1) {
					getWorld()->getYoshi()->setStars(getWorld()->getYoshi()->getStars() - 1);
				}
				else if (random_roll == 2) {
					int deduction;
					if (getWorld()->getYoshi()->getCoins() - 10 < 0) {
						deduction = getWorld()->getYoshi()->getCoins();
					}
					else {
						deduction = 10;
					}
					getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins() - deduction);
				}
			}
			else {
				int deduction;
				if (getWorld()->getYoshi()->getCoins() - 10 < 0) {
					deduction = getWorld()->getYoshi()->getCoins();
				}
				else {
					deduction = 10;
				}
				getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins() - deduction);
			}
			setYoshiWasHere(true);
		}
	}
	if (getPeachWasHere() && (getX() != getWorld()->getPeach()->getX() || getY() != getWorld()->getPeach()->getY())) {
		setPeachWasHere(false);
	}
	if (getYoshiWasHere() && (getX() != getWorld()->getYoshi()->getX() || getY() != getWorld()->getYoshi()->getY())) {
		setYoshiWasHere(false);
	}
	return;
}

// // moves Enemy based on walking direction and sprite direction
void Enemy::walk() {
	switch (getWalkDirection()) {
	case right:
		setDirection(right);
		moveForward(2);
		break;
	case up:
		setDirection(right);
		moveAtAngle(up, 2);
		break;
	case left:
		setDirection(left);
		moveForward(2);
		break;
	case down:
		setDirection(right);
		moveAtAngle(down, 2);
		break;
	}
}

// sets new walk direction for playerAvatar based on previous walking direction (hierarchy of walking directions)
void Enemy::setNewWalkDirection() {
	switch (getWalkDirection()) {
	case right:
		if (canWalkInDirection(up)) {
			setWalkDirection(up);
			break;
		}
		if (canWalkInDirection(down)) {
			setWalkDirection(down);
			break;
		}
		if (canWalkInDirection(left)) {
			setWalkDirection(left);
			break;
		}
	case up:
		if (canWalkInDirection(right)) {
			setWalkDirection(right);
			break;
		}
		if (canWalkInDirection(left)) {
			setWalkDirection(left);
			break;
		}
		if (canWalkInDirection(down)) {
			setWalkDirection(down);
			break;
		}
	case left:
		if (canWalkInDirection(up)) {
			setWalkDirection(up);
			break;
		}
		if (canWalkInDirection(down)) {
			setWalkDirection(down);
			break;
		}
		if (canWalkInDirection(right)) {
			setWalkDirection(right);
			break;
		}
	case down:
		if (canWalkInDirection(right)) {
			setWalkDirection(right);
			break;
		}
		if (canWalkInDirection(left)) {
			setWalkDirection(left);
			break;
		}
		if (canWalkInDirection(up)) {
			setWalkDirection(up);
			break;
		}
	}
}

void Boo::doSomething() {
	// if hit by vortex, teleport to random location on board and reset pause counter
	if (getHitByVortex()) {
		setHitByVortex(false);
		getWorld()->teleportToRandomSquare(this);
		setWalkDirection(right);
		setDirection(right);
		setWalking(false);
		setPauseCounter(180);
	}

	// if not walking, roll from 1 to 3 to determine how many squares to move
	// check if either peach of yoshi are on same square as boo
	// if a player is touching boo, randomly either swap the players' coins or stars.
	if (!isWalking()) {
		if (!(getWorld()->getPeach()->isWalking()) && getX() == getWorld()->getPeach()->getX() && getY() == getWorld()->getPeach()->getY() && !isPeachActivated()) {
			int random_roll = rand() % 2 + 1;
			if (random_roll == 1) {
				getWorld()->swapCoins();
			}
			else {
				getWorld()->swapStars();
			}
			setPeachActivated(true);
			getWorld()->playSound(SOUND_BOO_ACTIVATE);
		}
		if (!(getWorld()->getYoshi()->isWalking()) && getX() == getWorld()->getYoshi()->getX() && getY() == getWorld()->getYoshi()->getY() && !isYoshiActivated()) {
			int random_roll = rand() % 2 + 1;
			if (random_roll == 1) {
				getWorld()->swapCoins();
			}
			else {
				getWorld()->swapStars();
			}
			setYoshiActivated(true);
			getWorld()->playSound(SOUND_BOO_ACTIVATE);
		}
		setPauseCounter(getPauseCounter()-1);
		if (getPauseCounter() == 0) {
			int squares_to_move = rand() % 3 + 1;
			setTicksToMove(squares_to_move * 8);
			int direction = getRandomValidDirection();
			if (direction == right) {
				setDirection(right);
				setWalkDirection(right);
			}
			if (direction == up) {
				setDirection(right);
				setWalkDirection(up);
			}
			if (direction == left) {
				setDirection(left);
				setWalkDirection(left);
			}
			if (direction == down) {
				setDirection(right);
				setWalkDirection(down);
			}
			setWalking(true);
		}
	}
	// if boo is walking, choose direction based on specs
	if (isWalking()) {
		if (getX() % 16 == 0 && getY() == 0 && getNumValidDirections() > 2) {
			int direction = getRandomValidDirection();
			if (direction == right) {
				setDirection(right);
				setWalkDirection(right);
			}
			if (direction == up) {
				setDirection(right);
				setWalkDirection(up);
			}
			if (direction == left) {
				setDirection(left);
				setWalkDirection(left);
			}
			if (direction == down) {
				setDirection(right);
				setWalkDirection(down);
			}
		}
		else if (getX() % 16 == 0 && getY() % 16 == 0 && !canWalkInDirection(getWalkDirection())) {
			setNewWalkDirection();
		}
		walk();
		setPeachActivated(false);
		setYoshiActivated(false);
		setTicksToMove(getTicksToMove() - 1);
		if (getTicksToMove() == 0) {
			setWalking(false);
			setPauseCounter(180);
		}
	}
}

void Bowser::doSomething() {
	// if hit by vortex, teleport to random location on board and reset pause counter
	if (getHitByVortex()) {
		setHitByVortex(false);
		getWorld()->teleportToRandomSquare(this);
		setWalkDirection(right);
		setDirection(right);
		setWalking(false);
		setPauseCounter(180);
	}

	// if not walking, roll from 1 to 3 to determine how many squares to move
	// check if either peach of yoshi are on same square as bowser
	// if a player is touching bowser, 50% chance to remove all their stars and coins.
	if (!isWalking()) {
		if (!(getWorld()->getPeach()->isWalking()) && getX() == getWorld()->getPeach()->getX() && getY() == getWorld()->getPeach()->getY() && !isPeachActivated()) {
			int random_roll = rand() % 2 + 1;
			if (random_roll == 1) {
				getWorld()->getPeach()->setCoins(0);
				getWorld()->getPeach()->setStars(0);
				getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
			}
			setPeachActivated(true);
		}
		if (!(getWorld()->getYoshi()->isWalking()) && getX() == getWorld()->getYoshi()->getX() && getY() == getWorld()->getYoshi()->getY() && !isYoshiActivated()) {
			int random_roll = rand() % 2 + 1;
			if (random_roll == 1) {
				getWorld()->getYoshi()->setCoins(0);
				getWorld()->getYoshi()->setStars(0);
				getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
			}
			setYoshiActivated(true);
		}
		setPauseCounter(getPauseCounter() - 1);
		if (getPauseCounter() == 0) {
			int squares_to_move = rand() % 10 + 1;
			setTicksToMove(squares_to_move * 8);
			int direction = getRandomValidDirection();
			if (direction == right) {
				setDirection(right);
				setWalkDirection(right);
			}
			if (direction == up) {
				setDirection(right);
				setWalkDirection(up);
			}
			if (direction == left) {
				setDirection(left);
				setWalkDirection(left);
			}
			if (direction == down) {
				setDirection(right);
				setWalkDirection(down);
			}
			setWalking(true);
		}
	}
	// if bowser is walking, choose direction based on specs
	if (isWalking()) {
		if (getX() % 16 == 0 && getY() == 0 && getNumValidDirections() > 2) {
			int direction = getRandomValidDirection();
			if (direction == right) {
				setDirection(right);
				setWalkDirection(right);
			}
			if (direction == up) {
				setDirection(right);
				setWalkDirection(up);
			}
			if (direction == left) {
				setDirection(left);
				setWalkDirection(left);
			}
			if (direction == down) {
				setDirection(right);
				setWalkDirection(down);
			}
		}
		else if (getX() % 16 == 0 && getY() % 16 == 0 && !canWalkInDirection(getWalkDirection())) {
			setNewWalkDirection();
		}
		walk();
		setPeachActivated(false);
		setYoshiActivated(false);
		setTicksToMove(getTicksToMove() - 1);
		// if just finished walking, 25% chance to drop a DroppingSquare onto current square.
		if (getTicksToMove() == 0) {
			setWalking(false);
			setPauseCounter(180);
			int random_roll = rand() % 4 + 1; // CHANGE WHEN DONE TESTING
			if (random_roll == 1) {
				getWorld()->dropSquare(getX(), getY());
				getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
			}
		}
	}
	return;
}

// move vortex forward. If out of bounds setDead(). Check if vortex has any impactable actors.
void Vortex::doSomething() {
	moveForward(2);
	if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT) {
		setDead();
	}
	if (getWorld()->findImpactable(this)) {
		//cerr << "IMPACT" << endl;
	}
}

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
