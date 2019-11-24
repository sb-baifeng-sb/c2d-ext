
#include "TouchableView.hpp"

namespace c2dext {

bool TouchableView::Context::touchInView() {
    cocos2d::Vec2 touchPointInView = sender->getParent()->convertToNodeSpace(touch->getLocation());
    if( sender->getBoundingBox().containsPoint(touchPointInView) ) {
        return true;
    }
    return false;
}

TouchableView::TouchableView() {
    this->mDefault = [&](Context const& c) {
        // nothing to do
    };
    this->setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
}

void TouchableView::setTouch(std::string const& name, TouchFunc const& f) {
    this->mCallback.insert(std::make_pair(name, f));
}

TouchableView::TouchFunc const& TouchableView::get(std::string const& name) const {
    auto iter = this->mCallback.find(name);
    if (iter == this->mCallback.end()) {
        return this->mDefault;
    }
    return iter->second;
}

bool TouchableView::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(touch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        unused_event->stopPropagation();
        Context c(this, touch, unused_event);
        this->get("onTouchBegan")(c);
        return true;
    }
    return false;
}

void TouchableView::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    Context c(this, touch, unused_event);
    this->get("onTouchMoved")(c);
}

void TouchableView::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    Context c(this, touch, unused_event);
    this->get("onTouchEnded")(c);
}

void TouchableView::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    Context c(this, touch, unused_event);
    this->get("onTouchCancelled")(c);
}

ClickableView::ClickableView() {
    this->setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
}

void ClickableView::setClick(ClickCall const& f) {
    this->mClicked = f;
}

bool ClickableView::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(touch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        if (this->mClicked != nullptr) {
            this->mClicked(this);
        }
    }
    return false;
}

DoubleClickView::DoubleClickView():mClicked(false) {
    this->setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
}

void DoubleClickView::setClick(ClickCall const& f) {
    this->mClickCall = f;
}

void DoubleClickView::onCancel(float dt) {
    this->mClicked = false;
}

bool DoubleClickView::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    cocos2d::Vec2 touchPointInView = _parent->convertToNodeSpace(touch->getLocation());
    if( getBoundingBox().containsPoint(touchPointInView) ) {
        if (this->mClicked) {
            this->mClicked = false;
            if (this->mClickCall != nullptr) {
                this->mClickCall(this);
            }
        } else {
            this->mClicked = true;
            this->scheduleOnce(CC_SCHEDULE_SELECTOR(DoubleClickView::onCancel), 0.25f);
        }
    }
    return false;
}

}
