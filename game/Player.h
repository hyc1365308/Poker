//
//   by DarkProbe
//   15/4/2017
//

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "pattern.h"
#include "Card.h"

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
	virtual void doIt(Player* p);
};

class Fold: public Operate{
public:
	void doIt(Player* p);
};

class Allin: public Operate{
public:
	void doIt(Player* p);
};

class Bet: public Operate{
	int _money;
public:
	Bet(int money);
	void doIt(Player* p);
};

#endif