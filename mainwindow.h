#pragma once
#include "FindDialog.h"
#include "TableDialog.h"

#include <QtWidgets>
#include <QMainWindow>
#include <QApplication>
#include <QtGui>
#include <QMap>
#include <QPair>


class QLabel;
class QLineEdit;
class QPushButton;

typedef QPair<int, int> IntPair;
typedef QMap<QString, IntPair> BookEl;

class StudentBook : public QWidget
{
    Q_OBJECT

public:
    StudentBook(QWidget *parent = 0); // Конструктор
     enum Mode { NavigationMode, AddingMode, EditingMode };

public slots: // Слоты
    // Добавление
    void addStudent();
    void submitStudent();
    void cancel();
    // Навигация
    void next();
    void previous();
    // Изменение информации
    void editStudent();
    void removeStudent();
    // ПОИСК
    void findStudent();
    // Работа с файлами
    void saveToFile();
    void loadFromFile();
    // Создание таблицы
    void tableStudent();


private:
    // Режим работы интерфейса
    void updateInterface(Mode mode);
    // Поля для ввода данных
    QLineEdit *nameLine;
    QLineEdit *courseLine;
    QLineEdit *groupLine;

    // Кнопки //
    // Добавление
    QPushButton *addButton;
    QPushButton *submitButton;
    QPushButton *cancelButton;
    // Навигация
    QPushButton *nextButton;
    QPushButton *previousButton;
    // Изменение
    QPushButton *editButton;
    QPushButton *removeButton;

    BookEl student; // контейнер для хранения элементов
    QString oldName; // Отображаемое имя
    IntPair oldInf; // Отображаемая информация
    FindDialog *dialog; // Поиск
    Mode currentMode; // Режим работы приоложения
    TableDialoge* table; // Вывод в виде таблицы
};

