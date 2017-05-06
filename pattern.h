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
    HandPattern type;
    Card card[5];

    Card & operator[] (int i)
    {
        assert(i < 5 && i >= 0);
        return card[i];
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
