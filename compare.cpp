/*
 * Author   :   Feng Yulin
 * Time     :   2017-04-07(1st), 2017-05-03(2nd)
 * Function :   Compare two hands
*/

#include "pattern.h"

#include <vector>

using std::cout;
using std::endl;
using std::vector;

template <typename T>
void swap(T & a, T & b)
{
    T c = a;
    a = b;
    b = c;
}

Pattern getPattern(Card* in_hand)
{
    /*
     * hand: Card array(7 cards)
    */
    Card hand[7];
    for (int i = 0; i < 7; ++i)
    {
        hand[i] = in_hand[i];
    }

    // sort hand(reverse sequence)
    for (int i = 0; i < 6; ++i)
    {
        for (int j = i + 1; j < 7; ++j)
        {
            if (hand[j] > hand[i])
            {
                swap(hand[i], hand[j]);
            }
        }
    }

    // @debug
    cout << "Sorted cards:";
    for (int i = 0; i < 7; ++i)
    {
        cout << hand[i] << " ";
    }
    cout << endl;

    Pattern pat;

    vector<Card> suit_arr[4];
    vector<Card> num_arr[1 + 13 + 1];

    for (int i = 0; i < 7; ++i)
    {
        suit_arr[(int)hand[i].suit].push_back(hand[i]);
        num_arr[hand[i].num].push_back(hand[i]);
    }
    num_arr[14] = num_arr[1];

    // judge whether is straight
    int straight_num = -1;
    for (int i = 10; i >= 1; --i)
    {
        int j = 0;
        for (j = 0; j <= 4; ++j)
        {
            if (num_arr[i + j].size() == 0)
            {
                j = 0;
                break;
            }
        }

        if (j == 0)
        {
            continue;
        }
        
        straight_num = i;

        for (int j = 0; j <= 4; ++j)
        {
            pat[j] = num_arr[i + 4 - j][0];
        }
        break;
    }

    // judge whether is flush
    int flush_num = -1;
    for (int i = 0; i < 3; ++i)
    {
        if (suit_arr[i].size() >= 5)
        {
            flush_num = i;

            // copy card to pattern
            for (int j = 0; j < 5; ++j)
            {
                pat[j] = suit_arr[i][j];
            }

            break;
        }
    }

    // the hand pattern is flush or straight
    if (flush_num > -1 || straight_num > -1)
    {
        if (flush_num == -1)
        {
            pat.type = STRAIGHT;
        }
        else if (straight_num == -1)
        {
            pat.type = FLUSH;
        }
        else
        {
            // judge whether is Straight Flush or not
            int cont_num = 1;       // continuous #
            int flush_size = suit_arr[flush_num].size();
            assert(flush_size >= 5);
            for (int i = 1; i < flush_size; ++i)
            {
                if (suit_arr[flush_num][i] == suit_arr[flush_num][i - 1] - 1)
                {
                    ++cont_num;
                }
                else
                {
                    if (flush_size - i < 5)
                    {
                        // pattern is not STRAIGHT_FLUSH
                        pat.type = FLUSH;
                        break;
                    }
                    cont_num = 1;
                }

                if (cont_num == 5)
                {
                    // pattern is STRAIGHT_FLUSH
                    pat.type = STRAIGHT_FLUSH;

                    for (int j = 4; j >= 0; --j)
                    {
                        pat[j] = suit_arr[flush_num][i + j - 4];
                    }

                    break;
                }
            }
        }

        return pat;
    }

    // the hand pattern isn't flush nor straight
    for (int i = 13; i >= 1; --i)
    {
        // check whether is FOUR_OF_A_KIND or not
        if (num_arr[i].size() == 4)
        {
            pat.type = FOUR_OF_A_KIND;
            // set patther card array
            for (int j = 0; j < 4; ++j)
            {
                pat[j] = num_arr[i][j];
            }

            for (int j = 0; j < 7; ++j)
            {
                if (hand[j] != pat[0])
                {
                    pat[4] = hand[j];
                    break;
                }
            }
            return pat;
        }
    }

    int three[3] = {0};     // three[0]--size, three[1],three[2]--num
    int two[4] = {0};       // as above
    for (int i = 13; i >= 1; --i)
    {
        switch (num_arr[i].size())
        {
            case 3:
                three[++three[0]] = i;
                break;
            case 2:
                two[++two[0]] = i;
                break;
            default:
                break;
        }
    }

    if (three[0] == 2 || (three[0] == 1 && two[0] >= 1))
    {
        pat.type = FULL_HOUSE;
        for (int i = 0; i < 3; ++i)
        {
            pat[i] = num_arr[three[1]][i];
        }

        if (three[0] == 2)
        {
            pat[3] = num_arr[three[2]][0];
            pat[4] = num_arr[three[2]][1];
        }
        else
        {
            pat[3] = num_arr[two[1]][0];
            pat[4] = num_arr[two[1]][1];
        }       
    }
    else if (three[0] == 1)
    {
        pat.type = THREE_OF_A_KIND;
        for (int i = 0; i < 3; ++i)
        {
            pat[i] = num_arr[three[1]][i];
        }

        int t = 3;
        for (int i = 0; i < 7; ++i)
        {
            if (hand[i] != pat[0])
            {
                pat[t++] = hand[i];
            }
            
            if (t >= 5)
                break;
        }
    }
    else if (two[0] >= 2)
    {
        pat.type = TWO_PAIR;
        pat[0] = num_arr[two[1]][0];
        pat[1] = num_arr[two[1]][1];
        pat[2] = num_arr[two[2]][0];
        pat[3] = num_arr[two[2]][1];

        for (int i = 0; i < 7; ++i)
        {
            if (hand[i] != pat[0] && hand[i] != pat[2])
            {
                pat[4] = hand[i];
                break;
            }
        }
    }
    else if (two[0] == 1)
    {
        pat.type = ONE_PAIR;
        pat[0] = num_arr[two[1]][0];
        pat[1] = num_arr[two[1]][1];

        int t = 2;
        for (int i = 0; i < 7; ++i)
        {
            if (hand[i] != pat[0])
            {
                pat[t++] = hand[i];
            }
            
            if (t >= 5)
                break;
        }
    }
    else
    {
        pat.type = HIGH_CARD;
        for (int i = 0; i < 5; ++i)
        {
            pat[i] = hand[i];
        }
    }

    return pat;
}

