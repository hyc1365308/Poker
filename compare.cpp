/*
 * Author   :   Feng Yulin
 * Time     :   2017-04-07(1st)
 * Function :   Compare two hands
*/

#include <iostream>
#include <assert.h>
#include <string.h>

using std::cin;
using std::cout;
using std::endl;
using std::ostream;
using std::string;

enum Suit
{
    HEARTS = 0, 
    DIAMONDS = 1, 
    CLUB = 2, 
    SPADE = 3
};

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

struct Card
{
    Suit suit;
    short num;
    Card() {}
    Card(int s, int n)
    {
        suit = Suit(s);
        num = n;
    }

    bool operator< (const Card & right)
    {
        if (right.num == 1)
        {
            return (num == 1) ? false : true;
        }
        else if (num == 1)
        {
            return false;
        }

        return num < right.num;
    }

    short operator- (const Card & right)
    {
        if (num == 1 && right.num != 1)
        {
            return num + 13 - right.num;
        }
        else if (num != 1 && right.num == 1)
        {
            return num - (right.num + 13);
        }
        return num - right.num;
    }

    bool operator== (const int n)
    {
        return num == n;
    }

    bool operator== (const Card & right)
    {
        return num == right.num;
    }

    bool operator!= (const int n)
    {
        return num != n;
    }

    friend ostream & operator<< (ostream & out, const Card c)
    {
        out << c.num;
        return out;
    }
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

    friend ostream & operator<< (ostream & out, const Pattern & p)
    {
        // char* chr[4] = {"hearts", "diamonds", "club", "spade"};
        string chr[9] = {
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
        out << p.type << " " << chr[p.type] << " ";
        for (int i = 0; i < 5; ++i)
        {
            out << p.card[i] << " ";
        }
        out << endl;
        return out;
    }
};

template <typename T>
void swap(T & a, T & b)
{
    T c = a;
    a = b;
    b = c;
}

Pattern getPattern(Card* hand)
{
    Pattern pat;
    bool is_straight = true;
    bool is_flush = true;

    // judge whether is straight
    const Suit s = hand[0].suit;
    for (int i = 1; i <= 4; ++i)
    {
        if (hand[i].suit != s)
        {
            is_straight = false;
            break;
        }
    }

    // sort these cards
    for (int i = 0; i < 5; ++i)
    {
        pat[i] = hand[i];
    }

    for (int i = 0; i <= 3; ++i)
    {
        for (int j = i + 1; j <= 4; ++j)
        {
            if (pat[i] < pat[j])
            {
                swap(pat[i], pat[j]);
            }
        }
    }
    // Attention: A < 2 < 3 < ... < Q < K, need to think about A alone

    for (int i = 0; i < 5; ++i)
    {
        cout << pat.card[i].num << " ";
    }
    cout << endl;

    // judge whether is flush
    for (int i = 0; i <= 3; ++i)
    {
        if (pat[i] - pat[i + 1] != 1)
        {
            is_flush = false;
            break;
        }
    }

    // judge A 5 4 3 2
    if (pat[0] == 1)
    {
        is_flush = true;
        for (int i = 1; i <= 4; ++i)
        {
            if (pat[i] != 6 - i)
            {
                is_flush = false;
                break;
            }
        }
    }

    if (is_flush || is_straight)
    {
        if (!is_flush)
        {
            pat.type = STRAIGHT;
        }
        else if (!is_straight)
        {
            pat.type = FLUSH;
        }
        else
        {
            pat.type = STRAIGHT_FLUSH;
        }

        return pat;
    }

    // the hand pattern isn't flush nor straight
    bool is_equal[4];
    for (int i = 0; i < 4; ++i)
    {
        is_equal[i] = (pat[i] == pat[i + 1]);
    }

    int equal_num = is_equal[0] + is_equal[1] + is_equal[2] + is_equal[3];

    if (equal_num == 3)
    {
        if (is_equal[1] && is_equal[2])
        {
            // 4 + 1 : T T T F | F T T T
            pat.type = FOUR_OF_A_KIND;
            if (is_equal[3])
            {
                // F T T T, eg. 8 4 4 4 4 --> 4 4 4 4 8
                swap(pat[0], pat[4]);
            }
        }
        else
        {
            // 3 + 2 : T T F T | T F T T
            pat.type = FULL_HOUSE;
            if (is_equal[2])
            {
                // T F T T, eg. 3 3 2 2 2 --> 2 2 2 3 3
                swap(pat[0], pat[4]);
                swap(pat[1], pat[3]);
            }
        }
    }
    else if (equal_num == 2)
    {
        if ((is_equal[0] && is_equal[1]) || (is_equal[1] && is_equal[2]) || (is_equal[2] && is_equal[3]))
        {
            // 3 + 1 + 1 : T T F F | F T T F | F F T T
            pat.type = THREE_OF_A_KIND;
            if (is_equal[1] && is_equal[2])
            {
                // F T T F eg. 8 7 7 7 3 --> 7 7 7 8 3
                swap(pat[0], pat[3]);
            }
            else if (is_equal[2] && is_equal[3])
            {
                // F F T T eg. 8 7 4 4 4 --> 4 4 4 8 7
                swap(pat[0], pat[3]);
                swap(pat[1], pat[4]);
            }
        }
        else
        {
            // 2 + 2 + 1 : T F T F | T F F T | F T F T
            pat.type = TWO_PAIR;
            if (is_equal[0] && is_equal[3])
            {
                // T F F T eg. 8 8 6 5 5 --> 8 8 5 5 6
                // cout << "before swap:\n";
                // cout << pat[2].num << " " << pat[4].num << endl;
                swap(pat[2], pat[4]);
                // cout << "after swap:" << endl;
                // cout << pat[2].num << " " << pat[4].num << endl;
            }
            else if (is_equal[1] && is_equal[3])
            {
                // F T F T eg. 8 7 7 6 6 --> 7 7 6 6 8
                swap(pat[2], pat[4]);
                swap(pat[0], pat[4]);
            }
        }
    }
    else if (equal_num == 1)
    {
        pat.type = ONE_PAIR;
    }
    else
    {
        pat.type = HIGH_CARD;
    }

    return pat;
}

int compare(Card* hand_1, Card* hand_2)
{
    // hand_1, hand_2 : 5 card

}

void test_get_pattern()
{
    Card c[] = {
        Card(2, 1),     // 0
        Card(1, 2),     // 1
        Card(1, 3),     // 2
        Card(1, 4),     // 3
        Card(1, 6),     // 4
        Card(1, 5),     // 5
        Card(1, 8),     // 6
        Card(2, 8),     // 7
        Card(2, 5),     // 8
        Card(0, 5),     // 9
    };

    // test FLUSH
    cout << getPattern(c + 0) << endl;

    // test STRAIGHT_FLUSH
    cout << getPattern(c + 1) << endl;

    // test STRAIGHT
    cout << getPattern(c + 2) << endl;

    // test ONE_PAIR
    cout << getPattern(c + 3) << endl;

    // test TWO_PAIR
    cout << getPattern(c + 4) << endl;

    // test FULL_HOUSE
    cout << getPattern(c + 5) << endl;
}

int main(int argc, char const *argv[])
{
    test_get_pattern();
    return 0;
}
