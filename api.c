#include <stdio.h>
int print(char*a){
    printf("api.c    print\n");
    return printf("%s \n",a);
}

int int_indexOf(int query,int* table,int c){
    int i;
    printf("api.c    int_indexOf\n");
    for(i = 0;i<c;i++){
        if(query == table[i]){
            return i;
        }
    }
    return -1;
}
