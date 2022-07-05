#include "TableDialog.h"

TableDialoge::TableDialoge(QWidget *parent)
    : QDialog(parent)
{
    tableWidget = new QTableWidget;

    QHBoxLayout *layoutT = new QHBoxLayout;
    layoutT->addWidget(tableWidget);

    tableWidget->setColumnCount(3);

    QStringList longerList = (QStringList() << "Name" << "Course" << "Group");
    tableWidget->setHorizontalHeaderLabels(longerList);



    setLayout(layoutT);
    setWindowTitle(tr("Table a Contact"));
}

void TableDialoge::createT(BookEl& student_)
{
    int size = student_.size();
    tableWidget->setRowCount(size);

    BookEl::iterator iter = student_.begin();

    for (int i = 0; i < size; ++i)
    {

       tableWidget->setItem(i,0, new QTableWidgetItem(iter.key()));
       tableWidget->setItem(i,1, new QTableWidgetItem(QString::number(iter.value().first)));
       tableWidget->setItem(i,2, new QTableWidgetItem(QString::number(iter.value().second)));
       ++iter;
    }

}
