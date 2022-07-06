#include "mainwindow.h"
#include<QDebug>

QAction *findButton;

// Добавление
void StudentBook::addStudent()
{
    // Запоминаем выведенные данные
    oldName = nameLine->text();
    oldInf.first = courseLine->text().toInt();
    oldInf.second = groupLine->text().toInt();

    // Очищаем поля
    nameLine->clear();
    courseLine->clear();  groupLine->clear();

    //разрешаем изменять и даем фокус
    updateInterface(AddingMode);
}
void StudentBook::submitStudent()
{
    // Считываем данные
    QString name = nameLine->text();
    IntPair Inf = qMakePair(courseLine->text().toInt(), groupLine->text().toInt());

    // Проверяем что поля заполнены
    if (name == "" || Inf.first == 0 ||  Inf.second == 0) {
        QMessageBox::information(this, tr("Empty Field"),
            tr("Please enter a name, course and group."));
        return;
    }
    if (currentMode == AddingMode) { // Если это добавление
    if (!student.contains(name)) {
        student.insert(name, Inf);
        QMessageBox::information(this, tr("Add Successful"),
            tr("\"%1\" has been added to your address book.").arg(name));
    } else {
        QMessageBox::information(this, tr("Add Unsuccessful"),
            tr("Sorry, \"%1\" is already in your address book.").arg(name));
        return;
           }
    }
    else if (currentMode == EditingMode) { // Если изменение
        if (!student.contains(name)) {
            QMessageBox::information(this, tr("Edit Successful"),
                tr("\"%1\" has been edited in your address book.").arg(oldName));
            student.remove(oldName);
            student.insert(name, Inf);
        }
        else if (oldInf != Inf) {
            QMessageBox::information(this, tr("Edit Successful"),
                tr("\"%1\" has been edited in your address book.").arg(name));
            student[name] = Inf;
        }
        else {
            QMessageBox::information(this, tr("Edit Unsuccessful"),
                tr("Sorry, \"%1\" is already in your address book.").arg(name));
            return;
        }
    }
    updateInterface(NavigationMode); // Включаем навигацию
}
void StudentBook::cancel()
{
    // Просто ставим старые значения
    nameLine->setText(oldName);   
    courseLine->setText(QString::number(oldInf.first));
    groupLine->setText(QString::number(oldInf.second));

    updateInterface(NavigationMode); // Мод навигации
}

// Навигация
void StudentBook::next()
{
    // Получаем итеротор
    QString name = nameLine->text();
    BookEl::iterator i = student.find(name);

    // Передвигаем
    if (i != student.end())
        i++;
    if (i == student.end())
        i = student.begin();

    // Set Text
    nameLine->setText(i.key());
    courseLine->setText(QString::number(i.value().first));
    groupLine->setText(QString::number(i.value().second));
}
void StudentBook::previous()
{
    // Получаем итеротор
    QString name = nameLine->text();
    BookEl::iterator i = student.find(name);

    // Передвигаем
    if (i == student.end())
    {
        nameLine->clear();
        courseLine->clear();
        groupLine->clear();
        return;
    }
    if (i == student.begin())
        i = student.end();

    // Set Text
    i--;
    nameLine->setText(i.key());
    courseLine->setText(QString::number(i.value().first));
    groupLine->setText(QString::number(i.value().second));
}

// Изменение
void StudentBook::editStudent()
{
    // Запоминаем выведенные данные
    oldName = nameLine->text();
    oldInf.first = courseLine->text().toInt();
    oldInf.second = groupLine->text().toInt();

    updateInterface(EditingMode);
}
void StudentBook::removeStudent()
{ // Проверяем наличие, уточняем и удаляем
    QString name = nameLine->text();

    if (student.contains(name)) {
        int button = QMessageBox::question(this,
            tr("Confirm Remove"),
            tr("Are you sure you want to remove \"%1\"?").arg(name),
            QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::Yes) {
            previous();
            student.remove(name);
            QMessageBox::information(this, tr("Remove Successful"),
                tr("\"%1\" has been removed from your address book.").arg(name));
        }
    }

    updateInterface(NavigationMode);
}

