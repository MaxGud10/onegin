#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <cstring>  
#include <assert.h>
#include <ctype.h> 
#include <unistd.h>

const int MAX_ROWS = 100;

const char *TEXT_ONEGIN1 = "onegin.txt";
const char *TEXT_ONEGIN2 = "onegin_1.txt";

enum OneginError
{
    ONEGIN_FILE_NO_OPEN              = -1,
    ONEGIN_ERROR_ALLOCATION_BUFFER   = -2,
    ONEGIN_ERROR_ALLOCATION_POINTERS = -3,
    ERRO_R                           = -4,// TODO: ONEGIN_ERROR_NAME по name понятна суть должна быть
    ONEGIN_STATUS_OK                 =  0
};

enum WRITE_FILE
{
    TEXT_ONEGIN_1 = 1,
    TEXT_ONEGIN_2 = 2
};

struct Text_t //Text
{
    char*  buffer; //  массив для буфера
    int    text_len;
    char** pointers;
    int    lines_number;
    int difference_pointer;
};

//int write_command (int arg_c,const char *arg_v[], const char* file_to_read);
enum OneginError process_file (struct Text_t* onegin, const char* txt_file);
void print_error_message (enum OneginError error);
enum OneginError read_file (struct Text_t* onegin, const char* txt_file);
int print_lines (const char* str);
int check_excess_forward (char* pointer);
int check_excess_reverse(char* pointer, int length);
int bubble_sort (struct Text_t *onegin);
enum OneginError buffer_alloc (struct Text_t* onegin, const char* txt_file);
int str_case_cmp (char* pointer, char *pointer_1);
int reverse_comparator(char* pointer, char *pointer_1);
int print (struct Text_t* onegin);
OneginError pointers_alloc (struct Text_t* onegin);
int file_get_length (const char* str);
void cod_sym(const char* str);

int main (int arg_c, const char* arg_v[]) // TODO: получать название файла через командную строку ./output "onegin.txt" <--- argv[1] 
{
    printf ("arg_c = %d, arg_v[0] = %s, arg_v[1] = %s", arg_c, arg_v[0], arg_v[1]);

    if (arg_c != 2) 
    {
        printf ("Usage: %s <filename>\n", arg_v[0]);
        return -1;
    }
    
    const char* txt_file = arg_v[1];

    int mode = 0;
    if (access (txt_file, mode) != 0)
    {
        printf ("There is no such file: \"%s\"\n", arg_v[1]);
        return -1;
    }

    

    //const char* file_to_read = NULL;
    //write_command (arg_c, arg_v, file_to_read);

    struct Text_t onegin = {};

    printf (">>>> process_file\n");
    OneginError mxlx = process_file (&onegin, txt_file);
    if (mxlx != ONEGIN_STATUS_OK)
    {
        //print_error_message (error);
        //enum OneginError error = print_error_messega (er)

        return mxlx;
    }

    printf ("\n<<<< process_file\n");

    bubble_sort (&onegin);

    print (&onegin);
    // TODO сделать в функцию очистку памяти 
    free (onegin.buffer);
    free (onegin.pointers); // TODO: assert!
    return 0; 
}

/*int write_command (int arg_c,const char *arg_v[], const char* file_to_read)
{
    if (arg_c > 1 && (strcmp (arg_v[1], TEXT_ONEGIN1) == 0))
    {
        file_to_read = TEXT_ONEGIN1;
    } // TODO access

    if (arg_c > 1 && (strcmp (arg_v[1], TEXT_ONEGIN2) == 0))
    {
        file_to_read = TEXT_ONEGIN2;
    }

    if (file_to_read == NULL)
    {
        printf ("Ошибка: Неизвестный аргумент: %s\n", arg_v[1]);
        return -1;
    }

    return 0;
}*/

