#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class Snake; }
QT_END_NAMESPACE

class Snake : public QMainWindow
{
    Q_OBJECT

public:
    Snake(QWidget *parent = nullptr);
    ~Snake();
    void paintEvent(QPaintEvent* event);
    void InitSnake();
    void keyPressEvent(QKeyEvent* event);
    void isEat();
    void isHit();
    QRect CreateFood();

private slots:
    void SnakeUpdate();

private:
    Ui::Snake *ui;
    QTimer* timer; //定时器
    int direction; //蛇的移动方向

    int score;
    int hitNum;
    int speed;
    int life;
    int combo;
    bool isStart;
    bool isOver;
    QVector<QRect> vSnakeRect;
    QRect SnakeHead;
    QRect food;
    QString display;
};
#endif // SNAKE_H
