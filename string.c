int compare(char* s1,char* s2){
    int i = 0;
    printf("string.c    compare\n");
    while(s1[i] != '\0'){
        if(s1[i] != s2[i]) return 0;
        i++;
    }
    if(s2[i] != '\0') return 0;
    return 1;
}

int contain(char* query,char** table,int c){
    int i;
    printf("string.c    contain\n");
    for(i = 0;i<c;i++){
        if(compare(query,table[i])){
            return 1;
        }
    }
    return 0;
}

int indexOf(char* query,char** table,int c){
    int i;
    printf("string.c    indexOf\n");
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
    printf("string.c    strset\n");
    while(src[i] != '\0'){
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}
