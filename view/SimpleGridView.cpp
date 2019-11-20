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

QuickGridView* QuickGridView::create(cocos2d::Size const& size) {
    auto view = new (std::nothrow) QuickGridView;
    if (view && view->initWithViewSize(size)) {
        view->autorelease();
        return view;
    }
    return nullptr;
}

void QuickGridView::setCallback(std::string const& apiName, CallBack const& f) {
    this->mCallback.insert(std::make_pair(apiName, f));
}

void QuickGridView::tableCellTouched(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellTouched");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

void QuickGridView::tableCellHighlight(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellHighlight");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

void QuickGridView::tableCellUnhighlight(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellUnhighlight");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

void QuickGridView::tableCellWillRecycle(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellWillRecycle");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

cocos2d::Size QuickGridView::cellSizeForTable(TableView* table) {
    auto iter = mCallback.find("cellSizeForTable");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, 0);
        Result r;
        iter->second(c, r);
        return r.cellSize;
    }
    return cocos2d::Size::ZERO;
}

cocos2d::Size QuickGridView::tableCellSizeForIndex(TableView* table, ssize_t idx) {
    auto iter = mCallback.find("tableCellSizeForIndex");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, idx);
        Result r;
        iter->second(c, r);
        return r.cellSize;
    }
    return cellSizeForTable(table);
}

TableViewCell* QuickGridView::tableCellAtIndex(TableView *table, ssize_t idx) {
    auto iter = mCallback.find("tableCellAtIndex");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, idx);
        Result r;
        iter->second(c, r);
        return r.cell;
    }
    return nullptr;
}

ssize_t QuickGridView::numberOfCellsInTableView(TableView *table) {
    auto iter = mCallback.find("numberOfCellsInTableView");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, 0);
        Result r;
        iter->second(c, r);
        return r.number;
    }
    return 0;
}

}
