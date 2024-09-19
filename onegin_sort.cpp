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

struct String_t 
{
    char*  str;
    size_t len_str; 
};

typedef int (*compare_funct) (const void* str_line1, const void* str_line2);

struct Text_t //Text
{
    char*  buffer; //  массив для буфера
    int    buffer_len;  // длина буфера
    char** pointers; // массив указателей 
    int    lines_number; // количество строчек 
    char*  ded;          // лох 
    struct String_t *data;  // массив указателей на строки 
};

enum OneginError process_file (struct Text_t* onegin, const char* txt_file);
void             print_error_message (enum OneginError error);
enum OneginError read_file (struct Text_t* onegin, const char* txt_file);
int              print_lines (const char* str);
int              check_excess_forward(const char* pointer);
int              check_excess_reverse(const char* pointer, int length);
//int              bubble_sort (struct Text_t *onegin);
void             bubble_sort (void* data, int lines_number, size_t pointer_size, compare_funct);
void             swap(void* data1, void* data2, size_t pointer_size);
int              comparator_reverse (const void* str_line, const void* str_line1);
int              comparator_forward (const void* str_line, const void* str_line1);
//int              bubble_sort (struct Text_t *onegin, size_t size, int(*comparator_forward(String_t*, String_t*)));
enum OneginError alloc_buffer (struct Text_t* onegin, const char* txt_file);
//int comparator_forward (char* pointer, char *pointer_1);
//int              comparator_forward (struct String_t* str_line, struct String_t* str_line1);
//int              comparator_reverse (struct String_t* str_line, struct String_t* str_line1);
int              print (struct Text_t* onegin);
OneginError      alloc_pointers (struct Text_t* onegin);
int              file_get_length (const char* str);
void             cod_sym(const char* str); // для отладки 

int main (int arg_c, const char* arg_v[]) 
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

    struct Text_t onegin = {};

    printf ("\n>>>> process_file\n");
    OneginError ddlx = process_file (&onegin, txt_file);
    if (ddlx != ONEGIN_STATUS_OK)
    {
        //print_error_message (error);
        //enum OneginError error = print_error_messega (er)

        return ddlx;
    }

    printf ("\n<<<< process_file\n");

    //bubble_sort (&onegin);
    //bubble_sort (&onegin, onegin.lines_number, sizeof(String_t), comparator_forward); // сам массив, размер ячеек памяти для одной строчки  

    print (&onegin);
    // TODO сделать в функцию очистку памяти 
    free (onegin.buffer);
    free (onegin.pointers); // TODO: assert!
    free (onegin.data);

    return 0; 
}

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
    printf ("\n>>> read_file\n");

    if (error != ONEGIN_STATUS_OK)
    {
        return error; 
    }
    
 // надо сделаь цикл через структуру string_t
    int lines = 0; // 
    ((onegin->data) [lines]).str = onegin->buffer; // onegin->buffer + 0
    lines++;
    //for (int position = 0; position < onegin->onegin->data.tlen_str; position++) //  цикл по всем символам из буфера. если начало строки,
    for (int position = 0; position < onegin->buffer_len; position++) // 
    {                                                           // (onegin->data.len_str)
        if ((onegin->buffer) [position] == '\n')
        {
            printf ("< <pointers (%p)> > = buffer (%s) + position (%d)\n", onegin -> pointers, onegin->buffer, position + 1);
            (onegin->data) [lines].str = onegin->buffer + position + 1; // присваивает адрес первого элемента из масcива buffer в массив position  
            
            //(onegin->data)[lines].len_str = onegin->lines_number;

            int len = strchr ((onegin->data)[lines].str, '\n') - (onegin->data)[lines].str - 1;
            assert(len >= 0);
            //size_t str_len = strlen((onegin->data)[lines].str);
            printf("\n**lines = %d, len = %d, stroka = (%s), \n", lines, len, (onegin->data) [lines].str);
            //(onegin->data) [lines].str 

            lines++; 
        }

        if (onegin->buffer [position] == '\r')
        {
            printf ("\nonegin->buffer = %p\n", onegin->buffer);
            onegin->buffer [position] = '\0';
            continue;
        }

        printf("symbol_end = <%c>, => (%s)\n", onegin->buffer [position], onegin->buffer [position] );

        //printf("num_line = {%s}", onegin->buffer[position]);
        //onegin->data[] = 
    }

    for (int i = 0; i < lines; i++)   // сдлнф
    {
       //print_lines ((onegin->pointers) [i]);
        print_lines((onegin->data [i].str));
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

    enum OneginError error = alloc_buffer (onegin, txt_file);
    if (error != ONEGIN_STATUS_OK) 
    {
        printf("ERROR: ONEGIN_STATUS_OK");
        fclose (file);
        return error;
    }

    int num_symbol = fread (onegin->buffer, sizeof (char), onegin->buffer_len, file); // ? onegin->data[0].len_str
    fclose (file);

    onegin->buffer[num_symbol    ] ='\r'; 
    onegin->buffer[num_symbol + 1] = '\n';

    if (num_symbol != (onegin->buffer_len))
        printf ("ERRMSG: num_symbol (%d) != onegin->buffer_len (%d)\n", num_symbol, onegin->buffer_len);

    printf ("\n<<<fread\n");
    
    int num_lines = 0; // количество строчек 
    for (int i = 0; i < (onegin->buffer_len); i++) // цикл для прохождения по всем символам для проверки на \n  (сколько строк)
    {
        //printf ("i = %d - buffer[i] = %d, buffer[i] ='%c'\n", i, (onegin -> buffer)[i], (onegin -> buffer)[i]);
        
        if ((onegin->buffer) [i] == '\n')
        {
            num_lines++;
            printf ("\nCOUNT = %d\n", num_lines);
        } 
    }



//  onegin->lines_number = CountSymbol (onegin->buffer, onegin->buffer_len, '\n');
 
    onegin->lines_number = num_lines + 1;

    printf("lines_number = %d\n", onegin->lines_number);

    error = alloc_pointers (onegin);
    if (error != ONEGIN_STATUS_OK) 
    {
        return error;
    }

    return ONEGIN_STATUS_OK;
}

