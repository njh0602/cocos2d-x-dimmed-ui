#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class TestScene : public cocos2d::Scene
{
    
public:

    static cocos2d::Scene* createScene();
    bool init() override;

};
