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

#define POWERVALUE0_05 0.05
#define POWERVALUE0_10 0.10
#define POWERVALUE0_90 0.90
#define POWERVALUE0_95 0.95
#define POWERVALUE1_00 1.00
#define POWERVALUE1_15 1.15
#define POWERVALUE1_20 1.20

#define INITIALADDSCORE 1.0

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

    void addScoreInitially(double score);
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
    CGobangChessman::CChessmanType m_playerChessmanType = CGobangChessman::CChessmanType::Chessman_Black;

private slots:
    void ai();
    void checkWin(const int indexX, const int indexY, const CGobangChessman::CChessmanType type);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintChessboard();
    bool paintChessman(int indexX, int indexY, CGobangChessman::CChessmanType chessmanType);
    void paintPredictor();
    void updateScore(int indexX, int indexY, CGobangChessman::CChessmanType type);

};

#endif // CGOBANGCHESSBOARD_H
