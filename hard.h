#pragma once
#include "cocos2d.h"
using namespace std;
USING_NS_CC;


enum class ENUM_DIR3
{
    //class����������
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class Part3
{
    HEAD,
    BODY,
    FOOD
};

class SnakeNode3 :public Sprite//����
{
public:
    ENUM_DIR3 snake_dir;  //��¼�ƶ�����
    int snake_row, snake_col;    //��ǰ�ڵ����������
    void setPositionRC(int row, int col);//Ŀ�������¶��嵥λ���Ȳ���������

    static SnakeNode3* create(Part3 type);
    virtual bool init(Part3 type);

};

class Game_3 :public Scene
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Game_3);

    SnakeNode3* spFood;//ʳ��
    SnakeNode3* spHead;//��ͷ
    Vector<SnakeNode3*> allBody;//����
    int m_score = 0;
    float time = 0.4;

    void Game_3::menuCloseCallback(Ref* pSender);
    void game(float t);//ˢ�³�����t��¼ˢ��ʱ��
    void newBody();//���һ���µ�����ڵ�
    void moveBody();//�ƶ����е�����ڵ�

private:
    map<int, Value>trans;
    string trans_content;
};