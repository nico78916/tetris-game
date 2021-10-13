/**
 *Retourne 1 si la query est dans la table
 *Sinon retourne 0
 *@param query l'entier à chercher
 *@param table le tableau
 *@param c la taille du tableau
*/
int contain(int query,int* table,int c){
    int i;
    for(i = 0;i<c;i++){
        if(query == table[i]){
            return 1;
        }
    }
    return 0;
}