enum OneginError alloc_buffer (struct Text_t* onegin, const char* txt_file)
{
    printf("<<< alloc_buffer\n");
    int size = file_get_length (txt_file);
    if (size < 0) 
    {
        return ONEGIN_ERROR_ALLOCATION_BUFFER;
    }

    char* tmp_buffer = (char*) calloc (size + 3, sizeof (char)); // выделяю память для буфера //TODO: +1 для \0
    printf("++___tmp_buffer___++\n");

    if (tmp_buffer == NULL) 
    {
        printf ("Incorrect memory allocation in tmp_buffer\n");
        return ONEGIN_ERROR_ALLOCATION_BUFFER;
    }

    onegin->buffer   = tmp_buffer; 
    printf("??data??\n");
    onegin->buffer_len = size + 3;
    printf("??size??\n");
    printf("size = %d\n", size + 3);

    return ONEGIN_STATUS_OK;
}

int file_get_length (const char* str)
{
    struct stat buffer_len  = {}; 

    stat   (str, &buffer_len);  
    printf ("text.st_size = %ld\n", buffer_len.st_size);

    return buffer_len.st_size;
}

OneginError alloc_pointers (struct Text_t* onegin)
{
    printf("<<<< alloc_pointers\n");
    //char** tmp_pointers = (char**) calloc (onegin->lines_number + 1, sizeof (char*));
    struct  String_t* alloc = (struct String_t*) calloc (onegin->lines_number, sizeof(struct String_t)); 

    /*if (tmp_pointers == NULL)
    {
        printf ("Incorrect memory allocation in tmp_pointers\n");
        return ONEGIN_ERROR_ALLOCATION_POINTERS;
    }*/

   if (alloc == NULL)
   {
        printf ("Incorrect memory allocation in tmp_pointers\n");
        return ONEGIN_ERROR_ALLOCATION_POINTERS;    
   }
    
    onegin->data = alloc; 

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


//int bubble_sort (struct Text_t *onegin, size_t size, int(*comparator_forward(String_t*, String_t*))) // перераспределение ( партишин )
void bubble_sort (void* data, int lines_number, size_t pointer_size, compare_funct comparator)
{
    for (int i = 0; i < (lines_number) - 1; i++)// onegin->dta.len_str
    {
        printf ("\nSTARTING PASS i = %d\n", i);
        for (int j = 0; j < (lines_number) - 1 - i; j++)
        {
            printf ("j = %d...\n", j);
            printf ("i = %d...\n", i);
            //printf ("<%s> vs <%s>...\n", (onegin->pointers) [j], (onegin->pointers) [j + 1]);
            //if (comparator_forward ((onegin -> pointers) [j], (onegin -> pointers) [j + 1]) > 0)
            // if (comparator_reverse((onegin->pointers) [j], (onegin->pointers) [j + 1]) < 0)
           //if (comparator_forward (&(onegin->data)[j], &(onegin->data)[j + 1]) > 0)
           //if (comparator_reverse(&(onegin->data)[j], &(onegin->data)[j + 1]) < 0)  // (*onegin).data[j].str
           if (comparator((char*)data +  j      * pointer_size, 
                          (char*)data + (j + 1) * pointer_size) > 0)// len_str - длина строчки 
            {
                /*char* ded                 = (onegin->data)[j].str;
                (onegin->data)[j].str     = (onegin->data)[j + 1].str;
                (onegin->data)[j + 1].str = ded;*/

                swap((char*)data + j * pointer_size, (char*)data + (j + 1) * pointer_size, pointer_size);
            }
        }

        printf ("END PASS %d\n", i);
    }
}

void swap(void* data1, void* data2, size_t pointer_size)
{
    char* bytes1 = (char*)data1;
    char* bytes2 = (char*)data2;

    for (size_t i = 0; i < pointer_size; i++)
    {
        char ded  = bytes1[i];
        bytes1[i] = bytes2[i];
        bytes2[i] = ded;
    }
}

//int comparator_reverse (struct String_t*  str_line, struct String_t* str_line1)
int comparator_reverse (const void* str_line,const void* str_line1)
{
    const char* string1 = ((const String_t*) str_line) -> str;
    const char* string2 = ((const String_t*) str_line1)-> str;
    
    int length   = strlen (string1);
    int length_1 = strlen (string2);

    length   = check_excess_reverse (string1, length  - 1);
    length_1 = check_excess_reverse (string2, length_1 - 1);
    printf ("length = ((%d)), length_1 = ((%d)))", length, length_1);

// структуру 
    for (;;)
    {
        if (length < 0 || length_1 < 0) 
            break;
        int symbol1 = tolower (string1[length  ]);
        int symbol2 = tolower (string2[length_1]);
        //printf ("\nsymbol1 = %d, (%c)\n", symbol1, symbol1);
        //printf ("\nsymbol2 = %d, (%c)\n", symbol2, symbol2);
        if (symbol1 != symbol2)
        {
            printf ("\nSYMBOL1 = %d, (%c)\n", symbol1, symbol1);
            printf ("\nSYMBOL2 = %d, (%c)\n", symbol2, symbol2); // bb - aa = 98 - 97

            printf ("\nsyMbol1(%d) - syMol1(%d) = %d\n", symbol1, symbol2, (symbol1- symbol2));
            return (symbol1 - symbol2);

        }
    
        length--;
        length_1--;
    }

    return 0;
} 

int comparator_forward (const void* str_line, const void* str_line1)
{
    $$;
    const char* string1 = ((const String_t*) str_line )->str;
    const char* string2 = ((const String_t*) str_line1)->str;


    $(((const String_t*) str_line  )->len_str);
    $(((const String_t*) str_line1 )->len_str);

    printf("\nstring1 = (%d) - string1 - '%s'\n", strlen(string1), string1);
    //printf("\nstring2 = (%d) - string2 - '%s'\n", strlen(string2), string2);
    $(string1);
    $(string2);
    //printf("");
    
    $$;
    int start_of_ptr   = check_excess_forward (string1); // TODO: переименовать номера
    $$;
    int start_of_ptr_1 = check_excess_forward (string2);
    $$;
    
    for (int i = 0;; i++) 
    {
        $$;
        int symbol1 = tolower (string1  [start_of_ptr   + i]);
        int symbol2 = tolower (string2  [start_of_ptr_1 + i]);
        printf ("\nsymbol1 = %d, (%c)\n", symbol1, symbol1);
        printf ("\nsymbol2 = %d, (%c)\n", symbol2, symbol2);
        if (symbol1 != symbol2)
        {
            printf ("\nsymbol2 - symbol1 = %d\n", symbol2- symbol1);
            $$;
            return - (symbol2 - symbol1);
        }

        if (symbol1 == '\0' && symbol2 == '\0')
        {
            printf ("\nsymbol1 = %d and symbol2 = %d\n", symbol1, symbol2);
            $$;
            return 0;
        }
    }
    $$;
    return 0;
}

int check_excess_forward (const char* pointer)
{
    int i = 0;
    while (pointer[i] != '\0')
    {
        //printf ("\ni = %d, pointer = %s, pointer[%d]\n ", i, pointer, pointer[i] );
        int is_space = isspace (pointer [i]); 
        int is_punct = ispunct (pointer [i]); // TODO: isalnum ();
        printf ( "__is_space1 = %d, is_punct1 = %d__", is_space, is_punct);  

        if (!is_space &&
            !is_punct)
            break;   

        i++;
        $$;
        //printf("");
    }
    $$;
    return i;
}

int check_excess_reverse(const char* pointer, int length)
{

    printf("<<< length = %d, pointers = %s, pointer[length - 1] = %d\n", length, pointer, pointer[length]);
    while (1)
    {
        if (length == 0)
            break;
        //printf ("\ni = %d, pointer = %s, pointer[%d]\n ", i, pointer, pointer[i] );
        int is_space = isspace (pointer [length]); 
        int is_punct = ispunct (pointer [length]);
        printf ( "__is_space1 = %d, is_punct1 = %d__\n", is_space, is_punct);  

        if (!is_space && !is_punct)
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
        if ((onegin->data)[i].str    != NULL && 
            (onegin->data)[i].str[0] != '\0') 
        {
            printf ("<<< i = %02d: ", i);
            printf ("<%s>\n", (onegin->data)[i].str);

//            cod_sym(onegin->pointers[i]);
        }
    }
    return 0;
} // TODO санитайзер погуглить что это 

void cod_sym (const char* str)
{   
    for (int i = 0; str[i] != '\0' ; i++)
    {
        printf ("<%d|%c>", str[i], str[i]);
    }
    printf ("\n");
}