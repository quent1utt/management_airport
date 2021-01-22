/**
* \file main.c
* \brief Gestion d'un aeroport.
* \author Quentin Letellier & Nicolas Felixine
* \date Janvier 2020
*/


#include <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include <time.h>

/**
* \struct place
* \brief Structure d'une place dans un vol.
*
*/
typedef struct
{
    /// Chaine du numero du siege
    char numero[4];
    struct place *suivant;
} place;


/**
* \struct date
*
* \brief Structure de la date de naissance du passager.
*/
typedef struct
{
    int jour;
    int mois;
    int annee;
} date;


/**
* \struct pays
*
* \brief Structure du type pays.
*/
typedef struct
{
    /// Valeur a 1 si c'est un pays de l'UE, 0 si Hors UE
    int testUE;
    /// Nom de la ville de destination
    char nom[40];
    /// Pays de la ville de destination
    char countrie[40];
} pays;


/**
 * \struct passager
 *
 * \brief La structure passager permet de regrouper toutes les informations liees au passager d'un vol
 */
typedef struct
{
    char nom[40];
    char prenom[40];
    date naissance;
    int numpasseport;
    pays destination;
    /// 1 = Prioritaire 0 = Non Prioritaire
    int priorite;
    /// Nombre de bagaqe du passager
    int bagage;
    ///Syntaxe : Initiales du passager + Numero de vol + chiffres aleatoires
    char numerobillet[12];
    /// 1 Lettre et 2 Chiffres
    char siege[3];
    int poidbagage[1];
    pays nationalite;
    ///permet de verifier que le passager est enregistre, qu'il est passe aux frontieres et a la securite
    int verifenregistrement;
    ///permet de verifier que le passager est embarque
    int verifembarquement;
} passager;


/**
 * \struct vol
 *
 * \brief La structure vol permet de regrouper toutes les informations liees au vol
 */
typedef struct
{
    /// Si libre = 0, le logiciel remplira les informations a cet endroit du tableau, sinon il cherchera une place libre dans le tableau de vol
    int libre;
    /// 2 Lettres et 4 Chiffres d'apres le standards des compagnies aeriennes
    char numerodevol[6];
    pays destination;
    int capacitevol;
    ///plus grosse capacite d'un avion (A380-800)
    passager tabpassager[854];
    /// permet de voir le nombre de passager enregistre
    int nbpassagerenregistre;
    /// permet de voir le nombre de passager embarque dans l'avion
    int nbpassagerembarque;
     /// permet de voir combien de billet ont ete achete
    int nbbilletachete;
    /// indique les sieges libre dans un vol
    place* tabsiegeslibres;
    /// indique les siege occupe dans un vol
    place* tabsiegespris;
    /// permet de compter le nombre de passagers prioritaires dans le vol
    int nbpassagerprioritaire;
    /// permet de compter le nombre de passagers non prioritaire dans le vol
    int nbpassagernonprioritaire;
} vol;

/// choix de la fonctionnalite du logiciel
int choixaction;
int nombredevol=0;
vol tabdesvols[50];
char volvide[6]= {'v','i','d','e','0','0'};


/**
 * \fn genererbillet
 *
 * \brief genere un numero unique pour le numero de billet
 */
void genererbillet (passager *pass, char vol[])
{
    char billet[12];
    int i;
    billet[0]=(*pass).nom[0];
    billet[1]=(*pass).prenom[0];
    billet[2]=vol[0];
    billet[3]=vol[1];
    billet[4]=vol[2];
    billet[5]=vol[3];
    billet[6]=vol[4];
    billet[7]=vol[5];

    int nombre_aleatoire = 0;
    /// initialisation de rand sur l'horloge de l'ordinateur
    srand(time(NULL));

    for(i=8; i<=12; i++)
    {
        /// chiffre aleatoire entre 0 et 9
        nombre_aleatoire = (rand()%10)+48;
        billet[i]=nombre_aleatoire;
    }
    billet[12]='\0';
    strcpy(pass->numerobillet, billet);
}


/**
 * \fn afficher
 *
 * \brief Affiche les sieges du vol en fonction de la capacite de l'avion
 */
void afficher(place l)
{
    place *aux;
    aux=&l;
    while(aux!=NULL)
    {
        printf("%s \n",aux->numero);
        aux=aux ->suivant;
    }
}

/**
 * \fn compter
 *
 * \brief
 */
int compter(place l)
{
    place *aux;
    aux=&l;
    int compteur=0;
    while (aux!=NULL)
    {
        compteur++;
        aux=aux ->suivant;
    }
    return(compteur);
}

/**
 * \fn addfirst
 *
 * \brief
 */
place *add_first(char* a,place *l)
{
    place *aux;
    aux=(place *) malloc(sizeof(place));
    strcpy(aux->numero,a);
    aux->suivant=l;
    return(aux);
}

/**
 * \fn supprimer
 *
 * \brief Supprime le siege qui vient d'etre choisi par le passager dans la liste
 */
place *supprimer(char* a,place *l)
{
    place* temp;
    if(l==NULL)
    {
        return l;
    }

    else
    {
        if(strcmp(l->numero,a)==0)
        {
            temp=l->suivant;
            free(l);
            printf("place reservee\n");
            return (temp);
        }

        else
        {
            return(add_first(l->numero,supprimer(a,l->suivant)));
        }
    }
}


/**
 * \fn existant
 *
 * \brief Verifie que le passager indique un siege correspondant a la liste de la fonction afficher
 */
int existant(char* a, place* l)
{

    place *aux=l;
    while (aux!=NULL)
    {
        if (strcmp(aux->numero, a)==0)
        {
            return 1;
        }
        else
        {
            aux=aux->suivant;
        }
    }
    return 0;
}

char* info(int indice, place* l)
{
    int i;
    place* aux=l;
    while (i< indice)
    {
        aux=aux->suivant;
        i++;

    }
    return aux->numero;
}