void print_error_message (enum OneginError error)
{
    switch (error)
    {
        case ONEGIN_FILE_NO_OPEN:
            printf ("Error: File could not be opened.\n");
            break;
        case ONEGIN_ERROR_ALLOCATION_BUFFER:
            printf ("Error: Buffer allocation failed.\n");
            break;
        case ONEGIN_ERROR_ALLOCATION_POINTERS:
            printf ("Error: Pointers allocation failed.\n");
            break;
        case ERRO_R:
            printf ("DED_LOh");
            break;
        default:
            printf ("Unknown error occurred: %d\n", error);
            break;
    }
}

enum OneginError process_file (struct Text_t* onegin, const char* txt_file) // TODO: return value - enum OneginError
{
    assert (onegin != NULL);
    printf ("\n<<<< read_file\n");
    enum OneginError error = read_file (onegin, txt_file); 
    printf (">>> read_file\n");

    if (error != ONEGIN_STATUS_OK)
    {
        return error; 
    }
 
    int lines = 0;
    (onegin->pointers)[lines] = onegin->buffer; // onegin->buffer + 0
    lines++;
    for (int postion = 0; postion < onegin->text_len; postion++) //  цикл по всем символам из буфера. если начало строки,
    {                                                            //  эсправедь апичатгу
        if ((onegin->buffer) [postion] == '\n')
        {
            printf ("< <pointers (%p)> > = buffer (%s) + postion (%d)", onegin -> pointers, onegin->buffer, postion + 1);
            (onegin->pointers) [lines] = onegin->buffer + postion + 1; // присваивает адрес первого элемента из масcива buffer в массив postion  
            lines++; 
        } 

        //onegin->difference_pointer = onegin->pointers[lines] - onegin->pointers[lines - 1];

        if (onegin->buffer[postion] == '\r')
        {
            printf ("\nonegin->buffer = %p\n", onegin->buffer);
            onegin->buffer[postion] = '\0';
            continue;
        }
//[100][110][120][130][140][150][160][170]; 
//[  0 ][ 1  ][ 2  ][ 3   ][   ][   ][   ]  *140 -*130
        onegin->lines_number = lines;
         //\r\n
            //\n
        //return 0;
        //[a][n][d][\0][\n][B][][][] // TODO сделать if ++ и +1 (обработка \r\n)
    }

    for (int i = 0; i < lines; i++)   // сдлнф
    {
       print_lines ((onegin->pointers)[i]);
    }

    return ONEGIN_STATUS_OK;
}

enum OneginError read_file (struct Text_t* onegin, const char* txt_file) // TODO: разбить на более мелкие функции
{
    FILE* file = fopen (txt_file, "rb");
    printf ("<<< file opens\n");
    if (file == NULL)
    {
        printf ("File opening error: txt_file = (%s)\n", txt_file);
        return ONEGIN_FILE_NO_OPEN;
    }
    

    enum OneginError error = buffer_alloc (onegin, txt_file);
    if (error != ONEGIN_STATUS_OK) 
    {
        fclose (file);
        return error;
    }

    int nSmblsRd = fread (onegin->buffer, sizeof (char), onegin->text_len, file);
    fclose (file);

    if (nSmblsRd != onegin->text_len)
        printf ("ERRMSG: nSmblsRd (%d) != onegin->text_len (%d)\n"
                "+       and additionally, MAX is LOX.\n", nSmblsRd, onegin->text_len);

    printf ("\n<<<fread\n");
    
    int num_lines = 0; // количество строчек // TODO: переименовать (например, number_lines)
    for (int i = 0; i < onegin->text_len; i++) // цикл для прохождения по всем символам для проверки на \n  (сколько строк)
    {
        //printf ("i = %d - buffer[i] = %d, buffer[i] ='%c'\n", i, (onegin -> buffer)[i], (onegin -> buffer)[i]);
        
        if ((onegin->buffer) [i] == '\n')
        {
            num_lines++;
            printf ("\nCOUNT = %d\n", num_lines);
        } 
    }

//  onegin->lines_number = CountSymbol (onegin->buffer, onegin->text_len, '\n');
 
