#include "snake.h"
#include <QMessageBox>
#include <QString>
#include <QtDebug>
#include <windows.h>
#include <QThread>
#include <QStyle>
#include <QObject>
#include <QDebug>
#include <QMessageBox>

int food=0;

Snake::Snake(QWidget *parent) :
    QMainWindow(parent)
{
    srand(time(NULL));
    this->setWindowTitle("Snake");
    this->resize((X_MAX + 7) * BLOCK_SIZE,Y_MAX * BLOCK_SIZE);

    mLabel = new QLabel(this);
    mLabel->setGeometry(X_MAX * BLOCK_SIZE,BLOCK_SIZE,6 * BLOCK_SIZE,(8) * BLOCK_SIZE);
    QFont fontView("微软雅黑",16);
    mLabel->setFont(fontView);
//    mLabel->setAlignment(Qt::AlignCenter);

    mLabel->show();
    mLabel->setText("分数: " + QString::number(nowScore));
    mLabel->setAlignment(Qt::AlignCenter);  // 设置分数的label, 并给其设为居中
    init();

    drawBorder();
    snakeInit();
    foodupdate();
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(snakeMoveSlots()));


}

Snake::~Snake()
{
}

void Snake::gameOver() {
    timer.stop();
    QMessageBox::information(this,"游戏结束","游戏结束:是否重开",QMessageBox::Ok);
    for(int x=0; x<X_MAX+7; x++) {
        for(int y=0; y<Y_MAX; y++) {
            matrix[x][y]->label->setStyleSheet("background:gray");
            matrix[x][y]->label->hide();
            matrix[x][y]->type = NORMAL_LABEL;
        }
    }
    drawBorder();
    snakeInit();
    nowScore = 0;
    foodupdate();
    timer.start(500);
}

void Snake::foodupdate()
{
    int tag = rand()%3+1; // 判断是否高级果实
    food = 0;   // 先默认为初级果实
    while(1) {
        int x = rand()%X_MAX;
        int y = rand()%Y_MAX;
        if(matrix[x][y]->type != SNAKE_LABEL && matrix[x][y]->type != BORD_LABEL) {
            matrix[x][y]->label->setStyleSheet("background:green");
            if(tag == 3) {
                matrix[x][y]->label->setStyleSheet("background:red");
                food = 1;
            }
            matrix[x][y]->label->show();
            matrix[x][y]->type = FOOD_LABEL;
            break;
        }
    }
}


void Snake::init()
{

    for(int x=0; x<X_MAX+7; x++) {
        for(int y=0; y<Y_MAX; y++) {
            QLabel *label = new QLabel(this);
            label->setStyleSheet("background:gray");
            label->setGeometry(x*BLOCK_SIZE, y*BLOCK_SIZE, BLOCK_SIZE-1, BLOCK_SIZE-1);
            label->hide();

            matrix[x][y] = new snakeNode();
            matrix[x][y]->x = x;
            matrix[x][y]->y = y;
            matrix[x][y]->type = NORMAL_LABEL;
            matrix[x][y]->label = label;
        }
    }
    timer.start(500);
}

// 画边界
void Snake::drawBorder()
{
    for(int x=0; x<X_MAX+7; x++) {
        for(int y=0; y<Y_MAX; y++) {
            if(x == X_MAX-1 || y == Y_MAX-1 || x == 0 || y == 0 || x == X_MAX+6) {
                matrix[x][y]->type = BORD_LABEL;
                matrix[x][y]->label->show();
            }
        }
    }
}

// 得到蛇头和蛇尾
void Snake::getSnake()
{
    head = snake.at(snakeLenth-1);
    tail = snake.at(0);
}

// 蛇的初始化
void Snake::snakeInit()
{
    snake.clear();  // 游戏结束后重开清空蛇的大小
    snakeLenth = 3;
    derectionX=1,derectionY=0;   // 蛇一开始往右走
    int x=5,y=5;

    for(int i=0; i<snakeLenth; i++) {
        snake.append(matrix[x+i][y]);    // 综合下来, 头的x=8
        snake[i]->x = x+i;
        snake[i]->y = y;
        snake[i]->type = SNAKE_LABEL;
        snake[i]->label->setStyleSheet("background:pink");
        snake[i]->label->show();
    }
}

void Snake::snakeMoveSlots(){
    snakeMove();
    qDebug()<<"1"<<endl;
}

// 蛇的移动
void Snake::snakeMove()
{
    getSnake(); // 刷新表尾, 即蛇头
    snakeNode *newNode= matrix[head->x+derectionX][head->y+derectionY];
    newNode->label->setStyleSheet("background:pink");
    newNode->label->show();

    // 判断!
    if(newNode->type == BORD_LABEL || newNode->type == SNAKE_LABEL) {
        gameOver();
    }else {
        if(newNode->type == NORMAL_LABEL) {
            snake.append(newNode);
            newNode->type = SNAKE_LABEL;
            tail->type = NORMAL_LABEL;
            tail->label->hide();
            snake.removeFirst();

        }else if(newNode->type == FOOD_LABEL) {
            snake.append(newNode);
            snakeLenth++;
            newNode->type = SNAKE_LABEL;
            scoreAdd();
            foodupdate();
        }
    }
}

void Snake::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_Up:
        if(up()) {
            derectionY = -1;
            derectionX = 0;
            snakeMove();
            timerRestart(); // 移动完重启timer
        }
        break;

    case Qt::Key_Down:
        if(down()) {
            derectionY = 1;
            derectionX = 0;
            snakeMove();
            timerRestart();
        }
        break;

    case Qt::Key_Left:
        if(left()) {
            derectionY = 0;
            derectionX = -1;
            snakeMove();
            timerRestart();
        }
        break;
    case Qt::Key_Right:
        if(right()) {
            derectionY = 0;
            derectionX = 1;
            snakeMove();
            timerRestart();
        }
        break;

    default:
        break;
    }
}

void Snake::timerRestart()
{
    timer.stop();
    timer.start(500);
}

bool Snake::up()
{
    if(derectionY!=-1){
        return true;
    }
    return false;
}

bool Snake::down()
{
    if(derectionY!=1){
        return true;
    }
    return false;
}

bool Snake::left()
{
    if(derectionX!=-1){
        return true;
    }
    return false;
}

bool Snake::right()
{
    if(derectionX!=1){
        return true;
    }
    return false;
}

void Snake::scoreAdd()
{
    if(food == 1) {
        nowScore += 3;
    }else {
        nowScore++;
    }
    mLabel->setText("分数: " + QString::number(nowScore));

}
