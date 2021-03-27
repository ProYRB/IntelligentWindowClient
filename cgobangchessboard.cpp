/******************************
 *  @Author:YRB
 *  @Date:2021Year
******************************/

#include "cgobangchessboard.h"

CGobangChessboard::CGobangChessboard(QWidget *parent)
    : QLabel(parent)
{
    /** [InitializeVirtualChessboard] */
    m_virtualChessboard = (CGobangChessman **)new CGobangChessman*[15];
    for(int i = 0; i < 15; ++i)
    {
        m_virtualChessboard[i] = new CGobangChessman[15];
    }
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


    /** [power] */
    connect(this, &CGobangChessboard::humanPlayed, this, &CGobangChessboard::updateScore);
    connect(this, &CGobangChessboard::aiPlayed, this, &CGobangChessboard::updateScore);
    /* [power] **/


    /** [AI] */
    connect(this, &CGobangChessboard::humanPlayed, this, &CGobangChessboard::ai);
    /* [AI] **/
}

void CGobangChessboard::restart()
{
    m_chessboardPositionIndexX = -1;
    m_chessboardPositionIndexY = -1;

    m_vectorPlayImagesHistory.clear();
    m_vectorPlayPositionsIndexHistory.clear();

    for(int i = 0; i < 15; ++i)
    {
        delete[] m_virtualChessboard[i];
    }
    delete[] m_virtualChessboard;

    m_virtualChessboard = (CGobangChessman **)new CGobangChessman*[15];
    for(int i = 0; i < 15; ++i)
    {
        m_virtualChessboard[i] = new CGobangChessman[15];
    }

    m_playerChessmanType = CGobangChessman::CChessmanType::Type_Black;

    this->paintChessboard();
}

void CGobangChessboard::ai()
{
    int highestScore = 0;
    int highestScoreIndexX;
    int highestScoreIndexY;
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
    qsrand(time(NULL)); //设置时间种子
    int index = qrand() % highestChessmen.size();
    for(int i = 0; i < 15; ++i)
    {
        for(int j = 0; j < 15; ++j)
        {
            if(&m_virtualChessboard[i][j] == highestChessmen[index])
            {
                highestScoreIndexX = i;
                highestScoreIndexY = j;
            }
        }
    }
    /* [RandomSelec] **/


    /** [Play] */
//    qDebug() << "White:" << m_virtualChessboard[highestScoreIndexX][highestScoreIndexY].score();
    this->paintChessman(highestScoreIndexX, highestScoreIndexY, CGobangChessman::CChessmanType::Type_White);
    m_virtualChessboard[highestScoreIndexX][highestScoreIndexY].setChessman(CGobangChessman::CChessmanType::Type_White);
    m_playerChessmanType = CGobangChessman::CChessmanType::Type_Black;
    emit aiPlayed(highestScoreIndexX, highestScoreIndexY, CGobangChessman::CChessmanType::Type_White);
    /* [Play] **/
}

