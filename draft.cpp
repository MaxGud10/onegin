#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_ROWS 15
#define MAX_COLS 260

int sort_string(char text[][MAX_COLS]);
int print(char text[][MAX_COLS]);
void str_cpy_mx_kshtschTVR (char* dest, const char* src);

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
        printf ("%d...", row);
        if (fgets(text[row], MAX_COLS, file) == NULL)
            break; 

        // Удаление символа новой строки
        int symbol_gad    = strcspn(text[row], "\n");  
        char* address_gad = strchr (text[row], '\n');

 //     printf("symbol_gad = %d\n", symbol_gad);
        assert (symbol_gad == address_gad - text[row]);
          
        if (text[row][symbol_gad] == '\n')
        {
            text[row][symbol_gad] = '\0';
        }

        row++; 
//  printf("row = %d\n", row);
    }

    printf ("calling fclose...\n");

    fclose(file);

    printf ("exited from fclose\n");


    printf("<<<<< sort_string");
    sort_string(text);
    printf(">>>>> sort_string");
    
    printf ("calling print():\n");
    print(text);
    /*for (int i = 0; i < count; i++) 
    {
        printf("%s\n", lines[i]);
    }*/

    return 0;
}

int sort_string(char text[][MAX_COLS])
{
    char ded[MAX_COLS] = "";

    for (int i = 0; i < MAX_ROWS; i++)
    {
        printf("\nSTARTING PASS i = %d\n", i);

        for (int j = 0; j < MAX_ROWS - 1 - i; j++)
        {
            printf("j = %d...\n", j);
            assert(0 <= j   && j < MAX_ROWS);
            assert(0 <= j+1 && j+1 < MAX_COLS);

            printf ("<%s>\tvs <%s>...", text[j], text[j+1]);

            if (/*strlen(text[i]) > 0 && strlen(text[j]) > 0 &&*/ strcmp(text[j], text[j+1]) > 0) 
            {
                printf ("Swapping...\n");

                str_cpy_mx_kshtschTVR (ded, text[j]);
                str_cpy_mx_kshtschTVR (     text[j], text[j+1]);
                str_cpy_mx_kshtschTVR (              text[j+1], ded);
    
                printf ("<%s>\tvs <%s>: SWAPPED\n", text[j], text[j+1]);
            }
        }
        
        printf("END PASS %d\n", i);
    }
    return 0;
}

void str_cpy_mx_kshtschTVR (char* dest, const char* src)
{
    assert (src);
    assert (dest);

    for (int i = 0; i < MAX_COLS; i++)
    {
        dest[i] = src[i];
    } 
}

int print(char text[][MAX_COLS])
{
    printf ("THE TEXT:\n\n");

    for (int i = 0; i < MAX_ROWS; i++) 
    {
        printf("<<< i = %02d: ", i);
//        if (strlen(text[i]) > 0) // Печатаем только инициализированные строки
        {
            printf("<%s>\n", text[i]);
        }
    }
    return 0;
}
