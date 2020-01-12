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
	char* newKey = malloc(strlen(key) * sizeof(char));
	char* newValue = malloc(strlen(value) * sizeof(char));
	strcpy(newKey, key);
	strcpy(newValue, value);

	map->elem[map->size].key = newKey;
	map->elem[map->size].value = newValue;

	map->size = map->size + 1;
}

char* getFromHashMapStringString(hashMapStringString* map, char* key){
	for(int i=0 ; i<map->size ; i++){
		if(strcmp(map->elem[i].key,key) == 0){
			return map->elem[i].value;
		}
	}
	return NULL;
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

hashMapUserString mapUtilisateurs;

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

	map->elem[map->size].key = newKey;
	map->elem[map->size].value = newValue;
	map->size = map->size + 1;
	
}

char* getFromHashMapUserString(hashMapUserString* map, utilisateur* key){
	for(int i=0 ; i<map->size ; i++){
		if(map->elem[i].key == key){
			return map->elem[i].value;
		}
	}
}

//fonctions
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

utilisateur* getUserWithNomPrenom(hashMapUserString* map, char* nom, char* prenom){
	printf("     Entrée dans : getUserWithNomPrenom\n");

	for(int i=0 ; i<map->size ; i++){
		if(strcmp(map->elem[i].key->nom,nom) == 0 && strcmp(map->elem[i].key->prenom,prenom) == 0){
			
			printf("     Sortie de : getUserWithNomPrenom\n");
			return map->elem[i].key;
		}
	}
}

int getUserLineWithNomPrenom(char* nomParam, char* prenomParam){
	printf("     Entrée dans : getUserLineWithNomPrenom\n");

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
						//tant qu'on a pas d'et commerciale (marquant un nouveau parametre)
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

int ajouteUtilisateur(hashMapStringString mapParameters, char* admin){
	printf("     Entrée dans : ajouteUtilisateur\n");
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
	newUtilisateur.adressePostale = adressePostale;
	if((numTel = getFromHashMapStringString(&mapParameters, "numTel")) == NULL){
		numTel = " ";
	}
	newUtilisateur.numTel = numTel;
	if((remarque = getFromHashMapStringString(&mapParameters, "remarque")) == NULL){
		remarque = " ";
	}
	newUtilisateur.remarque = remarque;
	if((age = getFromHashMapStringString(&mapParameters, "age")) == NULL){
		age = "-1";
	}
	newUtilisateur.age = atoi(age);

	addToHashMapUserString(&mapUtilisateurs, newUtilisateur, "0");

	FILE* csv = fopen("mapUsers.csv", "r+");
	fseek(csv, 0, SEEK_END); //on va a la fin du fichier
	printf("Test2\n");
	printf("%s,%s,%s,%s,%s,%s,%s,%s\n", nom, prenom, mail, adressePostale, numTel, remarque, age, admin);
	fprintf(csv, "%s,%s,%s,%s,%s,%s,%s,%s\n", nom, prenom, mail, adressePostale, numTel, remarque, age, admin);
	printf("Test3\n");
	fclose(csv);

	printf("     Sortie de : ajouteUtilisateur\n");
	return 1;
}

int modifieUtilisateur(hashMapStringString mapParameters){
	printf("     Entrée dans : modifieUtilisateur\n");
	char *nom, *prenom, *mail, *adressePostale, *numTel, *remarque, *age, *admin;

	if((nom = getFromHashMapStringString(&mapParameters, "nom")) == NULL ||
	(prenom = getFromHashMapStringString(&mapParameters, "prenom")) == NULL){
		printf("Erreur, données manquantes pour modifier un utilisateur.\n");
		return 0;
	}
	
	utilisateur* user = getUserWithNomPrenom(&mapUtilisateurs ,nom, prenom);
	admin = getFromHashMapUserString(&mapUtilisateurs, user);

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
		}else if(copie == 0){
			fprintf(temp, "%s,%s,%s,%s,%s,%s,%s,%s\n", nom, prenom, mail, adressePostale, numTel, remarque, age, admin);
			copie = -1;
		}

		if(c == '\n')
			copie = 1;
    }
    fclose(csv);
    fclose(temp);
    remove("mapUsers.csv");
    rename("temp.csv", "mapUsers.csv");

	printf("     Sortie de : modifieUtilisateur\n");
	return 1;
}

int supprimeUtilisateur(hashMapStringString mapParameters){
	printf("     Entrée dans : supprimeUtilisateur\n");
	char *nom, *prenom, *mail, *adressePostale, *numTel, *remarque, *age, *admin;

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

void aiguillageServeur(hashMapStringString mapParameters){
	printf("     Entrée dans : aiguillageServeur\n");

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
	
	printf("     Sortie de : aiguillageServeur\n");
}

//est ce que la requete est une requete GET
int isRequeteGet(char *requete){
	if(requete[0] == 'G' && requete[1] == 'E' && requete[2] == 'T'){
		return 1;
	}
	return 0;
}

void initHashMapUserString(){
	printf("     Entrée dans : initHashMapUserString\n");

	mapUtilisateurs.size = 0;

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
		addToHashMapStringString(&mapParameters, "adressePostale", "7 Chemin de Dardagna");
		addToHashMapStringString(&mapParameters, "numTel", "0619422408");
		addToHashMapStringString(&mapParameters, "remarque", " ");
		addToHashMapStringString(&mapParameters, "age", "21");

		ajouteUtilisateur(mapParameters, "1");
	}

	printf("     Sortie de : initHashMapUserString\n");
}

//serveur sur localhost:13214
int main() {
	char *message = NULL;
	char *requeteExtrait = malloc(511*sizeof(char));


	initHashMapUserString();
	Initialisation();

	while(1) {
		AttenteClient();
		message = Reception();
		hashMapStringString mapParameters = {.size = 0};

		if(message != NULL) {
			printf("J'ai recu: %s\n", message);

			if(isRequeteGet(message) == 1 && extraitRequete(message, &mapParameters) == 1){
				aiguillageServeur(mapParameters);
			}

			free(message);
		}

		TerminaisonClient();
	}

	free(requeteExtrait);

	return 0;
}
