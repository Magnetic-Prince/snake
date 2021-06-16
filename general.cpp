#define schedule_selector(_SELECTOR) static_cast<cocos2d::SEL_SCHEDULE>(&_SELECTOR)
#include "general.h"
#include"menudemo.h"
#include"ui/CocosGUI.h"
#include"stdlib.h"
#include "place.h"
#include "HelloWorldScene.h"
using namespace std;
USING_NS_CC;

const float NET_SIZE = 50;//重设单位大小,否则移动位置不易控制，不容易吃到食物

Scene* Game_2::createScene()
{
    return Game_2::create();
}

void SnakeNode::setPositionRC(float row, float col)//设置节点的坐标
{
    this->snake_row = row;
    this->snake_col = col;
    setPosition(Vec2(col * NET_SIZE, row * NET_SIZE));
}

SnakeNode* SnakeNode::create(Part type)//应用init
{
    SnakeNode* part = new SnakeNode();
    if (part == NULL)
    {
        delete part;
        exit(0);
    }
    part->init(type);
    part->autorelease(); //具备release功能，但在return之后才执行
    return part;
}

bool SnakeNode::init(Part type)//创建蛇头，蛇身，以及食物
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
        sprite->setAnchorPoint(Vec2::ZERO);//锚点设为0，便于控制图形不出边界（只改变两个边界的位置即可）
        this->addChild(sprite);

        snake_dir = ENUM_DIR::RIGHT;//初始化移动方向，向右移动
       
        snake_row = 0;//初始化蛇头的位置,左下角
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

    auto configData = FileUtils::getInstance()->getValueMapFromFile("res/tips.plist");//显示汉字
    for (auto abs : configData)
    {
        int key = atoi(abs.first.c_str());
        Value val = abs.second;
        CCLOG("%d,%s", key, val);
        trans.insert(pair<int, Value>(key, val));
    }


    //确定界面大小和基坐标
    auto size = Director::getInstance()->getVisibleSize();
    Vec2 base = Vec2(size / 2);

    //添加背景
    auto spritebg = Sprite::create("game.jpg");
    spritebg->setPosition(base);
    spritebg->setOpacity(90);//设置窗口透明度
    this->addChild(spritebg);

    //添加蛇头
    spHead = SnakeNode::create(Part::HEAD);
    this->addChild(spHead);

    //添加食物
    spFood = SnakeNode::create(Part::FOOD);
    int row = rand() % 16;//随机生成食物
    int col = rand() % 16;
    spFood->setPositionRC(row, col);
    this->addChild(spFood);

    //得分显示
    m_score = 0;
    auto labelScore = Label::createWithSystemFont("grade: 0", "宋体", 50);
    labelScore->setTag(10);
    labelScore->setPosition(Point(size.width - 150, size.height - 50));
    labelScore->setColor(Color3B(250, 100, 150));
    this->addChild(labelScore);

    //返回按钮
    trans_content = trans.at(1008).asString();
    auto menuItemBack = MenuItemFont::create(trans_content, CC_CALLBACK_1(Game_2::menuCloseCallback, this));
    auto menu = Menu::create(menuItemBack, NULL);
    menu->setColor(Color3B(250, 100, 150));
    menu->setPosition(Vec2::ZERO);
    menuItemBack->setPosition(Vec2(20,20));
    this->addChild(menu);

    //刷新，以达到运动效果
    this->schedule(schedule_selector(Game_2::game), 0.8f);

    //加入用户键盘事件
    auto mykey = EventListenerKeyboard::create();
    mykey->onKeyPressed = [&](EventKeyboard::KeyCode keycode, Event* event)//按下即响应
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

    //加入用户触摸事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);//按钮重叠时只响应最上面那个按钮
    listener->onTouchBegan = [&](Touch* t, Event* e) 
    {
        //改变贪食蛇移动的方向
        int col = t->getLocation().x / NET_SIZE;
        int row = t->getLocation().y / NET_SIZE;
        int HeadCol = spHead->getPositionX() / NET_SIZE;
        int HeadRow = spHead->getPositionY() / NET_SIZE;
        if (abs(HeadCol - col) > abs(HeadRow - row))//左右
        {
            if (HeadCol < col)//点击右方
            {
                spHead->snake_dir = ENUM_DIR::RIGHT;
            }
            else
            {
                spHead->snake_dir = ENUM_DIR::LEFT;
            }
        }
        else//上下
        {
            if (HeadRow < row)//点击上方
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
    moveBody();//移动所有身体节点

    auto configData = FileUtils::getInstance()->getValueMapFromFile("res/tips.plist");//显示汉字
    for (auto abs : configData)
    {
        int key = atoi(abs.first.c_str());
        Value val = abs.second;
        CCLOG("%d,%s", key, val);
        trans.insert(pair<int, Value>(key, val));
    }

    //确定界面大小和基坐标
    auto size = Director::getInstance()->getVisibleSize();
    Vec2 base = Vec2(size / 2);

    //蛇头移动
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

    //头撞到边界死亡
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

    //食物碰撞检测
    if (spHead->snake_row == spFood->snake_row &&
        spHead->snake_col == spFood->snake_col)
    {
        //吃食物效果
        auto blink = Blink::create(0.7, 4);
        spHead->runAction(blink);

        //分数增加
        m_score += 1;
        auto label = (Label*)getChildByTag(10);
        char strscore[20];
        sprintf(strscore, "grade: %d", m_score);
        label->setColor(Color3B(250, 100, 150));
        label->setString(strscore);
           
        
        //食物产生新的位置
        float row = rand() % 16;
        float col = rand() % 16;
        spFood->setPositionRC(row, col);

        //吃到食物后加快速度
        time *= 0.8;

        //添加节点
        newBody();
    }
}

void Game_2::newBody()//添加一个新的身体节点
{
    auto bodynode = SnakeNode::create(Part::BODY);

    if (allBody.size() > 0)
    {   //有身体节点
        auto lastbody = allBody.at(allBody.size() - 1);//取出除新节点外的最后一个节点
        bodynode->snake_dir = lastbody->snake_dir;    //新节点的方向为最后一个身体的节点

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
        //新节点的方向等于蛇头的方向
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
    //添加新节点到当前图层
    this->addChild(bodynode);

    //添加新节点到集合中
    allBody.pushBack(bodynode);
}

void Game_2::moveBody()//移动所有的身体节点
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
    //移动完成之后，改变每个body的方向
    for (int i = allBody.size() - 1; i > 0; i--)
    { //每个节点的 方向调整为它前一个节点的方向
        allBody.at(i)->snake_dir = allBody.at(i - 1)->snake_dir;
    }
    allBody.at(0)->snake_dir = spHead->snake_dir;



    //头撞到身体死亡
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