/* Joue avec les liste d'entier :) */
/**
 * Permet d'avoir l'index d'un entier recherché
 * @param query L'entier à recherché
 * @param table le tableau
 * @param c la taille du tableau 
 * @returns l'index ou -1
*/
int int_indexOf(int query,int* table,int c){
    int i;
    for(i = 0;i<c;i++){
        if(query == table[i]){
            return i;
        }
    }
    return -1;
}
