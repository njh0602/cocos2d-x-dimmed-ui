#include "TestScene.hpp"
#include "Dimmed.hpp"
using namespace cocos2d;
using namespace std;

#define TEST_CASE 5

Scene* TestScene::createScene()
{
	auto ret = new (nothrow) TestScene();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool TestScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
    
    auto visibleSize = _director->getVisibleSize();
    auto bg = Sprite::create("res/HelloWorld.png");
    bg->setPosition(visibleSize * 0.5f);
    addChild(bg);
    
    for(int y = 0 ; y < visibleSize.height ; y += 40)
    {
        for(int x = 0 ; x < visibleSize.width ; x += 40)
        {
            auto b = ui::Button::create("res/CloseNormal.png", "res/CloseSelected.png");
            b->setAnchorPoint(Vec2::ZERO);
            b->setPosition(Vec2(x, y));
            addChild(b);
        }
    }
    
#if (TEST_CASE == 1)
    
    auto dimmed = Dimmed::create(visibleSize);
    dimmed->setDimmedColor(Color4F::BLACK); // default: (0.0, 0.0, 0.0, 0.5)
    dimmed->setSwallowTouches(false); // default: true
    dimmed->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.5f, 255), FadeTo::create(0.5f, 0), nullptr)));
    addChild(dimmed, 1); // zorder: touch priority
    
#elif (TEST_CASE == 2)
    
    auto dimmed = Dimmed::create(visibleSize);
    dimmed->addDimmedTouchEvent([](Dimmed* self, const Vec2& p){ self->removeFromParent(); });
    addChild(dimmed, 1);
    
    auto popup = Sprite::create("res/popup_bg.png");
    popup->setPosition(dimmed->getContentSize()*0.5f);
    
    auto text = ui::Text::create("Easy Popup", "", 40);
    text->setPosition(popup->getContentSize() * 0.5f);
    popup->addChild(text);
    
    dimmed->addStencilRect(popup->getBoundingBox());
    dimmed->addCustomNode(popup);
    
#elif (TEST_CASE == 3)
    
    auto dimmed = Dimmed::create(visibleSize);
    dimmed->runAction(Sequence::create(DelayTime::create(3.0f), FadeTo::create(0.5f, 0), RemoveSelf::create(), nullptr));
    addChild(dimmed, 1);
    
    auto popup = Sprite::create("res/popup_bg.png");
    popup->setPosition(dimmed->getContentSize()*0.5f);
    
    auto text = ui::Text::create("request something...", "", 40);
    text->setPosition(popup->getContentSize() * 0.5f);
    popup->addChild(text);
    
    dimmed->addCustomNode(popup);
    
#elif (TEST_CASE == 4)
    
    auto dimmed = Dimmed::create(visibleSize);
    dimmed->runAction(Sequence::create(DelayTime::create(3.0f), FadeTo::create(0.5f, 0), CallFunc::create([this]{
        auto dimmed = getChildByName<Dimmed*>("dimmed");
        dimmed->removeAllStencilRectsAndDisable();
    }), nullptr));
    dimmed->setName("dimmed");
    for(int i = 0 ; i < 10; ++ i)
    {
        dimmed->addStencilRectAndEnable(Rect(100 * i,100 * i, 100, 100));
    }
    addChild(dimmed, 1);
    
#elif (TEST_CASE == 5)
    
    for(int i = 0 ; i < 10 ; ++ i)
    {
        auto dimmed = Dimmed::create(visibleSize);
        dimmed->setTag(i);
        dimmed->setDimmedColor(0.0, 0.0, 0.0, 0.1);
        dimmed->runAction(Sequence::create(Hide::create(), DelayTime::create(0.25f * i), Show::create(), nullptr));
        addChild(dimmed, i); // zorder is important
        
        auto bt = ui::Button::create("res/button_n.png", "res/button_s.png");
        bt->setPosition(Vec2(visibleSize.width * 0.5f + (i * 20), visibleSize.height * 0.5f - (i * 20)));
        bt->addClickEventListener([this, i](Ref* self){
            auto dimmed = getChildByTag<Dimmed*>(i);
            dimmed->removeFromParent();
        });
        dimmed->addCustomNode(bt);
    }
    
#endif
    return true;
}
