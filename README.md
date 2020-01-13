# STRI_1A_Projet-C

- Compilation :
```gcc mainServeur.c serveur.c -o mainServeur```
  
- Execution :
```./mainServeur```

- Accés au serveur :
Allez sur Chrome (ou un autre navigateur)
Taper `http://localhost:13214/` dans votre barre de recherche, puis vos paramètres :
Le paramètre de base est 'ACTION' qui indiquera au serveur ce que vous souhaitez faire.

Pour chaque action certains autres paramètres sont nécessaire.
1. `ACTION=1` -> Connexion au serveur.
Il faudra en plus le login et le password de l'utilisateur qui souhaite se connecter.
exemple : `http://localhost:13214/ACTION=1&login=floliroy&password=stri`

2. `ACTION=2` -> Déonnexion du serveur de l'utilisateur précedemment connecté.
exemple : `http://localhost:13214/ACTION=2`

3. `ACTIOn=3` -> Ajoute un nouvel utilisateur (utilisable seulement si on est admin).
Il faudra en plus le nom, le prenom, le mail, le login, et le password du nouvel utilisateur a ajouter.
exemple : `http://localhost:13214/ACTION=3&nom=Genet&prenom=Duncan&mail=gduncan@gmail.fr&login=gduncan&password=user`

4. `ACTION=4` -> Modifie un utilisateur (utilisable seulement si on est admin).
Il faudra obligatoirement le nom et le prenom de l'utilisateur que l'on souhaite modifier, plus tout champ que l'on souhaite modifier sur cet utilisateur.
exemple : `http://localhost:13214/ACTION=4&nom=Genet&prenom=Duncan&age=21&mail=duncan.genet@hotmail.fr`

5. `ACTION=5` -> Supprime un utilisateur (utilisable seulement si on est admin).
Il faudra obligatoirement le nom et le prenom de l'utilisateur que l'on souhaite supprimer.
exemple : `http://localhost:13214/ACTION=5&nom=Genet&prenom=Duncan`