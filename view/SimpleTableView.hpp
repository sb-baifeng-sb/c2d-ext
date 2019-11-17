//
//  SimpleTableView.hpp
//
//  Created by baifeng on 2018/6/8.
//

#ifndef __C2DEXT_SIMPLE_TABLEVIEW_H__
#define __C2DEXT_SIMPLE_TABLEVIEW_H__

#include <stdio.h>
#include "TableView-ext.hpp"

namespace c2dext {

class SimpleTableView : public TableView, public TableViewDataSource, public TableViewDelegate {
public:
    static SimpleTableView* create(cocos2d::Size const& size);
    CREATE_FUNC(SimpleTableView);
    SimpleTableView();
    ~SimpleTableView();
public:
    virtual void setContentSize(cocos2d::Size const& size) override;
protected:
    // scroll view delegate
    virtual void scrollViewDidScroll(ScrollView* view) override;
    virtual void scrollViewDidZoom(ScrollView* view) override;
protected:
    // table view delegate
    virtual void tableCellTouched(TableView* table, TableViewCell* cell) override;
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell) override;
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell) override;
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell) override;
protected:
    // dataSource
    virtual cocos2d::Size cellSizeForTable(TableView* table) override; // no use
    virtual cocos2d::Size tableCellSizeForIndex(TableView* table, ssize_t idx) override;
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(TableView *table) override;
};

class QuickTableView : public SimpleTableView {
public:
    class Context {
    public:
        TableView* sender;
        TableViewCell* cell;
        ssize_t index;
    public:
        Context():sender(nullptr), cell(nullptr), index(0){}
        Context(TableView* table, TableViewCell* cell, ssize_t idx):sender(table), cell(cell), index(idx){}
    };
    class Result {
    public:
        cocos2d::Size cellSize;
        TableViewCell* cell;
        ssize_t size;
    public:
        Result():cell(nullptr), size(0){}
        Result(cocos2d::Size const& cellSize, TableViewCell* cell, ssize_t size):cellSize(cellSize), cell(cell), size(size){}
    };
    typedef std::function<void(Context& c, Result& r)> CallBack;
    typedef std::map<std::string, CallBack> CallBackMap;
public:
    CREATE_FUNC(QuickTableView);
public:
    void setCallback(std::string const& apiName, CallBack const& f);
protected:
    // scroll view delegate
    virtual void scrollViewDidScroll(ScrollView* view) override;
    virtual void scrollViewDidZoom(ScrollView* view) override;
protected:
    // table view delegate
    virtual void tableCellTouched(TableView* table, TableViewCell* cell) override;
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell) override;
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell) override;
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell) override;
protected:
    // dataSource
    virtual cocos2d::Size cellSizeForTable(TableView* table) override; // no use
    virtual cocos2d::Size tableCellSizeForIndex(TableView* table, ssize_t idx) override;
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(TableView *table) override;
private:
    CallBackMap mCallback;
};

}

#endif /* SimpleTableView_hpp */
