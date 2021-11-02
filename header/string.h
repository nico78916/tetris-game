/**
 * compare deux chaine de caractère
 */
int compare(char* s1,char* s2);
/**
 * Regarde si la chaine de caractère query est contenue dans la table
*/
int contain(char* query,char** table,int c);
/**
 * Renvoie l'index de query dans table ou -1 s'il n'est pas dans le tableau
*/
int indexOf(char* query,char** table,int c);

/** 
 * Equivalent de strcopy 
*/
void strset(char* dest,char* src);