    onegin->lines_number = num_lines;

    error = pointers_alloc (onegin);
    if (error != ONEGIN_STATUS_OK) 
    {
        return error;
    }


    return ONEGIN_STATUS_OK;
}

enum OneginError buffer_alloc (struct Text_t* onegin, const char* txt_file)
{
    int size = file_get_length (txt_file);
    if (size < 0) 
    {
        return ONEGIN_ERROR_ALLOCATION_BUFFER;
    }

    char* tmp_buffer = (char*) calloc (size + 1, sizeof (char)); // выделяю память для буфера //TODO: +1 для \0

    if (tmp_buffer == NULL) 
    {
        printf ("Incorrect memory allocation in tmp_buffer\n");
        return ONEGIN_ERROR_ALLOCATION_BUFFER;
    }

    onegin->buffer   = tmp_buffer; 
    onegin->text_len = size;

    return ONEGIN_STATUS_OK;
}

int file_get_length (const char* str)
{
    struct stat text_len  = {}; 

    stat (str, &text_len);  
    printf ("text.st_size = %ld\n", text_len.st_size);

    return text_len.st_size;
}

OneginError pointers_alloc (struct Text_t* onegin)
{
    char** tmp_pointers = (char**) calloc (onegin->lines_number + 1, sizeof (char*));

    if (tmp_pointers == NULL)
    {
        printf ("Incorrect memory allocation in tmp_pointers\n");
        return ONEGIN_ERROR_ALLOCATION_POINTERS;
    }

    onegin->pointers = tmp_pointers;

    return ONEGIN_STATUS_OK;
}

int print_lines (const char* str)
{
    while (1)
    {
        if (*str == '\n') break;
        if (*str == '\0') break;

        putchar (*str); // printf (%c)
        str++;
    }
    return 0;
}

int bubble_sort (struct Text_t *onegin)
{
//  char* ded =  сам ты  NULL;

    for (int i = 0; i < onegin -> lines_number - 1; i++)
    {
        printf ("\nSTARTING PASS i = %d\n", i);
        for (int j = 0; j < onegin -> lines_number - 1 - i; j++)
        {
            printf ("j = %d...\n", j);
            printf ("i = %d...\n", i);
            //assert ();
            printf ("<%s> vs <%s>...\n", onegin->pointers[j], onegin->pointers[j + 1]);
            //if (str_case_cmp ((onegin -> pointers) [j], (onegin -> pointers) [j + 1]) > 0)
            if (reverse_comparator((onegin->pointers)[j], (onegin -> pointers)[j + 1]) < 0)
            {
                char* ded                   = (onegin -> pointers)[j];
                (onegin -> pointers)[j]     = (onegin -> pointers)[j + 1];
                (onegin -> pointers)[j + 1] = ded;
            }
        }

        printf ("END PASS %d\n", i);
    }
    return 0;
}
// сделаем общий компаратор 
// вызывается с 5 параметрами  ( строки и их длины и в какую сторону двигается )
// 
// в бабал сорте делается общие компараторы 
// TODO МЕГАВАЖНО - сделать параметры в структуре 
int reverse_comparator(char* pointer, char *pointer_1)
{
    int length   = strlen (pointer  );
    int length_1 = strlen (pointer_1);

    length   = check_excess_reverse (pointer  , length   - 1 );
    length_1 = check_excess_reverse (pointer_1, length_1 - 1);
    printf("length = ((%d)), length_1 = ((%d)))", length, length_1);

    printf("");
// структуру 
    for (;;)
    {
        if (length < 0 || length_1 < 0) 
            break;
        int symbol1 = tolower (pointer  [length  ]);
        int symbol2 = tolower (pointer_1[length_1]);
        //printf ("\nsymbol1 = %d, (%c)\n", symbol1, symbol1);
        //printf ("\nsymbol2 = %d, (%c)\n", symbol2, symbol2);
        if (symbol1 != symbol2)
        {
            printf ("\nSYMBOL1 = %d, (%c)\n", symbol1, symbol1);
            printf ("\nSYMBOL2 = %d, (%c)\n", symbol2, symbol2); // bb - aa = 98 - 97

            printf ("\nsyMbol1(%d) - syMol1(%d) = %d\n",symbol1, symbol2, (symbol1- symbol2));
            return (symbol1 - symbol2);

        }
    
        length--;
        length_1--;
    }

    //printf("<<_<<%s>>_>>, <<_____<<%s>>____>>, length = %d, length_1= %d", pointer, pointer_1, length, length_1);
    return 0;
} 

