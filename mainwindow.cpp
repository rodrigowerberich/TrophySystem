#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globalsystemmanager.h"
#include "loginwindows.h"
#include "dbmanager.h"
#include <QDebug>
#include "constants.h"
#include <functional>

#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_id{-1},
    m_sliderCoinCollectedAmount{1},
    m_sliderDeathsAmount{1},
    m_sliderTurtlesAmount{1},
    m_sliderBowsersAmout{1},
    m_coinCollectedAmount{0},
    m_deathsAmount{0},
    m_turtlesAmount{0},
    m_bowsersAmout{0}
{
    ui->setupUi(this);
    // Setting up the checkbox trophy code
    static QVector<QVector<QString>> checkBoxNames = {
        {"coinTrophy1CheckBox", "coinTrophy100CheckBox", "coinTrophy1000CheckBox", "coinTrophy10000CheckBox", "coinTrophy100000CheckBox"},
        {"deathTrophy1CheckBox", "deathTrophy10CheckBox", "deathTrophy25CheckBox", "deathTrophy50CheckBox", "deathTrophy100CheckBox"},
        {"turtleTrophy1CheckBox", "turtleTrophy100CheckBox", "turtleTrophy1000CheckBox", "turtleTrophy10000CheckBox", "turtleTrophy100000CheckBox"},
        {"bowserTrophy1CheckBox", "bowserTrophy100CheckBox", "bowserTrophy1000CheckBox", "bowserTrophy10000CheckBox", "bowserTrophy100000CheckBox"}
    };
    for(const auto& category: checkBoxNames){
        QVector<QCheckBox*> categoryCheckBoxes;
        for(const auto& name: category){
            auto checkBox = this->findChild<QCheckBox*>(name);
            if(checkBox != nullptr){
                // This makes the user not able to set the trophy him/herself
                checkBox->setAttribute(Qt::WA_TransparentForMouseEvents);
                checkBox->setFocusPolicy(Qt::NoFocus);
                categoryCheckBoxes.push_back(checkBox);
            }
        }
        m_trophyDisplay.push_back(categoryCheckBoxes);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkTrophies()
{
    auto trophies =  GlobalSystemManager::dbManager()->getTrophies(m_id);
    for (const auto& trophy: trophies) {
        setTrophyWon(trophy.type(), trophy.level());

    }
}

void MainWindow::setTrophyWon(int trophyId, int trophyLevel)
{
    // This is not really safe, but due to scope and time of this project I will leave it like that
    m_trophyDisplay[trophyId-1][trophyLevel-1]->setCheckState(Qt::CheckState::Checked);
}

void MainWindow::clearTrophyDisplay()
{
    for(const auto& category: m_trophyDisplay){
        for(auto& checkBox: category){
            checkBox->setCheckState(Qt::CheckState::Unchecked);
        }
    }
}

void MainWindow::setId(int id)
{
    if( m_id != id ){
        clearTrophyDisplay();
        m_id = id;
        emit idChanged(id);
        setCoinCollectedAmount(GlobalSystemManager::dbManager()->coinsCollected(m_id));
        setDeathsAmount(GlobalSystemManager::dbManager()->userDeathCount(m_id));
        setTurtlesAmount(GlobalSystemManager::dbManager()->monsterKilled(m_id, constants::TURTLE_ID));
        setBowsersAmout(GlobalSystemManager::dbManager()->monsterKilled(m_id, constants::BOWSER_ID));
        checkTrophies();
    }
}

void MainWindow::setSliderCoinCollectedAmount(int sliderCoinCollectedAmount)
{
    m_sliderCoinCollectedAmount = sliderCoinCollectedAmount;
}

void MainWindow::setSliderDeathsAmount(int sliderDeathsAmount)
{
    m_sliderDeathsAmount = sliderDeathsAmount;
}

void MainWindow::setSliderTurtlesAmount(int sliderTurtlesAmount)
{
    m_sliderTurtlesAmount = sliderTurtlesAmount;
}

void MainWindow::setSliderBowsersAmout(int sliderBowsersAmout)
{
    m_sliderBowsersAmout = sliderBowsersAmout;
}

void MainWindow::setBowsersAmout(int bowsersAmout)
{
    m_bowsersAmout = bowsersAmout;
    emit killedBowsersChanged(m_bowsersAmout);
}

void MainWindow::earnCoinsButtonPressed()
{
    GlobalSystemManager::dbManager()->collectCoin(m_id, m_sliderCoinCollectedAmount);
    setCoinCollectedAmount(m_coinCollectedAmount+m_sliderCoinCollectedAmount);
    checkTrophies();
}

void MainWindow::dieButtonPressed()
{
    GlobalSystemManager::dbManager()->killUserMultipleTimes(m_id, m_sliderDeathsAmount);
    setDeathsAmount(m_deathsAmount+m_sliderDeathsAmount);
    checkTrophies();
}

void MainWindow::killTurtlesButtonPressed()
{
    GlobalSystemManager::dbManager()->killMonsters(m_id, constants::TURTLE_ID, m_sliderTurtlesAmount);
    setTurtlesAmount(m_turtlesAmount+m_sliderTurtlesAmount);
    checkTrophies();
}

void MainWindow::killBowsersButtonPressed()
{
    GlobalSystemManager::dbManager()->killMonsters(m_id, constants::BOWSER_ID, m_sliderBowsersAmout);
    setBowsersAmout(m_bowsersAmout+m_sliderBowsersAmout);
    checkTrophies();
}

void MainWindow::exitButtonPressed()
{
    this->hide();
    GlobalSystemManager::loginWindow()->show();
}

void MainWindow::setTurtlesAmount(int turtlesAmount)
{
    m_turtlesAmount = turtlesAmount;
    emit killedTurtlesChanged(m_turtlesAmount);
}

void MainWindow::setDeathsAmount(int deathsAmount)
{
    m_deathsAmount = deathsAmount;
    emit deathsChanged(m_deathsAmount);
}

void MainWindow::setCoinCollectedAmount(int coinCollectedAmount)
{
    m_coinCollectedAmount = coinCollectedAmount;
    emit coinsCollectedChanged(m_coinCollectedAmount);
}
