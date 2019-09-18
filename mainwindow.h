#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QCheckBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void checkTrophies();
    void setTrophyWon(int trophyId, int trophyLevel);
    void clearTrophyDisplay();

public slots:
    // Slider values slots
    void setSliderCoinCollectedAmount(int sliderCoinCollectedAmount);
    void setSliderDeathsAmount(int sliderDeathsAmount);
    void setSliderTurtlesAmount(int sliderTurtlesAmount);
    void setSliderBowsersAmout(int sliderBowsersAmout);
    // Display amounts slots
    void setId(int id);
    void setCoinCollectedAmount(int coinCollectedAmount);
    void setDeathsAmount(int deathsAmount);
    void setTurtlesAmount(int turtlesAmount);
    void setBowsersAmout(int bowsersAmout);
    // Button presses slots
    void earnCoinsButtonPressed();
    void dieButtonPressed();
    void killTurtlesButtonPressed();
    void killBowsersButtonPressed();
    void exitButtonPressed();
signals:
    void idChanged(int newId);
    void coinsCollectedChanged(int newValue);
    void deathsChanged(int newValue);
    void killedTurtlesChanged(int newValue);
    void killedBowsersChanged(int newValue);
private:
    Ui::MainWindow *ui;
    int m_id;
    int m_sliderCoinCollectedAmount;
    int m_sliderDeathsAmount;
    int m_sliderTurtlesAmount;
    int m_sliderBowsersAmout;
    int m_coinCollectedAmount;
    int m_deathsAmount;
    int m_turtlesAmount;
    int m_bowsersAmout;
    QVector<QVector<QCheckBox*>> m_trophyDisplay;
};

#endif // MAINWINDOW_H
