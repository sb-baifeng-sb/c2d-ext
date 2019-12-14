//
//  CcbView.cpp
//
//  Created by baifeng on 2018/5/20.
//

#include "CCBView.hpp"

namespace c2dext {

//======================================================================================
class CcbAnimationCompletedDelegate : public cocos2d::Ref, public cocosbuilder::CCBAnimationManagerDelegate {
public:
    typedef std::function<void(std::string const&)> AnimationCompletedCallback;
    typedef std::map<std::string, AnimationCompletedCallback> ListenerMap;
public:
    CREATE_FUNC(CcbAnimationCompletedDelegate);
public:
    void setAnimationCompletedCallback(std::string const& aniName, AnimationCompletedCallback callback) {
        mListeners[aniName] = callback;
    }
    void clearListeners() {
        mListeners.clear();
    }
private:
    bool init() {
        return true;
    }
private:
    virtual void completedAnimationSequenceNamed(const char *name) {
        std::string _name(name);
        auto iter = mListeners.find(_name);
        if (iter != mListeners.end() && iter->second != nullptr) {
            iter->second(_name);
        }
    }
private:
    ListenerMap mListeners;
};

//======================================================================================
CCB::CCBPtr CCB::New() {
    return std::make_shared<CCB>();
}

CCB::CCB():mLoader(nullptr) {
    
}

CCB::~CCB() {
    
}

void CCB::buildLoader() {
    if (mLoader == nullptr) {
        mLoader = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    }
}

void CCB::registerClass(std::string const& className, cocosbuilder::NodeLoader* loader) {
    this->buildLoader();
    mLoader->registerNodeLoader(className.c_str(), loader);
}

void CCB::removeClass(std::string const& className) {
    this->buildLoader();
    mLoader->unregisterNodeLoader(className.c_str());
}

cocos2d::Node* CCB::nodeFromCCB(std::string const& fileName) {
    return this->nodeFromCCB(fileName, nullptr);
}

cocos2d::Node* CCB::nodeFromCCB(std::string const& fileName, cocos2d::Ref* owner) {
    this->buildLoader();
    cocosbuilder::CCBReader* reader = new cocosbuilder::CCBReader(mLoader);
    auto node = reader->readNodeGraphFromFile(fileName.c_str(), owner);
    reader->release();
    return node;
}

//======================================================================================
CCBView::CCBView():mReader(nullptr), mAniCompleted(CcbAnimationCompletedDelegate::create()) {
    mAniCompleted->retain();
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(CCBView::onTouchBegan, this);
    this->registerTouchListener(listener);
}

CCBView::~CCBView() {
    if (mReader != nullptr) {
        // 避免引用野指针引起崩溃: rootNode == this
        // 原因是自动释放机制延迟了析构时机导致rootNode成为了野指针
        mReader->getAnimationManager()->setRootNode(nullptr);
    }
    CC_SAFE_RELEASE(this->mAniCompleted);
    CC_SAFE_RELEASE(this->mReader);
}

void CCBView::registerTouchListener(cocos2d::EventListener* listener) {
    auto dispatcher = this->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CCBView::removeTouchListener() {
    auto dispatcher = this->getEventDispatcher();
    dispatcher->removeEventListenersForTarget(this);
}

void CCBView::setTouchEnabled(bool value) {
    if (value) {
        removeTouchListener();
    }
    setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
    Layer::setTouchEnabled(value);
}

void CCBView::setCCBReader(cocosbuilder::CCBReader* reader) {
    CC_SAFE_RELEASE(this->mReader);
    this->mReader = reader;
    CC_SAFE_RETAIN(this->mReader);
    
    if (reader != nullptr) {
        this->mReader->getAnimationManager()->setDelegate(mAniCompleted);
    }
}

cocosbuilder::CCBReader* CCBView::getCCBReader() const {
    return this->mReader;
}

void CCBView::setAnimationCompletedCallBack(std::string const& aniName, AnimationCompletedCallback callback) {
    this->mAniCompleted->setAnimationCompletedCallback(aniName, callback);
}

void CCBView::playAnimation(std::string const& aniName, float fTweenDuration) {
    this->mReader->getAnimationManager()->runAnimationsForSequenceNamedTweenDuration(aniName.c_str(), fTweenDuration);
}

void CCBView::removeAllAnimationCompletedCallBack() {
    this->mAniCompleted->clearListeners();
}

bool CCBView::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(pTouch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        pEvent->stopPropagation();
        return true;
    }
    return false;
}

void DisableNodeTouchForTime(cocos2d::Node* node, float duration) {
    if (node == nullptr) {
        return;
    }
    using namespace cocos2d;
    auto layer = CCBView::create();
    layer->setContentSize(node->getContentSize());
    layer->runAction(Sequence::create(DelayTime::create(duration), CallFuncN::create([&](Node* sender) {
        sender->removeFromParent();
    }), NULL));
    node->addChild(layer);
}

#define DISABLE_TOUCH_TAG (0xE31010AE)

void DisableNodeTouchForever(cocos2d::Node* node) {
    if (node == nullptr) {
        return;
    }
    using namespace cocos2d;
    auto layer = CCBView::create();
    layer->setContentSize(node->getContentSize());
    layer->setTag(DISABLE_TOUCH_TAG);
    node->addChild(layer);
}

void EnableNodeTouch(cocos2d::Node* node) {
    if (node == nullptr) {
        return;
    }
    auto view = node->getChildByTag(DISABLE_TOUCH_TAG);
    if (view != nullptr) {
        view->removeFromParent();
    }
}

void EnableNodeOpacity(cocos2d::Node* node) {
    node->setCascadeOpacityEnabled(true);
    for (auto view : node->getChildren()) {
        EnableNodeOpacity(view);
    }
}

}
