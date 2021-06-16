#include "place.h"
#include"menudemo.h"
#include"ui/CocosGUI.h"
#include<string>
#include<iostream>

using namespace std;
USING_NS_CC;

Scene* place::createScene()
{
	return  place::create();
}

bool place::init()
{
    if (!Scene::init())
    {
        return false;
    }

    //确定界面大小和基坐标
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 base = Vec2(visibleSize / 2);

    auto sprite = Sprite::create("place_bg.jpg");
    sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprite->setPosition(base);
    this->addChild(sprite);

    FILE* out;
    if ((out = fopen("file.txt", "rb+")) == NULL)
    {
        exit(EXIT_FAILURE);
    }
    int num_place = 0;
    int place_num[50] = { 0 };
    for (; !feof(out) && num_place < 50; num_place++)
    {
        int m = 0;
        m=getc(out);
    }
    rewind(out);
    fread(place_num, 4*(num_place-1), 1, out);
    int temp = 0;
    for (int num = 0; num < num_place-1; num++)
    {
        for (int j = num+1; j <  num_place; j++)
        {
            if (place_num[num] < place_num[j])
            {
                temp = place_num[num];
                place_num[num] = place_num[j];
                place_num[j] = temp;
            }
        }
    }
    auto configData = FileUtils::getInstance()->getValueMapFromFile("res/tips.plist");
    for (auto abs : configData)
    {
        int key = atoi(abs.first.c_str());
        Value val = abs.second;
        CCLOG("%d,%s", key, val);
        trans.insert(pair<int, Value>(key, val));
    }

    float t = 50;
    for (int I = 0,n=1009; I < 3; I++,t=t-60,n++)
    {
        if (place_num[I] != 0)
        {
            trans_content = trans.at(n).asString();
            trans_label = Label::createWithSystemFont(trans_content, "方正姚体", 50);
            trans_label->setColor(Color3B(250, 100, 150));
            trans_label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            trans_label->setPosition(base + Vec2(0, t));
            trans_label->enableBold();
            this->addChild(trans_label);

            string str_1 = to_string(place_num[I]);
            Label* label_1 = Label::createWithSystemFont(str_1, "方正姚体", 50);
            label_1->setColor(Color3B(250, 100, 150));
            label_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            label_1->setPosition(base + Vec2(0, t));
            label_1->enableBold();
            this->addChild(label_1);
        }
    }
   
    auto startItem = MenuItemImage::create(
        "return.jpg",
        "returnbg.jpg",
        CC_CALLBACK_1(place::menuCloseCallback, this));
    auto menu = Menu::create(startItem, NULL);
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    menu->setPosition(base+Vec2(-300,-300));
    this->addChild(menu, 1);

    return true;
}

void place::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionProgressInOut::create(0.7f, Menu1::createScene()));
}