#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

const int MAX_ROWS = 15;
const int MAX_COLS = 260;

int  unpack_file(char (*text)[MAX_COLS], char* pointers[]);
int  clean_string(const char* src, char* dest);
int  sort_string(char* pointers[]);
int str_case_cmp(const char* pointer, const char *pointer_1);
void str_cpy(char* dest, const char* src);
int  print(char* pointers[]);

int main (void)
{
    // Выделяем память для массива строк

    //char* text2 = (char*) calloc (MAX_ROWS * MAX_COLS, sizeof (char));

    char (*text)[MAX_COLS] = (char (*)[MAX_COLS]) calloc(MAX_ROWS, sizeof(*text)); // объявление переменной, которая является указателем на массив из 260
                                                                                 // sizeof(*text) вычисляет размер одного элемента на который указывает text 
                                                                                // sizeof(*text) = MAX_COLS*sizeof(char)
    assert(text != NULL);

    // Выделяем память для массива указателей
    char** pointers = (char**)calloc(MAX_ROWS, sizeof(char*)); // объявляем переменную pointers которая является указателем на указатель типа char
                                                              // pointers будет хранить адреса строк, каждая из которых будет являться массивом символов 
                                                             // sizeof(char*) вычисляет размер одного указателя
    assert(pointers != NULL);

    printf("<<<< ANIME file opening\n");
    
    unpack_file(text, pointers);

    printf("<<<<< sort_string\n");
    sort_string(pointers); 
    //str_case_cmp("aaab", "aaaZ");
    //str_case_cmp(".aaaZ", ",.,,..aaaZ");
    str_case_cmp(*text, *pointers);

    printf("<<<<< sort_string\n");

    printf("calling print():\n");
    print(pointers);

    free(pointers);
    free(text);

    return 0;
}

int unpack_file(char (*text)[MAX_COLS], char* pointers[])
//int unpack_file(char *text2, char* pointers[])
{
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
        //if (fgets((text2 + MAX_COLS*row), MAX_COLS, file) == NULL)

        if (fgets(text[row], MAX_COLS, file) == NULL)
            break;

        // Удаление символа новой строки
        int symbol_gad = strcspn(text[row], "\n");  

        if (text[row][symbol_gad] == '\n')
        {
            text[row][symbol_gad] = '\0';
        } // TODO: strchr -> быстрее 

        pointers[row] = text[row]; // Сохраняем индекс строки
        row++; 
    }

    printf("calling fclose...\n");
    fclose(file);
    printf("exited from fclose\n");
    return 0;
}

/*int clean_string(const char* src, char* dest) // const char* src - указатель на исходную строку которую нужно очистить(const  показывает что содержимое не будет изменяться)
{
    while(*src) // для итерации по каждому символу исходной строки 
    {

        int is_punct = ispunct((unsigned char)*src); // проверка на пунктуацию
        int is_space = isspace((unsigned char)*src); // проверка на пробелы

// TODO:
        if (is_punct == 0 && 
            is_space == 0)
        {
            *dest++ = (char)tolower((unsigned char)*src);  // Записываем символ в нижнем регистре
        }
        src++; // TODO скопировать в другой массив и там с ним работать там 
    }
    return *dest = '\0';
}*/

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
            //clean_string(pointers[j],     clean_a);
            //clean_string(pointers[j + 1], clean_b);

            //if (strcasecmp(clean_a, clean_b) > 0) 
            if (str_case_cmp(pointers[j], pointers[j + 1]) > 0)
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

int str_case_cmp(const char* pointer, const char *pointer_1)
{
    int i = 0; 
    for (; i < MAX_ROWS; i++)
    {

        printf("\ni = %d, pointer = %s, pointer[%d]\n ", i, pointer, pointer[i] );
        int is_space1 = isspace(pointer[i]); 
        int is_punct1 = ispunct(pointer[i]);
        printf("is_space1 = %d, is_punct1 = %d", is_space1, is_punct1);  

        if (!is_space1 &&
            !is_punct1)
            break;   
    }
    printf("\n>>> i = %d\n", i);

    int j = 0;
    for (; j < MAX_ROWS; j++)
    {
        printf("\nj = %d, pointer_1 = %s, pointer_1[%d]\n", j, pointer_1, pointer_1[j]);
        int is_space2 = isspace(pointer_1[j]); 
        int is_punct2 = ispunct(pointer_1[j]);
        printf("is_space2 = %d, is_punct2 = %d", is_space2, is_punct2);  

        if (!is_space2  &&
            !is_punct2)
            break;  
    }
    printf("\n>>> j = %d\n", j);
    
    int x = 0;
    for (; x < MAX_ROWS - 1; x++)
    {
        printf("\nx = %d, i = %d, j = %d, i + x = %d, j + x = %d\n", x, i, j, x + i, j + x);
        int symbol1 = tolower(pointer[i + x]);
        int symbol2 = tolower(pointer_1[j + x]);
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





    
    /*for (int i = 0; i < MAX_ROWS; i++)
    {
        int str = tolower(*pointer) - tolower(*pointer_1);
        if (str != 0)
            return str;
    }

    char *src  = ""; 
    char *dest = "";  

    while (*src) 
    {
        int is_punct = ispunct((unsigned char)*src); 
        int is_space = isspace((unsigned char)*src); 

        if (!is_punct && !is_space)
        {
            *dest++ = (char)tolower((unsigned char)*src); 
        }
        src++;
    }
    *dest = '\0';*/
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

int print(char* pointers[])
{
    printf("THE TEXT:\n\n");

    for (int i = 0; i < MAX_ROWS; i++) 
    {
        if (pointers[i]    != NULL && 
            pointers[i][0] != '\0') 
        {
            printf("<<< i = %02d: ", i);
            printf("<%s>\n", pointers[i]);
        }
    }
    return 0;
}