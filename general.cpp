#define schedule_selector(_SELECTOR) static_cast<cocos2d::SEL_SCHEDULE>(&_SELECTOR)
#include "general.h"
#include"menudemo.h"
#include"ui/CocosGUI.h"
#include"stdlib.h"
#include "place.h"
#include "HelloWorldScene.h"
using namespace std;
USING_NS_CC;

const float NET_SIZE = 50;//���赥λ��С,�����ƶ�λ�ò��׿��ƣ������׳Ե�ʳ��

Scene* Game_2::createScene()
{
    return Game_2::create();
}

void SnakeNode::setPositionRC(float row, float col)//���ýڵ������
{
    this->snake_row = row;
    this->snake_col = col;
    setPosition(Vec2(col * NET_SIZE, row * NET_SIZE));
}

SnakeNode* SnakeNode::create(Part type)//Ӧ��init
{
    SnakeNode* part = new SnakeNode();
    if (part == NULL)
    {
        delete part;
        exit(0);
    }
    part->init(type);
    part->autorelease(); //�߱�release���ܣ�����return֮���ִ��
    return part;
}

bool SnakeNode::init(Part type)//������ͷ�������Լ�ʳ��
{
    if (!Sprite::init())
    {
        return false;
    }

    switch (type) 
    {
    case Part::HEAD:
    {
        auto sprite = Sprite::create("snake_head.jpg");
        sprite->setAnchorPoint(Vec2::ZERO);//ê����Ϊ0�����ڿ���ͼ�β����߽磨ֻ�ı������߽��λ�ü��ɣ�
        this->addChild(sprite);

        snake_dir = ENUM_DIR::RIGHT;//��ʼ���ƶ����������ƶ�
       
        snake_row = 0;//��ʼ����ͷ��λ��,���½�
        snake_col = 0;
    }
    break;

    case Part::BODY:
    {
        auto sprite = Sprite::create("snake_body.jpg");
        sprite->setAnchorPoint(Vec2::ZERO);
        this->addChild(sprite);
    }
    break;

    case Part::FOOD:
    {
        auto sprite = Sprite::create("food.jpg");
        sprite->setAnchorPoint(Vec2::ZERO);
        this->addChild(sprite);
    }
    break;

    default:
        break;
    }
    return true;
}



