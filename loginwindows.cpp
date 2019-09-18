#include "loginwindows.h"
#include "ui_loginwindows.h"
#include "constants.h"
#include <QDebug>
#include <QMessageBox>
#include <dbmanager.h>
#include "globalsystemmanager.h"
#include "mainwindow.h"

LoginWindows::LoginWindows(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindows){
    ui->setupUi(this);
}

void LoginWindows::processUserInput(){
    QLineEdit* userIdLineEdit = findChild<QLineEdit*>(LoginWindows::USER_ID_LINE_EDIT_NAME);
    if (userIdLineEdit->text().isEmpty()){
        QMessageBox::warning(this, tr(constants::GAME_NAME), tr("Por favor digite algo!"));
    }else{
        auto name = userIdLineEdit->text();
        DbManager* p_dbManager = GlobalSystemManager::dbManager();
        int id = p_dbManager->addOrReturnUser(name);
        if( id < 0){
            QMessageBox::warning(this, tr(constants::GAME_NAME), tr("Id deve ser um número positivo!"));
        }else{
            GlobalSystemManager::mainWindow()->setId(id);
            GlobalSystemManager::mainWindow()->show();
            this->hide();
        }
    }
}

LoginWindows::~LoginWindows(){
    delete ui;
}


