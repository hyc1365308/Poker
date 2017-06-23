//
//   by Huang Yueqin
//   2017-4-15
//

#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Operate.h"
#include "Card.h"
#include "../room.h"
#include <vector>

const int MIN_BET = 100;

class Room;

class Game{
	Player* _banker;
	Player* _presentPlayer; 
	Player* _maxBetPlayer;
	Room*   _room;

	Card    _cardList[52];
	Card    _publicCard[5];
	int     _cardIndex;    //record the next card to be licensed
	void    cardShuffle();

	int     _maxBet;       //record the max bet money
	int     _playerCount;  //record the players that still and call/refuel/allin
	void blindBet();
	void init();
	void betTurn();
	void lcsPlayer();      //license card for player
	void lcsPublic(int);   //license card for public card
	Operate* getOperate(); //get a operate from _presentPlayer

	void calcPattern();    //calculate pattern for all players
	void calcResult();     //calculate game result
	void showResult();     //show game result

	//network interfaces:
	Json::Value n_getOperate(Player*);
	void     n_castOperate(Player* , Operate* );
	void     n_licensePlayer(Player*, Card & , Card & );
	void     n_licensePublic(int, Card & );
	void     n_fresh(); // TODO
	void     n_showResult(std::vector<std::tuple<int, Card, Card>>);
public:
	Game(std::vector<Player*> v, Room* r);
	void start();
};

#endif