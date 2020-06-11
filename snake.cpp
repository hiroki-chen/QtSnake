#include "snake.h"
#include "ui_snake.h"
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <random>

Snake:: Snake(QWidget *parent): QMainWindow(parent), ui(new Ui::Snake), hitNum(0), speed(500), life(3), combo(0) {
    ui -> setupUi(this);
    this -> setGeometry(QRect(0, 0, 400, 400));
}

Snake:: ~Snake() {
    delete ui;
}

void Snake:: paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    if (!isStart) { InitSnake(); }

    //大的矩形里面套了一个小的矩形。
    painter.setPen(Qt::black);
    painter.setBrush(Qt::gray);
    painter.drawRect(15, 15, 260, 260);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(20, 20, 250, 250);

    //画格子
    painter.setPen(Qt::blue);
    for (int i = 2; i <= 27; i++) {
        painter.drawLine(20, i * 10, 270, i * 10); //起始点和终止点。
        painter.drawLine(i * 10, 20, i * 10, 270);
    }
    //游戏开始。
    QFont font1("Simsum", 24);
    painter.setFont(font1);
    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);
    painter.drawText(40, 150, display);

    if (isOver) {
        timer -> stop();
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        painter.drawText(20,50, "您的得分为:" + QString::number(score));
    }

    //画蛇。
    painter.setPen(Qt::black);
    painter.setBrush(Qt::yellow);
    painter.drawRects(&vSnakeRect[0], vSnakeRect.size());

    //画食物
    painter.setPen(Qt::black);
    painter.setBrush(Qt::red);
    painter.drawRect(food);

    //画生命和显示数据
    painter.setPen(Qt::black);
    painter.setBrush(Qt::red);
    painter.drawText(400, 50, QString::number(life));
    painter.setPen(Qt::green);
    painter.setBrush(Qt::green);
    painter.drawText(300, 100, "成功获得积分：" + QString::number(10 + hitNum * 10));
    if (combo >= 2) { painter.drawText(300, 130, "Combo数量：" + QString::number(combo)); }
}

void Snake:: InitSnake() {
    isOver = false;
    isStart = true;
    direction = 3;
    score = 0;
    speed = 500;
    food = CreateFood();
    display = "游戏开始!";
    vSnakeRect.resize(5);
    for (int i = 0; i < vSnakeRect.size(); ++i) {
        QRect rect(100, 70 + 10 * i, 10, 10);
        vSnakeRect[vSnakeRect.size() - i - 1] = rect;
    }

        timer = new QTimer(this); //设置计时器的时间。
        timer -> start(speed); //设置时间500ms
        connect(timer, SIGNAL(timeout()), SLOT(SnakeUpdate())); //信号和槽的关联，当timeout时，进行更新。

}

void Snake:: SnakeUpdate() {
    display = "";
    SnakeHead = vSnakeRect.first(); //获取蛇头。
    isEat();
    isHit();
    for (int j = 0; j < vSnakeRect.size() - 1; j++) {
        vSnakeRect[vSnakeRect.size() - j - 1] = vSnakeRect[vSnakeRect.size() - j - 2];
    }
    //键盘事件。
    switch(direction) {
    case 1:
        SnakeHead.setTop(SnakeHead.top() - 10);
        SnakeHead.setBottom(SnakeHead.bottom() - 10);
        break;
    case 2:
        SnakeHead.setTop(SnakeHead.top() + 10);
        SnakeHead.setBottom(SnakeHead.bottom() + 10);
        break;
    case 3:
        SnakeHead.setLeft(SnakeHead.left() - 10);
        SnakeHead.setRight(SnakeHead.right() - 10);
        break;
    case 4:
        SnakeHead.setLeft(SnakeHead.left() + 10);
        SnakeHead.setRight(SnakeHead.right() + 10);
        break;
    default:
        break;
    }
    vSnakeRect[0] = SnakeHead;
    if (SnakeHead.top() < 20 || SnakeHead.bottom() > 270 || SnakeHead.left() < 20 || SnakeHead.right() > 270) {
        isOver = true;
        display = "游戏结束！";
    }
    update(); //更新一下。

}

void Snake::keyPressEvent(QKeyEvent *event) {
    QKeyEvent* key = (QKeyEvent*) event;
    switch(key -> key()) {
    case Qt::Key_W:
        direction = 1;
        break;
    case Qt::Key_S:
        direction = 2;
        break;
    case Qt::Key_A:
        direction = 3;
        break;
    case Qt::Key_D:
        direction = 4;
        break;
    default:
        break;
    }
}

QRect Snake:: CreateFood() {
    std::random_device rd;
    std::mt19937 mt(rd());
    int x = mt() % 25;
    int y = mt() % 25;
    QRect rect(20 + x * 10, 20 + y * 10, 10, 10);
    return rect;
}

void Snake:: isEat() {
    if (SnakeHead == food) {
        hitNum++;
        combo++;
        SnakeHead = food;
        vSnakeRect.push_back(vSnakeRect.last());
        food = CreateFood();
        score += 10 + hitNum * 10;
        if (speed > 50) {
            speed -= 10 + hitNum * 2;
            timer -> stop();
            timer -> start(speed);
        }
    }
}

void Snake:: isHit() {
    for (int i = 1; i < vSnakeRect.size(); i++) {
        if (SnakeHead == vSnakeRect[i]) {
            combo = 0;
            if (--life == 0) {
                display = "游戏结束！";
                isOver = true;
            }
            update();
        }
    }
}
