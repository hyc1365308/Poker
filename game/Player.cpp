#include "Player.h"
#include <iostream>

void Player::bet(int money){
	_money -= money;
	_presentBet += money;
}

void Player::license(Card cd1, Card cd2){
	_card[0] = cd1;
	_card[1] = cd2;
}

void Player::print(){
	std::cout<<"name:"<<_name<<std::endl;
	printf("_money: %d, _presentBet: %d, _gameResult: %d\n", _money, _presentBet, _gameResult);
	printf("_fold : %d,      _allin: %d\n", _fold,  _allin);
	std::cout<<"Card: "<<_card[0]<<", "<<_card[1]<<std::endl<<std::endl;
}

//void Operate::doIt(Player* p){
//	printf("DEFAULT!\n");
//};

void Fold::doIt(Player* p){
	printf("FOLD\n");
	p->_fold = true;
}

void Allin::doIt(Player* p){
	printf("ALLIN!\n");
	_money = p->_money;
	p->_presentBet += p->_money;
	p->_money = 0;
	p->_allin = true;
}

void Bet::doIt(Player* p){
	printf("Bet %d!\n", _money);
	p->_presentBet += _money;
	p->_money -= _money;
}

int Fold::getType(){
	return FOLD;
}

int Allin::getType(){
	return ALLIN;
}

int Refuel::getType(){
	return REFUEL;
}

int Call::getType(){
	return CALL;
}

int Fold::getMoney(){
	return 0;
}

int Allin::getMoney(){
	return _money;
}

int Bet::getMoney(){
	return _money;
}

Bet::Bet(int money): _money(money){

}