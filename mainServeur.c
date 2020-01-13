#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serveur.h"

/* Constantes permettant d'associer les variables à des valeurs */
#define ACTION_CONNEXION 1
#define ACTION_DECONNEXION 2
#define ACTION_AJOUTE_UTILISATEUR 3
#define ACTION_MODIFIE_UTILISATEUR 4
#define ACTION_SUPPRIME_UTILISATEUR 5

//Structure utilisateur
typedef struct {
	char* nom; // champ obligatoire 
	char* prenom; //champ obligatoire
	char* mail; //champ obligatoire
	char* adressePostale; 
	char* numTel; 
	char* remarque;
	int age;
	char* login; //obligatoire
	char* password; //obligatoire
}utilisateur;

//HashMap entre deux strings (ou table de hashage): structure de donnée entre deux strings permettant l'association clé-valeur */
typedef struct{
	char* key;
	char* value;
}elementStringString;

typedef struct{
	int size;
	elementStringString elem[BUFSIZ];
}hashMapStringString;

/* fonction pour ajouter à la hash map une valeur et sa clé */
void addToHashMapStringString(hashMapStringString* map, char* key, char* value){
	//allocation dynamique de la clé et de la valeur
	char* newKey = malloc(strlen(key) * sizeof(char));
	char* newValue = malloc(strlen(value) * sizeof(char));
	
	/* strcpy copie la chaîne pointée par key dans la chaîne pointée par newKey */
	strcpy(newKey, key);
	strcpy(newValue, value);

	map->elem[map->size].key = newKey;
	map->elem[map->size].value = newValue;

	//incrémentation de la Hash map aprés ajout de la valeur 
	map->size = map->size + 1;
}

/* fonction pour obtenir une valeur de la HashMap à partir de sa clé   */
char* getFromHashMapStringString(hashMapStringString* map, char* key){
	for(int i=0 ; i<map->size ; i++){
		/* strcmp va comparer les la chaîne à l'indice i avec la clé */
		if(strcmp(map->elem[i].key,key) == 0){
			return map->elem[i].value;
		}
	}
	return NULL;// retourne nul si les deux chaînes sont égales
}

//HashMap entre user et string: clé utilisateur qui permet d'avoir une valeur String 
typedef struct{
	utilisateur* key;
	char* value;
}elementUserString;

typedef struct{
	int size; //nombre d'éléments de la structure 
	elementUserString elem[BUFSIZ];
}hashMapUserString;

hashMapUserString mapUtilisateurs; // creation de mapUtilisateur de type hashMapUserString

/* ajoute un utilisateur à la HashMap des utilisateurs du serveur. 
** Cette fonction consiste à faire des allocation dynamiques pour les différents champs obligatoire et non obligatoire, 
 avec en plus le login et le mot de passe de l'utilisateur */
void addToHashMapUserString(hashMapUserString* map, utilisateur key, char* value){
	utilisateur* newKey = malloc(sizeof(key));
	char* newValue = malloc(strlen(value) * sizeof(char));
	newKey->nom = malloc(strlen(key.nom) * sizeof(char));
	strcpy(newKey->nom,key.nom);
	newKey->prenom = malloc(strlen(key.prenom) * sizeof(char));
	strcpy(newKey->prenom,key.prenom);
	newKey->mail = malloc(strlen(key.mail) * sizeof(char));
	strcpy(newKey->mail,key.mail);
	newKey->adressePostale = malloc(strlen(key.adressePostale) * sizeof(char));
	strcpy(newKey->adressePostale,key.adressePostale);
	newKey->numTel = malloc(strlen(key.numTel) * sizeof(char));
	strcpy(newKey->numTel,key.numTel);
	newKey->remarque = malloc(strlen(key.remarque) * sizeof(char));
	strcpy(newKey->remarque,key.remarque);
	newKey->age = key.age;
	strcpy(newValue, value);
	newKey->login = malloc(strlen(key.login) * sizeof(char));
	strcpy(newKey->login,key.login);
	newKey->password = malloc(strlen(key.password) * sizeof(char));
	strcpy(newKey->password,key.password);

	map->elem[map->size].key = newKey;
	map->elem[map->size].value = newValue;
	map->size = map->size + 1;
	
}
/* fonction pour obtenir de la HashMap la valeur d'un utilisateur à partir de sa clé */
char* getFromHashMapUserString(hashMapUserString* map, utilisateur* key){
	for(int i=0 ; i<map->size ; i++){
		if(map->elem[i].key == key){
			return map->elem[i].value;
		}
	}
}

