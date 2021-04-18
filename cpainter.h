#ifndef CPAINTER_H
#define CPAINTER_H

#include <QDebug>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QRandomGenerator>

#define QIMAGE_COLOR_MAZE_BACKGROUND QColor(20, 20, 20, 255)
#define QIMAGE_COLOR_MAZE_ROAD QColor(250, 250, 250, 255)
#define QIMAGE_COLOR_MAZE_PLAYER QColor(255, 0, 0, 255)
#define QIMAGE_COLOR_MAZE_End QColor(0, 255, 0, 255)
#define QIMAGE_COLOR_MAZE_BEGIN QColor(0, 0, 255, 255)

class CPainter : public QObject
{
    Q_OBJECT
public:
    CPainter();
    ~CPainter();

    bool drawMaze(const quint16 width, const quint16 height, const QImage::Format format);
    QImage enlargeImage(const QImage target, const quint16 power);
    QVector<QImage> longestMazePathImages();


    QImage image();

private:
    QImage m_basicDrawBoard;
    QVector<QPoint> m_longestVirtualDrawBoardPath;

private slots:

};

#endif // CPAINTER_H
