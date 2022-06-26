/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef TABLEVIEWNEW3D_H
#define TABLEVIEWNEW3D_H

#include <QWidget>
#include <QTimer>

class TableViewNew3D : public QWidget
{
    Q_OBJECT
public:
    explicit TableViewNew3D(QWidget *parent = 0);
    void clear();
    void setRowCount(int count);
    void setColumnCount(int count);
    void setItem(int row,int column,QString text);
    void rebuildTable();
    int rowCount() { return m_rowCount; }
    int columnCount() { return m_columnCount; }
    QList<QPoint> selectedItems();
    QString item(int row,int column);
    void setXAxis(int index,QString number);
    void setYAxis(int index,QString number);
    QString xAxis(int index);
    QString yAxis(int index);
    void addHotkey(int key,Qt::KeyboardModifier modifier);
    void setTracingValue(double x,double y);
    void setTracingEnabled(bool enabled) { m_traceEnabled = enabled; update(); }
    void setMaxValues(double maxx,double maxy,double maxz);
private:
    double m_maxXValue;
    double m_maxYValue;
    double m_maxZValue;
    void drawCell(QPainter *painter,int cellx,int celly,QString text,bool highlight, bool current);
    QTimer *m_updateTimer;
    double m_traceX;
    double m_traceY;
    bool m_traceEnabled;
    QList<QPair<int,Qt::KeyboardModifier> > m_hotkeyMap;
    double m_itemHeight;
    double m_itemWidth;
    void resizeEvent(QResizeEvent *evt);
    void paintEvent (QPaintEvent *evt);
    QList<QString> xaxis;
    QList<QString> yaxis;
    QList<QList<QString> > values;
    QList<QPair<int, int> > m_selectionList;
    void mouseMoveEvent(QMouseEvent *evt);
    void mousePressEvent(QMouseEvent *evt);
    void mouseReleaseEvent(QMouseEvent *evt);
    int m_x;
    int m_y;
    QList<QList<int> > m_highlightList;
    void keyPressEvent(QKeyEvent *evt);
    QPoint currentCell;
    QPoint startSelectCell;
    QList<QPoint> m_selectedCells;
    int m_minSelectedRow;
    int m_maxSelectedRow;
    int m_minSelectedColumn;
    int m_maxSelectedColumn;
    bool multiSelect;
    int m_rowCount;
    int m_columnCount;
    void rebuildtable();
    bool m_inEdit;
    QString m_editText;

signals:
    void keyPressed(int key);
    void hotKeyPressed(int key,Qt::KeyboardModifier modifier);
    //void itemChangeRequest(int row,int column,QString text);
    void itemChangeRequest(int minrow,int maxrow,int mincolumn,int maxcolumn,QString text);
    void currentSelectionChanged(QList<QPair<int,int> > selectionList);
public slots:

};

#endif // TABLEVIEWNEW3D_