/**
 * \fn ajouterunvol
 *
 * \brief {Permet d'ajouter un vol dans le tableau des vols. Cette fonction ajoute un vol dans une case du tableau ou il n'y a pas encore de vol
 Rempli les informations de vol au fur et a mesure et cree le tableau de siege de l'avion en fonction de sa capacite}
 */
int ajouterunvol(int nombredevol, vol *tabdesvols)
{
    int indice, i;
    place *siege;
    char verifnumvol[6], chiffre1, chiffre2, lettre;
    printf("\nVous allez ajouter un vol\n\nSaisissez le numero que le vol possedera (2 lettres et 4 chiffres)\n");
    scanf("%s", verifnumvol);

    if(nombredevol==51)
    {
        printf("La memoire du logiciel est pleine. Veuillez faire decoller des vols\n");
        return(main());
    }

    for(indice=0; indice<=50; indice++)
    {
        if(strcmp(tabdesvols[indice].numerodevol,verifnumvol)==0)
        {
            printf("Desole, ce numero de vol existe deja\n");
            return(main());
        }
    }
    indice=0;
    while((tabdesvols[indice].libre==1)&&(indice<=50))
    {
        indice=indice+1;
    }
    if(tabdesvols[indice].libre==0)
    {
        tabdesvols[indice].libre=1;
        strcpy(tabdesvols[indice].numerodevol,verifnumvol);
        printf("Saisissez la destination du vol\n");
        scanf("%s", tabdesvols[indice].destination.nom);
        printf("Saisissez le pays de la destination\n");
        scanf("%s", tabdesvols[indice].destination.countrie);
        printf("Si la destination se trouve dans un pays de l'Union Europeene, tapez 1. Sinon, tapez 0\n");
        scanf("%d", &tabdesvols[indice].destination.testUE);
        printf("Saisissez la capacite de passager du vol\n");
        scanf("%d", &tabdesvols[indice].capacitevol);
        tabdesvols[indice].nbpassagerenregistre=0;
        tabdesvols[indice].nbpassagerembarque=0;
        tabdesvols[indice].nbbilletachete=0;
        tabdesvols[indice].nbpassagerprioritaire=0;
        tabdesvols[indice].nbpassagernonprioritaire=0;
    }

    tabdesvols[indice].tabsiegeslibres=(place*)malloc(sizeof(place));
    siege=tabdesvols[indice].tabsiegeslibres;
    chiffre1='0';
    chiffre2='1';
    lettre='A';
    siege->numero[0]=chiffre1;
    siege->numero[1]=chiffre2;
    siege->numero[2]=lettre;
    siege->numero[3]='\0';


    for (i=1; i<tabdesvols[indice].capacitevol; i++ )
    {
        siege->suivant=(place*)malloc(sizeof(place));
        siege=siege->suivant;

        lettre++;
        if(lettre=='J')
        {
            lettre='A';
            chiffre2++;
            if (chiffre2==':')
            {
                chiffre1++;
                chiffre2='0';
            }
        }
        siege->numero[0]=chiffre1;
        siege->numero[1]=chiffre2;
        siege->numero[2]=lettre;
        siege->numero[3]='\0';

    }
    siege->suivant=NULL;

    return(0);

}


/**
 * \fn ajouterpassager
 *
 * \brief {Permet d'acheter un billet sur un vol. Rempli les informations du passagers au fur et a mesure.
 Une verification du numero de vol est effectue au debut et indique egalement s'il y a encore des billets en vente
 Le billet est genere a la fin de la fonction est affiche dans le terminal}
 */
int ajouterpassager(int nombredevol, vol *tabdesvols)
{
    int i=0;
    int j=0;
    char choixvol[6];

    printf("\nSaisissez le numero de vol concerne\n");
    scanf("%s", choixvol);
    i=0;
    while(i<=nombredevol)
    {
        if(strcmp(tabdesvols[i].numerodevol,choixvol)!=0)
        {
            i=i+1;
        }
        else
        {
            if(tabdesvols[i].nbbilletachete==tabdesvols[i].capacitevol)
            {
                printf("L'avion est complet\n");
                return(0);
            }
            else
            {
                j=tabdesvols[i].nbbilletachete;
                printf("Vol trouve\nVeuillez saisir le nom du passager\n");
                scanf("%s", tabdesvols[i].tabpassager[j].nom);
                printf("Veuillez saisir le prenom du passager\n");
                scanf("%s", tabdesvols[i].tabpassager[j].prenom);
                genererbillet(&(tabdesvols[i].tabpassager[j]), tabdesvols[i].numerodevol);
                printf("Indiquer le jour de la date de naissance\n");
                scanf("%d", &tabdesvols[i].tabpassager[j].naissance.jour);
                printf("Indiquer le mois de la date de naissance\n");
                scanf("%d", &tabdesvols[i].tabpassager[j].naissance.mois);
                printf("Indiquer l annee de la date de naissance\n");
                scanf("%d", &tabdesvols[i].tabpassager[j].naissance.annee);
                printf("Entrer le numero de passeport\n");
                scanf("%d", &tabdesvols[i].tabpassager[j].numpasseport);
                printf("Le passager est-il prioritaire ? 0 = Non | 1 = Oui\n");
                scanf("%d", &tabdesvols[i].tabpassager[j].priorite);
                if(tabdesvols[i].tabpassager[j].priorite==1)
                {
                    tabdesvols[i].nbpassagerprioritaire=tabdesvols[i].nbpassagerprioritaire+1;
                }
                else
                {
                    tabdesvols[i].nbpassagernonprioritaire=tabdesvols[i].nbpassagernonprioritaire+1;
                }
                printf("De quelle nationalite est le passager ? \n");
                scanf("%s", tabdesvols[i].tabpassager[j].nationalite.nom);
                printf("Indiquez le pays de la nationalite\n");
                scanf("%s", tabdesvols[i].tabpassager[j].nationalite.countrie);
                printf("Si la nationalite du passager est dans l'Union Europeenne, entrez 1. Sinon entrez 0\n");
                scanf("%d", &tabdesvols[i].tabpassager[j].nationalite.testUE);

                tabdesvols[i].nbbilletachete=tabdesvols[i].nbbilletachete+1;
                tabdesvols[i].tabpassager[j].verifenregistrement=0;

                printf("Capacite de vol = %d\n", tabdesvols[i].capacitevol);
                printf("Il y a actuellement %d billet achete dans ce vol\n", tabdesvols[i].nbbilletachete);
                printf("\n****************************BILLET AIRPORT AIRLINE****************************\nNUMERO DE VOL : %s\nNOM : %s\nPRENOM : %s\nDATE DE NAISSANCE : %d/%d/%d\nPASSEPORT NUMERO : %d\nDESTINATION : %s\nNUMERO DE BILLET : %s\nPRIORITE : %d\n********************************************************", tabdesvols[i].numerodevol, tabdesvols[i].tabpassager[j].nom, tabdesvols[i].tabpassager[j].prenom, tabdesvols[i].tabpassager[j].naissance.jour, tabdesvols[i].tabpassager[j].naissance.mois, tabdesvols[i].tabpassager[j].naissance.annee, tabdesvols[i].tabpassager[j].numpasseport, tabdesvols[i].destination.nom,tabdesvols[i].tabpassager[j].numerobillet,tabdesvols[i].tabpassager[j].priorite);

                return(0);
            }
        }
    }
    if(strcmp(tabdesvols[i].numerodevol,choixvol)!=0)
    {
        printf("Le vol n'existe pas.");
        return(ajouterpassager(nombredevol, tabdesvols));
    }
    return(0);
}


