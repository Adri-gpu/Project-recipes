#include "mainwindow.h"
#include "FicheDonne.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonValue>
#include <QJsonArray>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    //Initialisation des éléments de la page

    setWindowTitle("Recettes de cuisine");
    this->resize(1100,700);

    LabelDescri = new QLabel("Description :",this);
    LabelIngredients = new QLabel("Liste des ingrédients:",this);
    LabelEtape = new QLabel("Faire défiler les étapes :",this);
    LabelUrl = new QLabel("Url :",this);

    nomrecette = new QTextEdit(this);
    nomrecette -> setReadOnly(true);

    ingredients = new QTextEdit(this);
    ingredients -> setReadOnly(true);

    etapes = new QTextEdit(this);
    etapes -> setReadOnly(true);
    etapes-> setAlignment(Qt::AlignCenter);

    bGauche = new QPushButton("Recette précédente");
    bDroite = new QPushButton("Recette suivante");
    bQuitter= new QPushButton("Quitter");


    //Positionnement par layout

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    QVBoxLayout *vlayout3 = new QVBoxLayout;
    QHBoxLayout *hlayout3 = new QHBoxLayout;
    QHBoxLayout *hlayout4 = new QHBoxLayout;
    QVBoxLayout *vlayout6 = new QVBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QMenuBar* menuBar = new QMenuBar();
    QMenu *fileMenu = new QMenu("Fichier");
    menuBar->addMenu(fileMenu);

    fileMenu->addAction(tr("Ouvrir"), this, SLOT(ChargeFile()),QKeySequence::New);
    fileMenu->addAction(tr("Quitter"), this, SLOT(close()),QKeySequence::Close);

    this->layout()->setMenuBar(menuBar);

    hlayout1->addWidget(LabelDescri);
    hlayout1->addWidget(LabelIngredients);
    hlayout2->addWidget(nomrecette);
    hlayout2->addWidget(ingredients);

    vlayout3->addWidget(LabelEtape);
    hlayout3->addWidget(bGauche);
    hlayout3->addWidget(etapes);
    hlayout3->addWidget(bDroite);

    hlayout4->addWidget(LabelUrl);

    vlayout6->addWidget(bQuitter);

    mainLayout->addLayout(hlayout1);
    mainLayout->addLayout(hlayout2);
    mainLayout->addLayout(vlayout3);
    mainLayout->addLayout(hlayout3);
    mainLayout->addLayout(hlayout4);
    mainLayout->addLayout(vlayout6);

    setLayout(mainLayout);


    //Initialisation des boutons

    bGauche->setDisabled(true);
    bDroite->setDisabled(true);


    //Variables globales

    f = new FicheDonne;


    //Connects

    connect(bQuitter,SIGNAL(clicked()),this,SLOT(close()));
    connect(bGauche,SIGNAL(clicked()),this,SLOT(Decrementation()));
    connect(bDroite,SIGNAL(clicked()),this,SLOT(Incrementation()));
    connect(f,SIGNAL(DonneRecette()),this,SLOT(Afficher()));
    connect(f,SIGNAL(DonneRecette()),this,SLOT(InitMachine()));
}

MainWindow::~MainWindow()
{

}


void MainWindow::Afficher()
{
    //Mise à 0 de la machine et nettoyage des données

    LabelUrl->setText("Url : "+QString::fromStdString(f->URL()));
    ingredients->clear();
    nomrecette->clear();
    etapeActive=0;
    bGauche->setDisabled(true);


    //Mises à jour.
    //Des étapes...
    if(etapeActive != f->getnbrinstructions()-1){
        bDroite->setDisabled(false);
    }
    if(etapeActive != 0)
    {
        bGauche->setDisabled(false);
    }
    etapes->setText(QString::fromStdString(f->gettabinstructions(etapeActive)));


    //...des ingrédients...
    for(int i = 0; i < f->NbrIngredients(); ++i)
        ingredients->append(QString::fromStdString(f->TablIngredients(i)));


    //...puis des autres variables.
    if (f->Nom() != ""){
        nomrecette->setText((QString::fromStdString(f->Nom())));
    }

    if (f->Description() != ""){
        nomrecette->append((QString::fromStdString(f->Description())));
    }

    if (f->getnbrpersonnes() != ""){
        nomrecette->append(("\nCette préparation est pour : "+QString::fromStdString(f->getnbrpersonnes()+" personne(s).")));
    }

    if (f->gettotaltemps() != ""){
         nomrecette->append(("\nLe temps total de préparation est de : "+QString::fromStdString(f->gettotaltemps())));
    }

    if (f->getcuisson() != ""){
        nomrecette->append(("\nLe temps de cuisson est : "+QString::fromStdString(f->getcuisson())));
    }

    if (f->gettype() != ""){
        nomrecette->append(("\nCe plat est un plat de type : "+QString::fromStdString(f->gettype())));
    }

    //Images

}


void MainWindow::InitMachine()
{
    //Supprime la machine précédente et en crée une autre
    delete Machine;
    Machine = new QStateMachine(this);

    //Initialisation des états
    QState *precedent = new QState();
    QState *suivant = new QState();
    QState *Debut = new QState();

    //Implémentation des états et démarrage
    Machine->addState(Debut);
    Machine->addState(suivant);
    Machine->addState(precedent);
    Machine->setInitialState(Debut);
    Machine->start();

    //Connects
    connect(precedent, SIGNAL(entered()), this, SLOT(Decrementation()));
    connect(suivant, SIGNAL(entered()), this, SLOT(Incrementation()));
}


void MainWindow::Decrementation()
{
    bDroite->setDisabled(false);
    if(etapeActive == 0)
    {
        qDebug() << "Il n'y a plus d'étapes en dessous";
    }
    else
    {
        etapeActive -=1;
        qDebug() << " Etape actuelle : " << QString::number(etapeActive+1);
        etapes->setText(QString::fromStdString(f->gettabinstructions(etapeActive)));
        if(etapeActive == 0)
        {
            bGauche->setDisabled(true);
        }
    }
}


void MainWindow::Incrementation()
{
    bGauche->setDisabled(false);
    if(etapeActive == f->getnbrinstructions()-1)
    {
        qDebug() << "Aucune étape de plus";

    }
    else
    {
        etapeActive +=1;
        qDebug() << " Etape actuelle : " << QString::number(etapeActive+1);
        etapes->setText(QString::fromStdString(f->gettabinstructions(etapeActive)));
        if(etapeActive == f->getnbrinstructions()-1){
            bDroite->setDisabled(true);
        }
    }
}


void MainWindow::ChargeFile()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Choisir une recette"));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Fichier JSON (*.json)"));
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileName;
    if (dialog.exec()){
        fileName = dialog.selectedFiles();
        QString file (fileName[0]);
        try {
            f->choisir(file);
        } catch(std::runtime_error& e) {
            QMessageBox::critical(0,tr("Erreur"), e.what());

        }
    }
}





