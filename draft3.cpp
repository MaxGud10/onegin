#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <ctype.h>  

const int MAX_ROWS = 15;
const int MAX_COLS = 260;


int filling_buffer(struct Text_t* onegin);
int check_excess(char* pointer);
int print_lines(const char* str);
int bubble_sort(struct Text_t *onegin);
int str_case_cmp(char* pointer, char *pointer_1);
int print(struct Text_t* onegin);

struct Text_t 
{
    char* buffer; //  массив для буфера
    int text_len;
    char** pointers;
    int lines_number;

};

int main (void)
{

    struct Text_t onegin = {};
    filling_buffer(&onegin) ; // вызов функции которую мы передает адрес структуры переменная которой равна onegin

    bubble_sort(&onegin);
    
    return 0;
}

int filling_buffer(struct Text_t* onegin)
{
    
    FILE* file = fopen("onegin.txt", "rb");

    printf("<<< file opens\n");

    if (file == NULL)
    {
        printf("File opening error: onegin.txt\n");
        return -1;
    }

    struct stat text_len = {};

    stat("onegin.txt", &text_len);
    printf("text.st_size = %d\n", text_len.st_size);
    char* tmp_buffer = (char*)calloc(text_len.st_size, sizeof(char)); // выделяю память для буфера

    if (tmp_buffer == NULL) 
    {
        printf("Incorrect memory allocation\n");
        return -1;
    }

    onegin -> buffer   = tmp_buffer; 
    onegin -> text_len = text_len.st_size;

    fread(onegin -> buffer, sizeof(char), text_len.st_size,file);
    // buffer - блок памяти который минимальный выделяемый для чтения 
    // sizeof(char) - размер в байтах каждого считываемого символа


    printf("\n<<<fread\n");

    
    int count = 0;
    for (int i = 0; i < text_len.st_size; i++) // цикл для прохождения по всем символам для проверки на \n  (сколько строк)
    {
        printf("i = %d - buffer[i] = %d, buffer[i] ='%c'\n", i, (onegin -> buffer)[i], (onegin -> buffer)[i]);
        if ((onegin -> buffer)[i] == '\n')
        {
            count++;
            printf("\nCOUNT = %d\n", count);
        }
        //return 0;
    }
    onegin -> lines_number = count;

    char** tmp_pointers = (char**)calloc(count, sizeof(char*));

    if (tmp_pointers == NULL)
    {
        printf("Incorrect memory allocation\n");
        return -1;
    }
    
    onegin -> pointers = tmp_pointers;

    int postion = 0; // счетчик (расстояние от начального адреса до \n)
    int lines = 0; 
    for (; postion < text_len.st_size; postion++) //  цикл по всем символам из буфера. если начало строки,
    {
        if ((onegin -> buffer)[postion] == '\n')
        {
            (onegin -> pointers)[lines] = onegin -> buffer + postion; // присваивает адрес первого элемента из масcива buffer в массив postion  
            lines++;
        }  
    }

    for (int i = 0; i < lines; i++)
    {
       // printf("\n[%d] -> pointers[lines] = <%s>\n", i, pointers[i]);
       //printf("<i> = %d -> pointers[i] = '%s'", i, pointers[i]);
       print_lines((onegin -> pointers)[i]);
    }

    return 0;
}



int print_lines(const char* str)
{
    while (1)
    {
        if (*str == '\n') break;
        if (*str == '\0') break;

        putchar(*str); // printf(%c)
        str++;
    }
    return 0;
}

int bubble_sort(struct Text_t *onegin)
{
    char* ded = NULL;

    for (int i = 0; i < onegin -> lines_number; i++)
    {
        for (int j = 0; j < onegin -> lines_number - 1 -i; j++)
        {
            //assert();
            if (str_case_cmp((onegin -> pointers)[j], (onegin -> pointers)[j + 1]) > 0)
            {
                ded                         = (onegin -> pointers)[j];
                (onegin -> pointers)[j]     = (onegin -> pointers)[j + 1];
                (onegin -> pointers)[j + 1] = ded;
            }
        }
    }
    return 0;
}

int str_case_cmp(char* pointer, char *pointer_1)
{
    int start_of_ptr  = check_excess(pointer);
    int start_of_ptr_1 = check_excess(pointer_1);
    
    int x = 0;
    for (; x < MAX_ROWS - 1; x++)
    {
        //printf("\nx = %d, i = %d, j = %d, i + x = %d, j + x = %d\n", x, i, j, x + i, j + x);
        int symbol1 = tolower(pointer[start_of_ptr + x]);
        int symbol2 = tolower(pointer_1[start_of_ptr_1 + x]);
        printf("\nsymbol1 = %d, (%c)\n", symbol1, symbol1);
        printf("\nsymbol2 = %d, (%c)\n", symbol2, symbol2);
        if (symbol1 != symbol2)
        {
            printf("\nsymbol2 - symbol1 = %d\n", symbol2- symbol1);
            return -(symbol2 - symbol1);
        }

        if (symbol1 == '\0' || symbol2 == '\0')
        {
            printf("\nsymbol1 = %d and symbol2 = %d\n", symbol1, symbol2);
            return 0;
        }
    }
    return 0;
}

int check_excess(char* pointer)
{
    int i = 0;
    while (pointer[i] != '\0')
    {

        printf("\ni = %d, pointer = %s, pointer[%d]\n ", i, pointer, pointer[i] );
        int is_space = isspace(pointer[i]); 
        int is_punct = ispunct(pointer[i]);
        printf("is_space1 = %d, is_punct1 = %d", is_space, is_punct);  

        if (!is_space &&
            !is_punct)
            break;   

        i++;
    }
    return i;
}

int print(struct Text_t* onegin)
{
    printf("THE TEXT:\n\n");

    for (int i = 0; i < onegin -> text_len; i++) 
    {
        if (onegin -> pointers[i]    != NULL && 
            onegin -> pointers[i][0] != '\0') 
        {
            printf("<<< i = %02d: ", i);
            printf("<%s>\n", onegin -> pointers[i]);
        }
    }
    return 0;
}
