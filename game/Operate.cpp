//
//   by DarkProbe
//   2017-6-20
//

#include "Operate.h"

void Fold::doIt(Player* p){
	printf("FOLD\n");
	p->setFold();
}

void Allin::doIt(Player* p){
	printf("ALLIN!\n");
	_money = p->getMoney();
	p->bet(_money);
	p->setAllin();
}

void Bet::doIt(Player* p){
	printf("Bet %d!\n", _money);
	p->bet(_money);
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