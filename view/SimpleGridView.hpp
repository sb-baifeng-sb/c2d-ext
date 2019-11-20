//
//  SimpleGridView.hpp
//  card
//
//  Created by baifeng on 2018/10/24.
//

#ifndef __C2DEXT_SIMPLE_GRIDVIEW_H__
#define __C2DEXT_SIMPLE_GRIDVIEW_H__

#include <stdio.h>
#include "gridview/GridView.hpp"

namespace c2dext {

class SimpleGridView : public GridView, public TableViewDataSource, public TableViewDelegate {
public:
    static SimpleGridView* create(cocos2d::Size const& size);
    CREATE_FUNC(SimpleGridView);
    SimpleGridView();
    ~SimpleGridView();
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

class QuickGridView : public SimpleGridView {
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
        ssize_t number;
    public:
        Result():cell(nullptr), number(0){}
        Result(cocos2d::Size const& cellSize, TableViewCell* cell, ssize_t size):cellSize(cellSize), cell(cell), number(size){}
    };
    typedef std::function<void(Context& c, Result& r)> CallBack;
    typedef std::map<std::string, CallBack> CallBackMap;
public:
    static QuickGridView* create(cocos2d::Size const& size);
    CREATE_FUNC(QuickGridView);
public:
    void setCallback(std::string const& apiName, CallBack const& f);
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

#endif /* SimpleGridView_hpp */
