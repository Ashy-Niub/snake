#ifndef SNAKE_H
#define SNAKE_H
#include <QLabel>
#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <time.h>
#include <stdlib.h>
#define X_MAX 25
#define Y_MAX 20
#define BLOCK_SIZE 30
// 蛇结点结构体
struct snakeNode {
    QLabel *label;   // 保存label，
    int x;
    int y;
    int type;
};

enum Label
{
    NORMAL_LABEL,   // 背景
    BORD_LABEL,     // 边界
    SNAKE_LABEL,    // 蛇
    FOOD_LABEL      // 食物
};


class Snake : public QMainWindow
{
    Q_OBJECT

public:
    explicit Snake(QWidget *parent = 0);
    ~Snake();
    void init();
    void drawBorder();
    void snakeInit();
    void getSnake();
    void snakeMove();
    void gameOver();
    void foodupdate();
    void timerRestart();
    bool up();
    bool down();
    bool left();
    bool right();
    void scoreAdd();

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void snakeMoveSlots();

private:
    snakeNode *matrix[X_MAX+7][Y_MAX+7];    // 界面储存,所有的格格
    QList<snakeNode*> snake;    // 蛇身
    QLabel *mLabel;
    snakeNode *head;
    snakeNode *tail;
    int snakeLenth;
    int derectionX=0, derectionY=0;
    QTimer timer;
    int nowScore=0;
};

#endif // SNAKE_H
