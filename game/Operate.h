//
//   by DarkProbe
//   2017-6-20
//

#ifndef OPERATE_H
#define OPERATE_H

#include <string>
#include "Player.h"
#include "../packet.h"

using std::string;

class Operate{
public:
	virtual void doIt(Player* p) = 0;
	virtual int getType() = 0;
	virtual int getMoney() = 0;
};

class Fold: public Operate{
public:
	void doIt(Player* p);
	int getType();
	int getMoney();
};

class Allin: public Operate{
	int _money;
public:
	void doIt(Player* p);
	int getType();
	int getMoney();
};

class Bet: public Operate{
	int _money;
public:
	Bet(int money);
	void doIt(Player* p);
	int getType() = 0;
	int getMoney();
};

class Refuel: public Bet{
public:
	Refuel(int money): Bet(money) {};
	int getType();
};

class Call: public Bet{
public:
	Call(int money): Bet(money) {};
	int getType();
};


#endif