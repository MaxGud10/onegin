#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

const int MAX_ROWS = 15;
const int MAX_COLS = 260;

//int sort_string(char* pointers[], char text[][MAX_COLS]);
int sort_string(char* pointers[]);
void str_cpy(char* dest, const char* src);
int clean_string(const char* src, char* dest);
//int print(char* pointers[], char text[][MAX_COLS]);
int print(char* pointers[]);

int main (void)
{
    char  text    [MAX_ROWS][MAX_COLS] = {};
    char* pointers[MAX_ROWS]           = {}; // массив указателей

    printf("<<<< ANIME file opening\n");

    FILE* file = fopen("onegin.txt", "r");

    if (file == NULL)
    {
        printf("File opening error: onegin.txt\n");
        return -1;
    }

    printf(">>>> file exists.\n");

    int row = 0;
    while (row < MAX_ROWS)
    {
        if (fgets(text[row], MAX_COLS, file) == NULL)
            break;

        // Удаление символа новой строки
        int symbol_gad = strcspn(text[row], "\n");  

        if (text[row][symbol_gad] == '\n')
        {
            text[row][symbol_gad] = '\0';
        }

        pointers[row] = text[row]; // Сохраняем индекс строки
        row++; 
    }

    printf("calling fclose...\n");
    fclose(file);
    printf("exited from fclose\n");


    printf("<<<<< sort_string\n");
    //sort_string(pointers, text); 
    sort_string(pointers); // Передаем массив указателей и массив текст
    printf("<<<<< sort_string\n");

    printf("calling print():\n");
    //print(pointers, text);
    print(pointers); // Печатаем с использованием указателей

    return 0;
}

int clean_string(const char* src, char* dest) // const char* src - указатель на исходную строку которую нужно очистить( const  показывает что содержимое не будет изменяться)
{
    while(*src) // для итерации по каждому символу исходной строки 
    {

        int is_punct = ispunct((unsigned char)*src); // проверка на пунктуацию
        int is_space = isspace((unsigned char)*src); // проверка на пробелы

        if (is_punct == 0 && is_space == 0)
        {
            *dest++ = (char)tolower((unsigned char)*src);  // Записываем символ в нижнем регистре
        }
        src++;
    }
    return *dest = '\0';
}

//int sort_string(char* pointers[], char text[][MAX_COLS])
int sort_string(char* pointers[])
{
    char ded    [MAX_COLS] = "";
    char clean_a[MAX_COLS] = "";
    char clean_b[MAX_COLS] = "";

    for (int i = 0; i < MAX_ROWS; i++)
    {
        printf("\nSTARTING PASS i = %d\n", i);

        for (int j = 0; j < MAX_ROWS - 1 - i; j++)
        {
            printf("j = %d...\n", j);
            assert(0 <= j     && j     < MAX_ROWS);
            assert(0 <= j + 1 && j + 1 < MAX_ROWS);

            printf("<%s> vs <%s>...\n", pointers[j], pointers[j + 1]);

            // Очищаем строки от знаков препинания и пробелов
            clean_string(pointers[j],     clean_a);
            clean_string(pointers[j + 1], clean_b);

            if (strcasecmp(clean_a, clean_b) > 0) 
            {
                printf("Swapping...\n");

                str_cpy(ded, pointers[j]);
                str_cpy(     pointers[j], pointers[j + 1]);
                str_cpy(                  pointers[j + 1], ded);

                printf("<%s> vs <%s>: SWAPPED\n", pointers[j], pointers[j + 1]);
            }
        }

        printf("END PASS %d\n", i);
    }
    return 0;
}

void str_cpy(char* dest, const char* src)
{
    assert(src);
    assert(dest);

    for (int i = 0; i < MAX_COLS; i++)
    {
        dest[i] = src[i];
    }
}

//int print(char* pointers[], char text[][MAX_COLS])
int print(char* pointers[])
{
    printf("THE TEXT:\n\n");

    for (int i = 0; i < MAX_ROWS; i++) 
    {
        if (pointers[i] != NULL) // Проверка, чтобы избежать вывода NULL
        {
            printf("<<< i = %02d: ", i);
            printf("<%s>\n", pointers[i]);
        }
    }
    return 0;
}