void test_get_pattern()
{
    Card c[] = {
        Card(1, 1),
        Card(1, 2),
        Card(1, 3),
        Card(1, 4),
        Card(1, 6),
        Card(1, 5),
        Card(1, 8),     // 0
        Card(2, 8),     // 1
        Card(2, 5),     // 2
        Card(0, 5),     // 3
        Card(0, 8),     // 4
        Card(1, 2),     // 5
        Card(2, 3),     // 6
        Card(3, 4),     // 7
        Card(0, 7),     // 8
        Card(1, 6),     // 9
        Card(2, 1),     // 10
        Card(3, 1),     // 11
        Card(0, 1),     // 12
        Card(1, 1),     // 13
    };

    // test STRAIGHT_FLUSH
    cout << getPattern(c + 0) << endl;
    cout << getPattern(c + 1) << endl;

    // test FOUR_OF_A_KIND
    cout << getPattern(c + 13) << endl;

    // test FULL_HOUSE
    cout << getPattern(c + 3) << endl;
    cout << getPattern(c + 4) << endl;
    cout << getPattern(c + 5) << endl;
    cout << getPattern(c + 6) << endl;

    // test FLUSH
    cout << getPattern(c + 2) << endl;

    // test STRAIGHT
    cout << getPattern(c + 9) << endl;

    // test THREE_OF_A_KIND
    cout << getPattern(c + 12) << endl;
    
    // test TWO_PAIR
    cout << getPattern(c + 7) << endl;

    // test ONE_PAIR
    cout << getPattern(c + 8) << endl;
    cout << getPattern(c + 11) << endl;

    // test HIGH_CARD
    cout << getPattern(c + 10) << endl;
}

void test_get_pattern2()
{
    Card c2[] = {
        Card(2,1),
        Card(1,13),
        Card(2,13),
        Card(0,12),
        Card(1,11),
        Card(1,8),
        Card(1,3)
    };
    cout<<getPattern(c2)<<endl;
}

int main(int argc, char const *argv[])
{
    test_get_pattern2();
    return 0;
}
