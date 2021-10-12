#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonValue>
#include <QJsonArray>
#include <QStateMachine>
#include "FicheDonne.h"

class MainWindow : public QWidget
{
    Q_OBJECT

private:

    QStateMachine *Machine = nullptr;

public:
    QLabel *intro;

    QComboBox *choixrecette;

    QTextEdit *nomrecette;
    QTextEdit *ingredients;
    QTextEdit *etapes;

    QPushButton *bGauche;
    QPushButton *bDroite;

    QPushButton *bQuitter;

    QVBoxLayout *mainLayout;

    QLabel *LabelDescri;
    QLabel *LabelIngredients;
    QLabel *LabelEtape;
    QLabel *LabelUrl;

    FicheDonne *f;

    int etapeActive = 0;

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void Incrementation();
    void Decrementation();
    void InitMachine();
    void ChargeFile();
    void Afficher();
};

#endif // MAINWINDOW_H
