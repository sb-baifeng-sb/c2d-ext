//
//  SimpleTableView.cpp
//
//  Created by baifeng on 2018/6/8.
//

#include "SimpleTableView.hpp"

namespace c2dext {

SimpleTableView* SimpleTableView::create(cocos2d::Size const& size) {
    auto view = new (std::nothrow) SimpleTableView;
    if (view && view->initWithViewSize(size)) {
        view->autorelease();
        return view;
    }
    return nullptr;
}

SimpleTableView::SimpleTableView() {
    this->setDataSource(this);
    this->setDelegate(this);
}

SimpleTableView::~SimpleTableView() {
}

void SimpleTableView::tableCellTouched(TableView* table, TableViewCell* cell) {

}

void SimpleTableView::tableCellHighlight(TableView* table, TableViewCell* cell) {

}

void SimpleTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell) {

}

void SimpleTableView::tableCellWillRecycle(TableView* table, TableViewCell* cell) {

}

cocos2d::Size SimpleTableView::cellSizeForTable(TableView* table) {
    return cocos2d::Size::ZERO;
}

cocos2d::Size SimpleTableView::tableCellSizeForIndex(TableView* table, ssize_t idx) {
    return cellSizeForTable(table);
}

TableViewCell* SimpleTableView::tableCellAtIndex(TableView *table, ssize_t idx) {
    return nullptr;
}

ssize_t SimpleTableView::numberOfCellsInTableView(TableView *table) {
    return 0;
}

//=======================================================================

class QuickTableView::Delegate : public TableViewDataSource, public TableViewDelegate {
    friend class QuickTableView;
private:
    typedef QuickTableView::TouchCellCallBackMap TouchCellCallBackMap;
    typedef QuickTableView::ScrollCallBackMap ScrollCallBackMap;
    typedef QuickTableView::CellSizeCallBack CellSizeCallBack;
    typedef QuickTableView::NewCellCallBack NewCellCallBack;
    typedef QuickTableView::NumberCallBack NumberCallBack;
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

void QuickTableView::Delegate::scrollViewDidScroll(ScrollView* view) {
    auto iter = mScrollCallBack.find("scrollViewDidScroll");
    if (iter != mScrollCallBack.end()) {
        iter->second(view);
    }
}

void QuickTableView::Delegate::scrollViewDidZoom(ScrollView* view) {
    auto iter = mScrollCallBack.find("scrollViewDidZoom");
    if (iter != mScrollCallBack.end()) {
        iter->second(view);
    }
}

void QuickTableView::Delegate::tableCellTouched(TableView* table, TableViewCell* cell) {
    auto iter = mTouchCallback.find("tableCellTouched");
    if (iter != mTouchCallback.end()) {
        iter->second(table, cell);
    }
}

void QuickTableView::Delegate::tableCellHighlight(TableView* table, TableViewCell* cell) {
    auto iter = mTouchCallback.find("tableCellHighlight");
    if (iter != mTouchCallback.end()) {
        iter->second(table, cell);
    }
}

void QuickTableView::Delegate::tableCellUnhighlight(TableView* table, TableViewCell* cell) {
    auto iter = mTouchCallback.find("tableCellUnhighlight");
    if (iter != mTouchCallback.end()) {
        iter->second(table, cell);
    }
}

void QuickTableView::Delegate::tableCellWillRecycle(TableView* table, TableViewCell* cell) {
    auto iter = mTouchCallback.find("tableCellWillRecycle");
    if (iter != mTouchCallback.end()) {
        iter->second(table, cell);
    }
}

cocos2d::Size QuickTableView::Delegate::cellSizeForTable(TableView* table) {
    if (this->mCellSizeCallBack != nullptr) {
        return this->mCellSizeCallBack(table, 0);
    }
    return cocos2d::Size::ZERO;
}

cocos2d::Size QuickTableView::Delegate::tableCellSizeForIndex(TableView* table, ssize_t idx) {
    if (this->mCellSizeCallBack != nullptr) {
        return this->mCellSizeCallBack(table, 0);
    }
    return cocos2d::Size::ZERO;
}

TableViewCell* QuickTableView::Delegate::tableCellAtIndex(TableView *table, ssize_t idx) {
    if (this->mNewCellCallBack != nullptr) {
        return this->mNewCellCallBack(table, idx);
    }
    return nullptr;
}

ssize_t QuickTableView::Delegate::numberOfCellsInTableView(TableView *table) {
    if (this->mNumberCallBack != nullptr) {
        return this->mNumberCallBack(table);
    }
    return 0;
}

QuickTableView* QuickTableView::create(cocos2d::Size const& size) {
    auto view = new (std::nothrow) QuickTableView;
    if (view && view->initWithViewSize(size)) {
        view->autorelease();
        return view;
    }
    return nullptr;
}

QuickTableView::QuickTableView():mDelegate(new Delegate) {
    this->setDataSource(this->mDelegate);
    this->setDelegate(this->mDelegate);
}

QuickTableView::~QuickTableView() {
    delete this->mDelegate;
}

bool QuickTableView::initWithViewSize(cocos2d::Size const& size, cocos2d::Node* container) {
    auto r = TableView::initWithViewSize(size, container);
    this->setVerticalFillOrder(VerticalFillOrder::TOP_DOWN);
    return r;
}

void QuickTableView::setCallback(std::string const& apiName, ScrollCallBack const& f) {
    this->mDelegate->mScrollCallBack[apiName] = f;
}

void QuickTableView::setCallback(std::string const& apiName, TouchCellCallBack const& f) {
    this->mDelegate->mTouchCallback[apiName] = f;
}

void QuickTableView::setCallback(std::string const& apiName, CellSizeCallBack const& f) {
    if (apiName == "tableCellSizeForIndex") {
        this->mDelegate->mCellSizeCallBack = f;
    }
}

void QuickTableView::setCallback(std::string const& apiName, NewCellCallBack const& f) {
    if (apiName == "tableCellAtIndex") {
        this->mDelegate->mNewCellCallBack = f;
    }
}

void QuickTableView::setCallback(std::string const& apiName, NumberCallBack const& f) {
    if (apiName == "numberOfCellsInTableView") {
        this->mDelegate->mNumberCallBack = f;
    }
}

}
