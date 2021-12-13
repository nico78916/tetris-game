#include <stdio.h>
int print(char*a){
    return printf("%s \n",a);
}

int int_indexOf(int query,int table[],int c){
    int i;
    for(i = 0;i<c;i++){
        if(query == table[i]){
            return i;
        }
    }
    return -1;
}