void CGobangChessboard::updateScore(int indexX, int indexY, CGobangChessman::CChessmanType chessmanType)
{
    /** [addScore] */
    for(int i = 1; i < 5; ++i)
    {
        switch(i)
        {
        case 1:
        {
            //"\"
            int basicScore = 10;
            QVector<CGobangChessman*> vectorTargets;
            for(int j = 1; indexX - j > -1 && indexY - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX - j][indexY - j].type() == chessmanType)
                {
                    basicScore *= m_chessModelGoodPower;
                }
                else if(m_virtualChessboard[indexX - j][indexY - j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY - j + 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX - j + 2][indexY - j + 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX - j == 0 || indexX - j == 14 || indexY - j == 0 || indexY - j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX - j][indexY - j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX - j][indexY - j]);
                    }
                }
                else
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY - j + 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX - j + 2][indexY - j + 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            basicScore /= m_chessModelBadPower;
                            break;
                        }
                        else
                        {
                            basicScore /= m_chessModelWorsePower;
                            break;
                        }
                    }
                    else
                    {
                        basicScore /= m_chessModelWorstPower;
                        break;
                    }
                }
            }
            for(int j = 1; indexX + j < 15 && indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY + j].type() == chessmanType)
                {
                    basicScore *= m_chessModelGoodPower;
                }
                else if(m_virtualChessboard[indexX + j][indexY + j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY + j - 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX + j - 2][indexY + j - 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX + j == 0 || indexX + j == 14 || indexY + j == 0 || indexY + j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX + j][indexY + j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX + j][indexY + j]);
                    }
                }
                else
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY + j - 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX + j - 2][indexY + j - 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            basicScore /= m_chessModelBadPower;
                            break;
                        }
                        else
                        {
                            basicScore /= m_chessModelWorsePower;
                            break;
                        }
                    }
                    else
                    {
                        basicScore /= m_chessModelWorstPower;
                        break;
                    }
                }
            }
            for(int j = 0; j < vectorTargets.size(); ++j)
            {
                vectorTargets.at(j)->addScore(basicScore);
            }
            break;
        }
        case 2:
        {
            //"/"
            int basicScore = 10;
            QVector<CGobangChessman*> vectorTargets;
            for(int j = 1; indexX + j < 15 && indexY - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY - j].type() == chessmanType)
                {
                    basicScore *= m_chessModelGoodPower;
                }
                else if(m_virtualChessboard[indexX + j][indexY - j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY - j + 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX + j - 2][indexY - j + 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX + j == 0 || indexX + j == 14 || indexY - j == 0 || indexY - j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX + j][indexY - j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX + j][indexY - j]);
                    }
                }
                else
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY - j + 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX + j - 2][indexY - j + 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            basicScore /= m_chessModelBadPower;
                            break;
                        }
                        else
                        {
                            basicScore /= m_chessModelWorsePower;
                            break;
                        }
                    }
                    else
                    {
                        basicScore /= m_chessModelWorstPower;
                        break;
                    }
                }
            }
            for(int j = 1; indexX - j > -1 && indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX - j][indexY + j].type() == chessmanType)
                {
                    basicScore *= m_chessModelGoodPower;
                }
                else if(m_virtualChessboard[indexX - j][indexY + j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY + j - 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX - j + 2][indexY + j - 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX - j == 0 || indexX - j == 14 || indexY + j == 0 || indexY + j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX - j][indexY + j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX - j][indexY + j]);
                    }
                }
                else
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY + j - 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX - j + 2][indexY + j - 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            basicScore /= m_chessModelBadPower;
                            break;
                        }
                        else
                        {
                            basicScore /= m_chessModelWorsePower;
                            break;
                        }
                    }
                    else
                    {
                        basicScore /= m_chessModelWorstPower;
                        break;
                    }
                }
            }
            for(int j = 0; j < vectorTargets.size(); ++j)
            {
                vectorTargets.at(j)->addScore(basicScore);
            }
            break;
        }
        case 3:
        {
            //"-"
            int basicScore = 10;
            QVector<CGobangChessman*> vectorTargets;
            for(int j = 1; indexX - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX - j][indexY].type() == chessmanType)
                {
                    basicScore *= m_chessModelGoodPower;
                }
                else if(m_virtualChessboard[indexX - j][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX - j + 2][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX - j == 0 || indexX - j == 14 || indexY == 0 || indexY == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX - j][indexY]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX - j][indexY]);
                    }
                }
                else
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX - j + 2][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            basicScore /= m_chessModelBadPower;
                            break;
                        }
                        else
                        {
                            basicScore /= m_chessModelWorsePower;
                            break;
                        }
                    }
                    else
                    {
                        basicScore /= m_chessModelWorstPower;
                        break;
                    }
                }
            }
            for(int j = 1; indexX + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY].type() == chessmanType)
                {
                    basicScore *= m_chessModelGoodPower;
                }
                else if(m_virtualChessboard[indexX + j][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX + j - 2][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX + j == 0 || indexX + j == 14 || indexY == 0 || indexY == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX + j][indexY]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX + j][indexY]);
                    }
                }
                else
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX + j - 2][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            basicScore /= m_chessModelBadPower;
                            break;
                        }
                        else
                        {
                            basicScore /= m_chessModelWorsePower;
                            break;
                        }
                    }
                    else
                    {
                        basicScore /= m_chessModelWorstPower;
                        break;
                    }
                }
            }
            for(int j = 0; j < vectorTargets.size(); ++j)
            {
                vectorTargets.at(j)->addScore(basicScore);
            }
            break;
        }
        case 4:
        {
            //"|"
            int basicScore = 10;
            QVector<CGobangChessman*> vectorTargets;
            for(int j = 1; indexY - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX][indexY - j].type() == chessmanType)
                {
                    basicScore *= m_chessModelGoodPower;
                }
                else if(m_virtualChessboard[indexX][indexY - j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX][indexY - j + 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX][indexY - j + 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX == 0 || indexX == 14 || indexY - j == 0 || indexY - j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX][indexY - j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX][indexY - j]);
                    }
                }
                else
                {
                    if(m_virtualChessboard[indexX][indexY - j + 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX][indexY - j + 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            basicScore /= m_chessModelBadPower;
                            break;
                        }
                        else
                        {
                            basicScore /= m_chessModelWorsePower;
                            break;
                        }
                    }
                    else
                    {
                        basicScore /= m_chessModelWorstPower;
                        break;
                    }
                }
            }
            for(int j = 1; indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX][indexY + j].type() == chessmanType)
                {
                    vectorTargets.append(&m_virtualChessboard[indexX][indexY + j]);
                    basicScore *= m_chessModelGoodPower;
                }
                else if(m_virtualChessboard[indexX][indexY + j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX][indexY + j - 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX][indexY + j - 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX == 0 || indexX == 14 || indexY + j == 0 || indexY + j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX][indexY + j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX][indexY + j]);
                    }
                }
                else
                {
                    if(m_virtualChessboard[indexX][indexY + j - 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX][indexY + j - 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            basicScore /= m_chessModelBadPower;
                            break;
                        }
                        else
                        {
                            basicScore /= m_chessModelWorsePower;
                            break;
                        }
                    }
                    else
                    {
                        basicScore /= m_chessModelWorstPower;
                        break;
                    }
                }
            }
            for(int j = 0; j < vectorTargets.size(); ++j)
            {
                vectorTargets.at(j)->addScore(basicScore);
            }
            break;
        }
        }
    }
    /* [addScore] **/


    /** [reduceScore] */
    for(int i = 1; i < 5; ++i)
    {
        switch(i)
        {
        case 1:
        {
            //"\"
            int basicScore = -5;
            QVector<CGobangChessman*> vectorTargets;
            for(int j = 1; indexX - j > -1 && indexY - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX - j][indexY - j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY - j + 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX - j + 2][indexY - j + 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX - j == 0 || indexX - j == 14 || indexY - j == 0 || indexY - j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX - j][indexY - j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX - j][indexY - j]);
                    }
                }
                else if(m_virtualChessboard[indexX - j][indexY - j].type() != chessmanType)
                {
                    basicScore *= m_chessModelWorsePower;
                }
                else
                {
                    break;
                }
            }
            for(int j = 1; indexX + j < 15 && indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY + j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY + j - 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX + j - 2][indexY + j - 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX + j == 0 || indexX + j == 14 || indexY + j == 0 || indexY + j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX + j][indexY + j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX + j][indexY + j]);
                    }
                }
                else if(m_virtualChessboard[indexX + j][indexY + j].type() != chessmanType)
                {
                    basicScore *= m_chessModelWorsePower;
                }
                else
                {
                    break;
                }
            }
            for(int j = 0; j < vectorTargets.size(); ++j)
            {
                vectorTargets.at(j)->addScore(basicScore);
            }
            break;
        }
        case 2:
        {
            //"/"
            int basicScore = -5;
            QVector<CGobangChessman*> vectorTargets;
            for(int j = 1; indexX + j < 15 && indexY - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY - j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY - j + 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX + j - 2][indexY - j + 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX + j == 0 || indexX + j == 14 || indexY - j == 0 || indexY - j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX + j][indexY - j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX + j][indexY - j]);
                    }
                }
                else if(m_virtualChessboard[indexX + j][indexY - j].type() != chessmanType)
                {
                    basicScore *= m_chessModelWorsePower;
                }
                else
                {
                    break;
                }
            }
            for(int j = 1; indexX - j > -1 && indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX - j][indexY + j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY + j - 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX - j + 2][indexY + j - 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX - j == 0 || indexX - j == 14 || indexY + j == 0 || indexY + j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX - j][indexY + j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX - j][indexY + j]);
                    }
                }
                else if(m_virtualChessboard[indexX - j][indexY + j].type() != chessmanType)
                {
                    basicScore *= m_chessModelWorsePower;
                }
                else
                {
                    break;
                }
            }
            for(int j = 0; j < vectorTargets.size(); ++j)
            {
                vectorTargets.at(j)->addScore(basicScore);
            }
            break;
        }
        case 3:
        {
            //"-"
            int basicScore = -5;
            QVector<CGobangChessman*> vectorTargets;
            for(int j = 1; indexX - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX - j][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX - j + 1][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX - j + 2][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX - j == 0 || indexX - j == 14 || indexY == 0 || indexY == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX - j][indexY]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX - j][indexY]);
                    }
                }
                else if(m_virtualChessboard[indexX - j][indexY].type() != chessmanType)
                {
                    basicScore *= m_chessModelWorsePower;
                }
                else
                {
                    break;
                }
            }
            for(int j = 1; indexX + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX + j][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX + j - 1][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX + j - 2][indexY].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX + j == 0 || indexX + j == 14 || indexY == 0 || indexY == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX + j][indexY]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX + j][indexY]);
                    }
                }
                else if(m_virtualChessboard[indexX + j][indexY].type() != chessmanType)
                {
                    basicScore *= m_chessModelWorsePower;
                }
                else
                {
                    break;
                }
            }
            for(int j = 0; j < vectorTargets.size(); ++j)
            {
                vectorTargets.at(j)->addScore(basicScore);
            }
            break;
        }
        case 4:
        {
            //"|"
            int basicScore = -5;
            QVector<CGobangChessman*> vectorTargets;
            for(int j = 1; indexY - j > -1; ++j)
            {
                if(m_virtualChessboard[indexX][indexY - j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX][indexY - j + 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX][indexY - j + 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX == 0 || indexX == 14 || indexY - j == 0 || indexY - j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX][indexY - j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX][indexY - j]);
                    }
                }
                else if(m_virtualChessboard[indexX][indexY - j].type() != chessmanType)
                {
                    basicScore *= m_chessModelWorsePower;
                }
                else
                {
                    break;
                }
            }
            for(int j = 1; indexY + j < 15; ++j)
            {
                if(m_virtualChessboard[indexX][indexY + j].type() == CGobangChessman::CChessmanType::Type_None)
                {
                    if(m_virtualChessboard[indexX][indexY + j - 1].type() == CGobangChessman::CChessmanType::Type_None)
                    {
                        if(m_virtualChessboard[indexX][indexY + j - 2].type() == CGobangChessman::CChessmanType::Type_None)
                        {
                            vectorTargets.pop_back();
                            break;
                        }
                        else if(indexX == 0 || indexX == 14 || indexY + j == 0 || indexY + j == 14)
                        {
                            break;
                        }
                        else
                        {
                            vectorTargets.append(&m_virtualChessboard[indexX][indexY + j]);
                        }
                    }
                    else
                    {
                        vectorTargets.append(&m_virtualChessboard[indexX][indexY + j]);
                    }
                }
                else if(m_virtualChessboard[indexX][indexY + j].type() != chessmanType)
                {
                    basicScore *= m_chessModelWorsePower;
                }
                else
                {
                    break;
                }
            }
            for(int j = 0; j < vectorTargets.size(); ++j)
            {
                vectorTargets.at(j)->addScore(basicScore);
            }
            break;
        }
        }
    }
    /* [reduceScore] **/
}

