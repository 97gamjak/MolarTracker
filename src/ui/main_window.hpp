#ifndef __UI__MAIN_WINDOW_HPP__
#define __UI__MAIN_WINDOW_HPP__

#include <QMainWindow>

class TopMenuBar;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

   public:
    explicit MainWindow(QWidget* parent = nullptr);

   private:
    void buildCentral();

    TopMenuBar* _topMenuBar;
};

#endif   // __UI__MAIN_WINDOW_HPP__