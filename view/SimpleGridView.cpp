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
    typedef QuickGridView::CallBackMap CallBackMap;
    typedef QuickGridView::Context Context;
    typedef QuickGridView::Result Result;
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

void QuickGridView::Delegate::scrollViewDidScroll(ScrollView* view) {
    auto iter = mCallback.find("scrollViewDidScroll");
    if (iter != mCallback.end()) {
        Context c(view);
        Result r;
        iter->second(c, r);
    }
}

void QuickGridView::Delegate::scrollViewDidZoom(ScrollView* view) {
    auto iter = mCallback.find("scrollViewDidZoom");
    if (iter != mCallback.end()) {
        Context c(view);
        Result r;
        iter->second(c, r);
    }
}

void QuickGridView::Delegate::tableCellTouched(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellTouched");
    if (iter != mCallback.end()) {
        Context c(table, cell, cell->getIdx());
        Result r;
        iter->second(c, r);
    }
}

void QuickGridView::Delegate::tableCellHighlight(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellHighlight");
    if (iter != mCallback.end()) {
        Context c(table, cell, cell->getIdx());
        Result r;
        iter->second(c, r);
    }
}

void QuickGridView::Delegate::tableCellUnhighlight(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellUnhighlight");
    if (iter != mCallback.end()) {
        Context c(table, cell, cell->getIdx());
        Result r;
        iter->second(c, r);
    }
}

void QuickGridView::Delegate::tableCellWillRecycle(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellWillRecycle");
    if (iter != mCallback.end()) {
        Context c(table, cell, cell->getIdx());
        Result r;
        iter->second(c, r);
    }
}

cocos2d::Size QuickGridView::Delegate::cellSizeForTable(TableView* table) {
    auto iter = mCallback.find("cellSizeForTable");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, 0);
        Result r;
        iter->second(c, r);
        return r.cellSize;
    }
    return cocos2d::Size::ZERO;
}

cocos2d::Size QuickGridView::Delegate::tableCellSizeForIndex(TableView* table, ssize_t idx) {
    auto iter = mCallback.find("tableCellSizeForIndex");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, idx);
        Result r;
        iter->second(c, r);
        return r.cellSize;
    }
    return cellSizeForTable(table);
}

TableViewCell* QuickGridView::Delegate::tableCellAtIndex(TableView *table, ssize_t idx) {
    auto iter = mCallback.find("tableCellAtIndex");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, idx);
        Result r;
        iter->second(c, r);
        return r.cell;
    }
    return nullptr;
}

ssize_t QuickGridView::Delegate::numberOfCellsInTableView(TableView *table) {
    auto iter = mCallback.find("numberOfCellsInTableView");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, 0);
        Result r;
        iter->second(c, r);
        return r.number;
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

void QuickGridView::setCallback(std::string const& apiName, CallBack const& f) {
    this->mDelegate->mCallback.insert(std::make_pair(apiName, f));
}

}
