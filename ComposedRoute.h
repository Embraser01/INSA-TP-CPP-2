/*************************************************************************
                           ComposedRoute  -  Représente un trajet composé
                             -------------------
    début                : 14/11/2016
*************************************************************************/

#ifndef TP2_C_COMPOSEDROUTE_H
#define TP2_C_COMPOSEDROUTE_H

//------------------------------------------- Interfaces utilisées, Types, Constantes

#include "Route.h"
#include "ListRoute.h"

class ComposedRoute : public Route
{

//--------------------------------------- Méthodes publiques

public:

    virtual char *getDeparture() const;

    virtual char *getArrival() const;

    virtual void display();
    // Mode d'emploi : cette méthode permet d'afficher sur la sortie standard le trajet sous la forme suivante
    //                  Départ de <Gare départ> pour <Gare arrivée>,
    //                          le trajet desservira les gares suivantes <Gare n (Transport)>,...


    bool addSimpleRoute(const SimpleRoute *simpleRoute);

    //--------------------------------------- Redéfinition d'opérateurs


    //--------------------------------------- Constructeurs - destructeur

    virtual ~ComposedRoute();


//--------------------------------------- Méthodes protégées ou privées

protected:
private:

//--------------------------------------- Attributs et types protégés ou privés

protected:
private:
    ListRoute *routes; // Liste des routes qui composent ce trajet
};

#endif //TP2_C_COMPOSEDROUTE_H
