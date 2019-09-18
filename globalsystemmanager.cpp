#include "globalsystemmanager.h"

DbManager* GlobalSystemManager::m_dbManager = nullptr;
MainWindow* GlobalSystemManager::m_mainWindow = nullptr;
LoginWindows* GlobalSystemManager::m_loginWindow = nullptr;

DbManager *GlobalSystemManager::dbManager()
{
    return m_dbManager;
}

void GlobalSystemManager::setDbManager(DbManager *dbManager)
{
    m_dbManager = dbManager;
}

LoginWindows *GlobalSystemManager::loginWindow()
{
    return m_loginWindow;
}

void GlobalSystemManager::setLoginWindow(LoginWindows *loginWindow)
{
    m_loginWindow = loginWindow;
}

MainWindow *GlobalSystemManager::mainWindow()
{
    return m_mainWindow;
}

void GlobalSystemManager::setMainWindow(MainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
}
