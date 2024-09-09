#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define MAX_ROWS 15
#define MAX_COLS 260

int sort_string(char text[][MAX_COLS]);
int print(char text[][MAX_COLS]);
void str_cpy(char* dest, const char* src);
void clean_string(const char* src, char* dest);

int main(void)
{
    char text[MAX_ROWS][MAX_COLS] = {};

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
        printf("%d...", row);
        if (fgets(text[row], MAX_COLS, file) == NULL)
            break; 

        // Удаление символа новой строки
        int symbol_gad = strcspn(text[row], "\n");  
        char* address_gad = strchr(text[row], '\n');

        assert(symbol_gad == address_gad - text[row]);
          
        if (text[row][symbol_gad] == '\n')
        {
            text[row][symbol_gad] = '\0';
        }

        row++; 
    }

    printf("calling fclose...\n");
    fclose(file);
    printf("exited from fclose\n");

    printf("<<<<< sort_string\n");
    sort_string(text);
    printf(">>>>> sort_string\n");
    
    printf("calling print():\n");
    print(text);

    return 0;
}

#include <ctype.h>

void clean_string(const char* src, char* dest)
{
    while (*src)
    {
        int is_punct = ispunct((unsigned char)*src); // проверка на пунктуацию 
        int is_space = isspace((unsigned char)*src); // проверка на пробелы, отступы и тд
        
        if (is_punct == 0 && is_space == 0) 
        {
            *dest++ = (char)tolower((unsigned char)*src);  // Записываем символ в нижнем регистре
        }
        src++;
    }
    *dest = '\0';
}


int sort_string(char text[][MAX_COLS])
{
    char ded[MAX_COLS] = "";
    char clean_a[MAX_COLS] = "";
    char clean_b[MAX_COLS] = "";

    for (int i = 0; i < MAX_ROWS; i++)
    {
        printf("\nSTARTING PASS i = %d\n", i);

        for (int j = 0; j < MAX_ROWS - 1 - i; j++)
        {
            printf("j = %d...\n", j);
            assert(0 <= j && j < MAX_ROWS);
            assert(0 <= j + 1 && j + 1 < MAX_ROWS);

            printf("<%s> vs <%s>...\n", text[j], text[j + 1]);

            // Очищаем строки от знаков препинания и пробелов
            clean_string(text[j],     clean_a);
            clean_string(text[j + 1], clean_b);

            if (strcasecmp(clean_a, clean_b) > 0) 
            {
                printf("Swapping...\n");

                str_cpy(ded, text[j]);
                str_cpy(     text[j], text[j + 1]);
                str_cpy(              text[j + 1], ded);
    
                printf("<%s> vs <%s>: SWAPPED\n", text[j], text[j + 1]);
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

int print(char text[][MAX_COLS])
{
    printf("THE TEXT:\n\n");

    for (int i = 0; i < MAX_ROWS; i++) 
    {
        printf("<<< i = %02d: ", i);
        printf("<%s>\n", text[i]);
    }
    return 0;
}
