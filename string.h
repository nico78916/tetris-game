int compare(char* s1,char* s2){
    int i = 0;
    while(s1[i] != '\0'){
        if(s1[i] != s2[i]) return 0;
        i++;
    }
    if(s2[i] != '\0') return 0;
    return 1;
}
/**
 *Retourne 1 si la query est dans la table
 *Sinon retourne 0
 *@param query la chaine à chercher
 *@param table le tableau
 *@param c la taille du tableau
*/
int contain(char* query,char** table,int c){
    int i;
    for(i = 0;i<c;i++){
        if(compare(query,table[i])){
            return 1;
        }
    }
    return 0;
}

/**
 * Permet d'avoir l'index d'un champ rechercher
 * @param query La chaine à recherché
 * @param table le tableau
 * @param c la taille du tableau 
 * @returns l'index ou -1
*/
int indexOf(char* query,char** table,int c){
    int i;
    for(i = 0;i<c;i++){
        if(compare(query,table[i])){
            return i;
        }
    }
    return -1;
}

/**
 * fonction permettant dest = src
*/
void strset(char* dest,char* src){
    int i = 0;
    while(src[i] != '\0'){
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}