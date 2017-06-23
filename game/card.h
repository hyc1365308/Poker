/************************************************
 * 名称 : card.h
 * 作者 : 冯瑜林
 * 时间 : 2017-04-18(1st)
 * 内容 : 纸牌类实现文件
************************************************/

#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/*
 * 花色，一共四种
*/
enum Suit
{
    HEARTS = 0,     // 红桃
    DIAMONDS = 1,   // 方块
    CLUB = 2,       // 梅花
    SPADE = 3       // 黑桃
};

/*
 * 一张牌
*/
struct Card
{
    Suit suit;      // 花色
    int num;        // 牌的大小，从1到13
    Card() {}
    Card(const int s, const int n)
    {
        suit = Suit(s);
        num = n;
    }

    // 这里由于Card中的运算符比较经常被使用，
    // 故全部设为内联函数，以减少调用开销

    bool operator< (const Card & right)
    {
        // 比较大小，注意，1(A)最大，2最小

        if (num == 1)
        {
            return false;
        }
        else if (right.num == 1)
        {
            return true;
        }
        else{
            return num < right.num;
        }
    }

    bool operator> (const Card & right)
    {
        // 比较大小，注意，1(A)最大，2最小

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
        // A 看作 14
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
        /*
         * 打印输出，输出格式类似 H4，代表红桃4
        */
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

    int toInt()
    {
        // 将Card转换成从 0 到 51 的一个整数
        return suit * 13 + num - 1;
    }
};

#endif