bool  Game_2::init() 
{
    if (!Scene::init())
    {
        return  false;
    }

    auto configData = FileUtils::getInstance()->getValueMapFromFile("res/tips.plist");//��ʾ����
    for (auto abs : configData)
    {
        int key = atoi(abs.first.c_str());
        Value val = abs.second;
        CCLOG("%d,%s", key, val);
        trans.insert(pair<int, Value>(key, val));
    }


    //ȷ�������С�ͻ�����
    auto size = Director::getInstance()->getVisibleSize();
    Vec2 base = Vec2(size / 2);

    //��ӱ���
    auto spritebg = Sprite::create("game.jpg");
    spritebg->setPosition(base);
    spritebg->setOpacity(90);//���ô���͸����
    this->addChild(spritebg);

    //�����ͷ
    spHead = SnakeNode::create(Part::HEAD);
    this->addChild(spHead);

    //���ʳ��
    spFood = SnakeNode::create(Part::FOOD);
    int row = rand() % 16;//�������ʳ��
    int col = rand() % 16;
    spFood->setPositionRC(row, col);
    this->addChild(spFood);

    //�÷���ʾ
    m_score = 0;
    auto labelScore = Label::createWithSystemFont("grade: 0", "����", 50);
    labelScore->setTag(10);
    labelScore->setPosition(Point(size.width - 150, size.height - 50));
    labelScore->setColor(Color3B(250, 100, 150));
    this->addChild(labelScore);

    //���ذ�ť
    trans_content = trans.at(1008).asString();
    auto menuItemBack = MenuItemFont::create(trans_content, CC_CALLBACK_1(Game_2::menuCloseCallback, this));
    auto menu = Menu::create(menuItemBack, NULL);
    menu->setColor(Color3B(250, 100, 150));
    menu->setPosition(Vec2::ZERO);
    menuItemBack->setPosition(Vec2(20,20));
    this->addChild(menu);

    //ˢ�£��Դﵽ�˶�Ч��
    this->schedule(schedule_selector(Game_2::game), 0.8f);

    //�����û������¼�
    auto mykey = EventListenerKeyboard::create();
    mykey->onKeyPressed = [&](EventKeyboard::KeyCode keycode, Event* event)//���¼���Ӧ
    {
        int key = static_cast<int>(keycode);
        switch (key)
        {
        case 28:
            spHead->snake_dir = ENUM_DIR::UP;
            break;
        case 29:
            spHead->snake_dir = ENUM_DIR::DOWN;
            break;
        case 26:
            spHead->snake_dir = ENUM_DIR::LEFT;
            break;
        case 27:
            spHead->snake_dir = ENUM_DIR::RIGHT;
            break;
        default:
            break;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mykey, this);

    //�����û������¼�
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);//��ť�ص�ʱֻ��Ӧ�������Ǹ���ť
    listener->onTouchBegan = [&](Touch* t, Event* e) 
    {
        //�ı�̰ʳ���ƶ��ķ���
        int col = t->getLocation().x / NET_SIZE;
        int row = t->getLocation().y / NET_SIZE;
        int HeadCol = spHead->getPositionX() / NET_SIZE;
        int HeadRow = spHead->getPositionY() / NET_SIZE;
        if (abs(HeadCol - col) > abs(HeadRow - row))//����
        {
            if (HeadCol < col)//����ҷ�
            {
                spHead->snake_dir = ENUM_DIR::RIGHT;
            }
            else
            {
                spHead->snake_dir = ENUM_DIR::LEFT;
            }
        }
        else//����
        {
            if (HeadRow < row)//����Ϸ�
            {
                spHead->snake_dir = ENUM_DIR::UP;
            }
            else
            {
                spHead->snake_dir = ENUM_DIR::DOWN;
            }
        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void Game_2::game(float t)
{
    moveBody();//�ƶ���������ڵ�

    auto configData = FileUtils::getInstance()->getValueMapFromFile("res/tips.plist");//��ʾ����
    for (auto abs : configData)
    {
        int key = atoi(abs.first.c_str());
        Value val = abs.second;
        CCLOG("%d,%s", key, val);
        trans.insert(pair<int, Value>(key, val));
    }

    //ȷ�������С�ͻ�����
    auto size = Director::getInstance()->getVisibleSize();
    Vec2 base = Vec2(size / 2);

    //��ͷ�ƶ�
    switch (spHead->snake_dir)
    {
    case ENUM_DIR::RIGHT:
        spHead->runAction(MoveBy::create(time, Vec2(NET_SIZE, 0)));
        spHead->snake_col++;
        break;
    case ENUM_DIR::LEFT:
        spHead->runAction(MoveBy::create(time,Vec2(-NET_SIZE, 0)));
        spHead->snake_col--;
        break;
    case ENUM_DIR::DOWN:
        spHead->runAction(MoveBy::create(time, Vec2(0, -NET_SIZE)));
        spHead->snake_row--;
        break;
    case ENUM_DIR::UP:
        spHead->runAction(MoveBy::create(time, Vec2(0, NET_SIZE)));
        spHead->snake_row++;
        break;
    default:
        break;
    }

    //ͷײ���߽�����
    if (spHead->snake_row < 0 || spHead->snake_col < 0 ||
        spHead->snake_row >= 16 || spHead->snake_col >= 16)
    {
        FILE* in;
        if ((in = fopen("file.txt", "ab+")) == NULL)
        {
            exit(EXIT_FAILURE);
        }
        fwrite(&m_score, 4, 1, in);
        fclose(in);
        auto blink = Blink::create(0.7f, 3);
        spHead->runAction(blink);
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.7f, HelloWorld::createScene()));
    }

    //ʳ����ײ���
    if (spHead->snake_row == spFood->snake_row &&
        spHead->snake_col == spFood->snake_col)
    {
        //��ʳ��Ч��
        auto blink = Blink::create(0.7, 4);
        spHead->runAction(blink);

        //��������
        m_score += 1;
        auto label = (Label*)getChildByTag(10);
        char strscore[20];
        sprintf(strscore, "grade: %d", m_score);
        label->setColor(Color3B(250, 100, 150));
        label->setString(strscore);
           
        
        //ʳ������µ�λ��
        float row = rand() % 16;
        float col = rand() % 16;
        spFood->setPositionRC(row, col);

        //�Ե�ʳ���ӿ��ٶ�
        time *= 0.8;

        //��ӽڵ�
        newBody();
    }
}

void Game_2::newBody()//���һ���µ�����ڵ�
{
    auto bodynode = SnakeNode::create(Part::BODY);

    if (allBody.size() > 0)
    {   //������ڵ�
        auto lastbody = allBody.at(allBody.size() - 1);//ȡ�����½ڵ�������һ���ڵ�
        bodynode->snake_dir = lastbody->snake_dir;    //�½ڵ�ķ���Ϊ���һ������Ľڵ�

        switch (bodynode->snake_dir) {
        case ENUM_DIR::UP:
            bodynode->setPositionRC(lastbody->snake_row - 1, lastbody->snake_col);
            break;
        case ENUM_DIR::DOWN:
            bodynode->setPositionRC(lastbody->snake_row + 1, lastbody->snake_col);
            break;
        case ENUM_DIR::LEFT:
            bodynode->setPositionRC(lastbody->snake_row, lastbody->snake_col + 1);
            break;
        case ENUM_DIR::RIGHT:
            bodynode->setPositionRC(lastbody->snake_row, lastbody->snake_col - 1);
            break;
        default:
            break;
        }

    }
    else
    {
        //�½ڵ�ķ��������ͷ�ķ���
        bodynode->snake_dir = spHead->snake_dir;
        switch (bodynode->snake_dir) {
        case ENUM_DIR::UP:
            bodynode->setPositionRC(spHead->snake_row - 1, spHead->snake_col);
            break;
        case ENUM_DIR::DOWN:
            bodynode->setPositionRC(spHead->snake_row + 1, spHead->snake_col);
            break;
        case ENUM_DIR::LEFT:
            bodynode->setPositionRC(spHead->snake_row, spHead->snake_col + 1);
            break;
        case ENUM_DIR::RIGHT:
            bodynode->setPositionRC(spHead->snake_row, spHead->snake_col - 1);
            break;
        default:
            break;
        }
    }
    //����½ڵ㵽��ǰͼ��
    this->addChild(bodynode);

    //����½ڵ㵽������
    allBody.pushBack(bodynode);
}

void Game_2::moveBody()//�ƶ����е�����ڵ�
{
    if (0==allBody.size()) 
    { 
        return;
    }
    for (auto bodynode : allBody)
    {
        switch (bodynode->snake_dir) {
        case ENUM_DIR::RIGHT:
            bodynode->runAction(MoveBy::create(time, Vec2(NET_SIZE, 0)));
            bodynode->snake_col++;
            break;
        case ENUM_DIR::LEFT:
            bodynode->runAction(MoveBy::create(time, Vec2(-NET_SIZE, 0)));
            bodynode->snake_col--;
            break;
        case ENUM_DIR::DOWN:
            bodynode->runAction(MoveBy::create(time, Vec2(0, -NET_SIZE)));
            bodynode->snake_row--;
            break;
        case ENUM_DIR::UP:
            bodynode->runAction(MoveBy::create(time, Vec2(0, NET_SIZE)));
            bodynode->snake_row++;
            break;
        default:
            break;
        }
    }
    //�ƶ����֮�󣬸ı�ÿ��body�ķ���
    for (int i = allBody.size() - 1; i > 0; i--)
    { //ÿ���ڵ�� �������Ϊ��ǰһ���ڵ�ķ���
        allBody.at(i)->snake_dir = allBody.at(i - 1)->snake_dir;
    }
    allBody.at(0)->snake_dir = spHead->snake_dir;



    //ͷײ����������
    for (int i = 1; i < allBody.size(); i++)
    {
        if (spHead->snake_row == allBody.at(i)->snake_row && spHead->snake_col == allBody.at(i)->snake_col)
        {
            FILE* in;
            if ((in = fopen("file.txt", "ab+")) == NULL)
            {
                exit(EXIT_FAILURE);
            }
            fwrite(&m_score, 4, 1, in);
            fclose(in);
            auto blink = Blink::create(0.7f, 3);
            spHead->runAction(blink);
            Director::getInstance()->replaceScene(TransitionCrossFade::create(0.7f, HelloWorld::createScene()));
        }
    }
}

void Game_2::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.7f, HelloWorld::createScene()));
}