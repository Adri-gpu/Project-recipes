#ifndef FICHE_H
#define FICHE_H

#include <QObject>
#include <QFile>
#include <string>
#include <QString>

/*!
 * \file FicheDonne.hpp
 * \brief Ce fichier d'en tête permet de définir la classe FicheDonnee. Elle sera utilisée dans le fichier FicheDonnee.cpp,mainwindows.cpp.
 * \author JUANEDA Nicolas BUFFAT Adrien
 * \version 3.4
 * \date 12 juin 2020 16:15
 */


using namespace std;

class FicheDonne : public QObject
{
    Q_OBJECT
public:
    FicheDonne();

    ~FicheDonne();

    string Nom() {
        return nom;
    };

    string Description() {
        return description;
    };

    string URL() {
        return url;
    };

    string Preparation() {
        return preparation;
    };

    string getcuisson() {
        return cuisson;
    };

    string gettotaltemps() {
        return totaltemps;
    };

    string getnbrpersonnes() {
        return nbrpersonnes;
    };

    string gettype() {
        return type;
    };

    int NbrIngredients() {
        return nbringredients;
    };

    string TablIngredients(size_t i) {
        return tabingredients[i];
    };

    int getnbrinstructions() {
        return nbrinstructions;
    };

    string gettabinstructions(size_t i) {
        return tabinstructions[i];
    };

private:

    unsigned int nbringredients = 0;
    string*tabingredients;

    unsigned int nbrinstructions;
    string*tabinstructions;

    string nom;

    string description;

    string url;

    string preparation;

    string cuisson;

    string totaltemps;

    string motscles;

    string nbrpersonnes;

    string type;

public slots:
    void choisir(QString d);
    void stockagerecette(QJsonObject *);

signals:
    void envoiejson(QJsonObject *);
    void DonneRecette();
};

#endif // FICHE_H
