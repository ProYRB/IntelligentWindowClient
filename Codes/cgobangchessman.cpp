/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#include "cgobangchessman.h"

CGobangChessman::CGobangChessman()
{
    ;
}

int CGobangChessman::score()
{
    if(m_type == Type_None)
    {
        return m_score;
    }
    return 0;
}

bool CGobangChessman::addScore(int score)
{
    if(!m_isEmpty)
    {
        return false;
    }
    m_score += score;
    return true;
}

bool CGobangChessman::isEmpty()
{
    return m_isEmpty;
}

bool CGobangChessman::oddsScore(int odds)
{
    if(m_type == Type_None && odds <= 0)
    {
        return false;
    }
    m_score /= odds;
    return true;
}

bool CGobangChessman::powerScore(int power)
{
    if(m_type == Type_None && power <= 0)
    {
        return false;
    }
    m_score *= power;
    return true;
}

bool CGobangChessman::setChessman(CChessmanType type)
{
    if(!m_isEmpty)
    {
        return false;
    }
    m_type = type;
    m_isEmpty = false;
    return true;
}

CGobangChessman::CChessmanType CGobangChessman::type()
{
    return m_type;
}
