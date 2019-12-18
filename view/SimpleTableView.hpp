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

class QuickTableView : public TableView {
    class Delegate;
public:
    typedef std::function<void(ScrollView* view)> ScrollCallBack;
    typedef std::function<void(TableView* table, TableViewCell* cell)> TouchCellCallBack;
    typedef std::function<cocos2d::Size(TableView *table, ssize_t idx)> CellSizeCallBack;
    typedef std::function<TableViewCell*(TableView *table, ssize_t idx)> NewCellCallBack;
    typedef std::function<ssize_t(TableView *table)> NumberCallBack;
    typedef std::map<std::string, TouchCellCallBack> TouchCellCallBackMap;
    typedef std::map<std::string, ScrollCallBack> ScrollCallBackMap;
public:
    static QuickTableView* create(cocos2d::Size const& size);
    CREATE_FUNC(QuickTableView);
    QuickTableView();
    ~QuickTableView();
public:
    bool initWithViewSize(cocos2d::Size const& size, cocos2d::Node* container = NULL) override;
public:
    void setCallback(std::string const& apiName, ScrollCallBack const& f);
    void setCallback(std::string const& apiName, TouchCellCallBack const& f);
    void setCallback(std::string const& apiName, CellSizeCallBack const& f);
    void setCallback(std::string const& apiName, NewCellCallBack const& f);
    void setCallback(std::string const& apiName, NumberCallBack const& f);
private:
    Delegate* mDelegate;
};

}

#endif /* SimpleTableView_hpp */
