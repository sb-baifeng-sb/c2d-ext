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
    typedef QuickTableView::CallBackMap CallBackMap;
    typedef QuickTableView::Context Context;
    typedef QuickTableView::Result Result;
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
    CallBackMap mCallback;
};

void QuickTableView::Delegate::scrollViewDidScroll(ScrollView* view) {
    auto iter = mCallback.find("scrollViewDidScroll");
    if (iter != mCallback.end()) {
        Context c(view);
        Result r;
        iter->second(c, r);
    }
}

void QuickTableView::Delegate::scrollViewDidZoom(ScrollView* view) {
    auto iter = mCallback.find("scrollViewDidZoom");
    if (iter != mCallback.end()) {
        Context c(view);
        Result r;
        iter->second(c, r);
    }
}

void QuickTableView::Delegate::tableCellTouched(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellTouched");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

void QuickTableView::Delegate::tableCellHighlight(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellHighlight");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

void QuickTableView::Delegate::tableCellUnhighlight(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellUnhighlight");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

void QuickTableView::Delegate::tableCellWillRecycle(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellWillRecycle");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

cocos2d::Size QuickTableView::Delegate::cellSizeForTable(TableView* table) {
    auto iter = mCallback.find("cellSizeForTable");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, 0);
        Result r;
        iter->second(c, r);
        return r.cellSize;
    }
    return cocos2d::Size::ZERO;
}

cocos2d::Size QuickTableView::Delegate::tableCellSizeForIndex(TableView* table, ssize_t idx) {
    auto iter = mCallback.find("tableCellSizeForIndex");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, idx);
        Result r;
        iter->second(c, r);
        return r.cellSize;
    }
    return cellSizeForTable(table);
}

TableViewCell* QuickTableView::Delegate::tableCellAtIndex(TableView *table, ssize_t idx) {
    auto iter = mCallback.find("tableCellAtIndex");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, idx);
        Result r;
        iter->second(c, r);
        return r.cell;
    }
    return nullptr;
}

ssize_t QuickTableView::Delegate::numberOfCellsInTableView(TableView *table) {
    auto iter = mCallback.find("numberOfCellsInTableView");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, 0);
        Result r;
        iter->second(c, r);
        return r.number;
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

void QuickTableView::setCallback(std::string const& apiName, CallBack const& f) {
    this->mDelegate->mCallback.insert(std::make_pair(apiName, f));
}

}