int str_case_cmp (char* pointer, char *pointer_1) // сделвть чтобы компоратор был без x 
{
    int start_of_ptr   = check_excess_forward (pointer  );
    int start_of_ptr_1 = check_excess_forward (pointer_1);
    
    int x = 0;
    for (; x < MAX_ROWS - 1; x++)
    {
        int symbol1 = tolower (pointer  [start_of_ptr   + x]);
        int symbol2 = tolower (pointer_1[start_of_ptr_1 + x]);
        printf ("\nsymbol1 = %d, (%c)\n", symbol1, symbol1);
        printf ("\nsymbol2 = %d, (%c)\n", symbol2, symbol2);
        if (symbol1 != symbol2)
        {
            printf ("\nsymbol2 - symbol1 = %d\n", symbol2- symbol1);
            return - (symbol2 - symbol1);
        }

        if (symbol1 == '\0' || symbol2 == '\0')
        {
            printf ("\nsymbol1 = %d and symbol2 = %d\n", symbol1, symbol2);
            return 0;
        }
    }
    
    return 0;
}

int check_excess_forward (char* pointer)
{
    int i = 0;
    while (pointer[i] != '\0')
    {
        //printf ("\ni = %d, pointer = %s, pointer[%d]\n ", i, pointer, pointer[i] );
        int is_space = isspace (pointer[i]); 
        int is_punct = ispunct (pointer[i]);
        printf ( "__is_space1 = %d, is_punct1 = %d__", is_space, is_punct);  

        if (!is_space &&
            !is_punct)
            break;   

        i++;
    }
    return i;
}

int check_excess_reverse(char* pointer, int length)
{

    printf("<<< length = %d, pointers = %s, pointer[length - 1] = %d\n", length, pointer, pointer[length]);
    while (1)
    {
        if (length == 0)
            break;
        //printf ("\ni = %d, pointer = %s, pointer[%d]\n ", i, pointer, pointer[i] );
        int is_space = isspace (pointer[length]); 
        int is_punct = ispunct (pointer[length]);
        printf ( "__is_space1 = %d, is_punct1 = %d__\n", is_space, is_punct);  

        if (!is_space &&
            !is_punct)
            break;   

        length--;
    }
    printf(">>> length = %d, pointers = %s, pointer[length - 1] = %d\n", length, pointer, pointer[length]);
    return length;
}

int print (struct Text_t* onegin)
{
    printf ("THE TEXT:\n\n");

    for (int i = 0; i < onegin -> lines_number; i++) 
    {
        if (onegin -> pointers[i]    != NULL && 
            onegin -> pointers[i][0] != '\0') 
        {
            printf ("<<< i = %02d: ", i);
            printf ("<%s>\n", onegin -> pointers[i]);

//            cod_sym(onegin->pointers[i]);
        }
    }
    return 0;
} // TODO санитайзер 

void cod_sym(const char* str)
{   
    printf("hahaha");
    for (int i = 0; str[i] != '\0' ; i++)
    {
        printf("<%d|%c>", str[i], str[i]);
    }
    printf("\n");
}