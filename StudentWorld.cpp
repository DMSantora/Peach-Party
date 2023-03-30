#include "StudentWorld.h"
#include "GameConstants.h"

#include <string>

// my changes
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
    myPeach = nullptr;
    myYoshi = nullptr;
    bankBalance = 0;
    actors.clear();
}

int StudentWorld::init()
{
    string boardFile = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    myBoard.Board::loadBoard(boardFile);
    Board::LoadResult result = myBoard.Board::loadBoard(boardFile);

    if (result == Board::load_fail_file_not_found) {
        //cerr << "Cannot find " << boardFile << " data file" << endl;
    }
    else if (result == Board::load_fail_bad_format) {
        //cerr << "Your board was improperly formatted" << endl;
    }
    else if (result == Board::load_success) {
        //cerr << "Successfully loaded level" << endl;

        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                Board::GridEntry ge = myBoard.Board::getContentsOf(x, y);
                switch (ge) {
                case Board::empty:
                    break;
                case Board::player:
                    myPeach = new Peach(this, x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                    myYoshi = new Yoshi(this, x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                    actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0));
                    break;
                case Board::blue_coin_square:
                    actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0));
                    break;
                case Board::red_coin_square:
                    actors.push_back(new CoinSquare(this, IID_RED_COIN_SQUARE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 1));
                    break;
                case Board::star_square:
                    actors.push_back(new StarSquare(this, x * SPRITE_WIDTH, y * SPRITE_HEIGHT));
                    break;
                case Board::event_square:
                    actors.push_back(new EventSquare(this, x * SPRITE_WIDTH, y * SPRITE_HEIGHT));
                    break;
                case Board::bank_square:
                    actors.push_back(new BankSquare(this, x * SPRITE_WIDTH, y * SPRITE_HEIGHT));
                    break;
                case Board::boo:
                    actors.push_back(new Boo(this, x * SPRITE_WIDTH, y * SPRITE_HEIGHT));
                    actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0));
                    break;
                case Board::bowser:
                    actors.push_back(new Bowser(this, x * SPRITE_WIDTH, y * SPRITE_HEIGHT));
                    actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0));
                    break;
                case Board::right_dir_square:
                    actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0));
                    break;
                case Board::up_dir_square:
                    actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 90));
                    break;
                case Board::left_dir_square:
                    actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 180));
                    break;
                case Board::down_dir_square:
                    actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 270));
                    break;
                }
            }
        }
    }

    startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    myPeach->doSomething();
    myYoshi->doSomething();
    for (auto it = actors.begin(); it != actors.end(); it++) {
        if ((*it)->getAlive()) {
            (*it)->doSomething();
        }
    }

    for (auto it = actors.begin(); it != actors.end(); it++) {
        if (!(*it)->getAlive()) {
            delete* it;
            actors.erase(it);
            it = actors.begin();
        }
    }

    string peachText = "P1 Roll: " + to_string(myPeach->getDieRoll()) + " Stars: " + to_string(myPeach->getStars()) + " $$: " + to_string(myPeach->getCoins());
    string yoshiText = "P2 Roll: " + to_string(myYoshi->getDieRoll()) + " Stars: " + to_string(myYoshi->getStars()) + " $$: " + to_string(myYoshi->getCoins());

    if (myPeach->hasVortex())
        peachText = peachText + " VOR";

    if (myYoshi->hasVortex())
        yoshiText = yoshiText + " VOR ";

    setGameStatText(peachText + " | Time: " + to_string(timeRemaining()) + " | " + "Bank: " + to_string(getBankBalance()) + " | " + yoshiText);

    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        if (myPeach->getStars() > myYoshi->getStars()) {
            setFinalScore(myPeach->getStars(), myPeach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (myPeach->getStars() < myYoshi->getStars()) {
            setFinalScore(myYoshi->getStars(), myYoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        else if (myPeach->getCoins() > myYoshi->getCoins()) {
            setFinalScore(myPeach->getStars(), myPeach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (myPeach->getCoins() < myYoshi->getCoins()) {
            setFinalScore(myYoshi->getStars(), myYoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        else {
            int random_roll = rand() % 2 + 1;
            if (random_roll == 1) {
                setFinalScore(myPeach->getStars(), myPeach->getCoins());
                return GWSTATUS_PEACH_WON;
            }
            else {
                setFinalScore(myYoshi->getStars(), myYoshi->getCoins());
                return GWSTATUS_YOSHI_WON;
            }
        }
        return GWSTATUS_NOT_IMPLEMENTED;
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (Actor* myActor : actors) {
        delete myActor;
    }
    actors.clear();

    delete myPeach;
    delete myYoshi;
    myPeach = nullptr;
    myYoshi = nullptr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

bool StudentWorld::isValidPos(int x, int y) {
    if (myBoard.getContentsOf(x, y) == Board::empty) {
        return false;
    }
    return true;
}

// creates a vector of valid teleportation squares and returns the X and Y position of one square in the form of int*
int* StudentWorld::getRandomTeleportationSquare() {
    vector<Actor*> validTeleportationActors;
    int validSquare[2];

    for (int i = 0; i < actors.size(); i++) {
        if (actors.at(i)->stationary()) {
            validTeleportationActors.push_back(actors.at(i));
        }
    }

    int random_roll = rand() % validTeleportationActors.size(); // random roll from 0 to size()-1;
    
    Actor* teleporationActor = validTeleportationActors.at(random_roll);
    validSquare[0] = teleporationActor->getX();
    validSquare[1] = teleporationActor->getY();
    //cerr << validSquare[0] << endl;
    return validSquare;
}

// uses getRandomTeleportationSquare to move myActor to random square on board
void StudentWorld::teleportToRandomSquare(Actor* myActor) {
    int* randomSquare = getRandomTeleportationSquare();
    myActor->moveTo(randomSquare[0], randomSquare[1]);
}

// swap player positions, walk directions, sprite directions, ticks_to_move, walking state
void StudentWorld::swapPlayers() {
    // peach current values
    int peachX = getPeach()->getX();
    int peachY = getPeach()->getY();
    int peachTicksToMove = getPeach()->getTicksToMove();
    int peachWalkDirection = getPeach()->getWalkDirection();
    int peachSpriteDirection = getPeach()->getDirection();
    bool peachIsWalking = getPeach()->isWalking();

    // yoshi current values
    int yoshiX = getYoshi()->getX();
    int yoshiY = getYoshi()->getY();
    int yoshiTicksToMove = getYoshi()->getTicksToMove();
    int yoshiWalkDirection = getYoshi()->getWalkDirection();
    int yoshiSpriteDirection = getYoshi()->getDirection();
    bool yoshiIsWalking = getYoshi()->isWalking();

    //cerr << "Before Teleport, Peach location: " << peachX << ", " << peachY << endl;
    //cerr << "Before Teleport, Yoshi location: " << yoshiX << ", " << yoshiY << endl;

    // set peach values to yoshi values
    getPeach()->moveTo(yoshiX, yoshiY);
    getPeach()->setTicksToMove(yoshiTicksToMove);
    getPeach()->setWalkDirection(yoshiWalkDirection);
    getPeach()->setDirection(yoshiSpriteDirection);
    getPeach()->setWalking(yoshiIsWalking);

    // set yoshi values to peach values
    getYoshi()->moveTo(peachX, peachY);
    getYoshi()->setTicksToMove(peachTicksToMove);
    getYoshi()->setWalkDirection(peachWalkDirection);
    getYoshi()->setDirection(peachSpriteDirection);
    getYoshi()->setWalking(peachIsWalking);

    //cerr << "After Teleport, Peach location: " << getPeach()->getX() << ", " << getPeach()->getY() << endl;
    //cerr << "After Teleport, Yoshi location: " << getYoshi()->getX() << ", " << getYoshi()->getY() << endl;
    //cerr << "~~~~~~~~~~~~~~~~~~" << endl;
}

// swap players' numbers of stars
void StudentWorld::swapStars() {
    int peachStars = getPeach()->getStars();
    int yoshiStars = getYoshi()->getStars();

    getPeach()->setStars(yoshiStars);
    getYoshi()->setStars(peachStars);
}

// swap players' number of coins
void StudentWorld::swapCoins() {
    int peachCoins = getPeach()->getCoins();
    int yoshiCoins = getYoshi()->getCoins();

    getPeach()->setCoins(yoshiCoins);
    getYoshi()->setCoins(peachCoins);
}

// create a new DroppingSquare on x, y. Set old square to dead state.
void StudentWorld::dropSquare(int x, int y) {
    int size = actors.size();
    for (int i = 0; i < size; i++) {
        if (actors.at(i)->getX() == x && actors.at(i)->getY() == y && actors.at(i)->stationary() && actors.at(i)->getAlive()) {
            actors.at(i)->setDead();
            actors.at(i)->setVisible(false);
            break;
        }
    }
    actors.push_back(new DroppingSquare(this, (x * SPRITE_WIDTH) / 16, (y * SPRITE_HEIGHT) / 16));
}

// returns true if two actors' sprites are intersecting. false if else.
bool StudentWorld::intersecting(Actor* a, Actor* b) {
    return intersecting(a->getX(), a->getY(), b->getX(), b->getY());
}

// returns true if two coordinates are intersecting. false if else.
bool StudentWorld::intersecting(int x1, int y1, int x2, int y2) {
    if (x1 + SPRITE_WIDTH > x2 && x1 < x2 + SPRITE_WIDTH) {
        if (y1 + SPRITE_HEIGHT > y2 && y1 < y2 + SPRITE_HEIGHT) {
            return true;
        }
    }
    return false;
}

// returns true if an actor is intersecting a impactable actor. false if else.
bool StudentWorld::findImpactable(Actor* myVortex) {
    bool impactableFound = false;
    for (auto a : actors) {
        if (intersecting(myVortex, a)) {
            if (a->isImpactable()) {
                impactableFound = true;
                a->setHit(true);
            }
        }
    }
    if (impactableFound) {
        myVortex->setDead();
        playSound(SOUND_HIT_BY_VORTEX);
        return true;
    }
    return false;
}

// creates new Vortex at x,y with direction
void StudentWorld::newVortex(int x, int y, int direction) {
    actors.push_back(new Vortex(this, x, y, direction));
}

// returns true if actor is touchingPeach. false if else.
bool StudentWorld::touchingPeach(Actor* actor) {
    int actorX = actor->getX();
    int actorY = actor->getY();
        if (actorX == getPeach()->getX() && actorY == getPeach()->getY()) {
        return true;
    }
    return false;
}

// returns true if actor is touchingYoshi. false if else.
bool StudentWorld::touchingYoshi(Actor* actor) {
    int actorX = actor->getX();
    int actorY = actor->getY();
        if (actorX == getYoshi()->getX() && actorY == getYoshi()->getY()) {
        return true;
    }
    return false;
}