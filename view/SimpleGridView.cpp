//
//  SimpleGridView.cpp
//  card
//
//  Created by baifeng on 2018/10/24.
//

#include "SimpleGridView.hpp"

namespace c2dext {

SimpleGridView* SimpleGridView::create(cocos2d::Size const& size) {
    auto view = new (std::nothrow) SimpleGridView;
    if (view && view->initWithViewSize(size)) {
        view->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
        view->autorelease();
        return view;
    }
    return nullptr;
}

SimpleGridView::SimpleGridView() {
    this->setDataSource(this);
    this->setDelegate(this);
}

SimpleGridView::~SimpleGridView() {
}

void SimpleGridView::tableCellTouched(TableView* table, TableViewCell* cell) {

}

void SimpleGridView::tableCellHighlight(TableView* table, TableViewCell* cell) {

}

void SimpleGridView::tableCellUnhighlight(TableView* table, TableViewCell* cell) {

}

void SimpleGridView::tableCellWillRecycle(TableView* table, TableViewCell* cell) {

}

cocos2d::Size SimpleGridView::cellSizeForTable(TableView* table) {
    return cocos2d::Size::ZERO;
}

cocos2d::Size SimpleGridView::tableCellSizeForIndex(TableView* table, ssize_t idx) {
    return cellSizeForTable(table);
}

TableViewCell* SimpleGridView::tableCellAtIndex(TableView *table, ssize_t idx) {
    return nullptr;
}

ssize_t SimpleGridView::numberOfCellsInTableView(TableView *table) {
    return 0;
}

//=======================================================================

class QuickGridView::Delegate : public TableViewDataSource, public TableViewDelegate {
    friend class QuickGridView;
private:
    typedef QuickGridView::TouchCellCallBackMap TouchCellCallBackMap;
    typedef QuickGridView::ScrollCallBackMap ScrollCallBackMap;
    typedef QuickGridView::CellSizeCallBack CellSizeCallBack;
    typedef QuickGridView::NewCellCallBack NewCellCallBack;
    typedef QuickGridView::NumberCallBack NumberCallBack;
private:
    // scroll view delegate
    void scrollViewDidScroll(ScrollView* view) override;
    void scrollViewDidZoom(ScrollView* view) override;
private:
    // table view delegate
    virtual void tableCellTouched(TableView* table, TableViewCell* cell) override;
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell) override;
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell) override;
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell) override;
private:
    // dataSource
    virtual cocos2d::Size cellSizeForTable(TableView* table) override; // no use
    virtual cocos2d::Size tableCellSizeForIndex(TableView* table, ssize_t idx) override;
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(TableView *table) override;
private:
    CellSizeCallBack mCellSizeCallBack;
    NewCellCallBack mNewCellCallBack;
    NumberCallBack mNumberCallBack;
    TouchCellCallBackMap mTouchCallback;
    ScrollCallBackMap mScrollCallBack;
};

void QuickGridView::Delegate::scrollViewDidScroll(ScrollView* view) {
    auto iter = mScrollCallBack.find("scrollViewDidScroll");
    if (iter != mScrollCallBack.end()) {
        iter->second(view);
    }
}

void QuickGridView::Delegate::scrollViewDidZoom(ScrollView* view) {
    auto iter = mScrollCallBack.find("scrollViewDidZoom");
    if (iter != mScrollCallBack.end()) {
        iter->second(view);
    }
}

void QuickGridView::Delegate::tableCellTouched(TableView* table, TableViewCell* cell) {
    auto iter = mTouchCallback.find("tableCellTouched");
    if (iter != mTouchCallback.end()) {
        iter->second(table, cell);
    }
}

void QuickGridView::Delegate::tableCellHighlight(TableView* table, TableViewCell* cell) {
    auto iter = mTouchCallback.find("tableCellHighlight");
    if (iter != mTouchCallback.end()) {
        iter->second(table, cell);
    }
}

void QuickGridView::Delegate::tableCellUnhighlight(TableView* table, TableViewCell* cell) {
    auto iter = mTouchCallback.find("tableCellUnhighlight");
    if (iter != mTouchCallback.end()) {
        iter->second(table, cell);
    }
}

void QuickGridView::Delegate::tableCellWillRecycle(TableView* table, TableViewCell* cell) {
    auto iter = mTouchCallback.find("tableCellWillRecycle");
    if (iter != mTouchCallback.end()) {
        iter->second(table, cell);
    }
}

cocos2d::Size QuickGridView::Delegate::cellSizeForTable(TableView* table) {
    if (this->mCellSizeCallBack != nullptr) {
        return this->mCellSizeCallBack(table, 0);
    }
    return cocos2d::Size::ZERO;
}

cocos2d::Size QuickGridView::Delegate::tableCellSizeForIndex(TableView* table, ssize_t idx) {
    if (this->mCellSizeCallBack != nullptr) {
        return this->mCellSizeCallBack(table, 0);
    }
    return cocos2d::Size::ZERO;
}

TableViewCell* QuickGridView::Delegate::tableCellAtIndex(TableView *table, ssize_t idx) {
    if (this->mNewCellCallBack != nullptr) {
        return this->mNewCellCallBack(table, idx);
    }
    return nullptr;
}

ssize_t QuickGridView::Delegate::numberOfCellsInTableView(TableView *table) {
    if (this->mNumberCallBack != nullptr) {
        return this->mNumberCallBack(table);
    }
    return 0;
}

QuickGridView* QuickGridView::create(cocos2d::Size const& size) {
    auto view = new (std::nothrow) QuickGridView;
    if (view && view->initWithViewSize(size)) {
        view->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
        view->autorelease();
        return view;
    }
    return nullptr;
}

QuickGridView::QuickGridView():mDelegate(new Delegate) {
    this->setDataSource(this->mDelegate);
    this->setDelegate(this->mDelegate);
}

QuickGridView::~QuickGridView() {
    delete this->mDelegate;
}

bool QuickGridView::initWithViewSize(cocos2d::Size const& size, cocos2d::Node* container) {
    auto r = TableView::initWithViewSize(size, container);
    this->setVerticalFillOrder(VerticalFillOrder::TOP_DOWN);
    return r;
}

void QuickGridView::setCallback(std::string const& apiName, ScrollCallBack const& f) {
    this->mDelegate->mScrollCallBack[apiName] = f;
}

void QuickGridView::setCallback(std::string const& apiName, TouchCellCallBack const& f) {
    this->mDelegate->mTouchCallback[apiName] = f;
}

void QuickGridView::setCallback(std::string const& apiName, CellSizeCallBack const& f) {
    if (apiName == "tableCellSizeForIndex") {
        this->mDelegate->mCellSizeCallBack = f;
    }
}

void QuickGridView::setCallback(std::string const& apiName, NewCellCallBack const& f) {
    if (apiName == "tableCellAtIndex") {
        this->mDelegate->mNewCellCallBack = f;
    }
}

void QuickGridView::setCallback(std::string const& apiName, NumberCallBack const& f) {
    if (apiName == "numberOfCellsInTableView") {
        this->mDelegate->mNumberCallBack = f;
    }
}

}
