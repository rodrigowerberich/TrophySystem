#include "loginwindows.h"
#include "mainwindow.h"
#include "dbmanager.h"
#include "constants.h"
#include "globalsystemmanager.h"
#include "trophysystem.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DbManager dbManager(constants::DATABASE_NAME);
    TrophySystem trophySystem(dbManager);
    LoginWindows loginWindow;
    MainWindow mw;
    GlobalSystemManager::setDbManager(&dbManager);
    GlobalSystemManager::setLoginWindow(&loginWindow);
    GlobalSystemManager::setMainWindow(&mw);

    dbManager.createAllTablesIfNotExists();

    loginWindow.show();

    return a.exec();
}
