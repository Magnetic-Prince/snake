#pragma once
#include "cocos2d.h"
using namespace std;
USING_NS_CC;


enum class ENUM_DIR
{  
    //class����������
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

class SnakeNode :public Sprite//����
{
public:
    ENUM_DIR snake_dir;  //��¼�ƶ�����
    float snake_row, snake_col;    //��ǰ�ڵ����������
    void setPositionRC(float row, float col);//Ŀ�������¶��嵥λ���Ȳ���������

    static SnakeNode* create(Part type);
    virtual bool init(Part type);

};

class Game_2 :public Scene
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Game_2);

    SnakeNode* spFood;//ʳ��
    SnakeNode* spHead;//��ͷ
    Vector<SnakeNode*> allBody;//����
    int m_score = 0;
    float time = 0.8;

    void Game_2::menuCloseCallback(Ref* pSender);
    void game(float t);//ˢ�³�����t��¼ˢ��ʱ��
    void newBody();//���һ���µ�����ڵ�
    void moveBody();//�ƶ����е�����ڵ�

private:
    map<int, Value>trans;
    string trans_content;
};