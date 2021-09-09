#include <iostream>

// Enregistrement du maillon
template <typename T> struct t_maillon {
	T donnee;
	t_maillon *suivant = nullptr;
};
 
// Enregistrement du chainage
template <typename T> struct t_chaine {
	t_maillon<T> *tete;
};

// Enregistrement d'une date
struct t_date {
	int jour;
	int mois;
	int an;
};

// Enregistrement d'un contact
struct t_contact {
	std::string pseudo1;
	std::string pseudo2;
	t_date date;
};

// Enregistrement d'un utilisateur (liste de pseudos, et liste de contacts)
struct t_utilisateurs {
	bool est_cas_contact = false;
	t_chaine<std::string> pseudos;
	t_chaine<t_contact> contacts;
};


//######################################################
//#                 GESTION DES CHAINAGES              #
//######################################################

//--------------------------------------------------------------------
// role : tester si une chaine est vide (donnee non défini)
// précondition : le type T doit être une chaîne de caractère ou un t_contact
template <typename T> bool is_empty(t_chaine<T> tested) {
	
	// Si la chaîne est une chaîne de contacts
	if constexpr (std::is_same_v<T, t_contact>) {
		return tested.tete->donnee.pseudo1 == "";
	}
	
	// Si la chaîne est une chaîne de pseudos
	else {
		return tested.tete->donnee == "";
	}
}

//--------------------------------------------------------------------
// role : compter le nombre de maillon d'une chaine
// précondition : entrer une chaine non vide
template <typename T> int taille_chaine(t_chaine<T> chaine) {
	int compteur = 1;
	while (chaine.tete->suivant != nullptr) {
		compteur++;
		chaine.tete = chaine.tete->suivant;
	}
	return compteur;
}

//--------------------------------------------------------------------
// role : afficher le contenu de notre chainee chaînée
template <typename T> void affiche(t_chaine<T> chaine, t_date date){
	int taille = taille_chaine(chaine);
	for (int i = 0; i < taille; i++) {
		if constexpr (std::is_same_v<T, t_contact>) {
		
			// Afficher que les contacts du jour
			if (nombre_jour_entre(date, chaine.tete->donnee.date) == 0) {
				std::cout << chaine.tete->donnee.pseudo1 << " " << chaine.tete->donnee.pseudo2 << " le " << chaine.tete->donnee.date.jour << "/" << chaine.tete->donnee.date.mois << "/" << chaine.tete->donnee.date.an << std::endl;
			}
		}
		
		// Afficher les pseudos
		else {
			std::cout << chaine.tete->donnee << std::endl;
		}
		chaine.tete = chaine.tete->suivant;
	}
}

//--------------------------------------------------------------------
// role : (q0) ajouter un maillon à la fin de la chaîne
// précondition : La tête de la liste doit être instancié (au moins
// une allocation d'un pointeur vers un t_maillon<typename T>)
// constante (en rentrant dans le if) ou lineaire (en rentrant dans le else)
template <typename T> void ajouter_maillon_fin(t_chaine<T> &chaine, T donnee) {
	t_maillon<T>* nouveau_maillon = new t_maillon<T>; 
	nouveau_maillon->donnee = donnee; 
	nouveau_maillon->suivant = nullptr;
	
	// Si la chaîne ne contient qu'un maillon ou est vide
	if (is_empty(chaine)) {
		chaine.tete = nouveau_maillon;
	}
	else {
		t_maillon<T>* maillon_temporaire = chaine.tete;
		while(maillon_temporaire->suivant != nullptr){ 
			maillon_temporaire = maillon_temporaire->suivant;
		}
		maillon_temporaire->suivant = nouveau_maillon; 
	}
}

//--------------------------------------------------------------------
// role : (q0) ajouter un maillon au debut de la chaîne
// précondition : La tête de la liste doit être instancié (au moins
// une allocation d'un pointeur vers un t_maillon<typename T>)
// constante
template <typename T> void ajouter_maillon_debut(t_chaine<T> &chaine, T donnee){
	t_maillon<T>* nouveau_maillon = new t_maillon<T>; 
	nouveau_maillon->donnee = donnee; 
	
	// Si la chaîne ne contient qu'un maillon ou est vide
	if (is_empty(chaine)) {
		chaine.tete = nouveau_maillon;
	}
	else {
		nouveau_maillon->suivant = chaine.tete; 
		chaine.tete = nouveau_maillon;
	}
}

