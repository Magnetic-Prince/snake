#pragma once
#include "cocos2d.h"
using namespace std;
USING_NS_CC;


enum class ENUM_DIR1
{
    //class����������
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

class SnakeNode1 :public Sprite//����
{
public:
    ENUM_DIR1 snake_dir;  //��¼�ƶ�����
    int snake_row, snake_col;    //��ǰ�ڵ����������
    void setPositionRC(int row, int col);//Ŀ�������¶��嵥λ���Ȳ���������

    static SnakeNode1* create(Part1 type);
    virtual bool init(Part1 type);

};

class Game_1 :public Scene
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Game_1);

    SnakeNode1* spFood;//ʳ��
    SnakeNode1* spHead;//��ͷ
    Vector<SnakeNode1*> allBody;//����
    int m_score = 0;
    float time = 0.3;

    void Game_1::menuCloseCallback(Ref* pSender);
    void game(float t);//ˢ�³�����t��¼ˢ��ʱ��
    void newBody();//���һ���µ�����ڵ�
    void moveBody();//�ƶ����е�����ڵ�

private:
    map<int, Value>trans;
    string trans_content;
    string trans_content_1;
    Label* trans_label;
};