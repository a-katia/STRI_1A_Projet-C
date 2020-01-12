#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serveur.h"

#define ACTION_AJOUTE_UTILISATEUR 1
#define ACTION_MODIFIE_UTILISATEUR 2
#define ACTION_SUPPRIME_UTILISATEUR 3

//Structure utilisateur
typedef struct {
	char* nom; //obligatoire 
	char* prenom; //obligatoire
	char* mail; //obligatoire
	char* adressePostale; 
	char* numTel; 
	char* remarque;
	int age;
}utilisateur;

//HashMap entre deux strings
typedef struct{
	char* key;
	char* value;
}elementStringString;

typedef struct{
	int size;
	elementStringString elem[BUFSIZ];
}hashMapStringString;

void addToHashMapStringString(hashMapStringString* map, char* key, char* value){
	//map->elem[map->size].key = malloc(strlen(key) * sizeof(char));
	map->elem[map->size].key = key;
	//map->elem[map->size].value = malloc(strlen(value) * sizeof(char));
	map->elem[map->size].value = value;
	map->size = map-> size + 1;
}

char* getFromHashMapStringString(hashMapStringString* map, char* key){
	for(int i=0 ; i<map->size ; i++){
		if(map->elem[i].key == key){
			return map->elem[i].value;
		}
	}
}

//HashMap entre user et string
typedef struct{
	utilisateur* key;
	char* value;
}elementUserString;

typedef struct{
	int size;
	elementUserString elem[BUFSIZ];
}hashMapUserString;

void addToHashMapUserString(hashMapUserString* map, utilisateur* key, char* value){
	map->elem[map->size].key = key;
	map->elem[map->size].value = value;
	map->size = map-> size + 1;
}

char* getFromHashMapUserString(hashMapUserString* map, utilisateur* key){
	for(int i=0 ; i<map->size ; i++){
		if(map->elem[i].key == key){
			return map->elem[i].value;
		}
	}
}

//fonctions
hashMapUserString mapUtilisateurs = {.size = 0};

utilisateur* getUserWithNomPrenom(char* nom, char* prenom){
	printf("Entrée dans : getUserWithNomPrenom\n");
	for(int i=0 ; i<mapUtilisateurs.size ; i++){
		if(mapUtilisateurs.elem[i].key->nom == nom && mapUtilisateurs.elem[i].key->prenom == prenom){
			
			printf("Sortie de : getUserWithNomPrenom\n");
			return mapUtilisateurs.elem[i].key;
		}
	}
}

int getUserLineWithNomPrenom(char* nomParam, char* prenomParam){
	printf("Entrée dans : getUserLineWithNomPrenom\n");
	FILE* csv = fopen("mapUsers.csv", "r");
	char *nom, *prenom, *mail, *adressePostale, *numTel, *remarque, *age, *admin;
	int retour = 0;
	while(fscanf(csv, "%s,%s,%s,%s,%s,%s,%s,%s\n", nom, prenom, mail, adressePostale, numTel, remarque, age, admin) == 8){
		if(nom == nomParam && prenom == prenomParam){				
			fclose(csv);
			
			printf("Sortie de : getUserLineWithNomPrenom\n");
			return retour;
		}
		retour = ftell(csv);
	}

	fclose(csv);
	return 0;
}

int isUserAdmin(utilisateur user){
	if(getFromHashMapUserString(&mapUtilisateurs, &user) != "1"){
		return 0;
	}else{
		return 1;
	}
}

int extraitRequete(char *requete, hashMapStringString mapParameters){
	printf("Entrée dans : extraitRequete\n");

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
						//tant qu'on a pas d'et commerciale (marquant un nouveau parametre)
						value[iString] = requete[i+cpt];
                        iString++;
                        cpt++;
					}

                    value[iString] = '\0';
					iString = 0;

					printf("Extrait dans HashMap : key = %s | value = %s\n", key, value);

					addToHashMapStringString(&mapParameters, key, value);
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
            
			printf("ACTION = %s\n", getFromHashMapStringString(&mapParameters, "ACTION"));
            return 1;
		}
	}

    return 0;
}

