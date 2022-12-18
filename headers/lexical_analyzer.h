#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H
/*
    Множество состояний автомата:
        S - начальное состояние
        N - формирование числа
        W - формирование имени
        COMorW - состояние, после поступления '('. По ' ' совершается переход в COM, по остальным символам в W
        COM - комментарии
        T - конечное состояние
*/ 

enum states
{
    S,
    N,
    W,
    COMorW,
    COM, 
    T,
    states_num
};


/*
    Множество категорий входных символов:
        number - числа
        letter - буквы
        space - пробел
        comment_bkt_open - '('
        comment_bkt_close - ')'
        other - остальные символы
        exit_symbol - символ окончания
        line_break - перенос строки
        minus - '-'
        input_num

*/
enum input_element
{
    number,
    letter,
    space,
    comment_bkt_open,
    comment_bkt_close,
    other,
    exit_symbol,
    line_break,
    minus,
    input_num
};

char* add_symbol_to_string(char* str, char c);

char* concat(char *s1, char *s2);

char* make_string_from_char(char c);

void word_create(enum states state, char r);

void number_done(enum states state, char r);

void word_done(enum states state, char r);

void error(enum states state, char r);

void write_lexems_to_file(char* file_name);

void write_name_table_to_file(char* file_name);

void terminate(enum states state, char r);

void fill_table_with_key_names(name** table);

char* read_from_file(char* file_name);

void lexical_analyzer();

typedef void (*transition_callback)(enum states state, char r);
// элемент массива таблицы переходов
// содержит состояние и функцию, которая выполняется при переходе
struct transition
{
    enum states new_state;
    transition_callback procedure;
};  

#endif