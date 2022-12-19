#ifndef NAME_TABLE_H
#define NAME_TABLE_H

// множество типов лексем: слова, константы
enum lexem_type{
    Word,
    Constant
};

// элемент таблицы имен
typedef struct name{
    char *string;
    struct name* next;
    int value;
    enum lexem_type type;
} name;

int table_size;

// тип вносимой лексемы: ключевое слово, слово, число
enum name_type
{
    key_name, 
    id_name,
    num
};

name* find_name(name** table,char* str, enum name_type type, int insert);

void print_table(name** table);

void free_table(name** table);

#endif /*NAME_TABLE_H*/