/**
 * \fn enregistrer un passager
 *
 * \brief {Permet d'enregistrer un passager sur un vol. Cette fonctionne simule le passager qui est � l'aeroport et qui renseigne les informations de si�ge et de bagages.
 Une v�rification du num�ro de vol et du billet est effectu�.
 Nous consid�rons qu'une fois enregistr�, les passagers seront obligatoirement contr�l�s par les fronti�res et la s�curit�
 Cette fonction g�n�re le Boarding Pass et le ou les ticket(s) bagage(s) du passager
 Cette fonction propose de choisir le num�ro de si�ge et mis � jour la liste des si�ges disponibles pour les futures passagers.
 Cependant nous sommes conscients que la liste n'est pas mise � jour}
 */
int enregistrerpassager(int nombredevol, vol *tabdesvols)
{
    int i, j, k;
    char recherche[30];
    char recherche2[30];
    char siege[4];
    int trouve=0;
    int nbagage, reponse, indice;
    float tamp;
    char choixvol[6];

    printf("Sur quel vol souhaitez-vous enregistrer un passager ?\n");
    scanf("%s", choixvol);
    i=0;
    ///verification que le vol existe
    while(i<=nombredevol)
    {
        if(strcmp(tabdesvols[i].numerodevol,choixvol)!=0)
        {
            i=i+1;
        }
        else
        {
            if(tabdesvols[i].nbbilletachete==tabdesvols[i].nbpassagerenregistre)
            {
                printf("Tous les passagers ont ete enregistre\n");
                return(0);
            }
            else
            {
                for(i=0; i<=50; i++)
                {
                    if(strcmp(tabdesvols[i].numerodevol,choixvol)==0)
                    {
                        printf("Saisissez le nom du passager\n"); ///recherche du passager
                        scanf("%s", recherche);
                        printf("Saisissez le prenom du passager\n");
                        scanf("%s", recherche2);

                        for(j=0; j<=tabdesvols[i].nbbilletachete; j++)
                        {

                            if(((strcmp(tabdesvols[i].tabpassager[j].nom, recherche))==0)&&((strcmp(tabdesvols[i].tabpassager[j].prenom, recherche2))==0)&&(tabdesvols[i].tabpassager[j].verifenregistrement==0))
                            {
                                printf("Le passager a ete trouve\n");
                                trouve=1;
                                break;
                            }

                            if(((strcmp(tabdesvols[i].tabpassager[j].nom, recherche))==0)&&((strcmp(tabdesvols[i].tabpassager[j].prenom, recherche2))==0)&&(tabdesvols[i].tabpassager[j].verifenregistrement==1))
                            {
                                printf("Le passager a deja ete enregistre\n");
                                return(main());
                            }

                        }

                        if(trouve==0)
                        {
                            printf("Le passager n'a pas ete trouve\n");
                            return(enregistrerpassager(nombredevol, tabdesvols));
                        }


                        if(trouve==1)
                        {
                            tabdesvols[i].tabpassager[j].verifenregistrement=1;
                            tabdesvols[i].nbpassagerenregistre=tabdesvols[i].nbpassagerenregistre+1;
                            printf("Voulez vous choisir votre siege ? 1 = Oui 0 = Non\n");
                            scanf("%d", &reponse);
                            if (reponse==0) ///Attribution du siege aleatoire.Les lignes en commentaires font beuguer le programme
                                            ///les lignes random sont donc une alternative
                            {
                                siege[0]=(rand()%10)+48;;
                                siege[1]=(rand()%10)+48;
                                siege[2]=(rand()%26)+65;
                                siege[3]='\0';
                                //tamp= (float) rand() / RAND_MAX;
                                //indice=(int) tamp*compter(*(tabdesvols[i].tabsiegeslibres));
                                //strcpy(siege,info(indice, tabdesvols[i].tabsiegeslibres));
                                //supprimer(siege, tabdesvols[i].tabsiegeslibres);
                                //tabdesvols[i].tabsiegespris=add_first(siege, tabdesvols[i].tabsiegespris);

                            }
                            else
                            {
                                afficher(*(tabdesvols[i].tabsiegeslibres));
                                printf("Quel siege parmis ceux affiches souhaitez vous ? \n");
                                scanf("%s", siege);
                                siege[3]='\0';
                                while (existant(siege, tabdesvols[i].tabsiegeslibres)==0)
                                {
                                    printf("Ce siege n'existe pas, reessayez \n");
                                    scanf("%s", siege);
                                    siege[3]='\0';
                                } ///Les fonctions supprimer et aff fisrt ne mettent pas a jour la liste.
                                  ///Il peut donc arriver qu'un passager choisissent un siege deja occupe
                                //supprimer(siege, tabdesvols[i].tabsiegeslibres);
                                //tabdesvols[i].tabsiegespris=add_first(siege, tabdesvols[i].tabsiegespris);

                            }
                            printf("Votre siege est le %s\n", siege);
                            tabdesvols[i].tabpassager[j].siege[0]=siege[0];
                            tabdesvols[i].tabpassager[j].siege[1]=siege[1];
                            tabdesvols[i].tabpassager[j].siege[2]=siege[2];
                            tabdesvols[i].tabpassager[j].siege[3]=siege[3];


                            if((tabdesvols[i].tabpassager[j].priorite)==1) ///Gestion des bagages VIP
                            {
                                printf("Le passager porte la mention priority. Combien de bagage dispose le passager ?\n");
                                scanf("%d", &nbagage);
                                while(nbagage<0 || nbagage>2)
                                {
                                    printf("Desole, vous pouvez emporter deux bagages au maximum.\nCombien de bagage dispose le passager ?\n");
                                    scanf("%d", &nbagage);
                                }
                                if(nbagage==0)
                                {
                                    printf("Le passager n'a pas de bagage");
                                }
                                else
                                {
                                    for(k=0; k<nbagage; k++)
                                    {
                                        printf("Indiquez le poids du bagage\n"); ///Affichage des tickets bagages
                                        scanf("%d", &tabdesvols[i].tabpassager[j].poidbagage[k]);
                                        printf("\n****************************TICKET BAGAGE n� %d / %d AIRPORT AIRLINE****************************\nNOM : %s\nPRENOM : %s\nVOL NUMERO : %s\nDESTINATION : %s\nPOIDS BAGAGE : %d kg\n", k+1, nbagage, tabdesvols[i].tabpassager[j].nom, tabdesvols[i].tabpassager[j].prenom, tabdesvols[i].numerodevol, tabdesvols[i].destination.nom, tabdesvols[i].tabpassager[j].poidbagage[k]);
                                    }
                                }
                            }

                            if(tabdesvols[i].tabpassager[j].priorite==0) ///Gestion des bagages pour les non VIP
                            {
                                printf("Le passager ne porte pas la mention priority. Combien de bagage dispose le passager ?\n");
                                scanf("%d", &nbagage);
                                while(nbagage<0 || nbagage>1)
                                {
                                    printf("Desole, vous pouvez emporter un bagages au maximum.\nCombien de bagage dispose le passager ?\n");
                                    scanf("%d", &nbagage);
                                }
                                if(nbagage==0)
                                {
                                    printf("Le passager n'a pas de bagage");
                                }
                                else
                                {
                                    for(k=0; k<nbagage; k++)
                                    {
                                        printf("Indiquez le poids du bagage\n");
                                        scanf("%d", &tabdesvols[i].tabpassager[j].poidbagage[k]);
                                        printf("\n****************************TICKET BAGAGE n� 1 / 1 AIRPORT AIRLINE****************************\nNOM : %s\nPRENOM : %s\nVOL NUMERO : %s\nDESTINATION : %s\nPOIDS BAGAGE : %d kg\n", tabdesvols[i].tabpassager[j].nom, tabdesvols[i].tabpassager[j].prenom, tabdesvols[i].numerodevol, tabdesvols[i].destination.nom, tabdesvols[i].tabpassager[j].poidbagage[k]);
                                    }
                                }
                            }


                            tabdesvols[i].tabpassager[j].bagage=nbagage;
                            ///Affichage du boarding pass
                            printf("\n****************************BOARDING PASS AIRPORT AIRLINE****************************\nNUMERO DE BILLET : %s\nNOM : %s\nPRENOM : %s\nDESTINATION : %s\nPRIORITE : %d\nNUMERO DE SIEGE : %s\n********************************************************", tabdesvols[i].tabpassager[j].numerobillet, tabdesvols[i].tabpassager[j].nom, tabdesvols[i].tabpassager[j].prenom, tabdesvols[i].destination.nom, tabdesvols[i].tabpassager[j].priorite, tabdesvols[i].tabpassager[j].siege);

                            return(0);
                        }

                    }

                }

            }
        }
    }

    if(strcmp(tabdesvols[i].numerodevol,choixvol)!=0)
    {
        printf("Le vol n'existe pas.");
        return(0);
    }
    return(0);
}

