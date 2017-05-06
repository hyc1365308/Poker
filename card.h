/*
 * Author   : Feng Yulin
 * Time     : 2017-04-18(1st)
*/

#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string.h>

enum Suit
{
    HEARTS = 0, 
    DIAMONDS = 1, 
    CLUB = 2, 
    SPADE = 3
};

struct Card
{
    Suit suit;
    int num;
    Card() {}
    Card(int s, int n)
    {
        suit = Suit(s);
        num = n;
    }

    bool operator< (const Card & right)
    {
        // compare two card by num(A,2,3,...,K)
        if (right.num == 1)
        {
            return (num == 1) ? false : true;
        }
        else if (num == 1)
        {
            return false;
        }
        else{
            return num < right.num;
        }
    }

    bool operator> (const Card & right)
    {
        // compare two card by num(A,2,3,...,K)
        if (right.num == 1)
        {
            return false;
        }
        else if (num == 1)
        {
            return true;
        }
        else{
            return num > right.num;
        }
    }

    int operator+ (const int r)
    {
        return num + r;
    }

    int operator- (const int r)
    {
        return num - r;
    }

    int operator- (const Card & right)
    {
        // left.num - right.num, A = 14
        if (num == 1 && right.num != 1)
        {
            return num + 13 - right.num;
        }
        else if (num != 1 && right.num == 1)
        {
            return num - (right.num + 13);
        }
        else
        {
            return num - right.num;
        }
    }

    bool operator== (const int n)
    {
        return num == n;
    }

    bool operator>= (const int n)
    {
        return num >= n;
    }

    bool operator== (const Card & right)
    {
        return num == right.num;
    }

    bool operator!= (const int n)
    {
        return num != n;
    }

    bool operator!= (const Card & right)
    {
        return num != right.num;
    }

    friend std::ostream & operator<< (std::ostream & out, const Card c)
    {
        std::string suit_str;
        switch (c.suit)
        {
            case HEARTS:
                suit_str = "H";
                break;
            case DIAMONDS:
                suit_str = "D";
                break;
            case CLUB:
                suit_str = "C";
                break;
            case SPADE:
                suit_str = "S";
                break;
            default:
                printf("suit is %d, which is beyond the range\n", c.suit);
                assert(false);
                break;
        }
        out << suit_str << c.num;
        return out;
    }
};
