 #include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    StudentBook *studentbook = new StudentBook;
    studentbook->show();


    return app.exec();
}
