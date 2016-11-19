using namespace std;

#include <iostream>

#define MAP
#define BUFFER_SIZE 1000
#define MAX_CONSOLE_LINES 20


#include "Catalog.h"
#include "SimpleRoute.h"
#include "ComposedRoute.h"


char buff[BUFFER_SIZE] = {'\0'};


void clearConsole()
{ // Permet d'effacer le contenu du terminal à defaut de pouvoir utiliser une autre lib...

    for (int i = 0; i < MAX_CONSOLE_LINES; ++i)
    {
        cout << endl;
    }
}


void typeToContinue()
{
    cout << "Appuyer sur ENTREE pour continuer..." << endl;
    cin.ignore(256, '\n');
}

char *trim(char *str)
{
    if (str == NULL) return NULL;

    size_t size = strlen(str);
    unsigned int first = 0;
    unsigned int last = (unsigned int) size;

    bool first_found = false;
    bool last_found = false;

    for (unsigned int i = 0; i < size && (!first_found || !last_found); ++i)
    {
        if (!first_found && str[i] != ' ')
        {
            first_found = true;
            first = i;
        }

        if (!last_found && str[size - i - 1] != ' ')
        {
            last_found = true;
            last = (unsigned int) size - i;
        }
    }

    size_t newSize = last - first;

    // On refuse les chaines vides
    if (!first_found || !last_found) return NULL;

    char *newStr = new char[newSize + 1];
    newStr[newSize] = '\0';

    for (unsigned int j = 0; j < newSize; ++j)
    {
        newStr[j] = str[j + first];
    }

    return newStr;
}


SimpleRoute *getSimpleRoute(char *string)
{
    if (string == NULL) return NULL;

    char *save_ptr;

    // On recupère le moyen de transport (avant le '(')
    char *transport = trim(strtok_r(string, "(", &save_ptr));

    // On recupère la ville de départ (avant le ',')
    //  Envoyer la valeur NULL permet d'utiliser la derniere chaine
    char *departure = trim(strtok_r(NULL, ",", &save_ptr));


    // On recupère la ville d'arrivée (avant le ')')
    //  Envoyer la valeur NULL permet d'utiliser la derniere chaine
    char *arrival = trim(strtok_r(NULL, ")", &save_ptr));


    // S'il manque un élément
    if (transport == NULL
        || departure == NULL
        || arrival == NULL)
    {
        // On supprime les autres chaines si elles existent
        if (transport != NULL) delete[] transport;
        if (departure != NULL) delete[] departure;
        if (arrival != NULL) delete[] arrival;

        return NULL;
    }

    SimpleRoute *simpleRoute = new SimpleRoute(departure, arrival, transport);

    delete[] transport;
    delete[] departure;
    delete[] arrival;

    return simpleRoute;
}

void addRoute(Catalog *catalog)
{
    clearConsole();
    cout << "#----------------------------------------------------------------------------------------------" << endl
         << "#\t Ajouter un trajet" << endl
         << "#\t" << endl
         << "#\t" << "\tAjoutez un trajet sous la forme suivante :" << endl
         << "#\t" << "\t  <Vehicule>(<Ville de départ>,<Ville d'arrivée>)" << endl
         << "#\t" << endl
         << "#\t" << "\tPour créer un trajet composé, séparez chaque trajet simple par \";\" " << endl
         << "#\t" << endl
         << "#\t" << endl
         << "#\t" << endl
         << "#\t" << endl
         << "#----------------------------------------------------------------------------------------------" << endl
         << "Saisisser le trajet : ";

    cin.getline(buff, BUFFER_SIZE);

    if (strpbrk(buff, ";") == NULL)
    { // Si c'est un trajet simple (pas de point virgule dans le buffer)

        SimpleRoute *route = getSimpleRoute(buff);

        if (route == NULL)
        { // Si une des chaines est vide

            cout << "Erreur lors de la saisie !" << endl;
            typeToContinue();
            return;
        }

        if (catalog->addRoute(route))
        {
            cout << "Le trajet suivant a bien été ajouté : ";
            route->display();
            cout << endl;
        } else
        {
            cerr << "Ce trajet existe déjà !" << endl;
            delete route;
        };
    } else
    { // Trajet composé
        char *save_ptr;
        SimpleRoute *simpleRoute = NULL;
        ComposedRoute *composedRoute = NULL;

        char *partial = strtok_r(buff, ";", &save_ptr);
        while (partial != NULL)
        {
            simpleRoute = getSimpleRoute(partial);

            if (simpleRoute == NULL)
            { // Si une des chaines est vide

                cout << "Erreur lors de la saisie !" << endl;
                if (composedRoute != NULL) delete composedRoute;
                typeToContinue();
                return;
            }

            if (composedRoute == NULL)
            { // Lors du premier loop, on instancie la route composée
                composedRoute = new ComposedRoute(simpleRoute);
            } else if (!composedRoute->addSimpleRoute(simpleRoute))
            { // Sinon erreur lors de l'ajout de la route
                cerr << "Erreur lors de l'insertion ! Vérifiez que les villes soient correctement rentrées" << endl;
                typeToContinue();

                delete composedRoute;
                delete simpleRoute;
                return;
            }

            partial = strtok_r(NULL, ";", &save_ptr);
        }


        if (catalog->addRoute(composedRoute))
        {
            cout << "Le trajet suivant a bien été ajouté : ";
            composedRoute->display();
            cout << endl;
        } else
        {
            cerr << "Ce trajet existe déjà !" << endl;
            delete composedRoute;
        };
    }

    typeToContinue();
}


