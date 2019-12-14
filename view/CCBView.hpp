//
//  CcbView.hpp
//
//  Created by baifeng on 2018/5/20.
//

#ifndef __C2DEXT_CCB_VIEW_HPP__
#define __C2DEXT_CCB_VIEW_HPP__

#include <stdio.h>
#include "editor-support/cocosbuilder/CocosBuilder.h"
#include "cocos2d.h"

namespace c2dext {

class CCB {
public:
    typedef std::shared_ptr<CCB> CCBPtr;
public:
    static CCBPtr New();
public:
    CCB();
    ~CCB();
public:
    void registerClass(std::string const& className, cocosbuilder::NodeLoader* loader);
    void removeClass(std::string const& className);
    cocos2d::Node* nodeFromCCB(std::string const& fileName);
    cocos2d::Node* nodeFromCCB(std::string const& fileName, cocos2d::Ref* owner);
private:
    void buildLoader();
private:
    cocosbuilder::NodeLoaderLibrary* mLoader;
};

class CcbAnimationCompletedDelegate;
class CCBView : public cocos2d::Layer, public cocosbuilder::CCBSelectorResolver, public cocosbuilder::CCBMemberVariableAssigner, public cocosbuilder::NodeLoaderListener {
public:
    typedef std::function<void(std::string const&)> AnimationCompletedCallback;
public:
    CREATE_FUNC(CCBView);
    CCBView();
    ~CCBView();
public:
    void setAnimationCompletedCallBack(std::string const& aniName, AnimationCompletedCallback callback);
    void playAnimation(std::string const& aniName, float fTweenDuration = 0.0f);
    void removeAllAnimationCompletedCallBack();
public:
    void setCCBReader(cocosbuilder::CCBReader* reader);
    cocosbuilder::CCBReader* getCCBReader() const;
public:
    void registerTouchListener(cocos2d::EventListener* listener);
    void removeTouchListener();
    void setTouchEnabled(bool value);
protected:
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
        //CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(TARGET, SELECTORNAME, METHOD)
        return nullptr;
    }
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
        //CCB_SELECTORRESOLVER_CALLFUNC_GLUE(TARGET, SELECTORNAME, METHOD)
        return nullptr;
    };
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
        //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(TARGET, SELECTORNAME, METHOD)
        return nullptr;
    }
protected:
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
        //CCB_MEMBERVARIABLEASSIGNER_GLUE(TARGET, MEMBERVARIABLENAME, MEMBERVARIABLETYPE, MEMBERVARIABLE)
        return false;
    }
    virtual bool onAssignCCBCustomProperty(cocos2d::Ref* pTarget, const char* pMemberVariableName, const cocos2d::Value& value) {
        return false;
    }
protected:
    virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) {
        // nothing to do
    }
protected:
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
protected:
    cocosbuilder::CCBReader* mReader;
    CcbAnimationCompletedDelegate* mAniCompleted;
};

void DisableNodeTouchForTime(cocos2d::Node* node, float duration);
void DisableNodeTouchForever(cocos2d::Node* node);
void EnableNodeTouch(cocos2d::Node* node);
void EnableNodeOpacity(cocos2d::Node* node);

#ifndef CCB_VIRTUAL_CREATE_NODE
#define CCB_VIRTUAL_CREATE_NODE(T) virtual T * createNode(cocos2d::Node * pParent, cocosbuilder::CCBReader * ccbReader) override { \
    using namespace c2dext; \
    auto node = T::create(); \
    auto p = dynamic_cast<CCBView*>(node); \
    if (p != nullptr) { \
        p->setCCBReader(ccbReader); \
    } \
    return node; \
}
#endif

#ifndef CCB_VIEW_LOADER_IMP
#define CCB_VIEW_LOADER_IMP(loaderName, parentClass, nodeName) \
class loaderName : public parentClass { \
public: \
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(loaderName, New); \
protected: \
    CCB_VIRTUAL_CREATE_NODE(nodeName); \
}
#endif

#ifndef CCB_SPRITE_LOADER_IMP
#define CCB_SPRITE_LOADER_IMP(loaderName, parentClass, nodeName) \
class loaderName : public parentClass { \
public: \
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(loaderName, New); \
protected: \
CCB_VIRTUAL_CREATE_NODE(nodeName); \
protected: \
    virtual void onHandlePropTypeSpriteFrame(cocos2d::Node * pNode, cocos2d::Node * pParent, const char* pPropertyName, cocos2d::SpriteFrame * pSpriteFrame, cocosbuilder::CCBReader * ccbReader) override { \
        /*忽略图片加载*/ \
    } \
    virtual void onHandlePropTypeByte(cocos2d::Node * pNode, cocos2d::Node * pParent, const char* pPropertyName, unsigned char pByte, cocosbuilder::CCBReader * ccbReader) override { \
        /*忽略透明值加载*/ \
    } \
    virtual void onHandlePropTypeColor3(cocos2d::Node * pNode, cocos2d::Node * pParent, const char * pPropertyName, cocos2d::Color3B pColor3B, cocosbuilder::CCBReader * ccbReader) override { \
        /*忽略颜色加载*/ \
    } \
    virtual void onHandlePropTypeBlendFunc(cocos2d::Node * pNode, cocos2d::Node * pParent, const char * pPropertyName, cocos2d::BlendFunc pCCBBlendFunc, cocosbuilder::CCBReader * ccbReader) override { \
        /*忽略BlendFunc加载*/ \
    } \
    virtual void onHandlePropTypeFlip(cocos2d::Node * pNode, cocos2d::Node * pParent, const char * pPropertyName, bool * pFlip, cocosbuilder::CCBReader * ccbReader) override { \
        /*忽略翻转加载*/ \
    } \
}
#endif

#ifndef CCB_VIEW_LOADER_DEFINE
#define CCB_VIEW_LOADER_DEFINE(loaderName, parentClass, nodeName) CCB_VIEW_LOADER_IMP(loaderName##Loader, parentClass, nodeName)
#endif

#ifndef CCB_VIEW_LOADER_FROM
#define CCB_VIEW_LOADER_FROM(className, parentName) CCB_VIEW_LOADER_IMP(className##Loader, parentName, className)
#endif

#ifndef CCB_VIEW_LOADER
#define CCB_VIEW_LOADER(className) CCB_VIEW_LOADER_FROM(className, cocosbuilder::NodeLoader)
#endif

/*
 *  用于重载布局上的Sprite类型
 */
#ifndef CCB_SPRITE_LOADER
#define CCB_SPRITE_LOADER(className) CCB_SPRITE_LOADER_IMP(className##Loader, cocosbuilder::NodeLoader, className)
#endif

CCB_VIEW_LOADER(CCBView);

}

#endif /* CcbView_hpp */
