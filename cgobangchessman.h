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
        Type_Black,
        Type_None,
        Type_White,
    };

    explicit CGobangChessman();
    ~CGobangChessman();

    void addScore(int score);
    bool isEmpty();
    bool powerScore(double power);
    void restart();
    double score();
    bool setChessman(CChessmanType type);

    CChessmanType type();

protected:

private:
    bool m_isEmpty;         //是否落子
    double m_score;         //棋子初始化分数

    CChessmanType m_type;   //棋子类型

private slots:

};

#endif // CGOBANGCHESSMAN_H
