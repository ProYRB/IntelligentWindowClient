/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#include "cgobangchessboard.h"

CGobangChessboard::CGobangChessboard(QWidget *parent)
    : QLabel(parent)
{
    /** [InitializeVirtualChessboard] */
    m_virtualChessboard = (CGobangChessman**) new CGobangChessman*[15];     //定义一个指向二维指针的指针
    for(int i = 0; i < 15; ++i)
    {
        m_virtualChessboard[i] = new CGobangChessman[15];   //为二维指针n分配空间
    }
    this->addScoreInitially(INITIALADDSCORE);
    /* [InitializeVirtualChessboard] **/


    /** [PaintChessboard] */
    this->paintChessboard();
    /* [PaintChessboard] **/


    /** [PaintChesses] */
    setMouseTracking(true);
    connect(this, &CGobangChessboard::targetChanged, this, &CGobangChessboard::paintPredictor);
    /* [PaintChesses] **/


    /** [CheckWin] */
    connect(this, &CGobangChessboard::humanPlayed, this, &CGobangChessboard::checkWin);
    connect(this, &CGobangChessboard::aiPlayed, this, &CGobangChessboard::checkWin);
    /* [CheckWin] **/


    /** [Win] */
    connect(this, &CGobangChessboard::win, [&](){ m_playerChessmanType = CGobangChessman::CChessmanType::Chessman_None; });
    /* [Win] **/


    /** [UpdateScore] */
    connect(this, &CGobangChessboard::checked, this, &CGobangChessboard::updateScore);
    /* [UpdateScore] **/


    /** [AI] */
    connect(this, &CGobangChessboard::aiReady, this, &CGobangChessboard::ai);
    /* [AI] **/
}

void CGobangChessboard::addScoreInitially(double score)
{
    //初始化棋盘的分数
    for(int i = 0; i < 8; ++i)
    {
        for(int j = i; (-1 + i) < j && j < (15 - i); ++j)
        {
            for(int k = i; (-1 + i) < k && k < (15 - i); ++k)
            {
                m_virtualChessboard[j][k].addScore(score);
            }
        }
    }
}

void CGobangChessboard::restart()
{
    /** [InitializeVirtualChessboard] */
    for(int i = 0; i < 15; ++i)     //初始化棋盘
    {
        for(int j = 0; j < 15; ++j)
        {
            m_virtualChessboard[i][j].restart();
        }
    }
    this->addScoreInitially(INITIALADDSCORE);
    /* [InitializeVirtualChessboard] **/


    m_chessboardPositionIndexX = -1;
    m_chessboardPositionIndexY = -1;

    m_vectorPlayImagesHistory.clear();
    m_vectorPlayPositionsIndexHistory.clear();

    m_playerChessmanType = CGobangChessman::CChessmanType::Chessman_Black;

    this->paintChessboard();

    qDebug() << "=====重新开始=====";
}

void CGobangChessboard::ai()
{
    double highestScore = 0;
    int highestScoreIndexX = -1;
    int highestScoreIndexY = -1;
    QVector<CGobangChessman*> highestChessmen;


    /** [SeekHighestScore] */
    for(int i = 0; i < 15; ++i)
    {
        for(int j = 0; j < 15; ++j)
        {
            if(m_virtualChessboard[i][j].score() > highestScore)
            {
                highestScore = m_virtualChessboard[i][j].score();
                highestScoreIndexX = i;
                highestScoreIndexY = j;
            }
        }
    }
    /* [SeekHighestScore] **/


    /** [SeekHighestChessmanVector] */
    for(int i = 0; i < 15; ++i)
    {
        for(int j = 0; j < 15; ++j)
        {
            if(m_virtualChessboard[i][j].score() == highestScore)
            {
                highestChessmen.append(&m_virtualChessboard[i][j]);
            }
        }
    }
    /* [SeekHighestChessmanVector] **/


    /** [RandomSelec] */
//    qsrand(time(NULL)); //设置时间种子
//    int index = qrand() % highestChessmen.size();
//    for(int i = 0; i < 15; ++i)
//    {
//        for(int j = 0; j < 15; ++j)
//        {
//            if(&m_virtualChessboard[i][j] == highestChessmen[index])
//            {
//                highestScoreIndexX = i;
//                highestScoreIndexY = j;
//                break;
//            }
//        }
//    }
    /* [RandomSelec] **/


    /** [Play] */
    qDebug() << "White:" << m_virtualChessboard[highestScoreIndexX][highestScoreIndexY].score();
    this->paintChessman(highestScoreIndexX, highestScoreIndexY, CGobangChessman::CChessmanType::Chessman_White);
    m_virtualChessboard[highestScoreIndexX][highestScoreIndexY].setChessman(CGobangChessman::CChessmanType::Chessman_White);
    m_playerChessmanType = CGobangChessman::CChessmanType::Chessman_Black;
    emit aiPlayed(highestScoreIndexX, highestScoreIndexY, CGobangChessman::CChessmanType::Chessman_White);
    /* [Play] **/
}

