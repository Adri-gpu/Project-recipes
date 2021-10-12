#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonValueRef>

#include "FicheDonne.h"
#include "mainwindow.h"

 FicheDonne::FicheDonne()
{
    connect(this, SIGNAL(envoiejson(QJsonObject *)), this, SLOT(stockagerecette(QJsonObject *)));
}

 FicheDonne::~FicheDonne()
{

}

void FicheDonne::choisir(QString d)
 {
     //Concaténation des chaînes de caractère
     QFile fichier(d); //Ouvre le fichier

     //Prend en paramètre le fichier
     QJsonParseError error; //CHERCHER

     //Ouverture du fichier en lecture uniquement
     if(fichier.open(QFile::ReadOnly | QFile ::Text)) {                 //Ouverture du fichier en read only
        QByteArray donnees = fichier.readAll();          //Récupération de toutes les données du fichier

        //Interprétation du fichier JSON
        QJsonDocument doc = QJsonDocument::fromJson(donnees, &error);
        if(error.error != QJsonParseError::NoError)
        {
            qCritical() << "Impossible d’interpréter le fichier : " << error.errorString();
        }
        else
        {
            {
                QTextStream in(&fichier);
                QString line = in.readLine();
                QJsonObject obj=doc.object();
                emit envoiejson(&obj);
            }
        }
     }
     else {
         qCritical() << "Impossible de lire le fichier : " << error.errorString();
     }
}




void  FicheDonne::stockagerecette(QJsonObject * obj)
{
    QJsonValue val = obj->value("recipeIngredient");
    QJsonArray valArray = val.toArray();
    tabingredients = new string[50];
    QString ingredient;
    string vrai_ingredient;
    for (auto value: valArray)
        {
            ingredient = value.toString();
            vrai_ingredient = ingredient.toStdString();
            tabingredients[nbringredients] = vrai_ingredient;
            nbringredients+=1;
        }
    val = obj->value("recipeInstructions");
    valArray = val.toArray();
    tabinstructions = new string[50];
    QString instruction;
    string vrai_instruction;
    nbrinstructions = 0;
    for (auto value: valArray)
        {
            instruction = value.toString();
            vrai_instruction = instruction.toStdString();
            tabinstructions[nbrinstructions] = vrai_instruction;
            nbrinstructions+=1;
        }

    QString fnom = obj->value("name").toString();
    nom = fnom.toStdString();

    QString fdescription = obj->value("description").toString();
    description = fdescription.toStdString();

    QString furl = obj->value("url").toString();
    url = furl.toStdString();

    QString fprepa = obj->value("prepTime").toString();
    preparation = fprepa.toStdString();

    QString fcuisson = obj->value("cookTime").toString();
    cuisson = fcuisson.toStdString();

    QString ftemps = obj->value("totalTime").toString();
    totaltemps = ftemps.toStdString();

    QString fmots = obj->value("keywords").toString();
    motscles = fmots.toStdString();

    QString fpersonnes= QString::number((obj->value("recipeYield")).toInt());
    nbrpersonnes = fpersonnes.toStdString();

    QString ftype= obj->value("recipeCategory").toString();
    type = ftype.toStdString();

    emit DonneRecette();

}
