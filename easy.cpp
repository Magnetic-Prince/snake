#define schedule_selector(_SELECTOR) static_cast<cocos2d::SEL_SCHEDULE>(&_SELECTOR)
#include "easy.h"
#include"menudemo.h"
#include"ui/CocosGUI.h"
#include"stdlib.h"
#include "place.h"
#include "HelloWorldScene.h"

using namespace std;
USING_NS_CC;

const float NET_SIZE = 50;//���赥λ��С,�����ƶ�λ�ò��׿��ƣ������׳Ե�ʳ��

Scene* Game_1::createScene()
{
    return Game_1::create();
}

void SnakeNode1::setPositionRC(int row, int col)//���ýڵ������
{
    this->snake_row = row;
    this->snake_col = col;
    setPosition(Vec2(col * NET_SIZE, row * NET_SIZE));
}

SnakeNode1* SnakeNode1::create(Part1 type)//Ӧ��init
{
    SnakeNode1* part = new SnakeNode1();
    if (part == NULL)
    {
        delete part;
        exit(0);
    }
    part->init(type);
    part->autorelease(); //�߱�release���ܣ�����return֮���ִ��
    return part;
}

bool SnakeNode1::init(Part1 type)//������ͷ�������Լ�ʳ��
{
    if (!Sprite::init())
    {
        return false;
    }

    switch (type)
    {
    case Part1::HEAD:
    {
        auto sprite = Sprite::create("snake_head.jpg");
        sprite->setAnchorPoint(Vec2::ZERO);//ê����Ϊ0�����ڿ���ͼ�β����߽磨ֻ�ı������߽��λ�ü��ɣ�
        this->addChild(sprite);

        snake_dir = ENUM_DIR1::RIGHT;//��ʼ���ƶ����������ƶ�

        snake_row = 0;//��ʼ����ͷ��λ��,���½�
        snake_col = 0;
    }
    break;

    case Part1::BODY:
    {
        auto sprite = Sprite::create("snake_body.jpg");
        sprite->setAnchorPoint(Vec2::ZERO);
        this->addChild(sprite);
    }
    break;

    case Part1::FOOD:
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



bool  Game_1::init()
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
    auto spritebg = Sprite::create("game_easy.jpg");
    spritebg->setPosition(base);
    spritebg->setOpacity(90);//���ô���͸����
    this->addChild(spritebg);


    //�����ͷ
    spHead = SnakeNode1::create(Part1::HEAD);
    this->addChild(spHead);

    //���ʳ��
    spFood = SnakeNode1::create(Part1::FOOD);
    int row = rand() % 16;//�������ʳ��
    int col = rand() % 16;
    spFood->setPositionRC(row, col);
    this->addChild(spFood);

    //�÷���ʾ
    m_score = 0;
    auto labelScore = Label::createWithSystemFont("grade: 0", "����", 40);
    labelScore->setTag(10);
    labelScore->setPosition(Point(size.width - 140, size.height - 55));
    labelScore->setColor(Color3B(250, 100, 150));
    this->addChild(labelScore);

    //������ʾ
    trans_content_1 = trans.at(1012).asString();
    trans_label = Label::createWithSystemFont("", "����Ҧ��", 25);
    trans_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    trans_label->setPosition(Vec2(size / 2) + Vec2(60, 375));
    trans_label->setColor(Color3B(255, 100, 100));//��ɫ��ɫ��ɫ
    trans_label->enableBold();
    trans_label->setDimensions(900, 0);
    this->addChild(trans_label);

    static unsigned int i = 0;//������
    this->schedule([&](float dlt) {//��������
        char ch = trans_content_1[i];
        if (ch > -127 && ch < 0)//�ж��ַ��Ƿ�Ϊ����
        {
            i += 3;//����ռ�����ֽ�
        }
        else
        {
            i++;
        }

        string str = trans_content_1.substr(0, i);//�����ӳ���������ò��ϴ�ӡЧ��

        trans_label->setString(str);

        if (i > trans_content_1.length())//�ж��ļ�����
        {
            this->unschedule("schedule_callback");
        }

        }, 0.3f, "schedule_callback");


    //���ذ�ť
    trans_content = trans.at(1008).asString();
    auto menuItemBack = MenuItemFont::create(trans_content, CC_CALLBACK_1(Game_1::menuCloseCallback, this));
    auto menu = Menu::create(menuItemBack, NULL);
    menu->setColor(Color3B(250, 100, 150));
    menu->setPosition(Vec2::ZERO);
    menuItemBack->setPosition(Vec2(20, 20));
    this->addChild(menu);

    //ˢ�£��Դﵽ�˶�Ч��
    this->schedule(schedule_selector(Game_1::game), time);

    //�����û������¼�
    auto mykey = EventListenerKeyboard::create();
    mykey->onKeyPressed = [&](EventKeyboard::KeyCode keycode, Event* event)//���¼���Ӧ
    {
        int key = static_cast<int>(keycode);
        switch (key)
        {
        case 28:
            spHead->snake_dir = ENUM_DIR1::UP;
            break;
        case 29:
            spHead->snake_dir = ENUM_DIR1::DOWN;
            break;
        case 26:
            spHead->snake_dir = ENUM_DIR1::LEFT;
            break;
        case 27:
            spHead->snake_dir = ENUM_DIR1::RIGHT;
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
                spHead->snake_dir = ENUM_DIR1::RIGHT;
            }
            else
            {
                spHead->snake_dir = ENUM_DIR1::LEFT;
            }
        }
        else//����
        {
            if (HeadRow < row)//����Ϸ�
            {
                spHead->snake_dir = ENUM_DIR1::UP;
            }
            else
            {
                spHead->snake_dir = ENUM_DIR1::DOWN;
            }
        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void Game_1::game(float t)
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
    case ENUM_DIR1::RIGHT:
        spHead->runAction(MoveBy::create(time, Vec2(NET_SIZE, 0)));
        spHead->snake_col++;
        break;
    case ENUM_DIR1::LEFT:
        spHead->runAction(MoveBy::create(time, Vec2(-NET_SIZE, 0)));
        spHead->snake_col--;
        break;
    case ENUM_DIR1::DOWN:
        spHead->runAction(MoveBy::create(time, Vec2(0, -NET_SIZE)));
        spHead->snake_row--;
        break;
    case ENUM_DIR1::UP:
        spHead->runAction(MoveBy::create(time, Vec2(0, NET_SIZE)));
        spHead->snake_row++;
        break;
    default:
        break;
    }

    //ͷײ���߽粻����
    if (spHead->snake_row < 0 || spHead->snake_col < 0 ||
        spHead->snake_row >= 16 || spHead->snake_col >= 16)
    {
        if (spHead->snake_row < 0)
        {
            spHead->snake_row = 14;
            spHead->runAction(MoveTo::create(0.0001, Vec2(spHead->snake_col * NET_SIZE, 15 * NET_SIZE)));
        }
        if (spHead->snake_col < 0)
        {
            spHead->snake_col = 14;
            spHead->runAction(MoveTo::create(0.0001, Vec2(15 * NET_SIZE, spHead->snake_row * NET_SIZE)));
        }
        if (spHead->snake_row >=16)
        {
            spHead->snake_row = 1;
            spHead->runAction(MoveTo::create(0.0001,Vec2(spHead->snake_col * NET_SIZE,0)));
        }
        if (spHead->snake_col >=16)
        {
            spHead->snake_col = 1;
            spHead->runAction(MoveTo::create(0.0001, Vec2(0, spHead->snake_row * NET_SIZE)));
        }
    }

    for (auto node : allBody)
    {
        if (node->snake_row < 0 || node->snake_col < 0 ||
            node->snake_row >= 16 || node->snake_col >= 16)
        {
            if (node->snake_row < 0)
            {
                node->snake_row = 14;
                node->runAction(MoveTo::create(0.0001, Vec2(node->snake_col * NET_SIZE, 15 * NET_SIZE)));
            }
            if (node->snake_col < 0)
            {
                node->snake_col = 14;
                node->runAction(MoveTo::create(0.0001, Vec2(15 * NET_SIZE, node->snake_row * NET_SIZE)));
            }
            if (node->snake_row >= 16)
            {
                node->snake_row = 1;
                node->runAction(MoveTo::create(0.0001, Vec2(node->snake_col * NET_SIZE, 0)));
            }
            if (node->snake_col >= 16)
            {
                node->snake_col = 1;
                node->runAction(MoveTo::create(0.0001, Vec2(0, node->snake_row * NET_SIZE)));
            }
        }
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
        int row = rand() % 16;
        int col = rand() % 16;
     
        spFood->setPositionRC(row, col);

        //��ӽڵ�
        newBody();
    }
}

void Game_1::newBody()//���һ���µ�����ڵ�
{
    auto bodynode = SnakeNode1::create(Part1::BODY);

    if (allBody.size() > 0)
    {   //������ڵ�
        auto lastbody = allBody.at(allBody.size() - 1);//ȡ�����½ڵ�������һ���ڵ�
        bodynode->snake_dir = lastbody->snake_dir;    //�½ڵ�ķ���Ϊ���һ������Ľڵ�

        switch (bodynode->snake_dir) {
        case ENUM_DIR1::UP:
            bodynode->setPositionRC(lastbody->snake_row - 1, lastbody->snake_col);
            break;
        case ENUM_DIR1::DOWN:
            bodynode->setPositionRC(lastbody->snake_row + 1, lastbody->snake_col);
            break;
        case ENUM_DIR1::LEFT:
            bodynode->setPositionRC(lastbody->snake_row, lastbody->snake_col + 1);
            break;
        case ENUM_DIR1::RIGHT:
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
        case ENUM_DIR1::UP:
            bodynode->setPositionRC(spHead->snake_row - 1, spHead->snake_col);
            break;
        case ENUM_DIR1::DOWN:
            bodynode->setPositionRC(spHead->snake_row + 1, spHead->snake_col);
            break;
        case ENUM_DIR1::LEFT:
            bodynode->setPositionRC(spHead->snake_row, spHead->snake_col + 1);
            break;
        case ENUM_DIR1::RIGHT:
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

void Game_1::moveBody()//�ƶ����е�����ڵ�
{
    if (0 == allBody.size())
    {
        return;
    }
    for (auto bodynode : allBody)
    {
        switch (bodynode->snake_dir) {
        case ENUM_DIR1::RIGHT:
            bodynode->runAction(MoveBy::create(time, Vec2(NET_SIZE, 0)));
            bodynode->snake_col++;
            break;
        case ENUM_DIR1::LEFT:
            bodynode->runAction(MoveBy::create(time, Vec2(-NET_SIZE, 0)));
            bodynode->snake_col--;
            break;
        case ENUM_DIR1::DOWN:
            bodynode->runAction(MoveBy::create(time, Vec2(0, -NET_SIZE)));
            bodynode->snake_row--;
            break;
        case ENUM_DIR1::UP:
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

void Game_1::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.7f, HelloWorld::createScene()));
}