//--------------------------------------------------------------------
// role : (q0) supprimer un maillon au début de la chaîne
// précondition : La tête de la liste doit être instancié (au moins
// une allocation d'un pointeur vers un t_maillon<typename T>)
// constante
template <typename T> void supprimer_maillon_debut(t_chaine<T> &chaine) {
	
	// Si la chaîne ne contient pas qu'un maillon ou n'est pas vide
	if (chaine.tete->suivant != nullptr) {
		t_maillon<T>* ancienne_tete = chaine.tete;
		chaine.tete = ancienne_tete->suivant;  
		delete ancienne_tete;
	}
	else {
		delete chaine.tete;
	}
}

//--------------------------------------------------------------------
// role : (q0) supprimer un maillon à la fin de la chaîne
// précondition : La tête de la liste doit être instancié (au moins
// une allocation d'un pointeur vers un t_maillon<typename T>)
// lineaire (en rentrant dans le if) ou constante (en rentrant dans le else)
template <typename T> void supprimer_maillon_fin(t_chaine<T> &chaine){
	
	// Si la chaîne ne contient pas qu'un maillon ou n'est pas vide
	if (chaine.tete->suivant != nullptr) {
		t_maillon<T>* maillon_temporaire = chaine.tete;
		while((maillon_temporaire->suivant)->suivant != nullptr) {
			maillon_temporaire = maillon_temporaire->suivant; 
		}
		delete maillon_temporaire->suivant;
		maillon_temporaire->suivant = nullptr; 
	}
	else {
		delete chaine.tete;
	}
}

//--------------------------------------------------------------------
// role : recupérer la position-ième donnee de la chaine
template <typename T> T choisir_donnee(t_chaine<T> chaine, int position) {
	for (int i = 0; i < position; i++) {
		chaine.tete = chaine.tete->suivant;
	}
	return chaine.tete->donnee;
}


//######################################################
//#                 GESTION DES PSEUDOS                #
//######################################################

//--------------------------------------------------------------------
// role : donner un pseudo aléatoire
std::string donne_pseudo(){
	std::string pseudo = "";
	
	// Liste des caractères utilisable
	char consonne[20] = {'b','c','d','f','g','h','j','k','l','m','n','p','q','r','s','t','v','w','x','z'}; 
	char voyelle[6] = {'a','e','i','o','u','y'};     
	
	// Former le début du pseudo
	for (int i=0; i<2; i++){                  
		pseudo = pseudo + consonne[rand()%20];  
		pseudo = pseudo + voyelle[rand()%6];      
	}
	
	// Former la fin du pseudo
	for (int i=0; i<2; i++){                   
		pseudo = pseudo + std::to_string(rand()%9);
	}
	return pseudo;
}

//--------------------------------------------------------------------
// role : (q1) initialiser une liste de pseudo
t_chaine<std::string> creer_liste_pseudo() {
	t_chaine<std::string> pseudos;
	pseudos.tete = new t_maillon<std::string>;
	ajouter_maillon_fin(pseudos, donne_pseudo());
	return pseudos;
}


//######################################################
//#                 GESTION DES DATES                  #
//######################################################

//--------------------------------------------------------------------
// role : donne le nombre de jour en deux t_date 
int nombre_jour_entre(t_date date1, t_date date2) {
	
	// Calcul le nombre de jour écouler entre date1 et le 01/01/0001
	// et entre date2 et le 01/01/0001, et fait la différence
	if (date1.mois < 3) { date1.an--, date1.mois += 12; }
	if (date2.mois < 3) { date2.an--, date2.mois += 12; }
	return std::abs((146097*(date1.an - date2.an))/400 + ((153*(date1.mois - date2.mois) + 5*(date1.jour - date2.jour))/5));
}

//--------------------------------------------------------------------
// role : ajoute 1 jour à une t_date
void ajouter_un_jour(t_date &date) {
	
	// Cas du dernier jour de l'année
	if (date.mois == 12 && date.jour == 31) {
		date.jour = 1;
		date.mois = 1;
		date.an++;
	}
	
	// Cas des fins de mois (hors février)
	else if (((date.mois == 4 || date.mois == 6 || date.mois == 9 || date.mois == 11) && date.jour == 30) || date.jour == 31) {
		date.jour = 1;
		date.mois++;
	}
	
	// Cas du mois de février
	else if (date.mois == 2 ) {
		
		// Cas de l'année bisextile (29 jours en février)
		if (date.an % 4 == 0 && (date.an % 100 > 0 || date.an % 400 == 0)) {
			if (date.jour == 29) {	
				date.jour = 1;
				date.mois++;
			}
		}
		
		// Cas de l'année non bisextile (28 jours en février)
		else {
			if (date.jour == 28) {
				date.jour = 1;
				date.mois++;
			}
		}
	}
	
	// Cas classique
	else {
		date.jour++;
	}
}


