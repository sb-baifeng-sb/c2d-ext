/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2010 Sangwoo Im
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __C2DEXT_CCSCROLLVIEW_H__
#define __C2DEXT_CCSCROLLVIEW_H__

#include "2d/CCLayer.h"
#include "base/CCEventListenerTouch.h"
#include "2d/CCActionTween.h"
#include "extensions/ExtensionMacros.h"
#include "extensions/ExtensionExport.h"

/**
 * @addtogroup ui
 * @{
 */
namespace c2dext {

class ScrollView;

class ScrollViewDelegate
{
public:
    /**
     * @js NA
     * @lua NA
     */
    virtual ~ScrollViewDelegate() {}
    /**
     * @js NA
     * @lua NA
     */
    virtual void scrollViewDidScroll(ScrollView* view) {};
    /**
     * @js NA
     * @lua NA
     */
    virtual void scrollViewDidZoom(ScrollView* view) {};
};


/**
 * ScrollView support for cocos2d-x.
 * It provides scroll view functionalities to cocos2d projects natively.
 */
class ScrollView : public cocos2d::Layer, public cocos2d::ActionTweenDelegate
{
public:
    enum class Direction
    {
        NONE = -1,
        HORIZONTAL = 0,
        VERTICAL,
        BOTH
    };
    /**
     * Returns an autoreleased scroll view object.
     *
     * @param size view size
     * @param container parent object
     * @return autoreleased scroll view object
     */
    static ScrollView* create(cocos2d::Size size, Node* container = NULL);

    /**
     * Returns an autoreleased scroll view object.
     *
     * @return autoreleased scroll view object
     */
    static ScrollView* create();
    /**
     * @js ctor
     * @lua new
     */
    ScrollView();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~ScrollView();

    bool init() override;
    /**
     * Returns a scroll view object
     *
     * @param size view size
     * @param container parent object
     * @return scroll view object
     */
    bool initWithViewSize(cocos2d::Size size, cocos2d::Node* container = NULL);

    /**
     * Sets a new content offset. It ignores max/min offset. It just sets what's given. (just like UIKit's UIScrollView)
     *
     * @param offset    The new offset.
     * @param animated  If true, the view will scroll to the new offset.
     */
    void setContentOffset(cocos2d::Vec2 offset, bool animated = false);
    cocos2d::Vec2 getContentOffset();
    /**
     * Sets a new content offset. It ignores max/min offset. It just sets what's given. (just like UIKit's UIScrollView)
     * You can override the animation duration with this method.
     *
     * @param offset    The new offset.
     * @param dt        The animation duration.
     */
    void setContentOffsetInDuration(cocos2d::Vec2 offset, float dt);
    /**
     * Halts the movement animation of the inner content started with setContentOffset() or setContentOffsetInDuration()
     */
    void stopAnimatedContentOffset();

    void setZoomScale(float s);
    /**
     * Sets a new scale and does that for a predefined duration.
     *
     * @param s         The new scale vale
     * @param animated  If true, scaling is animated
     */
    void setZoomScale(float s, bool animated);

    float getZoomScale();

    /**
     * Sets a new scale for container in a given duration.
     *
     * @param s     The new scale value
     * @param dt    The animation duration
     */
    void setZoomScaleInDuration(float s, float dt);

    /**
     * Set min scale
     *
     * @param minScale min scale
     */
    void setMinScale(float minScale) {
        _minScale = minScale;
    }
    /**
     * Set max scale
     *
     * @param maxScale max scale
     */
    void setMaxScale(float maxScale) {
        _maxScale = maxScale;
    }

    /**
     * Returns the current container's minimum offset. You may want this while you animate scrolling by yourself
     */
    cocos2d::Vec2 minContainerOffset();
    /**
     * Returns the current container's maximum offset. You may want this while you animate scrolling by yourself
     */
    cocos2d::Vec2 maxContainerOffset();
    /**
     * Determines if a given node's bounding box is in visible bounds
     *
     * @returns true if it is in visible bounds
     */
    bool isNodeVisible(cocos2d::Node * node);
    /**
     * Provided to make scroll view compatible with SWLayer's pause method
     */
    using Layer::pause;  // fix warning
    void pause(cocos2d::Ref* sender);
    /**
     * Provided to make scroll view compatible with SWLayer's resume method
     */
    using Layer::resume; // fix warning
    void resume(Ref* sender);

    void setTouchEnabled(bool enabled);
	bool isTouchEnabled() const;
    void setSwallowTouches(bool needSwallow);
    bool isDragging() const {return _dragging;}
    bool isTouchMoved() const { return _touchMoved; }
    bool isBounceable() const { return _bounceable; }
    void setBounceable(bool bBounceable) { _bounceable = bBounceable; }

    /**
     * size to clip. Node boundingBox uses contentSize directly.
     * It's semantically different what it actually means to common scroll views.
     * Hence, this scroll view will use a separate size property.
     */
    cocos2d::Size getViewSize() const { return _viewSize; }
    void setViewSize(cocos2d::Size size);

