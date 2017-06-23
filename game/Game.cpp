//
//   by Huang Yueqin
//   2017-4-15
//


#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include "Game.h"
#include "compare.cpp"

using std::cout;
using std::endl;
using std::string;

Game::Game(std::vector<Player*> v, Room* r): _banker(v[0]), _presentPlayer(v[0]), _room(r){
	int length = v.size();
	for (int i = 0; i < length; i++){
		v[i]->_nextPlayer = v[(i + 1) % length];
		v[i]->_lastPlayer = v[(i + length - 1) % length];
	}
	cout<<"init end"<<endl;
	for (int i = 0; i < length; i++){
		cout<<v[i]->_nextPlayer->_name<<" ";
		cout<<v[i]->_lastPlayer->_name<<" ";
		cout<<v[i]->_money<<endl;
	}

	cout << "room id is " << _room->get_id() << endl;
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

	//result
	printf("CalcPattern\n");
	calcPattern();
	printf("CalcResult\n");
	calcResult();
	showResult();
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
		p->_gameResult = 0;
		p->_fold = false;
		p->_allin = false;
		p = p->_nextPlayer;
	}while(p != _presentPlayer);
	_maxBet = 0;
}

//this func will init _maxBetPlayer & _maxBet
//and then do blindBet
void Game::blindBet(){
	cout<<"doing blindBet.....";
	_presentPlayer->bet(MIN_BET);
	_room->castOperate(_presentPlayer, REFUEL, _presentPlayer->_money, MIN_BET, true);
	_presentPlayer = _presentPlayer->_nextPlayer;
	_presentPlayer->bet(MIN_BET * 2);
	_room->castOperate(_presentPlayer, REFUEL, _presentPlayer->_money, MIN_BET * 2, true);
	_maxBetPlayer = _presentPlayer;
	_presentPlayer = _presentPlayer->_nextPlayer;
	_maxBet = MIN_BET * 2;
	cout<<"done"<<endl;
}

// The main process of the game
// Do a bet turn
void Game::betTurn(){
	cout<<"doing betTurn.....";
	do{
		//n_fresh();
		cout<<_playerCount<<endl;
		if (_playerCount == 0)
			return;

		if ((_playerCount == 1) && ( _presentPlayer->_money == _maxBet))
			return;

		if (_presentPlayer->skip()){
			_presentPlayer = _presentPlayer->_nextPlayer;
			continue;
		}

		Operate* t = getOperate();
		t->doIt(_presentPlayer);
		n_castOperate(_presentPlayer, t);
		delete t;
		_presentPlayer->print();
		if ( _presentPlayer->skip() )
			_playerCount --;
		if (_presentPlayer->_presentBet > _maxBet){
			_maxBetPlayer = _presentPlayer;
			_maxBet = _presentPlayer->_presentBet;
		}
		_presentPlayer = _presentPlayer->_nextPlayer;
	}while(_presentPlayer != _maxBetPlayer);
	cout<<"done"<<endl;
}

// Get a operate from the player: _presentPlayer
// And return the operate
Operate* Game::getOperate(){
	cout<<"ready for operate..."<<endl;
	//use a naive implementation first
	//int a,b;
	//scanf("%d %d", &a, &b);
	//switch(a){
	//	case 0:
	//		return new Fold();
	//	case 1:
	//		return new Allin();
	//	default:
	//		return new Bet(b);
	//}
	Json::Value temp = n_getOperate(_presentPlayer);
	if (temp["type"] == FOLD){
		cout<<"done"<<endl;
		return new Fold();
	}
	if (temp["type"] == ALLIN){
		cout<<"done"<<endl;
		return new Allin();
	}
	if (temp["type"] == CALL){
		cout<<"done"<<endl;
		return new Call(_maxBet - _presentPlayer->_presentBet);
	}
	if (temp["type"] == REFUEL){
		cout<<"done"<<endl;
		return new Refuel(temp["money"].asInt());
	}
	cout<<"invalid operate!"<<endl;
}

// Shuffle the cards before game start.
//
void Game::cardShuffle(){
	_cardIndex = 0;
	int temp[52];
	for (int i = 0; i < 52; i++)
		temp[i] = i + 1;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(temp, temp + 52, std::default_random_engine(seed));
	for (int i = 0; i < 52; i++){
		_cardList[i].num = (temp[i] % 13) + 1;
		_cardList[i].suit = Suit(temp[i] % 4);
	}
}

