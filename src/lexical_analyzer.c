/* 
    Файл с реализацией лексического анализатора.
    lexical_analyzer - функция, реализующая основную логику работы автомата
    остальные функции - вспомогательные или реализующие процедуры при переходе из состояние в состояние
    ---
    state_transition_table - 2-рный массив задающий таблицу переходов автомата
    
*/

#include"stdlib.h"
#include"stdio.h"
#include"ctype.h"
#include"string.h"
#include"name_table.h"
#include"lexical_analyzer.h"


name** name_table;
int* lex_stream; 
int* lex_p;
int lex_stream_len;
char* word;   
char* c;
char* input_string;

 // добавление символа к концу строки
char* add_symbol_to_string(char* str, char c){
    size_t len = strlen(str);
    char* res = (char*)calloc(len + 2, sizeof(char));
    memcpy(res, str, len);
    char* p = res + len;
    *p = c;
    p++;
    *p = '\0'; 
    return res;
}

// конкатенация двух строк 
char* concat(char *s1, char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);                      
    char *result = (char*)malloc(len1 + len2 + 1);
    if (!result) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);   
    return result;
}

// формирование строки из символа
char* make_string_from_char(char c){
    char* res = (char*)malloc(sizeof(char) + 1);
    *res = c;
    res++;
    *res = '\0';
    return res;
}


// для образования числовой константы
void number_create(enum states state, char r){
    word = add_symbol_to_string(word, r);
    return;
}

// для образования слова 
void word_create(enum states state, char r){
    if (state == COMorW){
        word = add_symbol_to_string(word, '(');
        c--;
        return;
    }
    if (isalpha(r)){
        r = tolower(r);
    }
    word = add_symbol_to_string(word, r);
    return;
}

// когда формирование числа завершено, оно заносится в таблицу имен и в выходную последовательность
void number_done(enum states state, char r){
    name* n = find_name(name_table, word, num, 1);
    *lex_p = n->value;
    lex_p++;
    lex_stream_len++;
    c--;
    memset(word, '\0', strlen(word));
    return;
}


// когда формирование слова завершено, оно заносится в таблицу имен и в выходную последовательность
void word_done(enum states state, char r){
    name* n = find_name(name_table, word, id_name, 1);
    *lex_p = n->value;
    lex_p++;
    lex_stream_len++;
    c--;
    memset(word, '\0', strlen(word));
    return;
}

// входная последовательность читается из файла 
char* read_from_file(char* file_name){
    char* text = (char*)calloc(200, sizeof(char));
    char* p = text;
    FILE* f;
    f = fopen(file_name, "r");
    while((*p = fgetc(f)) != EOF){
        p++;
    }
    *p = '\0';
    return text;
}

// поток лексем записывается в файл
void write_lexems_to_file(char* file_name){
    FILE* file;
    file = fopen(file_name, "w");
    for (int i = 0; i < lex_stream_len; i++){
        fprintf(file, "%d ", lex_stream[i]);
    }
    fclose(file);
}

// таблица имен записывается в файл
void write_name_table_to_file(char* file_name){
    FILE* file;
    file = fopen(file_name, "w");
    name* p = name_table[0];
    for (int i  = 1; i < table_size; p = name_table[i], i++)
    {    
        for(name* n = p; n; n=n->next){
            fprintf(file, "%d %s %d\n",n->value , n->string, n->type);
        }
    }
    fclose(file);
}

// окончание работы автомата
void terminate(enum states state, char r){
    printf("Input string was successfully recognised. Output sequence and table of names were written to files - lexem_stream.txt and name_table.txt.\n");
    write_lexems_to_file("lexem_stream.txt");
    write_name_table_to_file("name_table.txt");
    free(word);
    free_table(name_table);
    free(lex_stream);
    free(input_string);
    exit(0);
}

