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

QuickTableView* QuickTableView::create(cocos2d::Size const& size) {
    auto view = new (std::nothrow) QuickTableView;
    if (view && view->initWithViewSize(size)) {
        view->autorelease();
        return view;
    }
    return nullptr;
}

void QuickTableView::setCallback(std::string const& apiName, CallBack const& f) {
    this->mCallback.insert(std::make_pair(apiName, f));
}

void QuickTableView::tableCellTouched(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellTouched");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

void QuickTableView::tableCellHighlight(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellHighlight");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

void QuickTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellUnhighlight");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

void QuickTableView::tableCellWillRecycle(TableView* table, TableViewCell* cell) {
    auto iter = mCallback.find("tableCellWillRecycle");
    if (iter != mCallback.end()) {
        Context c(table, cell, 0);
        Result r;
        iter->second(c, r);
    }
}

cocos2d::Size QuickTableView::cellSizeForTable(TableView* table) {
    auto iter = mCallback.find("cellSizeForTable");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, 0);
        Result r;
        iter->second(c, r);
        return r.cellSize;
    }
    return cocos2d::Size::ZERO;
}

cocos2d::Size QuickTableView::tableCellSizeForIndex(TableView* table, ssize_t idx) {
    auto iter = mCallback.find("tableCellSizeForIndex");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, idx);
        Result r;
        iter->second(c, r);
        return r.cellSize;
    }
    return cellSizeForTable(table);
}

TableViewCell* QuickTableView::tableCellAtIndex(TableView *table, ssize_t idx) {
    auto iter = mCallback.find("tableCellAtIndex");
    if (iter != mCallback.end()) {
        Context c(table, nullptr, idx);
        Result r;
        iter->second(c, r);
        return r.cell;
    }
    return nullptr;
}

ssize_t QuickTableView::numberOfCellsInTableView(TableView *table) {
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
