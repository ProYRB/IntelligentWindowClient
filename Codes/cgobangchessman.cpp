/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#include "cgobangchessman.h"

CGobangChessman::CGobangChessman()
{
    m_isEmpty = true;
    m_score = INITIALSCORE;
    m_type = Type_None;
}

CGobangChessman::~CGobangChessman()
{
    ;
}

void CGobangChessman::addScore(int score)
{
    this->m_score += score;
}

bool CGobangChessman::isEmpty()
{
    return this->m_isEmpty;
}

bool CGobangChessman::powerScore(double power)
{
    if(this->m_isEmpty == true && power > 0)
    {
        this->m_score *= power;
        return true;
    }
    return false;
}

void CGobangChessman::restart()
{
    this->m_isEmpty = true;
    this->m_score = INITIALSCORE;
    this->m_type = Type_None;
}

double CGobangChessman::score()
{
    if(m_type == Type_None)
    {
        return this->m_score;
    }
    return 0;
}

bool CGobangChessman::setChessman(CChessmanType type)
{
    if(this->m_isEmpty)
    {
        this->m_type = type;
        this->m_isEmpty =  false;
        return true;
    }
    return false;
}

CGobangChessman::CChessmanType CGobangChessman::type()
{
    return this->m_type;
}
