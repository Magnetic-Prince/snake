#pragma once
#include "cocos2d.h"
using namespace std;
USING_NS_CC;


enum class ENUM_DIR
{  
    //class限制作用域
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class Part
{
    HEAD,
    BODY,
    FOOD
};

class SnakeNode :public Sprite//蛇类
{
public:
    ENUM_DIR snake_dir;  //记录移动方向
    float snake_row, snake_col;    //当前节点的行列坐标
    void setPositionRC(float row, float col);//目的是重新定义单位长度并加入坐标

    static SnakeNode* create(Part type);
    virtual bool init(Part type);

};

class Game_2 :public Scene
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Game_2);

    SnakeNode* spFood;//食物
    SnakeNode* spHead;//蛇头
    Vector<SnakeNode*> allBody;//身体
    int m_score = 0;
    float time = 0.8;

    void Game_2::menuCloseCallback(Ref* pSender);
    void game(float t);//刷新场景，t记录刷新时间
    void newBody();//添加一个新的身体节点
    void moveBody();//移动所有的身体节点

private:
    map<int, Value>trans;
    string trans_content;
};