/* Cette fonction permet de parcourir une ligne de hashMap en partant d'un indice start jusqu'au délimiteur pour récuperer une chaine de caratères */
void recupereString(char ligne[BUFSIZ], char string[BUFSIZ], int* start, char delimiteur){
	int i = 0;
	int cpt = *start;
	while(ligne[cpt] != delimiteur){
		string[i] = ligne[cpt];
		cpt++;
		i++;
	}
	string[i] = '\0';
	cpt++;
	*start = cpt;
}

/* Fonction de type utilisateur prenant en paramètres le nom et le prenom et nous retourne l'utilisateur */
utilisateur* getUserWithNomPrenom(hashMapUserString* map, char* nom, char* prenom){
	printf("     Entrée dans : getUserWithNomPrenom\n");

	for(int i=0 ; i<map->size ; i++){
		if(strcmp(map->elem[i].key->nom,nom) == 0 && strcmp(map->elem[i].key->prenom,prenom) == 0){
			
			printf("     Sortie de : getUserWithNomPrenom\n");
			return map->elem[i].key;
		}
	}
	return NULL;
}

/* Recuperer un utlilisateur avec son login */
utilisateur* getUserWithLogin(hashMapUserString* map, char* login){
	printf("     Entrée dans : getUserWithLogin\n");

	for(int i=0 ; i<map->size ; i++){
		if(strcmp(map->elem[i].key->login,login) == 0){
			
			printf("     Sortie de : getUserWithLogin\n");
			return map->elem[i].key;
		}
	}
	return NULL;
}

/* retourne la position du curseur pour être au debut de la ligne de cet utilisateur dans le fichier csv */
int getUserLineWithNomPrenom(char* nomParam, char* prenomParam){
	printf("     Entrée dans : getUserLineWithNomPrenom\n");
	
	//ouverture du fichier en lecture
	FILE* csv = fopen("mapUsers.csv", "r");
	char nom[BUFSIZ], prenom[BUFSIZ], mail[BUFSIZ], adressePostale[BUFSIZ], numTel[BUFSIZ], remarque[BUFSIZ], age[BUFSIZ], admin[BUFSIZ];
	char ligne[BUFSIZ];
	int retour = 0;
	while(fgets(ligne, BUFSIZ, csv) != NULL){
		int cpt = 0;
		recupereString(ligne, nom, &cpt, ',');
		recupereString(ligne, prenom, &cpt, ',');
		while(ligne[cpt] != '\n'){
			cpt++;
		}
		if(strcmp(nom,nomParam) == 0 && strcmp(prenom,prenomParam) == 0){				
			fclose(csv);
			
			printf("     Sortie de : getUserLineWithNomPrenom\n");
			return retour;
		}
		/* la fonction ftell permet de connaitre la position du pointeur de fichier csv */
		retour = ftell(csv);
	}

	fclose(csv);
	return -1;
}
/*fonction pour savoir si l'utilisateur est administrateur */
int isUserAdmin(utilisateur* user){
	if(strcmp(getFromHashMapUserString(&mapUtilisateurs, user),"1") != 0){
		return 0;
	}else{
		return 1;
	}
}