/**
 * \fn embarquer
 *
 * \brief {Permet de simuler l'embarquement des passagers qui attendent et qui se sont enregistr�s.
 Les passagers prioritaire embarquent en premier dans l'ordre ou ils se sont enregistr� puis les passagers non prioritaires embarquent dans l'ordre ou il se sont enregistres.
La fonction embarquer embarque egalement tous les bagages des passagers
La fonction embarquer peut-etre appeler a n'importe quel moment lorsque des passagers viennent de s'enregistrer et attendent.}
 */
void embarquer(int nombredevol, vol *tabdesvols, char volconcerne[6], int positiontab)
{
    int i=0;
    int placeachete;
    int passagerenregistre;
    placeachete=tabdesvols[positiontab].nbbilletachete;
    passagerenregistre=tabdesvols[positiontab].nbpassagerenregistre;
    int compteurVIP=tabdesvols[positiontab].nbpassagerprioritaire;
    int compteurNONVIP=passagerenregistre-compteurVIP;

    printf("Vol concerne : %s\nNombre de place achete pour ce vol %d\nNombre de passager qui se sont enregistre %d\nNombre de prioritaire %d\nNombre de non prioritaire %d\n", volconcerne, placeachete, passagerenregistre, tabdesvols[positiontab].nbpassagerprioritaire, tabdesvols[positiontab].nbpassagernonprioritaire);

    if(passagerenregistre==0)
    {
        printf("Veuillez enregistrer les passagers\n");
        return(main());
    }

    else
    {
        while(i<passagerenregistre) ///Premiere vague d'embarquement pour les prioritaires
        {
            while(compteurVIP!=0) ///Le compteur indique combien de prioraire se sont enregistres
            {

                if((tabdesvols[positiontab].tabpassager[i].priorite==1)&&(tabdesvols[positiontab].tabpassager[i].verifenregistrement==1)&&(tabdesvols[positiontab].tabpassager[i].verifembarquement==1))
                {
                    compteurVIP=compteurVIP-1;
                    break;
                }

                if((tabdesvols[positiontab].tabpassager[i].priorite==1)&&(tabdesvols[positiontab].tabpassager[i].verifenregistrement==1)&&(tabdesvols[positiontab].tabpassager[i].verifembarquement!=1))
                { ///Processus d'embarquement et Prise en charge des bagages.
                    tabdesvols[positiontab].nbpassagerembarque=tabdesvols[positiontab].nbpassagerembarque+1; ///compteur pour indiquer combien de personne enregistre ont embarque
                    compteurVIP=compteurVIP-1; ///Compteur indiquant combien de personne prioritaire sont encore en attente
                    tabdesvols[positiontab].tabpassager[i].verifembarquement=1;
                    printf("\nNom et Prenom du passager %s %s\n", tabdesvols[positiontab].tabpassager[i].nom, tabdesvols[positiontab].tabpassager[i].prenom);
                    printf("Vous etes prioritaire. Vous embarquez donc dans les premiers. Le chargement de %d bagages est en cours.\nNous vous souhaitons un agreable voyage vers %s\n", tabdesvols[positiontab].tabpassager[i].bagage, tabdesvols[positiontab].destination.nom);

                }

                i=i+1;
            }
            i=i+1;

        }
        printf("Tous les passagers prioritaires enregistres ont ete embarque\n");

        i=0;
        while(i<passagerenregistre) ///Lorsque tous les prioraires ont embarque, c'est au tour des non prioritaires
        {
            while(compteurNONVIP!=0)
            {

                if((tabdesvols[positiontab].tabpassager[i].priorite==0)&&(tabdesvols[positiontab].tabpassager[i].verifenregistrement==1)&&(tabdesvols[positiontab].tabpassager[i].verifembarquement==1))
                {
                    compteurNONVIP=compteurNONVIP-1;
                    break;
                }

                if((tabdesvols[positiontab].tabpassager[i].priorite==0)&&(tabdesvols[positiontab].tabpassager[i].verifenregistrement==1)&&(tabdesvols[positiontab].tabpassager[i].verifembarquement!=1))
                { ///Processus d'embarquement et prise en charge des bagages
                    tabdesvols[positiontab].nbpassagerembarque=tabdesvols[positiontab].nbpassagerembarque+1;
                    compteurNONVIP=compteurNONVIP-1;
                    printf("\nNom et Prenom du passager %s %s\n", tabdesvols[positiontab].tabpassager[i].nom, tabdesvols[positiontab].tabpassager[i].prenom);
                    printf("Vous n'etes pas prioritaire. Vous n'avez donc pas embarquez dans les premiers. Le chargement de %d bagages est en cours.\nNous vous souhaitons un agreable voyage vers %s\n", tabdesvols[positiontab].tabpassager[i].bagage, tabdesvols[positiontab].destination.nom);
                    tabdesvols[positiontab].tabpassager[i].verifembarquement=1;
                }

                i=i+1;
            }
            i=i+1;


        }
        printf("Tous les passagers non prioritaires enregistres ont ete embarque\n");
    }

    return(main());
}