void CGobangChessboard::checkWin(const int indexX, const int indexY, const CGobangChessman::CChessmanType type)
{
    const CGobangChessman::CChessmanType temporaryType = m_playerChessmanType;
    m_playerChessmanType = CGobangChessman::CChessmanType::Chessman_None;
    int chessmenNumber;
    for(int i = 1; i < 5; ++i)
    {
        switch(i)
        {
        case 1:
        {
            //"\"
            chessmenNumber = 1;
            for(int j = 1; indexX - j > -1 && indexY - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX - j][indexY - j].type() == type)
                {
                    ++chessmenNumber;
                }
                else
                {
                    break;
                }
            }
            for(int j = 1; indexX + j < 15 && indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY + j].type() == type)
                {
                    ++chessmenNumber;
                }
                else
                {
                    break;
                }
            }
            break;
        }
        case 2:
        {
            //"/"
            chessmenNumber = 1;
            for(int j = 1; indexX + j < 15 && indexY - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY - j].type() == type)
                {
                    ++chessmenNumber;
                }
                else
                {
                    break;
                }
            }
            for(int j = 1; indexX - j > -1 && indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX - j][indexY + j].type() == type)
                {
                    ++chessmenNumber;
                }
                else
                {
                    break;
                }
            }
            break;
        }
        case 3:
        {
            //"-"
            chessmenNumber = 1;
            for(int j = 1; indexX - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX - j][indexY].type() == type)
                {
                    ++chessmenNumber;
                }
                else
                {
                    break;
                }
            }
            for(int j = 1; indexX + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY].type() == type)
                {
                    ++chessmenNumber;
                }
                else
                {
                    break;
                }
            }
            break;
        }
        case 4:
        {
            //"|"
            chessmenNumber = 1;
            for(int j = 1; indexY - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX][indexY - j].type() == type)
                {
                    ++chessmenNumber;
                }
                else
                {
                    break;
                }
            }
            for(int j = 1; indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX][indexY + j].type() == type)
                {
                    ++chessmenNumber;
                }
                else
                {
                    break;
                }
            }
            break;
        }
        }
        if(chessmenNumber >= 5)
        {
            emit win();
            switch(type)
            {
            default:
            {
                break;
            }
            case CGobangChessman::CChessmanType::Chessman_Black:
            {
                emit blackWin();
                break;
            }
            case CGobangChessman::CChessmanType::Chessman_White:
            {
                emit whiteWin();
                break;
            }
            }
            return;
        }
    }
    m_playerChessmanType = temporaryType;
    emit checked(indexX, indexY, type);
}

