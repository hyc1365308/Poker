#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include "Game.h"

Game::Game(Player* firstBanker): _banker(firstBanker), _presentPlayer(firstBanker){
}

void Game::start(){
	//preflop
	init();
	blindBet();
	lcsPlayer();
	betTurn();

	//flop
	printf("Flop\n");
	lcsPublic(0);
	lcsPublic(1);
	lcsPublic(2);
	betTurn();

	//turn
	printf("Turn\n");
	lcsPublic(3);
	betTurn();

	//river
	printf("River\n");
	lcsPublic(4);
	betTurn();
}

//init all bet of Players, _playerCount
void Game::init(){
	cardShuffle();
	Player* p = _presentPlayer;
	_playerCount = 0;
	do{
		_playerCount ++;
		printf("money = %d\n", p->_money);
		p->_presentBet = 0;
		p->_fold = false;
		p->_allin = false;
		p = p->_nextPlayer;
	}while(p != _presentPlayer);
	_maxBet = 0;
}

//this func will init _maxBetPlayer & _maxBet
void Game::blindBet(){
	_presentPlayer->bet(MIN_BET);
	_presentPlayer = _presentPlayer->_nextPlayer;
	_presentPlayer->bet(MIN_BET * 2);
	_maxBetPlayer = _presentPlayer;
	_presentPlayer = _presentPlayer->_nextPlayer;
	_maxBet = MIN_BET * 2;
}

void Game::betTurn(){
	do{
		if (_playerCount == 1)
			return;

		if (_presentPlayer->skip()){
			_presentPlayer = _presentPlayer->_nextPlayer;
			continue;
		}

		Operate* t = getOperate();
		t->doIt(_presentPlayer);
		delete t;
		_presentPlayer->print();
		if (_presentPlayer->_fold)
			_playerCount --;
		if (_presentPlayer->_presentBet > _maxBet){
			_maxBetPlayer = _presentPlayer;
			_maxBet = _presentPlayer->_presentBet;
		}
		_presentPlayer = _presentPlayer->_nextPlayer;
	}while(_presentPlayer != _maxBetPlayer);
}

Operate* Game::getOperate(){
	//use a naive first
	int a,b;
	scanf("%d %d", &a, &b);
	switch(a){
		case 0:
			return new Fold();
		case 1:
			return new Allin();
		default:
			return new Bet(b);
	}
}

void Game::cardShuffle(){
	_cardIndex = 0;
	int temp[52];
	for (int i = 0; i < 52; i++)
		temp[i] = i + 1;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(temp, temp + 52, std::default_random_engine(seed));
	for (int i = 0; i < 52; i++)
		_cardList[i].num = temp[i];
}

void Game::lcsPlayer(){
	Player* p = _presentPlayer;
	do{
		p->license(_cardList[_cardIndex],_cardList[_cardIndex + 1]);
		_cardIndex += 2;
		p = p->_nextPlayer;
	}while(p != _presentPlayer);
}

void Game::lcsPublic(int i){
	_publicCard[i] = _cardList[_cardIndex];
	_cardIndex ++;
}

int main(){
	Player *t, *t2, *t3;
	t = new Player("one", 10000);
	t2 = new Player("two", 20000, t);
	t3 = new Player("three", 30000, t2, t);
	t->_lastPlayer = t3;
	t->_nextPlayer = t2;
	t2->_nextPlayer = t3;
	Game game(t);
	game.start();
	return 0;
}