void CGobangChessboard::checkWin(int indexX, int indexY, CGobangChessman::CChessmanType type)
{
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
            switch(type)
            {
            default:
            {
                break;
            }
            case CGobangChessman::CChessmanType::Type_Black:
            {
                emit blackWin();
                break;
            }
            case CGobangChessman::CChessmanType::Type_White:
            {
                emit whiteWin();
                break;
            }
            }
            return;
        }
    }
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

    if(m_playerChessmanType != CGobangChessman::CChessmanType::Type_Black)
    {
        return;
    }
    if(m_virtualChessboard[newChessboardPositionIndexX][newChessboardPositionIndexY].isEmpty())
    {
//        qDebug() << "Black:" << m_virtualChessboard[newChessboardPositionIndexX][newChessboardPositionIndexY].score();
        this->paintChessman(newChessboardPositionIndexX, newChessboardPositionIndexY, CGobangChessman::CChessmanType::Type_Black);
        m_virtualChessboard[newChessboardPositionIndexX][newChessboardPositionIndexY].setChessman(CGobangChessman::CChessmanType::Type_Black);
        m_playerChessmanType = CGobangChessman::CChessmanType::Type_White;
        emit humanPlayed(newChessboardPositionIndexX, newChessboardPositionIndexY, CGobangChessman::CChessmanType::Type_Black);
    }
}

