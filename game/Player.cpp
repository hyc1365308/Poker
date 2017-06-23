//
//   by DarkProbe
//   2017-4-15
//

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