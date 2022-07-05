#pragma once
#include <QDialog>
#include <QTableWidget>
#include <QtGui>
#include <QtWidgets>
#include <QMap>
#include <QPair>

typedef QPair<int, int> IntPair;
typedef QMap<QString, IntPair> BookEl;

class QTableWidget;

class TableDialoge : public QDialog
{
    Q_OBJECT

public:
    TableDialoge(QWidget *parent = 0);
    void createT(BookEl& student_);
private:

    QTableWidget* tableWidget;
};