int ajouteUtilisateur(hashMapStringString mapParameters, char* admin){
	printf("Entrée dans : ajouteUtilisateur\n");
	char *nom, *prenom, *mail, *adressePostale, *numTel, *remarque, *age;

	utilisateur newUtilisateur;

	if((nom = getFromHashMapStringString(&mapParameters, "nom")) == NULL ||
	(prenom = getFromHashMapStringString(&mapParameters, "prenom")) == NULL ||
	(mail = getFromHashMapStringString(&mapParameters, "mail")) == NULL){
		printf("Erreur, données manquantes pour ajouter un nouvel utilisateur.\n");
		return 0;
	}else{
		newUtilisateur.nom = nom;
		newUtilisateur.prenom = prenom;
		newUtilisateur.mail = mail;
	}

	if((adressePostale = getFromHashMapStringString(&mapParameters, "adressePostale")) == NULL){
		adressePostale = " ";
	}
	if((numTel = getFromHashMapStringString(&mapParameters, "numTel")) != NULL){
		numTel = " ";
	}
	if((remarque = getFromHashMapStringString(&mapParameters, "remarque")) != NULL){
		remarque = " ";
	}
	if((age = getFromHashMapStringString(&mapParameters, "age")) != NULL){
		age = " ";
	}
	newUtilisateur.adressePostale = adressePostale;
	newUtilisateur.numTel = numTel;
	newUtilisateur.remarque = remarque;
	newUtilisateur.age = atoi(age);

	addToHashMapUserString(&mapUtilisateurs, &newUtilisateur, "0");

	FILE* csv = fopen("mapUsers.csv", "w");
	fseek(csv, 0, SEEK_END); //on va a la fin du fichier

	fprintf(csv, "%s,%s,%s,%s,%s,%s,%s,%s\n", nom, prenom, mail, adressePostale, numTel, remarque, age, admin);
	fclose(csv);

	printf("Sortie de : ajouteUtilisateur\n");
	return 1;
}

int modifieUtilisateur(hashMapStringString mapParameters){
	printf("Entrée dans : modifieUtilisateur\n");
	char *nom, *prenom, *mail, *adressePostale, *numTel, *remarque, *age, *admin;

	if((nom = getFromHashMapStringString(&mapParameters, "nom")) == NULL ||
	(prenom = getFromHashMapStringString(&mapParameters, "prenom")) == NULL){
		printf("Erreur, données manquantes pour modifier un utilisateur.\n");
		return 0;
	}
	
	utilisateur* user = getUserWithNomPrenom(nom, prenom);

	if((adressePostale = getFromHashMapStringString(&mapParameters, "mail")) != NULL){
		user->mail = mail;
	}
	if((adressePostale = getFromHashMapStringString(&mapParameters, "adressePostale")) != NULL){
		user->adressePostale = adressePostale;
	}
	if((numTel = getFromHashMapStringString(&mapParameters, "numTel")) != NULL){
		user->numTel = numTel;
	}
	if((remarque = getFromHashMapStringString(&mapParameters, "remarque")) != NULL){
		user->remarque = remarque;
	}
	if((age = getFromHashMapStringString(&mapParameters, "age")) != NULL){
		user->age = atoi(age);
	}

	int pos = getUserLineWithNomPrenom(nom, prenom);
	FILE* csv = fopen("mapUsers.csv", "w");
	fseek(csv, pos + 1, SEEK_SET);
	fscanf(csv, "%s,%s,%s,%s,%s,%s,%s,%s\n", nom, prenom, mail, adressePostale, numTel, remarque, age, admin);
	fseek(csv, pos + 1, SEEK_SET);
	fprintf(csv, "%s,%s,%s,%s,%s,%s,%s,%s\n", nom, prenom, mail, adressePostale, numTel, remarque, age, admin);
	fclose(csv);

	printf("Sortie de : modifieUtilisateur\n");
	return 1;
}