// license 2 cards to every player
// send license info to every player
//
void Game::lcsPlayer(){
	cout<<"doing lcsPlayer...";
	Player* p = _presentPlayer;
	do{
		p->license(_cardList[_cardIndex],_cardList[_cardIndex + 1]);
		n_licensePlayer(p, _cardList[_cardIndex], _cardList[_cardIndex + 1]);
		_cardIndex += 2;
		p = p->_nextPlayer;
	}while(p != _presentPlayer);
	cout<<"done"<<endl;
}

// license public card with index i
// and broadcast the license information
//
void Game::lcsPublic(int i){
	cout<<"doing lcsPublic...";
	_publicCard[i] = _cardList[_cardIndex];
	_cardIndex ++;
	n_licensePublic(i + 1, _publicCard[i]);
	cout<<"done"<<endl;
}

// calculate the pattern for every player
// the results are saved in p->_pattern
//
void Game::calcPattern(){
	cout<<"calculating pattern...";
	Card temp[7];
	for (int i = 0; i < 5; i++)
		temp[i] = _publicCard[i];
	Player* p = _presentPlayer;
	do{
		if ( !p->_fold ){
			temp[5] = p->_card[0];
			temp[6] = p->_card[1];
			p->_pattern = getPattern(temp);
		}
		p = p->_nextPlayer;
	}while(p != _presentPlayer);
	cout<<"done"<<endl;
}

// calculate the game result based on _pattern, _fold, _allin of every player
// the results are saved in p->_gameResult
//
void Game::calcResult(){
	//init _gameResult and _mark
	Player* p = _presentPlayer;
	do{
		p->_mark = true;
		p->_money += p->_presentBet;
		p = p->_nextPlayer;
	}while(p != _presentPlayer);

	// First find allin player in every loop
	// Ignore all players with _presentBet = 0
	// If there exists players already allin, 
	//    the betMoney in this loop is the lowest _presentBet * _playerCount + money of fold players
	// If not, the betMoney in this loop is all the rest money
	// Calculate winners, and the betMoney is divided equally to them( add to Player->_money)
	// The money divided this loop will be deducted from Player->_presentBet
	while(true){
		cout<<"flag1"<<endl;
		p = _presentPlayer;
		Player* allin = NULL;
		int allinMoney = 0;
		//find allin player
		do{
			//find the allin player with lowest bet
			if ((p->_allin) && (p->_presentBet != 0)){
				if ((allin == NULL) || (p->_presentBet < allinMoney)){
					allin = p;
					allinMoney = p->_presentBet;
				}
			}
			p = p->_nextPlayer;
		}while(p != _presentPlayer);
		cout<<"flag2"<<endl;

		//if no one allin
		if (allin == NULL){
			cout<<"No allin"<<endl;
			//calculate money in this circle
			//calculate the biggest pattern and count the winner
			int moneyThisCircle = 0;
			Pattern* biggestPattern = NULL;
			int winnerCount = 0;

			p = _presentPlayer;
			do{
				//ignore players with _presentBet = 0
				if (p->_presentBet == 0){
					p->_mark = false;
					p->print();
					p = p->_nextPlayer;
					continue;
				}

				// sum the money in this loop
				moneyThisCircle += p->_presentBet;
				p->_money -= p->_presentBet;
				p->_gameResult -= p->_presentBet;
				p->_presentBet = 0;

				//ignore fold players
				if (p->_fold){
					p->_mark = false;
					p->print();
					p = p->_nextPlayer;
					continue;
				}

				if ((biggestPattern == NULL) || (p->_pattern > (*biggestPattern))){
					biggestPattern = &(p->_pattern);
					winnerCount = 1;
				}else if(p->_pattern == (*biggestPattern)){
					winnerCount ++;
				}

				p = p->_nextPlayer;
			}while(p != _presentPlayer);
			//no biggest patter:the game has end.
			if (biggestPattern == NULL)
				return;

			//update game result
			cout<<"the biggest pattern is :"<< (*biggestPattern)<<endl;
			int moneyPerWinner = moneyThisCircle / winnerCount;

			// If a player is marked, he will not attend the rest game, including pattern comparation.
			p = _presentPlayer;
			do{
				if ( !p->_mark ){
					p = p->_nextPlayer;
					continue;
				}
				cout<<"no allin_calc"<<endl;
				if (p->_pattern == (*biggestPattern)){
					p->_money += moneyPerWinner;
					p->_gameResult += moneyPerWinner;
				}
				p->print();
				p = p->_nextPlayer;
			}while(p != _presentPlayer);

			break;
		}
		cout<<"flag3"<<endl;

		//someone allin

		//calculate money in this circle
		//calculate the biggest pattern and count
		int moneyThisCircle = 0;
		Pattern* biggestPattern = NULL;
		int winnerCount = 0;

		p = allin;
		cout<<"allin"<<endl;
		do{
			//ignore players with _presentBet = 0
			if (p->_presentBet == 0){
				p->_mark = false;
				p->print();
				p = p->_nextPlayer;
				continue;
			}

			// sum the money in this loop
			if (p->_fold){
				moneyThisCircle += p->_presentBet;
				p->_money -= p->_presentBet;
				p->_gameResult -= p->_presentBet;
				p->_presentBet = 0;
				p->_mark = false;
				p->print();
				p = p->_nextPlayer;
				continue;
			}

			moneyThisCircle += allinMoney;
			p->_money -= allinMoney;
			p->_gameResult -= allinMoney;
			p->_presentBet -= allinMoney;

			if ((biggestPattern == NULL) || (p->_pattern > (*biggestPattern))){
				biggestPattern = &(p->_pattern);
				winnerCount = 1;
			}else if(p->_pattern == (*biggestPattern)){
				winnerCount ++;
			}
			p->print();

			p = p->_nextPlayer;
		}while(p != allin);
		cout<<"flag4"<<endl;

		//update game result
		int moneyPerWinner = moneyThisCircle / winnerCount;
		p = allin;
		cout<<"the biggest pattern is :"<< (*biggestPattern)<<endl;
		// If a player is marked, he will not attend the rest game, including pattern comparation.
		do{
			if ( !p->_mark ){
				p = p->_nextPlayer;
				continue;
			}
			cout<<"allin_calc"<<endl;
			if (p->_pattern == (*biggestPattern)){
				p->_money += moneyPerWinner;
				p->_gameResult += moneyPerWinner;
			}
			p->print();
			p = p->_nextPlayer;
		}while(p != allin);
	}
}

