//
//   by DarkProbe
//   15/4/2017
//

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "pattern.h"
#include "Card.h"
#include "../packet.h"

using std::string;

class Operate;

class Player{
	friend class Operate;
public:
	string _name;
	int    _money;
	int    _presentBet;
	int	   _gameResult;
	bool   _mark;//used in game result calculation

	bool   _fold;
	bool   _allin;
	Card   _card[2];
	Pattern _pattern;

	Player* _lastPlayer;
	Player* _nextPlayer;
	Player(string name, int money, Player* lastPlayer = NULL, Player* nextPlayer = NULL):
		 _name(name), _money(money), _lastPlayer(lastPlayer), _nextPlayer(nextPlayer){

	}
	void bet(int money);
	bool skip(){
		return _fold || _allin;
	}
	void license(Card cd1, Card cd2);
	void print();
};

class Operate{
public:
	virtual void doIt(Player* p) = 0;
	virtual int getType() = 0;
	virtual int getMoney() = 0;
};

class Fold: public Operate{
public:
	void doIt(Player* p);
	int getType();
	int getMoney();
};

class Allin: public Operate{
	int _money;
public:
	void doIt(Player* p);
	int getType();
	int getMoney();
};

class Bet: public Operate{
	int _money;
public:
	Bet(int money);
	void doIt(Player* p);
	int getType() = 0;
	int getMoney();
};

class Refuel: public Bet{
public:
	Refuel(int money): Bet(money) {};
	int getType();
};

class Call: public Bet{
public:
	Call(int money): Bet(money) {};
	int getType();
};


#endif