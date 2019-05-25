//
//  Dimmed.hpp
//  Realtrick Games
//  Created by NamJunHyeon on 2019. 05. 25.
//

#pragma once

#include "cocos2d.h"

class Dimmed : public cocos2d::Node
{
    
    
public:
    
    virtual ~Dimmed()
    {
        _stencilRenderer->release();
    }
    
    Dimmed(const Dimmed&) = delete;
    Dimmed& operator=(const Dimmed&) = delete;
    
    static Dimmed* create(float width, float height)
    {
        auto ret = new (std::nothrow) Dimmed();
        if ( ret && ret->init(width, height) )
        {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    
    static Dimmed* create(const cocos2d::Size& size)
    {
        return Dimmed::create(size.width, size.height);
    }
    
    void addStencilRect(const cocos2d::Rect& rect)
    {
        _stencilRects.push_back(rect);
    }
    
    void addStencilRectAndEnable(const cocos2d::Rect& rect)
    {
        addStencilRect(rect);
        setEnabled(true);
    }
    
    void removeAllStencilRects()
    {
        _stencilRects.clear();
    }
    
    void removeAllStencilRectsAndDisable()
    {
        removeAllStencilRects();
        setEnabled(false);
    }
    
    bool isEnabled() const
    {
        return isVisible();
    }
    
    void setEnabled(bool enable)
    {
        setVisible(enable);
    }
    
    void setSwallowTouches(bool enable)
    {
        _touchListener->setEnabled(enable);
    }
    
    bool isSwallowTouches() const
    {
        return _touchListener->isEnabled();
    }
    
    bool isAvailableTouch(const cocos2d::Vec2& touchPosition) const
    {
        if (isVisible() == false)
        {
            return true;
        }
        
        for (const auto& rect : _stencilRects)
        {
            if (rect.containsPoint(touchPosition))
            {
                return true;
            }
        }
        
        return false;
    }
    
    void setDimmedColor(const cocos2d::Color4F& color)
    {
        _dimmedColor = color;
    }
    
    void setDimmedColor(const cocos2d::Color4B& color)
    {
        setDimmedColor(color.r, color.g, color.b, color.a);
    }
    
    void setDimmedColor(float r, float g, float b, float a)
    {
        setDimmedColor(cocos2d::Color4F(r, g, b, a));
    }
    
    void setDimmedColor(double r, double g, double b, double a)
    {
        setDimmedColor(cocos2d::Color4F(r, g, b, a));
    }
    
    void setDimmedColor(int r, int g, int b, int a)
    {
        setDimmedColor(cocos2d::Color4F(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f));
    }
    
    void addDimmedTouchEvent(std::function<void(Dimmed*, const cocos2d::Vec2&)> listener)
    {
        _onDimmedTouchCallback = listener;
    }
    
    void addCustomNode(cocos2d::Node* node)
    {
        _customNodes->addChild(node);
    }

    void removeCustomNodeByTag(int tag, bool cleanup = true)
    {
        _customNodes->removeChildByTag(tag, cleanup);
    }
    
    void removeCustomNodeByName(const std::string &name, bool cleanup = true)
    {
        _customNodes->removeChildByName(name, cleanup);
    }
    
    void removeAllCustomNode()
    {
        _customNodes->removeAllChildren();
    }
    
private:
    
    Dimmed()
    {
        _stencilRects.reserve(50);
    }
    
    bool init(float width, float height)
    {
        using namespace cocos2d;
        
        if ( !Node::init() )
        {
            return false;
        }
        
        setContentSize(Size(width, height));
        setCascadeOpacityEnabled(true);
        
        _dimmedRenderer = RenderTexture::create(width, height);
        _dimmedRenderer->getSprite()->setAnchorPoint(Vec2::ZERO);
        addChild(_dimmedRenderer, 0);
        
        _customNodes = Node::create();
        _customNodes->setCascadeOpacityEnabled(true);
        addChild(_customNodes, 1);
        
        _stencilRenderer = DrawNode::create();
        _stencilRenderer->setBlendFunc({GL_ONE, GL_ZERO});
        _stencilRenderer->retain();
        
        _touchListener = EventListenerTouchOneByOne::create();
        _touchListener->setSwallowTouches(true);
        _touchListener->onTouchBegan = [this](Touch* t, Event* e)
        {
            auto touch = t->getLocation();
            auto swallow = !isAvailableTouch(touch);
            if (swallow)
            {
                _onDimmedTouchCallback(this, touch);
            }
            return swallow;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
        
        return true;
    }
    
    void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override
    {
        _dimmedRenderer->beginWithClear(_dimmedColor.r, _dimmedColor.g, _dimmedColor.b, _dimmedColor.a * getOpacity() / 255.0f);
        _stencilRenderer->clear();
        for (const auto& rect : _stencilRects)
        {
            _stencilRenderer->drawSolidRect(rect.origin, rect.origin + rect.size, cocos2d::Color4F(0.0f, 0.0f, 0.0f, 0.0f));
        }
        _stencilRenderer->visit(renderer, parentTransform, parentFlags);
        _dimmedRenderer->end();
        cocos2d::Node::visit(renderer, parentTransform, parentFlags);
    }
    
    // prevent use these functions
    using cocos2d::Node::addChild;
    using cocos2d::Node::removeChild;
    using cocos2d::Node::removeChildByTag;
    using cocos2d::Node::removeChildByName;
    using cocos2d::Node::removeAllChildren;
    using cocos2d::Node::removeAllChildrenWithCleanup;
    
private:
    
    cocos2d::RenderTexture* _dimmedRenderer = nullptr;
    cocos2d::DrawNode* _stencilRenderer = nullptr;
    cocos2d::Node* _customNodes = nullptr;
    cocos2d::EventListenerTouchOneByOne* _touchListener = nullptr;
    
    std::vector<cocos2d::Rect> _stencilRects;
    cocos2d::Color4F _dimmedColor = cocos2d::Color4F(0.0f, 0.0f, 0.0f, 0.5f);
    std::function<void(Dimmed*, const cocos2d::Vec2&)> _onDimmedTouchCallback = [](Dimmed*, cocos2d::Vec2){};
    
};
