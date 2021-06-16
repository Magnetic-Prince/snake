#include "choice.h"
#include "general.h"
#include "easy.h"
#include "hard.h"

Scene* choice::createScene()
{
	return choice::create();
}

bool choice::init()
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


	//添加背景
	auto spritebg = Sprite::create("bg_choice.jpg");
	spritebg->setPosition(base);
	spritebg->setOpacity(90);//设置窗口透明度
	this->addChild(spritebg);

	auto configData = FileUtils::getInstance()->getValueMapFromFile("res/tips.plist");
	for (auto abs : configData)
	{
		int key = atoi(abs.first.c_str());
		Value val = abs.second;
		CCLOG("%d,%s", key, val);
		trans.insert(pair<int, Value>(key, val));
	}


	//简单
	auto choiceItem_easy = MenuItemImage::create(
		"choice_level.jpg",
		"choice_levelbg.jpg",
		CC_CALLBACK_1(choice::menuCloseCallback_easy, this));
	choiceItem_easy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	choiceItem_easy->setPosition(Vec2(visibleSize / 2) + Vec2(-150,100));

	trans_content = trans.at(1004).asString();
	trans_label = Label::createWithSystemFont(trans_content, "方正姚体", 30);
	trans_label->enableBold();
	auto menulabel_choice_easy = MenuItemLabel::create(trans_label, NULL);
	menulabel_choice_easy->setColor(Color3B(250, 20, 150));//红绿蓝
	menulabel_choice_easy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	menulabel_choice_easy->setPosition(Vec2(visibleSize / 2) + Vec2(-150,100));

	Menu* menu_choice = Menu::create(choiceItem_easy, menulabel_choice_easy, NULL);
	menu_choice->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu_choice->setPosition(Vec2::ZERO);
	this->addChild(menu_choice, 1);


	//一般
	auto choiceItem_general = MenuItemImage::create(
		"choice_level.jpg",
		"choice_levelbg.jpg",
		CC_CALLBACK_1(choice::menuCloseCallback_general, this));
	choiceItem_general->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	choiceItem_general->setPosition(Vec2(visibleSize / 2) + Vec2(-150, 0));

	trans_content = trans.at(1005).asString();
	trans_label = Label::createWithSystemFont(trans_content, "方正姚体", 30);
	trans_label->enableBold();
	auto menulabel_choice_general = MenuItemLabel::create(trans_label, NULL);
	menulabel_choice_general->setColor(Color3B(250, 20, 150));//红绿蓝
	menulabel_choice_general->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	menulabel_choice_general->setPosition(Vec2(visibleSize / 2) + Vec2(-150, 0));

	Menu* menu_choice_general = Menu::create(choiceItem_general, menulabel_choice_general, NULL);
	menu_choice_general->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu_choice_general->setPosition(Vec2::ZERO);
	this->addChild(menu_choice_general, 1);


	//困难
	auto choiceItem_hard = MenuItemImage::create(
		"choice_level.jpg",
		"choice_levelbg.jpg",
		CC_CALLBACK_1(choice::menuCloseCallback_hard, this));
	choiceItem_hard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	choiceItem_hard->setPosition(Vec2(visibleSize / 2) + Vec2(-150, -100));

	trans_content = trans.at(1006).asString();
	trans_label = Label::createWithSystemFont(trans_content, "方正姚体", 30);
	trans_label->enableBold();
	auto menulabel_choice_hard = MenuItemLabel::create(trans_label, NULL);
	menulabel_choice_hard->setColor(Color3B(250, 20, 150));//红绿蓝
	menulabel_choice_hard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	menulabel_choice_hard->setPosition(Vec2(visibleSize / 2) + Vec2(-150, -100));

	Menu* menu_choice_hard = Menu::create(choiceItem_hard, menulabel_choice_hard, NULL);
	menu_choice_hard->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu_choice_hard->setPosition(Vec2::ZERO);
	this->addChild(menu_choice_hard, 1);


	return true;
}

void choice::menuCloseCallback_easy(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.f, Game_1::createScene()));
}

void choice::menuCloseCallback_general(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.f, Game_2::createScene()));
}

void choice::menuCloseCallback_hard(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.f, Game_3::createScene()));
}
