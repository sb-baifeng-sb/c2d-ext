
#ifndef __C2DEXT_TOUCHABLE_VIEW_H__
#define __C2DEXT_TOUCHABLE_VIEW_H__

#include "cocos2d.h"

namespace c2dext {

class TouchableView : public cocos2d::Layer {
public:
    CREATE_FUNC(TouchableView);
    class Context {
    public:
        cocos2d::Node* sender;
        cocos2d::Touch* touch;
        cocos2d::Event* event;
    public:
        Context(cocos2d::Node* sender, cocos2d::Touch* touch, cocos2d::Event* event):sender(sender), touch(touch), event(event){}
        bool touchInView();
    };
    typedef std::function<void(Context& c)> TouchFunc;
    typedef std::map<std::string, TouchFunc> TouchFuncMap;
public:
    TouchableView();
public:
    void setTouch(std::string const& name, TouchFunc const& f);
private:
    TouchFunc const& get(std::string const& name) const;
private:
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
private:
    TouchFunc mDefault;
    TouchFuncMap mCallback;
};

}

#endif