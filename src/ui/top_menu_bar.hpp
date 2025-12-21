#ifndef __UI__TOP_MENU_BAR_HPP__
#define __UI__TOP_MENU_BAR_HPP__

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class TopMenuBar : public QObject
{
    Q_OBJECT

   private:
    QMainWindow& _mainWindow;

   public:
    explicit TopMenuBar(QMainWindow& mainWindow);

    void build();
};

#endif   // __UI__TOP_MENU_BAR_HPP__