// Изменение работы интерфейса
void StudentBook::updateInterface(Mode mode)
 { // Просто устанавливаем нужные параметры
    currentMode = mode;

    switch (currentMode) {

    case AddingMode:
    case EditingMode:
        nameLine->setReadOnly(false);
        nameLine->setFocus(Qt::OtherFocusReason);
        courseLine->setReadOnly(false);
        groupLine->setReadOnly(false);

        addButton->setEnabled(false);
        editButton->hide();
        removeButton->hide();

        nextButton->setEnabled(false);
        previousButton->setEnabled(false);

        submitButton->show();
        cancelButton->show();

        break;

    case NavigationMode:
        if (student.isEmpty()) {
         nameLine->clear();
         courseLine->clear();
         groupLine->clear();
        }

        nameLine->setReadOnly(true);
        courseLine->setReadOnly(true);
        groupLine->setReadOnly(true);
        addButton->setEnabled(true);

        int size = student.size();
        editButton->show();
        removeButton->show();
        editButton->setEnabled(size >= 1);
        removeButton->setEnabled(size >= 1);
        findButton->setEnabled(size > 2);
        nextButton->setEnabled(size > 1);
        previousButton->setEnabled(size > 1);

        submitButton->hide();
        cancelButton->hide();
        break;
     }
 }

// Поиск
void StudentBook::findStudent()
 {
     dialog->show();

     if (dialog->exec() == QDialog::Accepted) {
         QString contactName = dialog->getFindText();

         if (student.contains(contactName)) {
             nameLine->setText(contactName);
             courseLine->setText(QString::number(student.value(contactName).first));
             groupLine->setText(QString::number(student.value(contactName).second));
         } else {
             QMessageBox::information(this, tr("Contact Not Found"),
                 tr("Sorry, \"%1\" is not in your address book.").arg(contactName));
             return;
         }
     }

     updateInterface(NavigationMode);
 }

// работа с файлами
void StudentBook::saveToFile()
{
// выводит на экране модальный диалог выбора файла
QString fileName = QFileDialog::getSaveFileName(this,
    tr("Save Address Book"), "",
    tr("Address Book (*.abk);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_3);
    out << student;
    }
}
void StudentBook::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Address Book"), "",
        tr("Address Book (*.abk);;All Files (*)"));
        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_3);
        student.empty();   // очистка существующих контактов
        in >> student;
        if (student.isEmpty()) {
            QMessageBox::information(this, tr("No contacts in file"),
                tr("The file you are attempting to open contains no contacts."));
        } else {
            BookEl::iterator i = student.begin();
            nameLine->setText(i.key());
            courseLine->setText(QString::number(i.value().first));
            groupLine->setText(QString::number(i.value().second));
        }
    }
    updateInterface(NavigationMode);
}

// Показать данные в виде таблицы
void StudentBook::tableStudent()
{
    table->show();
    table->createT(student);
    this->setWindowTitle("Table Students");
}

