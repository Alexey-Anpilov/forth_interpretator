#include"stdlib.h"
#include"stdio.h"
#include"ctype.h"
#include"string.h"
#include"name_table.h"

name *find_name(name** table, char *str, enum name_type type, int insert){
    static int val = 0;
    static int key_val = -1;

    int hi = 0;
    char *pp = str;
    while (*pp){
        hi <<= 1;
        hi ^= *pp++;
    }
    
    if (hi < 0){
        hi = -hi;    
    }

    hi %= table_size;
    for(name *n=table[hi];n;n=n->next){
        if(strcmp(str,n->string)==0) 
            return n;
    }

    if (insert == 0){ 
        return NULL;
    }
    name *nn = (name*)calloc(1, sizeof(name));
    nn->string = (char*)calloc(strlen(str) + 1, sizeof(char));
    strcpy(nn->string, str);
    if (type == key_name){
        nn->value = key_val;
        key_val--;
        nn->type = word;
    }
    else if (type == id_name){
        nn->value = val;
        val++;
        nn->type = word;
    } else{
        nn->value = val;
        val++;
        nn->type = constant;
    }
    nn->next = table[hi];
    table[hi] = nn;
    return nn;
}

void print_table(name** table){
    name* p = table[0];
    for (int i  = 1; i < table_size; p = table[i], i++)
    {    
        for(name* n = p; n; n=n->next){
            printf("%s: %d\n",n->string, n->value);
        }
    }
}