/**
 * \fn passagefrontiere
 *
 * \brief {Permet de simuler le passage aux frontieres. Une verification du numero de vol et de l'identite du passager est effectue.
La fonction indique s'il faut un visa en fonction de la nationalite et de la destination du passager.}
 */
int passagefrontiere(vol* tabdesvols, int nombredevol)
{
    int i,j;
    char recherche[30];
    char recherche2[30];
    char choixvol[6];
    int trouve=-1;

    printf("Sur quel vol se trouve le passager ?\n");
    scanf("%s", choixvol);
    i=0;

    while(i<=nombredevol) ///Verification si le vol existe
    {
        if(strcmp(tabdesvols[i].numerodevol,choixvol)!=0)
        {
            i=i+1;
        }

        else
        {
            for(i=0; i<=50; i++)
            {
                if(strcmp(tabdesvols[i].numerodevol,choixvol)==0)
                {
                    printf("Saisissez le nom du passager\n"); ///Verification si le passager existe
                    scanf("%s", recherche);
                    printf("Saisissez le prenom du passager\n");
                    scanf("%s", recherche2);

                    for(j=0; j<=tabdesvols[i].nbbilletachete; j++)
                    {
                        if(((strcmp(tabdesvols[i].tabpassager[j].nom, recherche))==0)&&((strcmp(tabdesvols[i].tabpassager[j].prenom, recherche2))==0))
                        {
                            printf("Le passager a ete trouve\n");

                            ///Affichage des informations de visa en fonction de la nationalite et de la destination
                            printf("Destination : %s\nNationalite : %s\n",tabdesvols[i].destination.nom, tabdesvols[i].tabpassager[j].nationalite.nom);
                            if (((tabdesvols[i].destination.testUE==1)&&(tabdesvols[i].tabpassager[j].nationalite.testUE==1))||(strcmp(tabdesvols[i].destination.countrie, tabdesvols[i].tabpassager[j].nationalite.countrie)==0))
                            {
                                printf("Il n'y a pas besoin de visa\n");
                                return(main());
                            }

                            else
                            {
                                printf("Il y a besoin de visa\n");
                                return(main());
                            }
                        }


                    }

                    if(((strcmp(tabdesvols[i].tabpassager[j].nom, recherche))!=0)&&((strcmp(tabdesvols[i].tabpassager[j].prenom, recherche2))!=0))

                    {
                        printf("Le passager n'a pas ete trouve\n");
                        printf("Passage frontiere impossible, une erreur est survenue\n");
                        return (main());
                    }

                }
            }
        }

    }

    if(strcmp(tabdesvols[i].numerodevol,choixvol)!=0)
    {
        printf("Le vol n'existe pas.");
        trouve=-1;
    }

}