void CGobangChessboard::paintChessboard()
{
    QImage *temporaryImage = new QImage(m_chessboardImagePixelLength, m_chessboardImagePixelLength, QImage::Format_ARGB32);
    QPainter painter;

    /** [SetPainter] */
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
                );
    painter.drawEllipse(
                QPoint(
                    m_chessboardImagePixelLength - (m_linesPixelSpace + m_penPixelLength) * 4 - 1,
                    (m_linesPixelSpace + m_penPixelLength) * 4 - 1
                    ),
                m_ellipsePixelRadius,
                m_ellipsePixelRadius);
    painter.drawEllipse(
                QPoint(
                    (m_linesPixelSpace + m_penPixelLength) * 8 - 1,
                    (m_linesPixelSpace + m_penPixelLength) * 8 - 1
                    ),
                m_ellipsePixelRadius,
                m_ellipsePixelRadius);
    painter.drawEllipse(
                QPoint(
                    (m_linesPixelSpace + m_penPixelLength) * 4 - 1,
                    m_chessboardImagePixelLength - (m_linesPixelSpace + m_penPixelLength) * 4 - 1
                    ),
                m_ellipsePixelRadius,
                m_ellipsePixelRadius);
    painter.drawEllipse(
                QPoint(
                    m_chessboardImagePixelLength - (m_linesPixelSpace + m_penPixelLength) * 4 - 1,
                    m_chessboardImagePixelLength - (m_linesPixelSpace + m_penPixelLength) * 4 - 1
                    ),
                m_ellipsePixelRadius,
                m_ellipsePixelRadius);
    /* [PaintEllipses] **/


    /** [LoadPaintedImage] */
    painter.end();
    this->setPixmap(QPixmap::fromImage(*temporaryImage));
    /* [LoadPaintedImage] **/


    /** [SavePaintedImage] */
    m_vectorPlayImagesHistory.append(temporaryImage);
    /* [SavePaintedImage] **/
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

bool CGobangChessboard::paintChessman(int indexX, int indexY, CGobangChessman::CChessmanType chessmanType)
{
    QImage *temporaryImage = new QImage(*m_vectorPlayImagesHistory.last());
    QPainter painter;


    /** [SetPainter] */
    painter.begin(temporaryImage);
    switch(chessmanType)
    {
    default:
    {
        return false;
    }
    case CGobangChessman::CChessmanType::Type_Black:
    {
        painter.setPen(QPen(COLOR_EDGE));
        painter.setBrush(QBrush(COLOR_BLACK));
        break;
    }
    case CGobangChessman::CChessmanType::Type_White:
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