void displayCatalog(Catalog *catalog)
{
    clearConsole();
    cout << "#----------------------------------------------------------------------------------------------" << endl
         << "#\t Voir le catalogue" << endl
         << "#\t" << endl;

    catalog->display();

    cout << "#----------------------------------------------------------------------------------------------" << endl;

    typeToContinue();
}

void searchRoute(Catalog *catalog, bool advance = false)
{
    clearConsole();
    cout << "#----------------------------------------------------------------------------------------------" << endl
         << "#\t Rechercher un voyage (" << (advance ? "Avancée" : "Simple") << ")" << endl
         << "#\t" << endl
         << "#\t" << "\tRechercher un voyage sous la forme suivante :" << endl
         << "#\t" << "\t  <Ville de départ>, <Ville d'arrivée>" << endl
         << "#\t" << endl
         << "#\t" << endl
         << "#\t" << endl
         << "#\t" << endl
         << "#\t" << endl
         << "#\t" << endl
         << "#----------------------------------------------------------------------------------------------" << endl
         << "Saisisser la recherche : ";

    cin.getline(buff, BUFFER_SIZE);

    char *save_ptr;

    // On recupère la ville de départ (avant le ',')
    //  Envoyer la valeur NULL permet d'utiliser la derniere chaine
    char *departure = trim(strtok_r(buff, ",", &save_ptr));


    // On recupère la ville d'arrivée (après le ',')
    //  Envoyer la valeur NULL permet d'utiliser la derniere chaine
    char *arrival = trim(strtok_r(NULL, "", &save_ptr));

    clearConsole();
    cout << "#----------------------------------------------------------------------------------------------" << endl
         << "#\t Résultat de la recherche " << (advance ? "avancée" : "simple") << endl
         << "#\t" << endl;

    if (advance)
    {
        catalog->advanceQuery(departure, arrival);
    } else
    {
        catalog->query(departure, arrival);
    }

    cout << "#----------------------------------------------------------------------------------------------" << endl;

    delete[] departure;
    delete[] arrival;

    typeToContinue();
}

void about()
{
    clearConsole();
    cout << "#----------------------------------------------------------------------------------------------"
         << endl
         << "#\t" << endl
         << "#\t" << endl
         << "#\tApplication : Ultra promo voyage" << endl
         << "#\tAuteurs : " << endl
         << "#\t" << "\tMarc-Antoine FERNANDES" << endl
         << "#\t" << "\tGuillaume MARCELIN" << endl
         << "#\t" << "\tLucas ONO" << endl
         << "#\t" << endl
         << "#\tCréé pour le TP C++ n.2 - INSA Lyon - 2016" << endl
         << "#\t" << endl
         << "#----------------------------------------------------------------------------------------------"
         << endl;

    typeToContinue();
}

int main()
{
    Catalog *catalog = new Catalog();


    for (;;)
    { // Tant que l'utilisateur ne veut pas quitter (à travers un break;)
        clearConsole();
        cout << "#----------------------------------------------------------------------------------------------"
             << endl
             << "#\t Ultra promo voyage" << endl
             << "#\t" << endl
             << "#\t" << "\t1. Ajouter un trajet au catalogue" << endl
             << "#\t" << "\t2. Consulter le catalogue" << endl
             << "#\t" << "\t3. Rechercher un parcours (simple)" << endl
             << "#\t" << "\t4. Rechercher un parcours (avancée)" << endl
             << "#\t" << endl
             << "#\t" << endl
             << "#\t" << "\ta. A propos" << endl
             << "#\t" << "\tq. Quitter l'application" << endl
             << "#----------------------------------------------------------------------------------------------"
             << endl
             << "Saisisser l'option voulue : ";
        cin.getline(buff, BUFFER_SIZE);

        // Si plus d'1 caractère on recommence
        if (strlen(buff) > 1) continue;

        if (buff[0] == 'q')
        { // S'il veut quitter, on quitte imediatement
            break;
        }

        switch (buff[0])
        {
            case '1':
                addRoute(catalog);
                break;
            case '2':
                displayCatalog(catalog);
                break;
            case '3':
                searchRoute(catalog);
                break;
            case '4':
                searchRoute(catalog, true);
                break;
            case 'a':
                about();
                break;
            default:
                break;
        }
    }


    delete catalog;
    return 0;
}