/**
 * \fn checkpoint
 *
 * \brief {Permet de simuler le passage aux services de securite. Une verification du numero de vol et de l'identite du passager est effectue.
La fonction indique une petite liste des elements interdits en cabine. Le passager doit indiquer les objets qu'il d�pose.
La fonction indique s'il doit les enlever. Si l'objet est interdit le passager doit obligatoirement l'enleve sinon il est bloque � la securite.}
 */
int checkpoint(vol* tabdesvols, int nbvols)
{
    int i, j, k, l;
    int nombreverification;
    char objet[20];
    int reponse;
    char *objetinterdit[11]= {"aerosol","peinture","vernis","essence","alcool","briquet","batterie","gaz","arme","munition"};


    printf("**********INTERDIT EN CABINE***********\n"); ///Affichage liste
    for (i=0; i<11; i++)
    {
        printf(objetinterdit[i]);
        printf("\n");
    }
    printf("**********INTERDIT EN CABINE***********\n\n");

    int m,n;
    char recherche[30];
    char recherche2[30];
    char choixvol[6];
    int trouve=-1;

    printf("Sur quel vol se trouve le passager ?\n"); ///Verification si le vol existe
    scanf("%s", choixvol);
    m=0;

    while(m<=nombredevol)
    {
        if(strcmp(tabdesvols[m].numerodevol,choixvol)!=0)
        {
            m=m+1;
        }

        else
        {
            for(m=0; m<=50; m++)
            {
                if(strcmp(tabdesvols[m].numerodevol,choixvol)==0)
                {
                    printf("Saisissez le nom du passager\n"); ///Verification si le passager existe
                    scanf("%s", recherche);
                    printf("Saisissez le prenom du passager\n");
                    scanf("%s", recherche2);

                    for(n=0; n<=tabdesvols[m].nbbilletachete; n++)
                    {
                        if(((strcmp(tabdesvols[m].tabpassager[n].nom, recherche))==0)&&((strcmp(tabdesvols[m].tabpassager[n].prenom, recherche2))==0))
                        {
                            printf("Le passager a ete trouve\n");

                            printf("Combien d'objet possedez-vous dans votre valise ?\n");
                            scanf("%d", &nombreverification);

                            for(j=1; j<=nombreverification; j++) ///Controle des objets
                            {
                                printf("Indiquer l'un des objets que vous possedez dans votre valise\n");
                                scanf("%s", objet);

                                i=0;
                                while(i<10)
                                {
                                    if((strcmp(objet, objetinterdit[i]))==0)
                                    {
                                        printf("Veuiller enlever l'objet de la valise\n");
                                        printf("Avez-vous enlever l'objet ? 1= Oui 0 = Non\n");
                                        scanf("%d", &reponse);
                                        if(reponse==1)
                                        {
                                            i=i+1;
                                            break;
                                        }
                                        if(reponse==0)
                                        {
                                            while(reponse==0) ///Boucle qui force le passager a enlever l'objet interdit
                                            {
                                                printf("Veuiller enlever l'objet de la valise\n");
                                                printf("Avez-vous enlever l'objet ? 1 = Oui 2 = Non\n");
                                                scanf("%d", &reponse);
                                            }
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        i=i+1;
                                    }
                                }

                                if((strcmp(objet, objetinterdit[9]))!=0)
                                {
                                    printf("Objet autorise\n");
                                }

                            }

                            return(main());

                        }


                    }

                    if(((strcmp(tabdesvols[m].tabpassager[n].nom, recherche))!=0)&&((strcmp(tabdesvols[m].tabpassager[n].prenom, recherche2))!=0))

                    {
                        printf("Le passager n'a pas ete trouve\n");
                        printf("Passage security chechpoint impossible, une erreur est survenue\n");
                        return (main());
                    }

                }
            }
        }


    }

}

int main()
{
    choixaction=0;
    int i;
    char tarmac[6];
    int indice=0;

    int h, min, s;
    time_t now;

    time(&now); /// Renvoie l'heure actuelle

    struct tm *local = localtime(&now); /// Converti au format heure locale
    h = local->tm_hour;
    min = local->tm_min;
    s = local->tm_sec;

    ///Menu du logiciel
    printf("\n*******************************\nBienvenue dans AirportOnline, le logiciel qui gere les passagers de votre aeroport\n\nQue souhaitez-vous effectuer ?\n\n1)Ajouter un vol\n2)Ajouter un passager\n3)Enregistrer un passager\n4)Passage aux frontieres\n5)Security Checkpoint\n6)Embarquement\n7)Decollage\n8)Afficher les vols en cours et les statistiques\n9)Quitter le logiciel\n*******************************\n");
    scanf("%d", &choixaction);
    if(choixaction==1) ///Ajout d'un vol
    {
        ajouterunvol(nombredevol, tabdesvols);
        nombredevol=nombredevol+1;
        return(main());
    }

    if(choixaction==2) ///Achat billet
    {
        if(nombredevol==0)
        {
            printf("Il n'y a pas encore de vol.\n");
            return(main());
        }
        printf("\nVous allez ajouter un passager\n");
        ajouterpassager(nombredevol, tabdesvols);
        return(main());
    }

    if(choixaction==3) ///Enregistrement passager
    {
        if(nombredevol==0)
        {
            printf("Il n'y a pas encore de vol.\n");
            return(main());
        }
        printf("Vous aller enregistrer un passager.\n");
        enregistrerpassager(nombredevol, tabdesvols);
        return(main());
    }

    if(choixaction==4) ///Passage frontiere
    {
        if(nombredevol==0)
        {
            printf("Il n'y a pas encore de vol.\n");
            return(main());
        }
        passagefrontiere(tabdesvols, nombredevol);
        return(main());
    }

    if(choixaction==5) ///Passage securite
    {
        if(nombredevol==0)
        {
            printf("Il n'y a pas encore de vol.\n");
            return(main());
        }
        checkpoint(tabdesvols, nombredevol);
        return(main());
    }

    if(choixaction==6) ///Embarquement
    {
        if(nombredevol==0)
        {
            printf("Il n'y a pas encore de vol.\n");
            return(main());
        }
        printf("Quel est le vol que vous souhaitez embarquer ?\n");
        scanf("%s", tarmac);
        indice=0;
        while(indice<300) ///Verification si le vol existe
        {
            if(strcmp(tarmac,tabdesvols[indice].numerodevol)!=0)
            {
                indice=indice+1;
            }
            else
            {
                break;
            }
        }
        if(strcmp(tarmac,tabdesvols[indice].numerodevol)==0)
        {
            embarquer(nombredevol, &tabdesvols, tarmac, indice);
        }
        else
        {
            printf("Indiquer un numero de vol qui existe pour l embarquement\n");
        }
        return(main());
    }


    if(choixaction==7) ///Decollage
    {
        if(nombredevol==0)
        {
            printf("Il n'y a pas encore de vol.\n");
            return(main());
        }
        printf("Quel vol est pret au decollage ?\n");
        scanf("%s", tarmac);
        indice=0;
        int z=0;
        int enregistrement=0;
        int embarquement=0;
        int decollageforce;
        while(indice<nombredevol)
        {
            if(strcmp(tarmac,tabdesvols[indice].numerodevol)!=0)
            {
                indice=indice+1;
            }
            else
            {
                break;
            }
        }

        if(strcmp(tarmac,tabdesvols[indice].numerodevol)!=0)
        {
            printf("Indiquer un numero de vol qui existe pour le decollage\n");
        }


        for(z=0; z<tabdesvols[indice].nbbilletachete; z++) ///Permet de verifier le nombre de personne qui se sont enregistres parmi les billets achetes
        {
            if(tabdesvols[indice].tabpassager[z].verifenregistrement==1)
            {
                enregistrement=enregistrement+1;
            }
        }

        for(z=0; z<tabdesvols[indice].nbbilletachete; z++) ///Permet de verifier le nombre de personne qui ont embarque parmi les billets achetes
        {

            if(tabdesvols[indice].tabpassager[z].verifembarquement==1)
            {
                embarquement=embarquement+1;
            }
        }


        if((strcmp(tarmac,tabdesvols[indice].numerodevol)==0)&&((enregistrement==0)||(embarquement==0))) ///Decollage impossible si l'avion est vide
        {
            printf("L'avion est actuellement vide. Il ne peut pas decoller.\n");
            return(main());
        }



        if((strcmp(tarmac,tabdesvols[indice].numerodevol)==0)&&(enregistrement==tabdesvols[indice].nbpassagerembarque)&&(embarquement==tabdesvols[indice].nbpassagerenregistre))
        { ///Verification effectue, tous les passagers avec billets sont enregistres et embarques
            printf("Il est : %02d:%02d:%02d, le vol %s a decolle avec succes !\n", h, min, s, tarmac);
            nombredevol=nombredevol-1;
            tabdesvols[indice].libre=0;
            tabdesvols[indice].nbbilletachete=0;
            tabdesvols[indice].nbpassagerembarque=0;
            tabdesvols[indice].nbpassagernonprioritaire=0;
            tabdesvols[indice].nbpassagerprioritaire=0;
            tabdesvols[indice].capacitevol=0;
            tabdesvols[indice].nbpassagerenregistre=0;
            for(i=0; i<=tabdesvols[indice].nbbilletachete; i++)
            {
                tabdesvols[indice].tabpassager[i].verifembarquement=0;
                tabdesvols[indice].tabpassager[i].verifenregistrement=0;
            }
            strcpy(tabdesvols[indice].numerodevol, volvide);
        }

        if((strcmp(tarmac,tabdesvols[indice].numerodevol)==0)&&(enregistrement!=tabdesvols[indice].nbpassagerembarque)&&(embarquement==tabdesvols[indice].nbpassagerembarque))
        { ///Premier scenario : tous les billets achete ne se sont pas enregistre. L'avion peut tout de meme decoller. Des passagers peuvent finalement ne pas etre present pour raisons personnelles
            printf("Tous les passagers ne se sont pas enregistres. Veuillez verifier les enregistrements.\nSi vous voulez tout de meme faire decoller l'avion tapez 1 sinon tapez 0\n");
            scanf("%d", &decollageforce);
            if(decollageforce==1)
            {
                printf("Il est : %02d:%02d:%02d, le vol %s a decolle avec succes !\n", h, min, s, tarmac);
                nombredevol=nombredevol-1;
                tabdesvols[indice].libre=0;
                tabdesvols[indice].nbbilletachete=0;
                tabdesvols[indice].nbpassagerembarque=0;
                tabdesvols[indice].nbpassagernonprioritaire=0;
                tabdesvols[indice].nbpassagerprioritaire=0;
                tabdesvols[indice].capacitevol=0;
                tabdesvols[indice].nbpassagerenregistre=0;
                for(i=0; i<=tabdesvols[indice].nbbilletachete; i++)
                {
                    tabdesvols[indice].tabpassager[i].verifembarquement=0;
                    tabdesvols[indice].tabpassager[i].verifenregistrement=0;
                }
                strcpy(tabdesvols[indice].numerodevol, volvide);
            }
            else
            {
                return(main());
            }
        }

        if((strcmp(tarmac,tabdesvols[indice].numerodevol)==0)&&(enregistrement==tabdesvols[indice].nbpassagerenregistre)&&(embarquement!=tabdesvols[indice].nbpassagerenregistre))
        { ///Deuxieme scenario : les passagers enregistres ne sont pas tous embarque. Ils sont peut etre encore aux frontieres ou a la securite
            printf("Tous les passagers ne sont pas embarques. Veuillez verifier les embarquements.\nSi vous voulez tout de meme faire decoller l'avion tapez 1 sinon tapez 0\n");
            scanf("%d", &decollageforce);
            if(decollageforce==1)
            {
                printf("Il est : %02d:%02d:%02d, le vol %s a decolle avec succes !\n", h, min, s, tarmac);
                nombredevol=nombredevol-1;
                tabdesvols[indice].libre=0;
                tabdesvols[indice].nbbilletachete=0;
                tabdesvols[indice].nbpassagerembarque=0;
                tabdesvols[indice].nbpassagernonprioritaire=0;
                tabdesvols[indice].nbpassagerprioritaire=0;
                tabdesvols[indice].capacitevol=0;
                tabdesvols[indice].nbpassagerenregistre=0;
                for(i=0; i<=tabdesvols[indice].nbbilletachete; i++)
                {
                    tabdesvols[indice].tabpassager[i].verifembarquement=0;
                    tabdesvols[indice].tabpassager[i].verifenregistrement=0;
                }

                strcpy(tabdesvols[indice].numerodevol, volvide);
            }
            else
            {
                return(main());
            }
        }

        if((strcmp(tarmac,tabdesvols[indice].numerodevol)==0)&&(enregistrement!=tabdesvols[indice].nbpassagerembarque)&&(embarquement!=tabdesvols[indice].nbpassagerenregistre))
        { ///Troisieme scenario : parmi les billets achetes, tous les clients ne sont pas encore dans l'aeroport et tout le monde parmi les enregistres n'est pas encore embarque
            printf("Tous les passagers ne sont pas enregistres et embarques. Veuillez verifier les enregistrements et les embarquements.\nSi vous voulez tout de meme faire decoller l'avion tapez 1 sinon tapez 0\n");
            scanf("%d", &decollageforce);
            if(decollageforce==1)
            {
                printf("Il est : %02d:%02d:%02d, le vol %s a decolle avec succes !\n", h, min, s, tarmac);
                nombredevol=nombredevol-1;
                tabdesvols[indice].libre=0;
                tabdesvols[indice].nbbilletachete=0;
                tabdesvols[indice].nbpassagerembarque=0;
                tabdesvols[indice].nbpassagernonprioritaire=0;
                tabdesvols[indice].nbpassagerprioritaire=0;
                tabdesvols[indice].capacitevol=0;
                tabdesvols[indice].nbpassagerenregistre=0;
                for(i=0; i<=tabdesvols[indice].nbbilletachete; i++)
                {
                    tabdesvols[indice].tabpassager[i].verifembarquement=0;
                    tabdesvols[indice].tabpassager[i].verifenregistrement=0;
                }
                strcpy(tabdesvols[indice].numerodevol, volvide);
            }
            else
            {
                return(main());
            }
        }

        printf("\nNombre de vol au depart : %d", nombredevol);
        return(main());
    }

    if(choixaction==8) ///Affichage des informations des vols en cours
    {
        int placedispo;
        float statVIP;
        float statremplissage;
        i=0;
        if(nombredevol==0)
        {
            printf("Il n'y a pas encore de vol.\n");
            return(main());
        }
        else
        {
            printf("Actuellement il y a %d vols enregistre.\n\n", nombredevol);
            while(i<nombredevol)
            {
                if(tabdesvols[i].numerodevol!=0)
                {
                    placedispo=tabdesvols[i].capacitevol-tabdesvols[i].nbbilletachete;

                    if((tabdesvols[i].nbbilletachete==0))
                    {
                        printf(" Numero de vol : %s\nDestination : %s\nCapacite total : %d\nBillet Disponible : %d\nNombre de passager enregistre : %d\nNombre de passager embarque : %d\nPourcentage de passager prioritaire actuel : Indisponible pour le moment\nTaux de remplissage de l'avion actuel : Indisponible pour le moment\n", tabdesvols[i].numerodevol, tabdesvols[i].destination.nom, tabdesvols[i].capacitevol, placedispo, tabdesvols[i].nbpassagerenregistre, tabdesvols[i].nbpassagerembarque);

                    }
                    else
                    {   ///Calcul des statistiques correspondant aux bonus
                        statVIP=(((float)(tabdesvols[i].nbpassagerprioritaire)/(tabdesvols[i].nbbilletachete))*100);
                        statremplissage=((float)(tabdesvols[i].nbpassagerembarque)/(tabdesvols[i].capacitevol))*100;
                        printf(" Numero de vol : %s\nDestination : %s\nCapacite total : %d\nBillet Disponible : %d\nNombre de passager enregistre : %d\nNombre de passager embarque : %d\nPourcentage de passager prioritaire actuel : %4.2f %%\nTaux de remplissage de l'avion actuel : %4.2f %%\n", tabdesvols[i].numerodevol, tabdesvols[i].destination.nom, tabdesvols[i].capacitevol, placedispo, tabdesvols[i].nbpassagerenregistre, tabdesvols[i].nbpassagerembarque, statVIP, statremplissage);
                    }

                    i=i+1;
                }
                else
                {
                    i=i+1;
                }
            }
        }
        return(main());
    }

    if(choixaction==9)
    {
        printf("A bientot sur AirportOnline\n");

    }


    return 0;
}