//######################################################
//#                 GESTION DES CONTACTS               #
//######################################################

//--------------------------------------------------------------------
// role : verifier si un utilisateur est cas contact ou non
void est_cas_contact(t_utilisateurs &utilisateur) {
	if (utilisateur.est_cas_contact == true) {
		std::cout << "Vous êtes cas contact... Restez chez vous et contacter votre médecin !" << std::endl;
		utilisateur.est_cas_contact = false;
	}
}

//--------------------------------------------------------------------
// role : supprimer les vieux contacts et les vieux pseudos
void suppression_vieux_contact_utilisateur(t_utilisateurs &utilisateur, t_date date) {
	while (nombre_jour_entre(utilisateur.contacts.tete->donnee.date, date) >= 14) {

		// (q1) Supprimer un pseudo
		supprimer_maillon_debut(utilisateur.pseudos);

		// (q2) Suppression des contacts de plus de deux semaines
		supprimer_maillon_debut(utilisateur.contacts);
	}
}

//--------------------------------------------------------------------
// role : supprimer les vieux contacts de la liste globale
void suppression_vieux_contact_global(t_chaine<t_contact> &global, t_date date) {
	while (nombre_jour_entre(global.tete->donnee.date, date) >= 14) {

		// q(3) Suppression des contacts de plus de deux semaines
		supprimer_maillon_debut(global);
	}
  }
//--------------------------------------------------------------------
// role : simuler un jour de contact
void simuler_jour(t_utilisateurs &utilisateur, t_date date, t_chaine<t_contact> &global) {
	for (int i = 0; i <= rand()%21; i++) {
		t_contact contact;
		contact.date = date;
		contact.pseudo1 = choisir_donnee(utilisateur.pseudos, taille_chaine(utilisateur.pseudos)-1);

		// (q1) Recharger la liste de pseudo
		ajouter_maillon_fin(utilisateur.pseudos, donne_pseudo());
      
		contact.pseudo2 = donne_pseudo();

		// (q2) Ajouter un nouveau contact à la liste de contact
		ajouter_maillon_fin(utilisateur.contacts, contact);

		// (q3) Ajouter un contact à la liste de tous les contacts
		0ajouter_maillon_fin(global, contact);
	}
	
	// Suppression des contacts datant de plus de 2 semaines
	suppression_vieux_contact_utilisateur(utilisateur, date);
	suppression_vieux_contact_global(global, date);
    
	// (q3) Envois de message si l'utilisateur est cas contact
	est_cas_contact(utilisateur);
}

//--------------------------------------------------------------------
// role : forcer le contact entre 2 utilisateurs
void forcer_contact(t_utilisateurs &utilisateur1, t_utilisateurs &utilisateur2, t_date date, t_chaine<t_contact> &global) {
	
	// Création du premier contact (utilisateur1 - utilisateur2)
	t_contact contact1;
	contact1.date = date;

	contact1.pseudo1 = choisir_donnee(utilisateur1.pseudos, taille_chaine(utilisateur1.pseudos)-1);
	ajouter_maillon_fin(utilisateur1.pseudos, donne_pseudo());
      
	contact1.pseudo2 = choisir_donnee(utilisateur2.pseudos, taille_chaine(utilisateur2.pseudos)-1);
	ajouter_maillon_fin(utilisateur2.pseudos, donne_pseudo());

	// Ajout des contacts à la liste global et à la liste de l'utilisateur1
	ajouter_maillon_fin(utilisateur1.contacts, contact1);
	ajouter_maillon_fin(global, contact1);

	// Création du deuxième contact (utilisateur2 - utilisateur1)
	t_contact contact2;
	contact2.date = date;

	contact2.pseudo1 = choisir_donnee(utilisateur2.pseudos, taille_chaine(utilisateur2.pseudos)-1);
	jouter_maillon_fin(utilisateur2.pseudos, donne_pseudo());

	contact2.pseudo2 = choisir_donnee(utilisateur1.pseudos, taille_chaine(utilisateur1.pseudos)-1);
	ajouter_maillon_fin(utilisateur1.pseudos, donne_pseudo());

	// Ajout des contacts à la liste global et à la liste de l'utilisateur2
	ajouter_maillon_fin(utilisateur2.contacts, contact2);
	ajouter_maillon_fin(global, contact2);
}

