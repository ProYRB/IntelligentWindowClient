#include "cpainter.h"

CPainter::CPainter()
{

}

CPainter::~CPainter()
{
    ;
}

bool CPainter::drawMaze(const quint16 width, const quint16 height, const QImage::Format format)
{
    if(width == 0 || height == 0)   //判断参数是否正常
    {
        return false;
    }

    const QPoint startPoint(QRandomGenerator::global()->bounded(width), QRandomGenerator::global()->bounded(height - 1));   //随机获取起始点的坐标
    QPoint pointing(startPoint);    //焦点
    qint16 virtualBoard[width][height];     //虚拟画板
    qint16 direction;               //行进的方向<0无,1向上,2向下,3向左,4向右>
    QVector<QPoint> pointHistory;           //行进历史
    QVector<quint16> possibleDirections;    //可能的行进方向
    QVector<QPoint> longestVirtualPath;    //最长的行进记录

    for(int i = 0; i < width; ++i)          //初始化虚拟画板数据
    {
        for(int j = 0; j < height; ++j)
        {
            virtualBoard[i][j] = 0;         //默认值
        }
    }
    virtualBoard[startPoint.x()][startPoint.y()] = -1;

    pointHistory.append(pointing);
    for(int i = 0; pointHistory.size() > 0; ++i)
    {
        possibleDirections.clear();
        pointing = pointHistory.last();

        for(int j = pointing.x() - 1; j < pointing.x() + 2; ++j)     //获取可能的行进方向
        {
            if(j > -1 && j < width)
            {
                for(int k = pointing.y() - 1; k < pointing.y() + 2; ++k)
                {
                    if(k > -1 && k < height)
                    {
                        if(virtualBoard[j][k] == 0)
                        {
                            if(j == pointing.x() && k == pointing.y() - 1)
                            {
                                possibleDirections.append(1);
                            }
                            else if(j == pointing.x() && k == pointing.y() + 1)
                            {
                                possibleDirections.append(2);
                            }
                            else if(j == pointing.x() - 1 && k == pointing.y())
                            {
                                possibleDirections.append(3);
                            }
                            else if(j == pointing.x() + 1 && k == pointing.y())
                            {
                                possibleDirections.append(4);
                            }
                        }
                    }
                }
            }
        }
        if(possibleDirections.size() > 0)   //判断是否具有可能的行进方向
        {
            bool ifForword = false;
            do  //从可能的行进方向中随机获取一个方向
            {
                direction = QRandomGenerator::global()->bounded(1, 5);
                for(int j = 0; j < possibleDirections.size(); ++j)
                {
                    if(direction == possibleDirections.at(j))
                    {
                        ifForword = true;
                        break;
                    }
                }
            }
            while(!ifForword);
            switch(direction)
            {
            default:
            {
                return false;
                break;
            }
            case 1:
            {
                pointing.setY(pointing.y() - 1);
                pointHistory.append(pointing);
                virtualBoard[pointing.x()][pointing.y()] = 1;
                break;
            }
            case 2:
            {
                pointing.setY(pointing.y() + 1);
                pointHistory.append(pointing);
                virtualBoard[pointing.x()][pointing.y()] = 2;
                break;
            }
            case 3:
            {
                pointing.setX(pointing.x() - 1);
                pointHistory.append(pointing);
                virtualBoard[pointing.x()][pointing.y()] = 3;
                break;
            }
            case 4:
            {
                pointing.setX(pointing.x() + 1);
                pointHistory.append(pointing);
                virtualBoard[pointing.x()][pointing.y()] = 4;
                break;
            }
            }
        }
        else
        {
            if(pointHistory.size() > longestVirtualPath.size())
            {
                longestVirtualPath = pointHistory;
            }
            pointHistory.pop_back();
        }
    }

    //生成迷宫图片
    QImage basicDrawBoard(width * 2 + 1, height * 2 + 1, format);       //基础画板
    for(int i = 0; i < basicDrawBoard.width(); ++i)     //初始化迷宫背景色
    {
        for(int j = 0; j < basicDrawBoard.height(); ++j)
        {
            basicDrawBoard.setPixelColor(i, j, QIMAGE_COLOR_MAZE_BACKGROUND);   //设置指定像素的颜色
        }
    }
    for(int i = 0; i < width; ++i)
    {
        for(int j = 0; j < height; ++j)
        {
            //虚拟画板坐标与基本画板坐标转换关系：basicDrawBoardX(Y) = virtualBoardX(Y) * 2 + 1
            basicDrawBoard.setPixelColor(i * 2 + 1, j * 2 + 1, QIMAGE_COLOR_MAZE_ROAD);             //设置指定像素的颜色
            switch(virtualBoard[i][j])
            {
            default:
            {
                return false;
                break;
            }
            case -1:
            {
                basicDrawBoard.setPixelColor(i * 2 + 1, j * 2 + 1, QIMAGE_COLOR_MAZE_BEGIN);        //设置指定像素的颜色
                break;
            }
            case 1:
            {
                basicDrawBoard.setPixelColor(i * 2 + 1, (j * 2 + 1) + 1, QIMAGE_COLOR_MAZE_ROAD);   //设置指定像素的颜色
                break;
            }
            case 2:
            {
                basicDrawBoard.setPixelColor(i * 2 + 1, (j * 2 + 1) - 1, QIMAGE_COLOR_MAZE_ROAD);   //设置指定像素的颜色
                break;
            }
            case 3:
            {
                basicDrawBoard.setPixelColor((i * 2 + 1) + 1, j * 2 + 1, QIMAGE_COLOR_MAZE_ROAD);   //设置指定像素的颜色
                break;
            }
            case 4:
            {
                basicDrawBoard.setPixelColor((i * 2 + 1) - 1, j * 2 + 1, QIMAGE_COLOR_MAZE_ROAD);   //设置指定像素的颜色
                break;
            }
            }
        }
    }
    virtualBoard[longestVirtualPath.last().x()][longestVirtualPath.last().y()] = -2;    //设置迷宫终点
    basicDrawBoard.setPixelColor(longestVirtualPath.last().x() * 2 + 1, longestVirtualPath.last().y() * 2 + 1, QIMAGE_COLOR_MAZE_End);    //设置指定像素的颜色
    this->m_basicDrawBoard = basicDrawBoard;        //保存迷宫画板数据
    this->m_longestVirtualDrawBoardPath = longestVirtualPath;       //保存最长迷宫路径

    return true;
}