// show the game result
// send the info to all players
void Game::showResult(){
	std::vector<std::tuple<int, Card, Card>> result;
	Player* p = _banker;
	do{
		//p->_money += p->_gameResult;
		p->print();
		if ( !p->_fold )
			cout<<p->_pattern<<endl;
		else
			cout<<"The player has folded"<<endl;

		int cur_money = p->_money;
		Card card1    = p->_card[0];
		Card card2    = p->_card[1];

		result.push_back(std::make_tuple(cur_money, card1, card2));
		p = p->_nextPlayer;
	}while(p != _banker);

	
	n_showResult(result);
}

//-------------------------- network interfaces -----------------------------------
Json::Value Game::n_getOperate(Player* p){
	//get a operate from Player p's socket and return the operate
	return _room->getOperate(p, _maxBet);
}

void Game::n_castOperate(Player* p, Operate* op){
	//send a bet operate to all players
	//Player*  p  : the player that did the operation
	//Operate* op : the operation
	return _room->castOperate(p, op->getType(), p->_money, p->_presentBet);
}

void Game::n_licensePlayer(Player* p, Card & c1, Card & c2){
	//tell Player p :he was licensed 2 Card: c1 & c2
	//Player* p   : the player to be licenced
	//Card    c1  : the 1st card
	//Card    c2  : the 2nd card
	_room->licensePlayer(p, c1);
	_room->licensePlayer(p, c2);
}

void Game::n_licensePublic(int index, Card & card){
	//tell all Players about first index public card(stored in _publicCard[5])
	_room->licensePublic(index, card);
}

void Game::n_fresh(){
	//TODO
	//tell all Players about bet conditions of all players(stored in Player* _presentPlayer and other Player*)
	//int    _money
	//int    _presentBet
}

void Game::n_showResult(std::vector<std::tuple<int, Card, Card>> result){
	//tell all Players about game result
	//int    _money
	//int    _gameResult(record the money changes in this game)
	_room->showResult(result);
}


/*int main(){
	Player *t, *t2, *t3;
	t = new Player("one", 10000);
	t2 = new Player("two", 20000);
	t3 = new Player("three", 30000);
	std::vector<Player*> v;
	v.push_back(t);
	v.push_back(t2);
	v.push_back(t3);
	//t->_lastPlayer = t3;
	//t->_nextPlayer = t2;
	//t2->_nextPlayer = t3;
	Room* r;
	Game game(v, r);
	game.start();
	return 0;
}*/