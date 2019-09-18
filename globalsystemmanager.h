#ifndef GLOBALSYSTEMMANAGER_H
#define GLOBALSYSTEMMANAGER_H

class LoginWindows;
class DbManager;
class MainWindow;

class GlobalSystemManager
{
public:

    static DbManager* dbManager();
    static void setDbManager(DbManager* dbManager);

    static LoginWindows* loginWindow();
    static void setLoginWindow(LoginWindows* loginWindow);

    static MainWindow* mainWindow();
    static void setMainWindow(MainWindow* mainWindow);

private:
    static LoginWindows* m_loginWindow;
    static DbManager* m_dbManager;
    static MainWindow* m_mainWindow;
    GlobalSystemManager();
};

#endif // GLOBALSYSTEMMANAGER_H