QImage CPainter::image()
{
    return this->m_basicDrawBoard;
}

QImage CPainter::enlargeImage(const QImage target, const quint16 power)
{
    QImage magnifiedBoard(target.width() * power, target.height() * power, target.format());
    for(int i = 0; i < target.width(); ++i)     //放大图像
    {
        for(int j = 0; j < target.height(); ++j)
        {
            for(int k = 0; k < power; ++k)      //为放大区域的像素填充颜色
            {
                for(int l = 0; l < power; ++l)
                {
                    magnifiedBoard.setPixelColor(i * power + k, j * power + l, target.pixelColor(i, j));    //设置指定像素的颜色
                }
            }
        }
    }
    return magnifiedBoard;
}

QVector<QImage> CPainter::longestMazePathImages()
{
    QVector<QImage> longestMazePathImages(this->m_longestVirtualDrawBoardPath.size() * 2 - 1, this->m_basicDrawBoard);      //预先定义容器大小和内容
    qint16 distanceX;
    qint16 distanceY;
    for(int i = 0; i < longestMazePathImages.size(); ++i)
    {
        if(i % 2 == 0)
        {
            longestMazePathImages[i].setPixelColor(this->m_longestVirtualDrawBoardPath.at(i / 2).x() * 2 + 1, this->m_longestVirtualDrawBoardPath.at(i / 2).y() * 2 + 1, QIMAGE_COLOR_MAZE_PLAYER);
        }
        else
        {
            if(i < this->m_longestVirtualDrawBoardPath.size() * 2 - 1)
            {
                distanceX = this->m_longestVirtualDrawBoardPath.at(i / 2 + 1).x() - this->m_longestVirtualDrawBoardPath.at(i / 2).x();
                distanceY = this->m_longestVirtualDrawBoardPath.at(i / 2 + 1).y() - this->m_longestVirtualDrawBoardPath.at(i / 2).y();
                longestMazePathImages[i].setPixelColor(this->m_longestVirtualDrawBoardPath.at(i / 2).x() * 2 + 1 + distanceX, this->m_longestVirtualDrawBoardPath.at(i / 2).y() * 2 + 1 + distanceY, QIMAGE_COLOR_MAZE_PLAYER);
            }
        }
    }
    return longestMazePathImages;
}
