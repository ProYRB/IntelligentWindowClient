/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#include "cgobangchessman.h"

CGobangChessman::CGobangChessman()
{
    this->m_isEmpty = true;
    this->m_backslash = INITIALSCORE;
    this->m_slash = INITIALSCORE;
    this->m_horizontal = INITIALSCORE;
    this->m_vertical = INITIALSCORE;
    this->m_type = Chessman_None;
}

CGobangChessman::~CGobangChessman()
{
    ;
}

void CGobangChessman::addScore(int score)
{
    this->m_backslash += score;
    this->m_slash += score;
    this->m_horizontal += score;
    this->m_vertical += score;
}

bool CGobangChessman::isEmpty()
{
    return this->m_isEmpty;
}

bool CGobangChessman::powerScore(CDirectionType directionType, double power)
{
    if(this->m_isEmpty == true && power > 0)
    {
        switch(directionType)
        {
        default:
        {
            return false;
        }
        case Direction_Backslash:
        {
            this->m_backslash *= power;
            break;
        }
        case Direction_Slash:
        {
            this->m_slash *= power;
            break;
        }
        case Direction_Horizontal:
        {
            this->m_horizontal *= power;
            break;
        }
        case Direction_Vertical:
        {
            this->m_vertical *= power;
            break;
        }
        }
        return true;
    }
    return false;
}

void CGobangChessman::restart()
{
    this->m_isEmpty = true;
    this->m_backslash = INITIALSCORE;
    this->m_slash = INITIALSCORE;
    this->m_horizontal = INITIALSCORE;
    this->m_vertical = INITIALSCORE;
    this->m_type = Chessman_None;
}

double CGobangChessman::score()
{
//    if(m_type == Chessman_None)
//    {
//        return this->m_score;
//    }
//    return 0;
    if(m_type == Chessman_None)
    {
        return (this->m_backslash + this->m_slash + this->m_horizontal + this->m_vertical);
    }
    return 0.0;
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
