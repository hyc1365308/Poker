/*
 * Author   :   Feng Yulin
 * Time     :   2017-05-03(1st)
 * Function :   Define hand pattern struct
*/

#ifndef PATTERN_H
#define PATTERN_H

#include "card.h"

#include <iostream>

enum HandPattern
{
    STRAIGHT_FLUSH,
    FOUR_OF_A_KIND,
    FULL_HOUSE,
    FLUSH,
    STRAIGHT,
    THREE_OF_A_KIND,
    TWO_PAIR,
    ONE_PAIR,
    HIGH_CARD
};

struct Pattern
{
    HandPattern type;       // hand type
    Card card[5];           // the largest five cards

    Card & operator[] (const int i)
    {
        assert(i < 5 && i >= 0);
        return card[i];
    }

    bool operator< (const Pattern & r)
    {
        if (type > r.type)
        {
            return true;
        }
        else if (type == r.type)
        {
            for (int i = 0; i < 5; ++i)
            {
                if (card[i] < r.card[i])
                {
                    return true;
                }
                else if (card[i] > r.card[i])
                {
                    return false;
                }
            }
        }

        return false;
    }

    bool operator== (const Pattern & r)
    {
        if (type == r.type)
        {
            for (int i = 0; i < 5; ++i)
            {
                if (card[i] != r.card[i])
                {
                    return false;
                }
            }
            return true;
        }

        return false;
    }

    bool operator> (const Pattern & r)
    {
        if (type < r.type)
        {
            return true;
        }
        else if (type == r.type)
        {
            for (int i = 0; i < 5; ++i)
            {
                if (card[i] > r.card[i])
                {
                    return true;
                }
                else if (card[i] < r.card[i])
                {
                    return false;
                }
            }
        }

        return false;
    }

    friend std::ostream & operator<< (std::ostream & out, const Pattern & p)
    {
        static std::string type_str[9] = {
            "STRAIGHT_FLUSH", 
            "FOUR_OF_A_KIND", 
            "FULL_HOUSE", 
            "FLUSH", 
            "STRAIGHT", 
            "THREE_OF_A_KIND", 
            "TWO_PAIR", 
            "ONE_PAIR", 
            "HIGH_CARD"
        };
        out << p.type << " " << type_str[p.type] << " ";
        for (int i = 0; i < 5; ++i)
        {
            out << p.card[i] << " ";
        }
        out << std::endl;
        return out;
    }
};

Pattern getPattern(Card* in_hand);

#endif
