/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#ifndef CGOBANGCHESSMAN_H
#define CGOBANGCHESSMAN_H

#include <QObject>

#define INITIALSCORE 1000

class CGobangChessman : public QObject
{
    Q_OBJECT

signals:

public:
    enum CChessmanType
    {
        Chessman_Black,
        Chessman_None,
        Chessman_White,
    };
    enum CDirectionType
    {
        Direction_Backslash,
        Direction_Slash,
        Direction_Horizontal,
        Direction_Vertical,
    };

    explicit CGobangChessman();
    ~CGobangChessman();

    void addScore(int score);
    bool isEmpty();
    bool powerScore(double power);
    bool powerScore(CDirectionType directionType, double power);
    void restart();
    double score();
    bool setChessman(CChessmanType type);

    CChessmanType type();

protected:

private:
    bool m_isEmpty;         //是否落子
    double m_backslash;     //反斜杠方向的分数
    double m_slash;         //斜杠方向的分数
    double m_horizontal;    //斜杠方向的分数
    double m_vertical;      //斜杠方向的分数

    CChessmanType m_type;   //棋子类型

private slots:

};

#endif // CGOBANGCHESSMAN_H
