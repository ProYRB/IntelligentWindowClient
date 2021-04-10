/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#ifndef CGOBANGCHESSBOARD_H
#define CGOBANGCHESSBOARD_H

#include "cgobangchessman.h"

#include <QTime>
#include <QImage>
#include <QLabel>
#include <QDebug>
#include <QObject>
#include <QWidget>
#include <QVector>
#include <QtGlobal>
#include <QPainter>
#include <QMouseEvent>

#define CHESSMAN_PIXEL_RADIUS 13
#define COLOR_BLACK QColor(0, 0, 0)
#define COLOR_EDGE QColor(130, 130, 130)
#define COLOR_WHITE QColor(255, 255, 255)

#define POWERVALUE0_1 0.1
#define POWERVALUE0_8 0.8
#define POWERVALUE0_9 0.9
#define POWERVALUE1_0 1.0
#define POWERVALUE1_2 1.2
#define POWERVALUE1_4 1.4

class CGobangChessboard : public QLabel
{
    Q_OBJECT

signals:
    void aiReady();
    void aiPlayed(int indexX, int indexY, CGobangChessman::CChessmanType type);
    void blackWin();
    void checked(int indexX, int indexY, CGobangChessman::CChessmanType type);
    void humanPlayed(int indexX, int indexY, CGobangChessman::CChessmanType type);
    void targetChanged();
    void whiteWin();
    void win();

public:
    explicit CGobangChessboard(QWidget *parent = nullptr);

    void restart();

protected:

private:
    const int m_chessboardImagePixelLength = 511;
    const int m_ellipsePixelRadius = 2;
    const int m_linesPixelSpace = 31;
    const int m_penPixelLength = 1;

    int m_chessboardPositionIndexX = -1;
    int m_chessboardPositionIndexY = -1;

    QVector<CGobangChessman*> m_vectorPlayPositionsIndexHistory;
    QVector<QImage*> m_vectorPlayImagesHistory;

    CGobangChessman **m_virtualChessboard = nullptr;
    CGobangChessman::CChessmanType m_playerChessmanType = CGobangChessman::CChessmanType::Type_Black;

private slots:
    void ai();
    void updateScore(int indexX, int indexY, CGobangChessman::CChessmanType type);
    void checkWin(const int indexX, const int indexY, const CGobangChessman::CChessmanType type);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintChessboard();
    void paintPredictor();

    bool paintChessman(int indexX, int indexY, CGobangChessman::CChessmanType chessmanType);

};

#endif // CGOBANGCHESSBOARD_H