/* fonction pour savoir si le mot de passe est valide. En faisant appel à la fonction getUserWithLogin,  
strcmp compare le mot de passe passé en paramètre avec le mot de passe de l'utilisateur. 
Si ils sont égaux, elle retourne 1 sinon elle retourne 0*/
int isMotDePasseValide(char* login, char* password){
	utilisateur* user = getUserWithLogin(&mapUtilisateurs, login);
	if(strcmp(password, user->password) == 0){
		return 1;
	}else{
		return 0;
	}
}

int extraitRequete(char *requete, hashMapStringString* mapParameters){
	printf("     Entrée dans : extraitRequete\n");

	int i, cpt, iString, fin;
	int taille = strlen(requete);
	char key[BUFSIZ], value[BUFSIZ];

	for(i=0 ; i<taille-1 && requete[i+1] != '\n' ; i++){
		if(requete[i] == '/'){
			cpt = 1;
            fin = 0;
			iString = 0;
            
			while(fin == 0){
				//tant qu'on a pas d'espace (marquant la fin de la requete)
				if(requete[i+cpt] == '='){
					//si on a un egal (marquant la valeur du parametre)
                    key[iString] = '\0';
					iString = 0;
					cpt++;

					while(requete[i+cpt] != '&' && requete[i+cpt] != ' '){
						//tant qu'on a pas de et commerciale (marquant un nouveau parametre)
						value[iString] = requete[i+cpt];
                        iString++;
                        cpt++;
					}

                    value[iString] = '\0';
					iString = 0;

					printf("Extrait dans HashMap : key = %s // value = %s\n", key, value);

					addToHashMapStringString(mapParameters, key, value);
				}else{
					key[iString] = requete[i+cpt];
					iString++;
				}
                
                if(requete[i+cpt] == ' '){	
                    fin = 1;
                }else{
                    cpt++;
                }
			}
            return 1;
		}
	}

    return 0;
}