void CGobangChessboard::mouseMoveEvent(QMouseEvent *mouseMoveEvent)
{
    double ratioX = double(mouseMoveEvent->x()) / (m_linesPixelSpace + m_penPixelLength);
    double ratioY = double(mouseMoveEvent->y()) / (m_linesPixelSpace + m_penPixelLength);
    int interRatioX = ratioX;
    int interRatioY = ratioY;
    double remainderX = ratioX - interRatioX;
    double remainderY = ratioY - interRatioY;
    int newChessboardPositionIndexX;
    int newChessboardPositionIndexY;


    /** [UpdateNewChessboardPositionIndexX] */
    if(remainderX < 0.5)
    {
        if(interRatioX == 0)
        {
            newChessboardPositionIndexX = 0;
        }
        else
        {
            newChessboardPositionIndexX = interRatioX - 1;
        }
    }
    else
    {
        if(interRatioX == 15)
        {
            newChessboardPositionIndexX = interRatioX - 1;
        }
        else
        {
            newChessboardPositionIndexX = interRatioX;
        }
    }
    /* [UpdateNewChessboardPositionIndexX] **/


    /** [UpdateNewChessboardPositionIndexY] */
    if(remainderY < 0.5)
    {
        if(interRatioY == 0)
        {
            newChessboardPositionIndexY = 0;
        }
        else
        {
            newChessboardPositionIndexY = interRatioY - 1;
        }
    }
    else
    {
        if(interRatioY == 15)
        {
            newChessboardPositionIndexY = interRatioY - 1;
        }
        else
        {
            newChessboardPositionIndexY = interRatioY;
        }
    }
    /* [UpdateNewChessboardPositionIndexY] **/


    /** [UpdateChessboardPositionIndex] */
    if(m_chessboardPositionIndexX != newChessboardPositionIndexX || m_chessboardPositionIndexY != newChessboardPositionIndexY)
    {
        m_chessboardPositionIndexX = newChessboardPositionIndexX;
        m_chessboardPositionIndexY = newChessboardPositionIndexY;
        emit targetChanged();
    }
    /* [UpdateChessboardPositionIndex] **/
}

void CGobangChessboard::mouseReleaseEvent(QMouseEvent *mouseReleaseEvent)
{
    int newChessboardPositionIndexX = m_chessboardPositionIndexX;
    int newChessboardPositionIndexY = m_chessboardPositionIndexY;

    if(m_playerChessmanType != CGobangChessman::CChessmanType::Chessman_Black)
    {
        return;
    }
    if(m_virtualChessboard[newChessboardPositionIndexX][newChessboardPositionIndexY].isEmpty())
    {
        qDebug() << "Black:" << m_virtualChessboard[newChessboardPositionIndexX][newChessboardPositionIndexY].score();
        this->paintChessman(newChessboardPositionIndexX, newChessboardPositionIndexY, CGobangChessman::CChessmanType::Chessman_Black);
        m_virtualChessboard[newChessboardPositionIndexX][newChessboardPositionIndexY].setChessman(CGobangChessman::CChessmanType::Chessman_Black);
        m_playerChessmanType = CGobangChessman::CChessmanType::Chessman_White;
        emit humanPlayed(newChessboardPositionIndexX, newChessboardPositionIndexY, CGobangChessman::CChessmanType::Chessman_Black);
    }
}

