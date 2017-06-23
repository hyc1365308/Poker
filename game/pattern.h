/************************************************
 * 名称 : pattern.h
 * 作者 : 冯瑜林
 * 时间 : 2017-05-03(1st)
 * 内容 : 当前手牌模式类文件
************************************************/

#ifndef PATTERN_H
#define PATTERN_H

#include "card.h"
#include "../json/json.h"

#include <iostream>

/*
 * 手牌的枚举类型，共9种
*/
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
    HIGH_CARD,
};


/*
 * 当前的
*/
struct Pattern
{
    HandPattern type;       // 手牌类型
    Card card[5];           // 最大的五张牌(7张牌中选出的)

    Card & operator[] (const int i)
    {
        assert(i < 5 && i >= 0);
        return card[i];
    }

    /*
     * 比较两副手牌的大小关系
     *     比较时先判断手牌类型是否一致
     *         不一致时直接得到结果
     *         一致时根据五张手牌的大小判断结果
    */
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
        /*
         * 功能 : 重载流运算符，打印当前的模式
        */

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

    Json::Value toJsonObject()
    {
        /*
         * 功能 : 将该模式转换成json对象
        */
        Json::Value root;
        root["type"] = type;
        for(int i = 0; i < 4; i++){
            root[i] = card[i].toInt();
        }
        return root;
    }

    static Json::Value foldPattern()
    {
        Json::Value root;
        root["type"] = -1;
        for(int i = 0; i < 4; i++){
            root[i] = -1;
        }
        return root;
    }
};

/*
 * 给出一组牌(7个)，获取当前的Pattern(当前最大的类型及其对应的5张牌)
 * 在compare.cpp文件中实现
*/
Pattern getPattern(Card* in_hand);

#endif
