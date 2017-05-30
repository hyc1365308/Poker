//
//   by DarkProbe
//   15/4/2017
//

#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Card.h"
#include "../room.h"
#include <vector>

const int MIN_BET = 100;

class Room;

class Game{
public:
	Player* _banker;
	Player* _presentPlayer;
	Player* _maxBetPlayer;
	Room*   _room;

	Card    _cardList[52];
	Card    _publicCard[5];
	int     _cardIndex;
	void    cardShuffle();

	int     _maxBet;
	int     _playerCount;
	Game(std::vector<Player*> v, Room* r);
	void blindBet();
	void init();
	void start();
	void betTurn();
	void lcsPlayer();
	void lcsPublic(int);
	Operate* getOperate();

	void calcPattern();
	void calcResult();
	void showResult();

	//network interfaces;
	Operate* n_getOperate(Player*);
	void     n_licensePlayer(Player*, Card, Card);
	void     n_licensePublic(int);
	void     n_fresh();
	void     n_showResult();
};

#endif