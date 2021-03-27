/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#ifndef CGOBANGCHESSMAN_H
#define CGOBANGCHESSMAN_H

#include <QObject>

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

    int score();

    bool addScore(int score);
    bool isEmpty();
    bool oddsScore(int odds);
    bool powerScore(int power);
    bool setChessman(CChessmanType type);

    CChessmanType type();

protected:

private:
    int m_score = 100;

    bool m_isEmpty = true;

    CChessmanType m_type = Type_None;

private slots:

};

#endif // CGOBANGCHESSMAN_H
