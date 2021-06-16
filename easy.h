#pragma once
#include "cocos2d.h"
using namespace std;
USING_NS_CC;


enum class ENUM_DIR1
{
    //class限制作用域
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class Part1
{
    HEAD,
    BODY,
    FOOD
};

class SnakeNode1 :public Sprite//蛇类
{
public:
    ENUM_DIR1 snake_dir;  //记录移动方向
    int snake_row, snake_col;    //当前节点的行列坐标
    void setPositionRC(int row, int col);//目的是重新定义单位长度并加入坐标

    static SnakeNode1* create(Part1 type);
    virtual bool init(Part1 type);

};

class Game_1 :public Scene
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Game_1);

    SnakeNode1* spFood;//食物
    SnakeNode1* spHead;//蛇头
    Vector<SnakeNode1*> allBody;//身体
    int m_score = 0;
    float time = 0.3;

    void Game_1::menuCloseCallback(Ref* pSender);
    void game(float t);//刷新场景，t记录刷新时间
    void newBody();//添加一个新的身体节点
    void moveBody();//移动所有的身体节点

private:
    map<int, Value>trans;
    string trans_content;
    string trans_content_1;
    Label* trans_label;
};