    cocos2d::Node * getContainer();
    void setContainer(cocos2d::Node * pContainer);

    /**
     * direction allowed to scroll. ScrollViewDirectionBoth by default.
     */
    Direction getDirection() const { return _direction; }
    virtual void setDirection(Direction eDirection) { _direction = eDirection; }
    /**
     * @js NA
     * @lua NA
     */
    ScrollViewDelegate* getDelegate() { return _delegate; }
    /**
     * @code
     * when this function bound to js or lua,the input param are changed
     * in js: var setDelegate(var jsObject)
     * in lua: local setDelegate()
     * @endcode
     */
    void setDelegate(ScrollViewDelegate* pDelegate) { _delegate = pDelegate; }

	void updateInset();

    /**
     * Determines whether it clips its children or not.
     */
    bool isClippingToBounds() { return _clippingToBounds; }
    void setClippingToBounds(bool bClippingToBounds) { _clippingToBounds = bClippingToBounds; }

    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) override;
    
    // Overrides
    virtual void setContentSize(const cocos2d::Size & size) override;
    virtual const cocos2d::Size& getContentSize() const override;
    /**
     * @js NA
     * @lua NA
     */
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override;
    
    using Node::addChild;
    virtual void addChild(cocos2d::Node * child, int zOrder, int tag) override;
    virtual void addChild(cocos2d::Node * child, int zOrder, const std::string &name) override;

    virtual void removeAllChildren() override;
    virtual void removeAllChildrenWithCleanup(bool cleanup) override;
    virtual void removeChild(cocos2d::Node* child, bool cleanup = true) override;
    /**
     * CCActionTweenDelegate
     */
    void updateTweenAction(float value, const std::string& key) override;

    bool hasVisibleParents() const;
protected:
    /**
     * Relocates the container at the proper offset, in bounds of max/min offsets.
     *
     * @param animated If true, relocation is animated
     */
    void relocateContainer(bool animated);
    /**
     * implements auto-scrolling behavior. change SCROLL_DEACCEL_RATE as needed to choose
     * deacceleration speed. it must be less than 1.0f.
     *
     * @param dt delta
     */
    void deaccelerateScrolling(float dt);
    /**
     * This method makes sure auto scrolling causes delegate to invoke its method
     */
    void performedAnimatedScroll(float dt);
    /**
     * Expire animated scroll delegate calls
     */
    void stoppedAnimatedScroll(cocos2d::Node* node);
    /**
     * clip this view so that outside of the visible bounds can be hidden.
     */
    void beforeDraw();
    void onBeforeDraw();
    /**
     * retract what's done in beforeDraw so that there's no side effect to
     * other nodes.
     */
    void afterDraw();
    void onAfterDraw();
    /**
     * Zoom handling
     */
    void handleZoom();

    cocos2d::Rect getViewRect();

    /**
     * scroll view delegate
     */
    ScrollViewDelegate* _delegate;

    Direction _direction;
    /**
     * If YES, the view is being dragged.
     */
    bool _dragging;

    /**
     * Content offset. Note that left-bottom point is the origin
     */
    cocos2d::Vec2 _contentOffset;

    /**
     * Container holds scroll view contents, Sets the scrollable container object of the scroll view
     */
    cocos2d::Node* _container;
    /**
     * Determines whether user touch is moved after begin phase.
     */
    bool _touchMoved;
    /**
     * max inset point to limit scrolling by touch
     */
    cocos2d::Vec2 _maxInset;
    /**
     * min inset point to limit scrolling by touch
     */
    cocos2d::Vec2 _minInset;
    /**
     * Determines whether the scroll view is allowed to bounce or not.
     */
    bool _bounceable;

    bool _clippingToBounds;

    /**
     * scroll speed
     */
    cocos2d::Vec2 _scrollDistance;
    /**
     * Touch point
     */
    cocos2d::Vec2 _touchPoint;
    /**
     * length between two fingers
     */
    float _touchLength;
    /**
     * Touch objects to detect multitouch
     */
    std::vector<cocos2d::Touch*> _touches;
    /**
     * size to clip. Node boundingBox uses contentSize directly.
     * It's semantically different what it actually means to common scroll views.
     * Hence, this scroll view will use a separate size property.
     */
    cocos2d::Size _viewSize;
    /**
     * max and min scale
     */
    float _minScale, _maxScale;
    /**
     * scissor rect for parent, just for restoring GL_SCISSOR_BOX
     */
    cocos2d::Rect _parentScissorRect;
    bool _scissorRestored;
    
    /** Touch listener */
    cocos2d::EventListenerTouchOneByOne* _touchListener;
    
    cocos2d::CustomCommand _beforeDrawCommand;
    cocos2d::CustomCommand _afterDrawCommand;

    /**
     * Action created with setContentOffsetInDuration(), saved so it can be halted
     */
    cocos2d::Action* _animatedScrollAction;
};


}
// end of ui group
/// @}

#endif /* __CCSCROLLVIEW_H__ */
