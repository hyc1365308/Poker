//
//   by DarkProbe
//   15/4/2017
//

#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Card.h"

const int MIN_BET = 100;

class Game{
public:
	Player* _banker;
	Player* _presentPlayer;
	Player* _maxBetPlayer;

	Card    _cardList[52];
	Card    _publicCard[5];
	int     _cardIndex;
	void    cardShuffle();

	int     _maxBet;
	int     _playerCount;
	Game(Player* firstBanker);
	void blindBet();
	void init();
	void start();
	void betTurn();
	void lcsPlayer();
	void lcsPublic(int);
	Operate* getOperate();
};

#endif