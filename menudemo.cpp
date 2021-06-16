#include"menudemo.h"
#include"ui/CocosGUI.h"
#include<string>
#include "place.h"
#include "choice.h"
#include"cocos2d.h"

using namespace std;

USING_NS_CC;



Scene* Menu1::createScene()
{
	return Menu1::create();
}

bool Menu1::init()
{
	//super init first
	if (!Scene::init())
	{
		return false;
	}
	
	//确定界面大小和基坐标
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 base = Vec2(visibleSize / 2);

	//菜单界面
	auto sprite = Sprite::create("menu.jpg");
	sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sprite->setPosition(base);
	this->addChild(sprite);


	/**************************/
	//声音控制
	/**************************/
	auto spriteOn = Sprite::create("sound.jpg");
	auto spriteOff = Sprite::create("nosound.jpg");
	auto menu1 = MenuItemSprite::create(spriteOn, spriteOn);
	auto menu2 = MenuItemSprite::create(spriteOff, spriteOff);

	auto toggleMENU = MenuItemToggle::createWithCallback([&](Ref* ref)
		//从网上视频学习到的，但是没有掌握对声音控制的方法，QQ群里发的书本上的方法好像不能用。
		{
			auto item = dynamic_cast<MenuItemToggle*>(ref);
			if (item)
			{
			
				if (item->getSelectedIndex() == 0)
				{
					CCLOG("silence");//打印文本，显示该方法可以控制的声音。
				}
				else if (item->getSelectedIndex() == 1)
				{
					CCLOG("play sound");
				}
			}

		}, menu1, menu2, NULL);
	toggleMENU->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	toggleMENU->setPosition(Vec2(visibleSize / 2) + Vec2(-50,100));

	auto configData = FileUtils::getInstance()->getValueMapFromFile("res/tips.plist");
	for (auto abs : configData)
	{
		int key = atoi(abs.first.c_str());
		Value val = abs.second;
		CCLOG("%d,%s", key, val);
		trans.insert(pair<int, Value>(key, val));
	}

	trans_content = trans.at(1001).asString();
	trans_label = Label::createWithSystemFont(trans_content, "方正姚体", 50);
	trans_label->enableBold();
	auto menulabel = MenuItemLabel::create(trans_label, NULL);
	menulabel->setColor(Color3B(10, 20, 250));//红绿蓝
	menulabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	menulabel->setPosition(Vec2(visibleSize / 2)+ Vec2(-50, 100));

	Menu* menu = Menu::create(toggleMENU, menulabel,NULL);
	menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu,1);


	/**************************/
	//排行榜
	/**************************/
	auto placeItem = MenuItemImage::create(
		"place.jpg",
		"placebg.jpg",
		CC_CALLBACK_1(Menu1::menuCloseCallback_place, this));
	placeItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	placeItem->setPosition(Vec2(visibleSize / 2) + Vec2(-50, 30));

	trans_content = trans.at(1002).asString();
	trans_label = Label::createWithSystemFont(trans_content, "方正姚体", 50);
	trans_label->enableBold();
	auto menulabel_place = MenuItemLabel::create(trans_label, NULL);
	menulabel_place->setColor(Color3B(10, 20, 250));//红绿蓝
	menulabel_place->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	menulabel_place->setPosition(Vec2(visibleSize / 2) + Vec2(-50, 30));

	Menu* menu_place = Menu::create(placeItem, menulabel_place, NULL);
	menu_place->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu_place->setPosition(Vec2::ZERO);
	this->addChild(menu_place, 1);

	/**************************/
	//游戏难度
	/**************************/
	auto choiceItem = MenuItemImage::create(
		"choice.jpg",
		"choicebg.jpg",
		CC_CALLBACK_1(Menu1::menuCloseCallback_choice, this));
	choiceItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	choiceItem->setPosition(Vec2(visibleSize / 2) + Vec2(-50, -40));

	trans_content = trans.at(1003).asString();
	trans_label = Label::createWithSystemFont(trans_content, "方正姚体", 50);
	trans_label->enableBold();
	auto menulabel_choice = MenuItemLabel::create(trans_label, NULL);
	menulabel_choice->setColor(Color3B(10, 20, 250));//红绿蓝
	menulabel_choice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	menulabel_choice->setPosition(Vec2(visibleSize / 2) + Vec2(-50, -40));

	Menu* menu_choice = Menu::create(choiceItem, menulabel_choice, NULL);
	menu_choice->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu_choice->setPosition(Vec2::ZERO);
	this->addChild(menu_choice, 1);

	return true;
}

void Menu1::menuCloseCallback_place(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.7f, place::createScene()));
}

void Menu1::menuCloseCallback_choice(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.7f, choice::createScene()));
}