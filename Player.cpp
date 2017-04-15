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
	printf("_money: %d, _presentBet: %d\n", _money, _presentBet);
	printf("_fold : %d,      _allin: %d\n", _fold,  _allin);
	printf("Card: %d, %d\n\n", _card[0].num, _card[1].num);
}

void Operate::doIt(Player* p){
	printf("DEFAULT!\n");
};

void Fold::doIt(Player* p){
	printf("FOLD\n");
	p->_fold = true;
}

void Allin::doIt(Player* p){
	printf("ALLIN!\n");
	p->_presentBet += p->_money;
	p->_money = 0;
	p->_allin = true;
}

Bet::Bet(int money): _money(money){

}

void Bet::doIt(Player* p){
	printf("Bet %d!\n", _money);
	p->_presentBet += _money;
	p->_money -= _money;
}