//--------------------------------------------------------------------
// role : (q3) signalement, et détection des personnes ayant été en contact avec la personne malade
void alerte(t_utilisateurs malade, t_utilisateurs &tester, t_chaine<t_contact> global) {
	int compteur = 0;
	
	// On regarde tous les contacts dans liste globale
	while (global.tete->suivant != nullptr) {
		
		// Si le pseudo1 du contact est un pseudo de l'utilisateur malade
		if (global.tete->donnee.pseudo1 == choisir_donnee(malade.pseudos, compteur)) {
			compteur++;
			
			// On regarde si le pseudo2 appartient à l'utilisateur testé
			for (int i = 0; i < taille_chaine(malade.pseudos); i++) {
        
				// Si la chaine de pseudo util1 > util2
				if (i < taille_chaine(tester.pseudos)) {
					if (global.tete->donnee.pseudo2 == choisir_donnee(tester.pseudos, i)) {
						tester.est_cas_contact = true;
						return;
					}
				}
			}
		}
		global.tete = global.tete->suivant;
	}
}


//######################################################
//#            SIMULATION DE SITUATIONS                #
//######################################################

//--------------------------------------------------------------------
// role : tester le cas: Deux utilisateurs se sont rencontré il y a moins de 14 jours, et l’un deux est malade
void test_simulation(int jour_de_rencontre, int jour_d_alerte) {
	
	// Instanciation de la date et des utilisateurs
	t_date date = {29, 2, 2000};
	t_utilisateurs utilisateur1;
	t_utilisateurs utilisateur2;
	t_chaine<t_contact> global;

	global.tete = new t_maillon<t_contact>;
	utilisateur1.contacts.tete = new t_maillon<t_contact>;
	utilisateur2.contacts.tete = new t_maillon<t_contact>;

	utilisateur1.pseudos = creer_liste_pseudo();
	utilisateur2.pseudos = creer_liste_pseudo();
	
	std::cout << "Il y a deux utilisateur: Bob et Jean" << std::endl;
	std::cout << "Interaction de Bob:" << std::endl;
	
	for (int i = 1; i <= 17; i++) {
		std::cout << "----------- Jour " << i << " --------" << std::endl;
		
		// Rencontre des deux utilisables
		if (i == jour_de_rencontre) {
			forcer_contact(utilisateur1, utilisateur2, date, global);
		}
		
		// L'utilisateur1 est malade et le signal
		if (i == jour_d_alerte) {
			std::cout << "Bob signale qu'il est malade..." << std::endl;
			alerte(utilisateur1, utilisateur2, global);
		}
		simuler_jour(utilisateur1,  date, global);
		
		// Si l'utilisateur2 est cas contact
		if (utilisateur2.est_cas_contact) {
			std::cout << date.jour << "/" << date.mois << "/" << date.an << ": Message pour Jean..." << std::endl; 
			simuler_jour(utilisateur2,  date, global);
			return;
		}
		simuler_jour(utilisateur2,  date, global);
		affiche(utilisateur1.contacts, date);
		ajouter_un_jour(date);
	}
}

//--------------------------------------------------------------------
// role : test
int main() {
	srand(time(0));

	// 4 scénarios ont été créer ci dessous. Il suffit d'enlever les // devant la simulation que l'ont veut tester.

	// Le jour 2, Bob et Jean se rencontre. Le jour 3 Bob signal qu'il est malade. Jean est alerté... Fin de la simulation.
	test_simulation(2,3);

	// Le jour 1, Bob et Jean se rencontre. Le jour 16 Bob signal qu'il est malade. La rencontre a eu lieu il y a plus de 14 jours... La simulation continue.
	//test_simulation(1,16);

	// Le jour 6, Bob a signalé qu'il était malade, mais il n'a jamais rencontré Jean... La simulation continue. (la simulation s'arrétant au jour 17).
	//test_simulation(18,6);

	// Le jour 1, Bob et Jean se rencontre. Personne ne signale qu'il est malade... La simulation continue. (la simulation s'arrétant au jour 17).
	//test_simulation(10, 18);
 
}