/*Ajout d'un utilisateur en utilisant les informations de la requête */
int ajouteUtilisateur(hashMapStringString mapParameters, char* admin){
	printf("     Entrée dans : ajouteUtilisateur\n");
	char *nom, *prenom, *mail, *adressePostale, *numTel, *remarque, *age, *login, *password;

	utilisateur newUtilisateur;
	
	/*Si les informations obligatoires (nom, prenom, adresse mail, login et mot de passe) sont égales à NULL, 
	la fonction affiche un message d'erreur*/
	if((nom = getFromHashMapStringString(&mapParameters, "nom")) == NULL ||
	(prenom = getFromHashMapStringString(&mapParameters, "prenom")) == NULL ||
	(mail = getFromHashMapStringString(&mapParameters, "mail")) == NULL ||
	(login = getFromHashMapStringString(&mapParameters, "login")) == NULL ||
	(password = getFromHashMapStringString(&mapParameters, "password")) == NULL){
		printf("Erreur, données manquantes pour ajouter un nouvel utilisateur.\n");
		return 0;
	}else{
		newUtilisateur.nom = nom;
		newUtilisateur.prenom = prenom;
		newUtilisateur.mail = mail;
		newUtilisateur.login = login;
		newUtilisateur.password = password;
	}
	/*Vérifications et affichage des messages d'erreur*/
	/*si la fonction trouve le nom et prenom ou le login de l'utilisateur dans la hashMap, un message est retourné pour indiquer que l'utilisateur existe déja  */
	utilisateur* user = getUserWithNomPrenom(&mapUtilisateurs, nom, prenom);
	if(user != NULL){
		printf("Erreur, cet utilisateur existe deja.\n");
		return 0;
	}
	user = getUserWithLogin(&mapUtilisateurs, login);
	if(user != NULL){
		printf("Erreur, cet utilisateur existe deja.\n");
		return 0;
	}
	/*Si il n'y a pas d'adresse postale, mettre un espace à la place (un vide)*/
	if((adressePostale = getFromHashMapStringString(&mapParameters, "adressePostale")) == NULL){
		adressePostale = " ";
	}
	/*Si il n'y a pas de numéro de téléphone, mettre un espace à la place (un vide)*/
	newUtilisateur.adressePostale = adressePostale;
	if((numTel = getFromHashMapStringString(&mapParameters, "numTel")) == NULL){
		numTel = " ";
	}
	newUtilisateur.numTel = numTel;
	if((remarque = getFromHashMapStringString(&mapParameters, "remarque")) == NULL){
		remarque = " ";
	}
	newUtilisateur.remarque = remarque;
	/*Si il n'y a pas l'age de l'utilisateur,le mettre à -1 par default*/
	if((age = getFromHashMapStringString(&mapParameters, "age")) == NULL){
		age = "-1";
	}
	newUtilisateur.age = atoi(age); // convertir une chaine de caractère en int

	addToHashMapUserString(&mapUtilisateurs, newUtilisateur, "0");

	FILE* csv;
	if(strcmp(admin,"1") == 0){
		csv = fopen("mapUsers.csv", "w"); // création du fichier csv en écriture seule 
	}else{
		csv = fopen("mapUsers.csv", "r+"); // ouverture du fichier en lecture et ecriture sans écraser le fichier
	}
	fseek(csv, 0, SEEK_END); //on va a la fin du fichier
	fprintf(csv, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", nom, prenom, mail, adressePostale, numTel, remarque, age, login, password, admin);
	fclose(csv);// fermeture du fichier 

	printf("     Sortie de : ajouteUtilisateur\n");
	return 1;
}
/*fonction permettant de modifier les onformations d'un utilisateur*/
int modifieUtilisateur(hashMapStringString mapParameters){
	printf("     Entrée dans : modifieUtilisateur\n");
	char *nom, *prenom, *mail, *adressePostale, *numTel, *remarque, *age, *admin, *login, *password;
	
	/*Si le nom et prenom de l'utilisateur manquent, alors des modifications ne peuvent pas être apportées */
	if((nom = getFromHashMapStringString(&mapParameters, "nom")) == NULL ||
	(prenom = getFromHashMapStringString(&mapParameters, "prenom")) == NULL){
		printf("Erreur, données manquantes pour modifier un utilisateur.\n");
		return 0;
	}
	/*Vérifier que l'utilisateur existe bien avant toute modification */
	utilisateur* user = getUserWithNomPrenom(&mapUtilisateurs ,nom, prenom);
	if(user == NULL){
		printf("Erreur, utilisateur introuvable.\n");
		return 0;
	}
	admin = getFromHashMapUserString(&mapUtilisateurs, user);
	// modification des informations du contact
	if((mail = getFromHashMapStringString(&mapParameters, "mail")) != NULL){
		user->mail = mail;
	}else{
		mail = user->mail;
	}
	if((adressePostale = getFromHashMapStringString(&mapParameters, "adressePostale")) != NULL){
		user->adressePostale = adressePostale;
	}else{
		adressePostale = user->adressePostale;
	}
	if((numTel = getFromHashMapStringString(&mapParameters, "numTel")) != NULL){
		user->numTel = numTel;
	}else{
		numTel = user->numTel;
	}
	if((remarque = getFromHashMapStringString(&mapParameters, "remarque")) != NULL){
		user->remarque = remarque;
	}else{
		remarque = user->remarque;
	}
	if((age = getFromHashMapStringString(&mapParameters, "age")) != NULL){
		user->age = atoi(age);
	}else{
		sprintf(age,"%d",user->age);
	}
	if((login = getFromHashMapStringString(&mapParameters, "login")) != NULL){
		user->login = login;
	}else{
		login = user->login;
	}
	if((password = getFromHashMapStringString(&mapParameters, "password")) != NULL){
		user->password = password;
	}else{
		password = user->password;
	}

	int pos = getUserLineWithNomPrenom(nom, prenom);
	int cpt = 0, copie = 1;

	FILE* csv = fopen("mapUsers.csv", "r");
    FILE* temp = fopen("temp.csv", "w"); // si on veut modifier une ligne au milieu du fichier, on réécrit le fichier entier dans un autre fichier temporaire
	char c;

    while((c = getc(csv)) != EOF){
		cpt++;
		if (cpt == pos+1)
			copie = 0;

		if(copie == 1){
			putc(c, temp);
		}else if(copie == 0){
			fprintf(temp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", nom, prenom, mail, adressePostale, numTel, remarque, age, login, password, admin);
			copie = -1;
		}

		if(c == '\n')
			copie = 1;
    }
    // fermeture des fichiers ouverts
    fclose(csv);
    fclose(temp);
    remove("mapUsers.csv");
    rename("temp.csv", "mapUsers.csv");

	printf("     Sortie de : modifieUtilisateur\n");
	return 1;
}

int supprimeUtilisateur(hashMapStringString mapParameters){
	printf("     Entrée dans : supprimeUtilisateur\n");
	char *nom, *prenom;

	if((nom = getFromHashMapStringString(&mapParameters, "nom")) == NULL ||
	(prenom = getFromHashMapStringString(&mapParameters, "prenom")) == NULL){
		printf("Erreur, données manquantes pour supprimer un utilisateur.\n");
		return 0;
	}
	
	utilisateur* user = getUserWithNomPrenom(&mapUtilisateurs, nom, prenom);
	user = NULL;

	int pos = getUserLineWithNomPrenom(nom, prenom);
	int cpt = 0, copie = 1;

	FILE* csv = fopen("mapUsers.csv", "r");
    FILE* temp = fopen("temp.csv", "w");
	char c;

    while((c = getc(csv)) != EOF){
		cpt++;
		if (cpt == pos+1)
			copie = 0;

		if(copie == 1){
			putc(c, temp);
		}

		if(c == '\n')
			copie = 1;
    }
    fclose(csv);
    fclose(temp);
    remove("mapUsers.csv");
    rename("temp.csv", "mapUsers.csv");


	printf("     Sortie de : supprimeUtilisateur\n");
	return 1;
}

/*Fonction pointant vers une fonction à laquelle l'utiisateur souhaite accéder*/
int aiguillageServeur(hashMapStringString mapParameters, utilisateur* userLogged){
	printf("     Entrée dans : aiguillageServeur\n");

	char* action;
	printf("ACTION = %s\n", getFromHashMapStringString(&mapParameters, "ACTION"));
	if((action = getFromHashMapStringString(&mapParameters, "ACTION")) != NULL){
		int actionCod = atoi(action);
		//choix de l'action
		switch (actionCod){
		case ACTION_DECONNEXION:
			return 0;
			break;
		case ACTION_AJOUTE_UTILISATEUR:
			if(isUserAdmin(userLogged) == 1){
				ajouteUtilisateur(mapParameters, "0");
			}
			break;
		case ACTION_MODIFIE_UTILISATEUR:
			if(isUserAdmin(userLogged) == 1){
				modifieUtilisateur(mapParameters);
			}
			break;
		case ACTION_SUPPRIME_UTILISATEUR:
			if(isUserAdmin(userLogged) == 1){
				supprimeUtilisateur(mapParameters);
			}
			break;
		}
	}
	
	printf("     Sortie de : aiguillageServeur\n");
	return 1;
}

//est ce que la requete est une requete GET
int isRequeteGet(char *requete){
	//Comparaison des caractères 
	if(requete[0] == 'G' && requete[1] == 'E' && requete[2] == 'T'){
		return 1;
	}
	return 0;
}

/*Initialisation desutilisateurs au démarrage du serveur*/
void initHashMapUserString(){
	printf("     Entrée dans : initHashMapUserString\n");

	mapUtilisateurs.size = 0;

	FILE* csv;
	if((csv = fopen("mapUsers.csv", "r"))){	
		char nom[BUFSIZ], prenom[BUFSIZ], mail[BUFSIZ], adressePostale[BUFSIZ], numTel[BUFSIZ], remarque[BUFSIZ], age[BUFSIZ], admin[BUFSIZ], login[BUFSIZ], password[BUFSIZ];
		char ligne[BUFSIZ];
		int cptUser = 0;
		while(fgets(ligne, BUFSIZ, csv) != NULL){
			int cpt = 0;
			recupereString(ligne, nom, &cpt, ',');
			recupereString(ligne, prenom, &cpt, ',');
			recupereString(ligne, mail, &cpt, ',');
			recupereString(ligne, adressePostale, &cpt, ',');
			recupereString(ligne, numTel, &cpt, ',');
			recupereString(ligne, remarque, &cpt, ',');
			recupereString(ligne, age, &cpt, ',');
			recupereString(ligne, login, &cpt, ',');
			recupereString(ligne, password, &cpt, ',');
			recupereString(ligne, admin, &cpt, '\n');

			utilisateur user;
			user.nom = nom;
			user.prenom = prenom;
			user.mail = mail;
			user.adressePostale = adressePostale;
			user.numTel = numTel;
			user.remarque = remarque;
			user.age = atoi(age);
			user.login = login;
			user.password = password;
			addToHashMapUserString(&mapUtilisateurs, user, admin);

			cptUser++;
		}
		printf("%d utilisateur récupéré.\n", cptUser);
		fclose(csv);
	}else{
		hashMapStringString mapParameters = {.size = 0};
		addToHashMapStringString(&mapParameters, "nom", "Terrier");
		addToHashMapStringString(&mapParameters, "prenom", "Florian");
		addToHashMapStringString(&mapParameters, "mail", "florian.leeroy@hotmail.com");
		addToHashMapStringString(&mapParameters, "adressePostale", "7 Chemin du Semalou");
		addToHashMapStringString(&mapParameters, "numTel", "0619244208");
		addToHashMapStringString(&mapParameters, "remarque", " ");
		addToHashMapStringString(&mapParameters, "age", "21");
		addToHashMapStringString(&mapParameters, "login", "floliroy");
		addToHashMapStringString(&mapParameters, "password", "stri");

		ajouteUtilisateur(mapParameters, "1");
	}

	printf("     Sortie de : initHashMapUserString\n");
}

//serveur sur localhost:13214
int main() {
	char *message = NULL;
	utilisateur* userLogged;
	int logged = 0;
	int retour;

	initHashMapUserString();
	Initialisation();

	while(1) {
		AttenteClient();
		message = Reception();
		hashMapStringString mapParameters = {.size = 0};

		if(message != NULL && isRequeteGet(message) == 1 && extraitRequete(message, &mapParameters) == 1) {
			printf("J'ai recu: %s\n", message);

			if(logged == 1){
				retour = aiguillageServeur(mapParameters, userLogged);
				if(retour == 0){
					printf("Deconnexion du login: \"%s\".\n", userLogged->login);
					logged = 0;
					free(userLogged);
				}
			}else{
				printf("Attente de connexion...\n");
				char* login = getFromHashMapStringString(&mapParameters, "login");
				char* password = getFromHashMapStringString(&mapParameters, "password");
				char* action = getFromHashMapStringString(&mapParameters, "ACTION");
				if(login != NULL && password != NULL && action != NULL &&
				strcmp(action,"1") == 0 && isMotDePasseValide(login, password) == 1){
					userLogged = malloc(sizeof(utilisateur));
					userLogged = getUserWithLogin(&mapUtilisateurs, login);
					logged = 1;
					printf("Connexion réussie avec le login: \"%s\".\n", login);
				}
			}		

			free(message);
		}

		TerminaisonClient();
		//logged = 0;
		//free(userLogged);
	}

	return 0;
}