int supprimeUtilisateur(hashMapStringString mapParameters){
	printf("Entrée dans : supprimeUtilisateur\n");
	char *nom, *prenom, *mail, *adressePostale, *numTel, *remarque, *age, *admin;

	if((nom = getFromHashMapStringString(&mapParameters, "nom")) == NULL ||
	(prenom = getFromHashMapStringString(&mapParameters, "prenom")) == NULL){
		printf("Erreur, données manquantes pour supprimer un utilisateur.\n");
		return 0;
	}
	
	utilisateur* user = getUserWithNomPrenom(nom, prenom);
	user = NULL;

	int pos = getUserLineWithNomPrenom(nom, prenom);
	FILE* csv = fopen("mapUsers.csv", "w");
	fseek(csv, pos + 1, SEEK_SET);
	while((char) fgetc(csv) != '\n'){
		fputs("", csv);
	}
	fclose(csv);

	printf("Sortie de : supprimeUtilisateur\n");
	return 1;
}

void aiguillageServeur(hashMapStringString mapParameters){
	printf("Entrée dans : aiguillageServeur\n");

	char* action;
	printf("ACTION = %s\n", getFromHashMapStringString(&mapParameters, "ACTION"));
	if((action = getFromHashMapStringString(&mapParameters, "ACTION")) != NULL){
		int actionCod = atoi(action);

		switch (actionCod){
		case ACTION_AJOUTE_UTILISATEUR:
			ajouteUtilisateur(mapParameters, "0");
			break;
		case ACTION_MODIFIE_UTILISATEUR:
			modifieUtilisateur(mapParameters);
			break;
		case ACTION_SUPPRIME_UTILISATEUR:
			supprimeUtilisateur(mapParameters);
			break;
		}
	}
	
	printf("Sortie de : aiguillageServeur\n");
}

//est ce que la requete est une requete GET
int isRequeteGet(char *requete){
	if(requete[0] == 'G' && requete[1] == 'E' && requete[2] == 'T'){
		return 1;
	}
	return 0;
}

void recupereString(char ligne[BUFSIZ], char string[BUFSIZ], int* start, char delimiteur){
	int i = 0;
	int cpt = *start;
	while(ligne[cpt] != delimiteur){
		string[i] = ligne[cpt];
		cpt++;
		i++;
	}
	cpt++;
	*start = cpt;
}

hashMapUserString initHashMapUserString(){
	printf("Entrée dans : initHashMapUserString\n");

	FILE* csv;
	if((csv = fopen("mapUsers.csv", "r"))){	
		char nom[BUFSIZ], prenom[BUFSIZ], mail[BUFSIZ], adressePostale[BUFSIZ], numTel[BUFSIZ], remarque[BUFSIZ], age[BUFSIZ], admin[BUFSIZ];
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
			recupereString(ligne, admin, &cpt, '\n');

			utilisateur user;
			user.nom = nom;
			user.prenom = prenom;
			user.mail = mail;
			user.adressePostale = adressePostale;
			user.numTel = numTel;
			user.remarque = remarque;
			user.age = atoi(age);
			addToHashMapUserString(&mapUtilisateurs, &user, admin);

			cptUser++;
		}
		printf("%d utilisateur récupéré.\n", cptUser);
		fclose(csv);
	}else{
		hashMapStringString mapParameters = {.size = 0};
		addToHashMapStringString(&mapParameters, "nom", "Terrier");
		addToHashMapStringString(&mapParameters, "prenom", "Florian");
		addToHashMapStringString(&mapParameters, "mail", "florian.leeroy@hotmail.com");
		addToHashMapStringString(&mapParameters, "adressePostale", "7 Chemin de Dardagna");
		addToHashMapStringString(&mapParameters, "numTel", "0619422408");
		addToHashMapStringString(&mapParameters, "remarque", " ");
		addToHashMapStringString(&mapParameters, "age", "21");

		ajouteUtilisateur(mapParameters, "1");
	}

	printf("Sortie de : initHashMapUserString\n");
}

//serveur sur localhost:13214
int main() {
	char *message = NULL;
	char *requeteExtrait = malloc(511*sizeof(char));
	hashMapStringString mapParameters = {.size = 0};

	mapUtilisateurs = initHashMapUserString();
	Initialisation();

	while(1) {
		AttenteClient();
		message = Reception();

		if(message != NULL) {
			printf("J'ai recu: %s\n", message);

			if(isRequeteGet(message) == 1 && extraitRequete(message, mapParameters) == 1){
				aiguillageServeur(mapParameters);
			}

			free(message);
		}

		TerminaisonClient();
	}

	free(requeteExtrait);

	return 0;
}