// таблица переходов
const struct transition state_transition_table[states_num][input_num] = {
    [S][number] = {N, number_create},
    [S][letter] = {W, word_create},
    [S][space] = {S, NULL},
    [S][comment_bkt_open] = {COMorW, NULL},
    [S][comment_bkt_close] = {W, word_create},
    [S][minus] = {N, number_create},
    [S][other] = {W, word_create},
    [S][exit_symbol] = {T, terminate},
    [S][line_break] = {S, NULL},
    [N][number] = {N, number_create},
    [N][letter] = {W, word_create},
    [N][space] = {S, number_done},
    [N][comment_bkt_open] = {W, word_create},
    [N][comment_bkt_close] = {W, word_create},
    [N][other] = {W, word_create},
    [N][exit_symbol] = {S, number_done},
    [N][line_break] = {S, number_done},
    [N][minus] = {W, word_create},
    [W][number] = {W, word_create},
    [W][letter] = {W, word_create},
    [W][space] = {S, word_done},
    [W][comment_bkt_open] = {W, word_create},
    [W][comment_bkt_close] = {W, word_create},
    [W][other] = {W, word_create},
    [W][exit_symbol] = {S, word_done},
    [W][line_break] = {S, word_done},
    [W][minus] = {W, word_create},
    [COMorW][number] = {W, word_create},
    [COMorW][letter] = {W, word_create},
    [COMorW][space] = {COM, NULL},
    [COMorW][comment_bkt_open] = {W, word_create},
    [COMorW][comment_bkt_close] = {W, word_create},
    [COMorW][other] = {W, word_create},
    [COMorW][exit_symbol] = {W, word_create},
    [COMorW][line_break] = {W, word_create},
    [COMorW][minus] = {W, word_create},
    [COM][number] = {COM, NULL},
    [COM][letter] = {COM, NULL},
    [COM][space] = {COM, NULL},
    [COM][comment_bkt_open] = {COM, NULL},
    [COM][comment_bkt_close] = {S, NULL},
    [COM][other] = {COM, NULL},
    [COM][exit_symbol] = {S, NULL},
    [COM][line_break] = {S, NULL},
    [COM][minus] = {COM, NULL},
    [T][number] = {T, NULL},
    [T][letter] = {T, NULL}, 
    [T][space] = {T, NULL},
    [T][comment_bkt_open] = {T, NULL},
    [T][comment_bkt_close] = {T, NULL},
    [T][other] = {T, NULL},
    [T][exit_symbol] = {T, NULL},
    [T][line_break] = {T, NULL},
    [T][minus] = {T, NULL}, 
}; 

// массив ключевых слов 
char* key_names[44] = {"+", "-", "*", "/", ".", "dup", "drop", "mod",
                       "over", "rot", "swap", "roll", "abs", "negate", "1+",
                       "1-", "pick", ".", ":", ";", "@", "!", "constant",
                       "variable", "and", "or", "xor", "not", ">", "<",
                       "=", "0<", "0=", "0>", "if", "else", "begin", "until",
                       "repeat", "while", "do", "loop", "then", "+loop"};

// заполнение таблицы имен ключевыми словами 
void fill_table_with_key_names(name** table){
    for (int i = 0; i < 44; i++){
        find_name(table, key_names[i], key_name, 1);
    }
}

// обработка входных символов и работа автомата
void lexical_analyzer(){
    // задаем начальные параметры и заполняем таблицу имен ключевыми словами
    table_size = 40;
    lex_stream_len = 0;
    name_table = (name**)calloc(table_size, sizeof(name));
    fill_table_with_key_names(name_table);
    lex_stream = (int*)calloc(100, sizeof(int));
    lex_p = lex_stream;
    word = (char*)calloc(31, sizeof(char));

    
    input_string = read_from_file("input_string.txt");
    c = input_string;
    enum states current_state = S;
    enum input_element current_element;
    struct transition current_transition;
    
    // считываем символ и определяем его категорию
    while (1){
        if (isdigit(*c)){
            current_element = number;
        }
        else if (isalpha(*c)){ 
            current_element = letter;
        }
        else if (isspace(*c)){
            current_element = space;
        }   
        else if (*c == '\0'){
            current_element = exit_symbol;
        }
        else if (*c == ')'){
            current_element = comment_bkt_close;
        }
        else if (*c == '('){
            current_element = comment_bkt_open;
        }
        else if (*c == '\n'){
            current_element = line_break;
        }
        else if (*c == '-'){
            current_element = minus;
        } 
        else{
            current_element = other;
        }
        // переходим в новое состояние, выполняя соответствующую процедуру
        current_transition = state_transition_table[current_state][current_element];
        if (current_transition.procedure != NULL){
                current_transition.procedure(current_state, *c);
            
            }
                current_state = current_transition.new_state;
        
        c++;
    }  
}

int main(){
    lexical_analyzer();
    return 0;
}