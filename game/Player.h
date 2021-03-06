//
//   by Huang Yueqin
//   2017-4-15
//

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "pattern.h"
#include "Card.h"

using std::string;

class Player{
	friend class Game;
	string _name;
	int    _money;
	int    _presentBet;//bet money in this game
	int	   _gameResult;//money change in this game
	bool   _mark;//used in game result calculation

	bool   _fold;
	bool   _allin;
	Card   _card[2];
	Pattern _pattern;

	Player* _lastPlayer;
	Player* _nextPlayer;
	void license(Card cd1, Card cd2);
public:
	void print();
	Player(string name, int money):
		 _name(name), _money(money){

	}
	bool skip(){
		return _fold || _allin;
	}
	void bet(int money);
	string getName() const{ return _name; }
	int getMoney() const{ return _money; }
	void setFold() { _fold = true;}
	void setAllin() { _allin = true;}
};


#endif