void CGobangChessboard::paintChessboard()
{
    /** [SetPainter] */
    QImage *temporaryImage = new QImage(m_chessboardImagePixelLength, m_chessboardImagePixelLength, QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(temporaryImage);
    painter.setPen(QPen(COLOR_BLACK));
    painter.setBrush(QBrush(COLOR_BLACK));
    painter.setRenderHint(QPainter::Antialiasing);   //抗锯齿
    /* [SetPainter] **/


    /** [PaintLines] */
    for(int i = 0; i < 15; ++i)
    {
        painter.drawLine(
                    m_linesPixelSpace + (m_linesPixelSpace + m_penPixelLength) * i,
                    m_linesPixelSpace,
                    m_linesPixelSpace + (m_linesPixelSpace + m_penPixelLength) * i,
                    m_chessboardImagePixelLength - m_linesPixelSpace - 1
                    );
        painter.drawLine(
                    m_linesPixelSpace,
                    m_linesPixelSpace + (m_linesPixelSpace + m_penPixelLength) * i,
                    m_chessboardImagePixelLength - m_linesPixelSpace - 1,
                    m_linesPixelSpace + (m_linesPixelSpace + m_penPixelLength) * i
                    );
    }
    /* [PaintLines] **/


    /** [PaintEllipses] */
    painter.drawEllipse(
                QPoint(
                    (m_linesPixelSpace + m_penPixelLength) * 4 - 1,
                    (m_linesPixelSpace + m_penPixelLength) * 4 - 1
                    ),
                m_ellipsePixelRadius,
                m_ellipsePixelRadius
                );                          //左上
    painter.drawEllipse(
                QPoint(
                    m_chessboardImagePixelLength - (m_linesPixelSpace + m_penPixelLength) * 4,
                    (m_linesPixelSpace + m_penPixelLength) * 4 - 1
                    ),
                m_ellipsePixelRadius,
                m_ellipsePixelRadius);      //右上
    painter.drawEllipse(
                QPoint(
                    (m_linesPixelSpace + m_penPixelLength) * 8 - 1,
                    (m_linesPixelSpace + m_penPixelLength) * 8 - 1
                    ),
                m_ellipsePixelRadius,
                m_ellipsePixelRadius);      //中间
    painter.drawEllipse(
                QPoint(
                    (m_linesPixelSpace + m_penPixelLength) * 4 - 1,
                    m_chessboardImagePixelLength - (m_linesPixelSpace + m_penPixelLength) * 4
                    ),
                m_ellipsePixelRadius,
                m_ellipsePixelRadius);      //左下
    painter.drawEllipse(
                QPoint(
                    m_chessboardImagePixelLength - (m_linesPixelSpace + m_penPixelLength) * 4,
                    m_chessboardImagePixelLength - (m_linesPixelSpace + m_penPixelLength) * 4
                    ),
                m_ellipsePixelRadius,
                m_ellipsePixelRadius);      //右下
    /* [PaintEllipses] **/


    /** [LoadAndSavePaintedImage] */
    painter.end();
    this->setPixmap(QPixmap::fromImage(*temporaryImage));
    m_vectorPlayImagesHistory.append(temporaryImage);
    /* [LoadPaintedImage] **/
}

bool CGobangChessboard::paintChessman(int indexX, int indexY, CGobangChessman::CChessmanType chessmanType)
{
    /** [SetPainter] */
    QImage *temporaryImage = new QImage(*m_vectorPlayImagesHistory.last());
    QPainter painter;
    painter.begin(temporaryImage);
    switch(chessmanType)
    {
    default:
    {
        return false;
    }
    case CGobangChessman::CChessmanType::Chessman_Black:
    {
        painter.setPen(QPen(COLOR_EDGE));
        painter.setBrush(QBrush(COLOR_BLACK));
        break;
    }
    case CGobangChessman::CChessmanType::Chessman_White:
    {
        painter.setPen(QPen(COLOR_EDGE));
        painter.setBrush(QBrush(COLOR_WHITE));
        break;
    }
    }
    painter.setRenderHint(QPainter::Antialiasing);  //抗锯齿
    /* [SetPainter] **/


    /** [PaintChessman] */
    painter.drawEllipse(
                QPoint(
                    (indexX + 1) * (m_linesPixelSpace + m_penPixelLength) - 1,
                    (indexY + 1) * (m_linesPixelSpace + m_penPixelLength) - 1
                    ),
                CHESSMAN_PIXEL_RADIUS,
                CHESSMAN_PIXEL_RADIUS
                );
    /* [PaintChessman] **/


    /** [LoadPaintedImage] */
    painter.end();
    this->setPixmap(QPixmap::fromImage(*temporaryImage));
    m_vectorPlayImagesHistory.append(temporaryImage);
    /* [LoadPaintedImage] **/


    return true;
}

void CGobangChessboard::paintPredictor()
{
    QImage *temporaryImage = new QImage(*m_vectorPlayImagesHistory.last());
    QPainter painter;

    /** [SetPainter] */
    painter.begin(temporaryImage);
    painter.setPen(QPen(COLOR_BLACK));
    painter.setRenderHint(QPainter::Antialiasing);   //抗锯齿
    /* [SetPainter] **/


    /** [PaintPredictor] */
    painter.drawEllipse(
                QPoint(
                    (m_chessboardPositionIndexX + 1) * (m_linesPixelSpace + m_penPixelLength) - 1,
                    (m_chessboardPositionIndexY + 1) * (m_linesPixelSpace + m_penPixelLength) - 1
                    ),
                CHESSMAN_PIXEL_RADIUS,
                CHESSMAN_PIXEL_RADIUS
                );
    /* [PaintPredictor] **/


    /** [LoadPaintedImage] */
    painter.end();
    this->setPixmap(QPixmap::fromImage(*temporaryImage));
    delete temporaryImage;
    /* [LoadPaintedImage] **/
}

void CGobangChessboard::updateScore(int indexX, int indexY, CGobangChessman::CChessmanType chessmanType)
{
    /** [reduceScore] */
    for(int i = 1; i < 5; ++i)
    {
        switch(i)
        {
        case 1:
        {
            //"\"
            QVector<CGobangChessman*> powerTargets;
            bool ifPower = false;
            for(int j = 1; indexX - j > -1 && indexY - j > -1; ++j)     //在棋盘范围内搜索
            {
                if(m_virtualChessboard[indexX - j][indexY - j].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY - j + 1].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexX - j == 0 || indexY - j == 0)      //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX - j - 1][indexY - j - 1].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX - j][indexY - j]);
                        }
                        else
                        {
                            break;  //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX - j][indexY - j]);  //如果当前位置的反向一个单位的位置存在棋子（只能是对手棋子）则记录当前位置
                    }
                }
                else if(m_virtualChessboard[indexX - j][indexY - j].type() != chessmanType)
                {
                    ifPower = true;     //只有检索到对手的棋子才会进行分数计算
                }
                else
                {
                    break;      //遇到相同类型的棋子则退出
                }
            }
            if(ifPower)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Backslash, POWERVALUE0_95);
                }
            }
            powerTargets.clear();
            ifPower = false;
            for(int j = 1; indexX + j < 15 && indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY + j].type() == CGobangChessman::CChessmanType::Chessman_None)
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY + j - 1].type() == CGobangChessman::CChessmanType::Chessman_None)
                    {
                        if(indexX + j == 14 || indexY + j == 14)
                        {
                            powerTargets.append(&m_virtualChessboard[indexX + j][indexY + j]);
                            break;
                        }
                        else if(m_virtualChessboard[indexX + j + 1][indexY + j + 1].type() != CGobangChessman::CChessmanType::Chessman_None)
                        {
                            powerTargets.append(&m_virtualChessboard[indexX + j][indexY + j]);
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX + j][indexY + j]);
                    }
                }
                else if(m_virtualChessboard[indexX + j][indexY + j].type() != chessmanType)
                {
                    ifPower = true;     //只有检索到对手的棋子才会进行分数计算
                }
                else
                {
                    break;      //遇到相同类型的棋子则退出
                }
            }
            if(ifPower)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Backslash, POWERVALUE0_95);
                }
            }
            break;
        }
        case 2:
        {
            //"/"
            QVector<CGobangChessman*> powerTargets;
            bool ifPower = false;
            for(int j = 1; indexX + j < 15 && indexY - j > -1; ++j)     //在棋盘范围内搜索
            {
                if(m_virtualChessboard[indexX + j][indexY - j].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY - j + 1].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexX + j == 14 || indexY - j == 0)     //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX + j + 1][indexY - j - 1].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX + j][indexY - j]);
                        }
                        else
                        {
                            break;  //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX + j][indexY - j]);          //如果当前位置的反向一个单位的位置存在棋子则记录当前位置
                    }
                }
                else if(m_virtualChessboard[indexX + j][indexY - j].type() != chessmanType)
                {
                    ifPower = true;     //只有检索到对手的棋子才会进行分数计算
                }
                else
                {
                    break;      //遇到相同类型的棋子则退出
                }
            }
            if(ifPower)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Slash, POWERVALUE0_95);
                }
            }
            powerTargets.clear();
            ifPower = false;
            for(int j = 1; indexX - j > -1 && indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX - j][indexY + j].type() == CGobangChessman::CChessmanType::Chessman_None)
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY + j - 1].type() == CGobangChessman::CChessmanType::Chessman_None)
                    {
                        if(indexX - j == 0 || indexY + j == 14)
                        {
                            powerTargets.append(&m_virtualChessboard[indexX - j][indexY + j]);
                            break;
                        }
                        else if(m_virtualChessboard[indexX - j - 1][indexY + j + 1].type() != CGobangChessman::CChessmanType::Chessman_None)
                        {
                            powerTargets.append(&m_virtualChessboard[indexX - j][indexY + j]);
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX - j][indexY + j]);
                    }
                }
                else if(m_virtualChessboard[indexX - j][indexY + j].type() != chessmanType)
                {
                    ifPower = true;     //只有检索到对手的棋子才会进行分数计算
                }
                else
                {
                    break;      //遇到相同类型的棋子则退出
                }
            }
            if(ifPower)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Slash, POWERVALUE0_95);
                }
            }
            break;
        }
        case 3:
        {
            //"-"
            QVector<CGobangChessman*> powerTargets;
            bool ifPower = false;
            for(int j = 1; indexX - j > -1; ++j)    //在棋盘范围内搜索
            {
                if(m_virtualChessboard[indexX - j][indexY].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexX - j == 0)     //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX - j - 1][indexY].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX - j][indexY]);
                        }
                        else
                        {
                            break;  //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX - j][indexY]);      //如果当前位置的反向一个单位的位置存在棋子则记录当前位置
                    }
                }
                else if(m_virtualChessboard[indexX - j][indexY].type() != chessmanType)
                {
                    ifPower = true;     //只有检索到对手的棋子才会进行分数计算
                }
                else
                {
                    break;      //遇到相同类型的棋子则退出
                }
            }
            if(ifPower)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Horizontal, POWERVALUE0_95);
                }
            }
            powerTargets.clear();
            ifPower = false;
            for(int j = 1; indexX + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY].type() == CGobangChessman::CChessmanType::Chessman_None)
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY].type() == CGobangChessman::CChessmanType::Chessman_None)
                    {
                        if(indexX + j == 14)
                        {
                            powerTargets.append(&m_virtualChessboard[indexX + j][indexY]);
                            break;
                        }
                        else if(m_virtualChessboard[indexX + j + 1][indexY].type() != CGobangChessman::CChessmanType::Chessman_None)
                        {
                            powerTargets.append(&m_virtualChessboard[indexX + j][indexY]);
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX + j][indexY]);
                    }
                }
                else if(m_virtualChessboard[indexX + j][indexY].type() != chessmanType)
                {
                    ifPower = true;     //只有检索到对手的棋子才会进行分数计算
                }
                else
                {
                    break;      //遇到相同类型的棋子则退出
                }
            }
            if(ifPower)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Horizontal, POWERVALUE0_95);
                }
            }
            break;
        }
        case 4:
        {
            //"|"
            QVector<CGobangChessman*> powerTargets;
            bool ifPower = false;
            for(int j = 1; indexY - j > -1; ++j)    //在棋盘范围内搜索
            {
                if(m_virtualChessboard[indexX][indexY - j].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX][indexY - j + 1].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexY - j == 0)     //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX][indexY - j - 1].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX][indexY - j]);
                        }
                        else
                        {
                            break;      //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX][indexY - j]);      //如果当前位置的反向一个单位的位置存在棋子则记录当前位置
                    }
                }
                else if(m_virtualChessboard[indexX][indexY - j].type() != chessmanType)
                {
                    ifPower = true;     //只有检索到对手的棋子才会进行分数计算
                }
                else
                {
                    break;      //遇到相同类型的棋子则退出
                }
            }
            if(ifPower)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Vertical, POWERVALUE0_95);
                }
            }
            powerTargets.clear();
            ifPower = false;
            for(int j = 1; indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX][indexY + j].type() == CGobangChessman::CChessmanType::Chessman_None)
                {
                    if(m_virtualChessboard[indexX][indexY + j - 1].type() == CGobangChessman::CChessmanType::Chessman_None)
                    {
                        if(indexY + j == 14)
                        {
                            powerTargets.append(&m_virtualChessboard[indexX][indexY + j]);
                            break;
                        }
                        else if(m_virtualChessboard[indexX][indexY + j + 1].type() != CGobangChessman::CChessmanType::Chessman_None)
                        {
                            powerTargets.append(&m_virtualChessboard[indexX][indexY + j]);
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX][indexY + j]);
                    }
                }
                else if(m_virtualChessboard[indexX][indexY + j].type() != chessmanType)
                {
                    ifPower = true;     //只有检索到对手的棋子才会进行分数计算
                }
                else
                {
                    break;      //遇到相同类型的棋子则退出
                }
            }
            if(ifPower)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Vertical, POWERVALUE0_95);
                }
            }
            break;
        }
        }
    }
    /* [reduceScore] **/


    /** [addScore] */
    for(int i = 1; i < 5; ++i)
    {
        switch(i)
        {
        case 1:
        {
            //"\"
            QVector<CGobangChessman*> powerTargets;
            double powerValue = POWERVALUE1_00;
            for(int j = 1; indexX - j > -1 && indexY - j > -1; ++j)     //在棋盘范围内搜索
            {
                if(m_virtualChessboard[indexX - j][indexY - j].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY - j + 1].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexX - j == 0 || indexY - j == 0)      //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX - j - 1][indexY - j - 1].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX - j][indexY - j]);
                        }
                        else
                        {
                            break;  //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX - j][indexY - j]);      //如果当前位置存在棋子（必定是类型一致的棋子）
                    }
                }
                else if(m_virtualChessboard[indexX - j][indexY - j].type() == chessmanType)      //如果检索的棋子与当前棋子类型一致
                {
                    powerValue *= POWERVALUE1_20;
                }
                else
                {
                    powerValue *= POWERVALUE0_95;
                    break;
                }
            }
            for(int j = 1; indexX + j < 15 && indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY + j].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY + j - 1].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexX + j == 14 || indexY + j == 14)        //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX + j + 1][indexY + j + 1].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX + j][indexY + j]);
                        }
                        else
                        {
                            break;  //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX + j][indexY + j]);      //如果当前位置存在棋子（必定是类型一致的棋子）
                    }
                }
                else if(m_virtualChessboard[indexX + j][indexY + j].type() == chessmanType)      //如果检索的棋子与当前棋子类型一致
                {
                    powerValue *= POWERVALUE1_20;
                }
                else
                {
                    powerValue *= POWERVALUE0_95;
                    break;
                }
            }
            powerValue -= powerTargets.size() * POWERVALUE0_05;
            if(powerValue > 1.0)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Backslash, powerValue);
                }
            }
            break;
        }
        case 2:
        {
            //"/"
            QVector<CGobangChessman*> powerTargets;
            double powerValue = POWERVALUE1_00;
            for(int j = 1; indexX + j < 15 && indexY - j > -1; ++j)     //在棋盘范围内搜索
            {
                if(m_virtualChessboard[indexX + j][indexY - j].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY - j + 1].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexX + j == 14 || indexY - j == 0)      //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX + j + 1][indexY - j - 1].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX + j][indexY - j]);
                        }
                        else
                        {
                            break;  //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX + j][indexY - j]);      //如果当前位置存在棋子（必定是类型一致的棋子）
                    }
                }
                else if(m_virtualChessboard[indexX + j][indexY - j].type() == chessmanType)      //如果检索的棋子与当前棋子类型一致
                {
                    powerValue *= POWERVALUE1_20;
                }
                else
                {
                    powerValue *= POWERVALUE0_95;
                    break;
                }
            }
            for(int j = 1; indexX - j > -1 && indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX - j][indexY + j].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY + j - 1].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexX - j == 0 || indexY + j == 14)        //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX - j - 1][indexY + j + 1].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX - j][indexY + j]);
                        }
                        else
                        {
                            break;  //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX - j][indexY + j]);      //如果当前位置存在棋子（必定是类型一致的棋子）
                    }
                }
                else if(m_virtualChessboard[indexX - j][indexY + j].type() == chessmanType)      //如果检索的棋子与当前棋子类型一致
                {
                    powerValue *= POWERVALUE1_20;
                }
                else
                {
                    powerValue *= POWERVALUE0_95;
                    break;
                }
            }
            powerValue -= powerTargets.size() * POWERVALUE0_05;
            if(powerValue > 1.0)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Slash, powerValue);
                }
            }
            break;
        }
        case 3:
        {
            //"-"
            QVector<CGobangChessman*> powerTargets;
            double powerValue = POWERVALUE1_00;
            for(int j = 1; indexX - j > -1; ++j)     //在棋盘范围内搜索
            {
                if(m_virtualChessboard[indexX - j][indexY].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexX - j == 0)      //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX - j - 1][indexY].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX - j][indexY]);
                        }
                        else
                        {
                            break;  //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX - j][indexY]);      //如果当前位置存在棋子（必定是类型一致的棋子）
                    }
                }
                else if(m_virtualChessboard[indexX - j][indexY].type() == chessmanType)      //如果检索的棋子与当前棋子类型一致
                {
                    powerValue *= POWERVALUE1_20;
                }
                else
                {
                    powerValue *= POWERVALUE0_95;
                    break;
                }
            }
            for(int j = 1; indexX + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexX + j == 14)        //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX + j + 1][indexY].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX + j][indexY]);
                        }
                        else
                        {
                            break;  //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX + j][indexY]);      //如果当前位置存在棋子（必定是类型一致的棋子）
                    }
                }
                else if(m_virtualChessboard[indexX + j][indexY].type() == chessmanType)      //如果检索的棋子与当前棋子类型一致
                {
                    powerValue *= POWERVALUE1_20;
                }
                else
                {
                    powerValue *= POWERVALUE0_95;
                    break;
                }
            }
            powerValue -= powerTargets.size() * POWERVALUE0_05;
            if(powerValue > 1.0)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Horizontal, powerValue);
                }
            }
            break;
        }
        case 4:
        {
            //"|"
            QVector<CGobangChessman*> powerTargets;
            double powerValue = POWERVALUE1_00;
            for(int j = 1; indexY - j > -1; ++j)     //在棋盘范围内搜索
            {
                if(m_virtualChessboard[indexX][indexY - j].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX][indexY - j + 1].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexY - j == 0)      //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX][indexY - j - 1].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX][indexY - j]);
                        }
                        else
                        {
                            break;  //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX][indexY - j]);      //如果当前位置存在棋子（必定是类型一致的棋子）
                    }
                }
                else if(m_virtualChessboard[indexX][indexY - j].type() == chessmanType)      //如果检索的棋子与当前棋子类型一致
                {
                    powerValue *= POWERVALUE1_20;
                }
                else
                {
                    powerValue *= POWERVALUE0_95;
                    break;
                }
            }
            for(int j = 1; indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX][indexY + j].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置否存在棋子
                {
                    if(m_virtualChessboard[indexX][indexY + j - 1].type() == CGobangChessman::CChessmanType::Chessman_None)     //判断当前位置的反向一个单位的位置是否存在棋子
                    {
                        if(indexY + j == 14)        //判断是否抵达边界
                        {
                            break;
                        }
                        else if(m_virtualChessboard[indexX][indexY + j + 1].type() != CGobangChessman::CChessmanType::Chessman_None)    //如果当前位置的正向一个单位的位置存在棋子
                        {
                            powerTargets.append(&m_virtualChessboard[indexX][indexY + j]);
                        }
                        else
                        {
                            break;  //如果当前位置的正向一个单位的位置不存在棋子
                        }
                    }
                    else
                    {
                        powerTargets.append(&m_virtualChessboard[indexX][indexY + j]);      //如果当前位置存在棋子（必定是类型一致的棋子）
                    }
                }
                else if(m_virtualChessboard[indexX][indexY + j].type() == chessmanType)      //如果检索的棋子与当前棋子类型一致
                {
                    powerValue *= POWERVALUE1_20;
                }
                else
                {
                    powerValue *= POWERVALUE0_95;
                    break;
                }
            }
            powerValue -= powerTargets.size() * POWERVALUE0_05;
            if(powerValue > 1.0)
            {
                for(int j = 0; j < powerTargets.size(); ++j)
                {
                    powerTargets[j]->powerScore(CGobangChessman::CDirectionType::Direction_Vertical, powerValue);
                }
            }
            break;
        }
        }
    }
    /* [addScore] **/


    /** [aiReady] */
    if(chessmanType == CGobangChessman::CChessmanType::Chessman_Black)
    {
        emit aiReady();
    }
    /* [aiReady] **/
}