// Конструктор
StudentBook::StudentBook(QWidget *parent)
    : QWidget(parent)
{
    // Главный Label
    QVBoxLayout  *mainLayout = new QVBoxLayout;

    // Выделяем память под компоненты
    QLabel *nameLabel = new QLabel(tr("Name:"));
    nameLine = new QLineEdit;
    nameLine->setReadOnly(true);

    QLabel *courseLabel = new QLabel(tr("Course:"));
    courseLine = new QLineEdit;
    courseLine->setReadOnly(true);

    QLabel *groupLabel = new QLabel(tr("Group:"));
    groupLine = new QLineEdit;
    groupLine->setReadOnly(true);

    editButton = new QPushButton(tr("&Edit"));
    editButton->setEnabled(false);

    removeButton = new QPushButton(tr("&Remove"));
    removeButton->setEnabled(false);


    // Иннициализируем QGridLayout  и распологаем все компоненты
    QGridLayout  *gridLayout = new QGridLayout;
    gridLayout->addWidget(nameLabel, 0, 0);
    gridLayout->addWidget(nameLine, 0, 1, 1, 3);
    gridLayout->addWidget(courseLabel, 1, 0);
    gridLayout->addWidget(courseLine, 1, 1);
    gridLayout->addWidget(groupLabel, 1, 2);
    gridLayout->addWidget(groupLine, 1, 3);

    //================= Меню ========================
    // Элементы
    QMenu * fileMenu = new QMenu("&File");
    QMenu * helpMenu = new QMenu("&Help");

    // Меню и инициализация
    QMenuBar * mainMenu = new QMenuBar;
    mainMenu->addMenu(fileMenu);
    mainMenu->addMenu(helpMenu);


    // Поиск
    dialog = new FindDialog;
    findButton = new QAction("&Search", this);
    fileMenu->addAction(findButton);
    findButton->setEnabled(false);

    // Работа с файлами
    QAction *loadButton = new QAction("&Open...", this);
    fileMenu->addAction(loadButton);
    QAction *saveButton = new QAction("&Save...", this);
    fileMenu->addAction(saveButton);
    // Подсказки
    loadButton->setToolTip(tr("Load contacts from a file"));
    saveButton->setToolTip(tr("Save contacts to a file"));

    // Показатьь в виде таблице
    table = new TableDialoge;
    QAction *tableButton = new QAction("&Table", this);
    fileMenu->addAction(tableButton);

    // Выход + разделитель
    fileMenu->addSeparator();
    QAction *quit = new QAction("&Quit", this);
    fileMenu->addAction(quit);
    connect(quit, &QAction::triggered, qApp, QApplication::quit);

    mainLayout->setMenuBar(mainMenu);
    //================= Меню ========================

    // Изначальное состоение кнопок
    // Добавление
    addButton = new QPushButton(tr("&Add"));
    addButton->show();
    submitButton = new QPushButton(tr("&Submit"));
    submitButton->hide();    //submitButton->setEnabled(false);
    cancelButton = new QPushButton(tr("&Cancel"));
    cancelButton->hide();    //cancelButton->setEnabled(false);
    // Навигация
    nextButton = new QPushButton(tr("&Next"));
    nextButton->setEnabled(false);
    previousButton = new QPushButton(tr("&Previous"));
    previousButton->setEnabled(false);

    // Привязываес кнопки (Сигналы и слоты)
    connect(addButton, SIGNAL(clicked()), this, SLOT(addStudent()));
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitStudent()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    // Навигация
    connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
    connect(previousButton, SIGNAL(clicked()), this, SLOT(previous()));
    // Изменение
    connect(editButton, SIGNAL(clicked()), this, SLOT(editStudent()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeStudent()));
    // Поиск
    connect(findButton, SIGNAL(triggered()), this, SLOT(findStudent()));
    // Работа с файлами
    connect(loadButton, SIGNAL(triggered()), this, SLOT(loadFromFile()));
    connect(saveButton, SIGNAL(triggered()), this, SLOT(saveToFile()));
    // Показать в виде таблицы
    connect(tableButton, SIGNAL(triggered()), this, SLOT(tableStudent()));

    // Распологаем кнопки
    // добавления
    QHBoxLayout *buttonLayout1 = new QHBoxLayout;
    buttonLayout1->addWidget(addButton);
    buttonLayout1->addWidget(submitButton);
    buttonLayout1->addWidget(cancelButton);
    // Навигация
    QHBoxLayout *buttonLayout2 = new QHBoxLayout;
    buttonLayout2->addWidget(previousButton);
    buttonLayout2->addWidget(nextButton);
    // Изменение
    buttonLayout1->addWidget(editButton);
    buttonLayout1->addWidget(removeButton);

    // Добавляем и выводим Layout
    mainLayout->addLayout(gridLayout, 0);
    mainLayout->addLayout(buttonLayout2, 1);
    mainLayout->addLayout(buttonLayout1, 2);
    setLayout(mainLayout);


    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint); // Запрет на изменение размеров окна
    setWindowTitle(tr("Simple Student Book"));
}
