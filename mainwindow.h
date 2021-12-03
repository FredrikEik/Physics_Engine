#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_pushButton_toggled(bool checked);

    //knapp 1 lager trekant
    void on_pushButton_clicked();
    //knapp 2 lager plane
    void on_pushButton_2_clicked();
    //knapp 3 simulerer
    void on_pushButton_3_clicked();

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
};

#endif // MAINWINDOW_H
