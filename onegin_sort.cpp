#define DEBUG

#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <cstring>  
#include <assert.h>
#include <ctype.h> 
#include <unistd.h> // TODO: на файлы разбить

#ifdef DEBUG
    #define DBG  if (1)
#else
    #define DBG  if (0) 
#endif

enum OneginError
{
    ONEGIN_ERROR_FILE_NO_OPEN        = -1, // TODO: сделать общую приставку ERROR
    ONEGIN_ERROR_ALLOCATION_BUFFER   = -2,
    ONEGIN_ERROR_ALLOCATION_POINTERS = -3,
    ERRO_R                           = -4,
    ONEGIN_STATUS_OK                 =  0
};

struct String_t 
{
    char*  str;
    size_t len_str; 
};

typedef int (*compare_funct) (const void* str_line1, const void* str_line2);

#define ded max

struct Text_info // TODO: TextInfo переименовать
{
    char*  buffer;          //  массив для буфера
    int    buffer_len;      // длина буфера
    char** pointers;        // массив указателей 
    int    lines_number;    // количество строчек 
    char*  ded;             // лох 
    struct String_t *data;  // массив указателей на строки 
};

enum OneginError process_file         (struct Text_info* onegin, const char* txt_file); // TODO: выравнять аргументы
enum OneginError read_file            (struct Text_info* onegin, const char* txt_file);
enum OneginError alloc_buffer         (struct Text_info* onegin, const char* txt_file);
OneginError      alloc_pointers       (struct Text_info* onegin);
int              do_process           (struct Text_info* onegin, int lines);
int              file_get_length      (const char* str);
int              count_string         (struct Text_info* onegin, char ddlx);
int              print_lines          (const char* str);
void             print_error_message  (enum OneginError error);

void             bubble_sort          (void* data,  int lines_number, size_t pointer_size, compare_funct);

void             swap                 (void* data1, void* data2,      size_t pointer_size);

int              comparator_reverse   (const void* str_line1, const void* str_line2);
int              comparator_forward   (const void* str_line1, const void* str_line2);
int              check_excess_forward (const char* pointer);
int              check_excess_reverse (const char* pointer, int length);

void             cod_sym              (const char* str); // для отладки

int              print                (struct Text_info* onegin);
void             check_strings_array  (struct Text_info* onegin);

int main (int arg_c, const char* arg_v[]) 
{
    printf ("arg_c = %d, arg_v[0] = %s, arg_v[1] = %s", arg_c, arg_v[0], arg_v[1]);

    if (arg_c != 2) 
    {
        printf ("Usage: %s <filename>\n", arg_v[0]); 
        printf("You need to enter it ./do and after the file with the text that will need to be sorted\n"
               "for example:\n"
               "./do onegin.txt");
        return -1;
    }   

    const char* txt_file = arg_v[1];

    int mode = 0;
    if (access (txt_file, mode) != 0)
    {
        printf ("There is no such file: \"%s\"\n", arg_v[1]);
        return -1;
    }

    struct Text_info onegin = {};

    DBG printf ("\n>>>> process_file\n");
    
    OneginError ded_ne_loh = process_file (&onegin, txt_file); // TODO: название поменять
    if (ded_ne_loh != ONEGIN_STATUS_OK)
    {
        print_error_message (ded_ne_loh);

        return ded_ne_loh;
    }

    DBG printf ("\n<<<< process_file\n");

    // Проверка на NULL перед сортировкой
    check_strings_array(&onegin);

    bubble_sort (onegin.data, onegin.lines_number, sizeof(String_t), comparator_reverse);
    printf      ("reverse(1)");
    print       (&onegin);

    bubble_sort (onegin.data, onegin.lines_number, sizeof(String_t), comparator_reverse);
    printf      ("reverse(2)");
    print       (&onegin);

    bubble_sort (onegin.data, onegin.lines_number, sizeof(String_t), comparator_forward); // сам массив, размер ячеек памяти для одной строчки
    printf      ("forward(1)");
    print       (&onegin);
    
    // TODO сделать в функцию очистку памяти 
    free (onegin.buffer);
    free (onegin.pointers); 
    free (onegin.data);

    return 0; 
}

void print_error_message (enum OneginError error)
{
    switch (error)
    {
        case ONEGIN_ERROR_FILE_NO_OPEN:
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

enum OneginError process_file (struct Text_info* onegin, const char* txt_file) 
{
    assert (onegin != NULL);

    DBG printf ("\n<<<< read_file\n");
    enum OneginError error = read_file (onegin, txt_file); 
    DBG printf ("\n>>> read_file\n");

    if (error != ONEGIN_STATUS_OK)
    {
        return error; 
    }

    int lines = 0;  
    ((onegin->data) [lines]).str = onegin->buffer;
    lines++;
        // TODO сделать в одну функцию и возвращать через return lines
    do_process(onegin, lines);

    /*lines++;
    //  цикл по всем символам из буфера. если начало строки,
    for (int position = 0; position < onegin->buffer_len; position++) // TODO: функция пусть выделяет память под массив указателей и расставляет их
    {  
        if ((onegin->buffer) [position] == '\n')
        {
            DBG printf("ded loh\n");
            DBG printf ("< <pointers (%p)> > = buffer (%s) + position (%d)\n", onegin -> pointers, onegin->buffer, position + 1);
            (onegin->data) [lines].str = onegin->buffer + position + 1; // присваивает адрес первого элемента из масcива buffer в массив position  
            
            char* chr = strchr ((onegin->data)[lines].str, '\n');
            int len =  chr - (onegin->data)[lines].str;
            if ( chr == NULL)
            {
                len = strlen((onegin->data)[lines].str);  
            }

            assert(len >= 0);
            (onegin->data)[lines].len_str = len;

            DBG printf("\n**lines = %d, len = %d, stroka = (%s), \n", lines, len, (onegin->data) [lines].str);

            lines++; 
        }

        if (onegin->buffer [position] == '\r')
        {
            DBG printf ("\nonegin->buffer = %p\n", onegin->buffer);
            onegin->buffer [position] = '\0';
        }
        DBG printf("position = %d\n", position);

        DBG printf("\nsymbol_end = <%c> => (%d)\n", onegin->buffer [position], onegin->buffer [position] );
    }*/
    $(((onegin->data) [lines]).str);
    $((onegin->data)[lines].len_str);


    for (int i = 0; i < lines; i++)   // сдлнф
    {
        DBG printf ("\n>>> printf_lines i = %d\n", i);
        print_lines((onegin->data [i].str));
    } // TODO: сделать функцию для распечатки

    return ONEGIN_STATUS_OK;
}

int do_process(struct Text_info* onegin, int lines)
{
    //  цикл по всем символам из буфера. если начало строки,
    for (int position = 0; position < onegin->buffer_len; position++) // TODO: функция пусть выделяет память под массив указателей и расставляет их
    {  
        if ((onegin->buffer) [position] == '\n')
        {
            //DBG printf("ded loh\n");
            //DBG printf ("< <pointers (%p)> > = buffer (%s) + position (%d)\n", onegin -> pointers, onegin->buffer, position + 1);
            (onegin->data) [lines].str = onegin->buffer + position + 1; // присваивает адрес первого элемента из масcива buffer в массив position  
            
            char* chr = strchr ((onegin->data)[lines].str, '\n');
            int len = chr - (onegin->data)[lines].str;
            if (chr == NULL)
            {
                len = strlen ((onegin->data)[lines].str);  
            }
// TODO сделать position + len 
            assert(len >= 0);
            (onegin->data)[lines].len_str = len;

            DBG printf("\n**lines = %d, len = %d, stroka = (%s), \n", lines, len, (onegin->data) [lines].str);

            lines++; 
        }

        if (onegin->buffer [position] == '\r')
        {
            DBG printf ("\nonegin->buffer = %p\n", onegin->buffer);
            onegin->buffer [position] = '\0';
        }

        DBG printf("position = %d\n", position);

        DBG printf("\nsymbol_end = <%c> => (%d)\n", onegin->buffer [position], onegin->buffer [position] );

    }

    DBG printf("<<<puts lines = %d\n", lines);

    return 0;
}

enum OneginError read_file (struct Text_info* onegin, const char* txt_file) // TODO: разбить на более мелкие функции
{
    FILE* file = fopen (txt_file, "rb");
    //DBG printf ("<<< file opens\n");
    if (file == NULL)
    {
        printf ("File opening error: txt_file = (%s)\n", txt_file);
        return ONEGIN_ERROR_FILE_NO_OPEN;
    }

    enum OneginError error = alloc_buffer (onegin, txt_file);
    if (error != ONEGIN_STATUS_OK) 
    {
        printf("ERROR: ONEGIN_STATUS_OK");
        fclose (file);
        return error;
    }

    int num_symbol = fread (onegin->buffer, sizeof (char), onegin->buffer_len, file);
    fclose (file);

    if (num_symbol != (onegin->buffer_len))
        printf ("ERRMSG: num_symbol (%d) != onegin->buffer_len (%d)\n", num_symbol, onegin->buffer_len);

    //DBG printf ("\n<<<fread\n");
    
    /*int num_lines = 0; // количество строчек 
    for (int i = 0; i < (onegin->buffer_len); i++) // цикл для прохождения по всем символам для проверки на \n  (сколько строк)
    {
        //DBG printf ("i = %d - buffer[i] = %d, buffer[i] ='%c'\n", i, (onegin -> buffer)[i], (onegin -> buffer)[i]);
        
        if ((onegin->buffer) [i] == '\n')
        {
            num_lines++;
            DBG printf ("\nCOUNT = %d\n", num_lines);
        } 
    }
 
    onegin->lines_number = num_lines + 1;

    DBG printf("lines_number = %d\n", onegin->lines_number);*/

    count_string(onegin, '\n');
    
    error = alloc_pointers (onegin);
    if (error != ONEGIN_STATUS_OK) 
    {
        return error;
    }

    return ONEGIN_STATUS_OK;
}

int count_string(struct Text_info* onegin, char ddlx)
{
    int num_lines = 0; // количество строчек 
    for (int i = 0; i < (onegin->buffer_len); i++) // цикл для прохождения по всем символам для проверки на \n  (сколько строк)
    {
        DBG printf ("i = %d - buffer[i] = %d, buffer[i] ='%c'\n", i, (onegin -> buffer)[i], (onegin -> buffer)[i]);
        
        if ((onegin->buffer) [i] == ddlx)
        {
            num_lines++;
            DBG printf ("\nCOUNT = %d\n", num_lines);
        } 
    }
     
    onegin->lines_number = num_lines + 1;

    DBG printf("lines_number = %d\n", onegin->lines_number);

    return 0;

}

enum OneginError alloc_buffer (struct Text_info* onegin, const char* txt_file)
{
    //DBG printf("<<< alloc_buffer\n");
    int size = file_get_length (txt_file);
    if (size < 0) 
    {
        return ONEGIN_ERROR_ALLOCATION_BUFFER;
    }

    char* tmp_buffer = (char*) calloc (size + 1, sizeof (char)); // [x] +1 для \0 // выделяю память для буфера

    if (tmp_buffer == NULL) 
    {
        printf ("Incorrect memory allocation in tmp_buffer\n");
        return ONEGIN_ERROR_ALLOCATION_BUFFER;
    }

    onegin->buffer     = tmp_buffer; 
    onegin->buffer_len = size; // [x] + 3  
    //DBG printf("\nsize = %d\n", size);  

    return ONEGIN_STATUS_OK;
}

int file_get_length (const char* str)
{
    struct stat buffer_len  = {}; 

    stat (str, &buffer_len);  
    // TODO проверить на null 
    DBG printf ("text.st_size = %ld\n", buffer_len.st_size);

    return buffer_len.st_size;
}

OneginError alloc_pointers (struct Text_info* onegin)
{
    DBG printf("<<<< alloc_pointers\n");
    //char** tmp_pointers = (char**) calloc (onegin->lines_number + 1, sizeof (char*));
    struct  String_t* alloc = (struct String_t*) calloc (onegin->lines_number, sizeof(struct String_t)); 

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
    //DBG printf("str = %s\n", str);
    while (1)
    {
        if (*str == '\n') break;
        if (*str == '\0') break;

        putchar (*str); // printf (%c)
        str++;
    }

    return 0;
}


//int bubble_sort (struct Text_info *onegin, size_t size, int(*comparator_forward(String_t*, String_t*))) // перераспределение ( партишин )
void bubble_sort (void* data, int lines_number, size_t pointer_size, compare_funct comparator)
{
    for (int i = 0; i < (lines_number) - 1; i++)// onegin->dta.len_str
    {
        DBG printf ("\nSTARTING PASS i = %d\n", i);
        for (int j = 0; j < (lines_number) - 1 - i; j++)
        {
            DBG printf ("j = %d...\n", j);
            DBG printf ("i = %d...\n", i);

            DBG printf("string = %p\n", (char*)data +   j              * pointer_size);
            DBG printf("string = %p\n", (char*)data +  (j + 1)         * pointer_size);

            assert(((char*)data +  j          * pointer_size));
            assert(((char*)data + (j + 1)     * pointer_size));

           if (comparator((char*)data +  j      * pointer_size, 
                          (char*)data + (j + 1) * pointer_size) > 0) 
            {
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

//int comparator_reverse (struct String_t*  str_line1, struct String_t* str_line2)
int comparator_reverse (const void* str_line1,const void* str_line2)
{
    const String_t* str1 = (const String_t*) str_line1;
    const String_t* str2 = (const String_t*) str_line2;

    const char* string1 = str1 -> str;
    const char* string2 = str2 -> str;
    
    int length_1 = str1 -> len_str; // strlen (string1);
    int length_2 = str2 -> len_str;

    printf(" str1 -> len_str = %d\n",  str1 -> len_str);
    printf(" str2 -> len_str = %d\n",  str2 -> len_str);

    printf("length1 = %s\n", string1);
    printf("length2 = %s\n", string2);


    // распечать массив из четырех срок 
    assert ($(length_1) == $(strlen (string1)));
    assert ($(length_2) == $(strlen (string2)));

    length_1 = check_excess_reverse (string1, length_1 - 1);
    length_2 = check_excess_reverse (string2, length_2 - 1);

// структуру 
    for (;;)
    {
        if (length_1 < 0 || length_2 < 0) 
            break;

        int symbol1 = tolower (string1[length_1]);
        int symbol2 = tolower (string2[length_2]);

        DBG printf ("\nsymbol1 = %d, (%c)\n", symbol1, symbol1);
        DBG printf ("\nsymbol2 = %d, (%c)\n", symbol2, symbol2);
        
        if (symbol1 != symbol2)
        {
            DBG printf ("\nSYMBOL1 = %d, (%c)\n", symbol1, symbol1);
            DBG printf ("\nSYMBOL2 = %d, (%c)\n", symbol2, symbol2); 

            DBG printf ("\nsyMbol1(%d) - syMol1(%d) = %d\n", symbol1, symbol2, (symbol1- symbol2));
        
            return (symbol1 - symbol2);
        }
    
        length_1--;
        length_2--;
    }

    return 0;
} 

/*#ifdef NDEBUG
    #define DBG            if (1) 
    #define ASSERT(cond)   ;
#else
    #define DBG            if (1) 
    #define ASSERT(cond)   if (! (cond)) { printf ("Your ASS in ERT\n"); abort(); }
#endif*/

int comparator_forward (const void* str_line1, const void* str_line2) 
{
    //ASSERT (str_line1 != NULL);

    //$$;
    const char* string1 = ((const String_t*) str_line1)->str;
    const char* string2 = ((const String_t*) str_line2)->str;


    //$(((const String_t*) str_line1  )->len_str);
    //$(((const String_t*) str_line2 )->len_str);

    //$(string1);
    //$(string2);
    
    //$$;
    int start_of_ptr_1 = check_excess_forward (string1); 
    //$$;
    int start_of_ptr_2 = check_excess_forward (string2);
    //$$;
    
    for (int i = 0;; i++) 
    {
        //$$;
        int symbol1 = tolower (string1  [start_of_ptr_1 + i]);
        int symbol2 = tolower (string2  [start_of_ptr_2 + i]);
        
        DBG printf ("\nsymbol1 = %d, (%c)\n", symbol1, symbol1);
        DBG printf ("\nsymbol2 = %d, (%c)\n", symbol2, symbol2);
        
        if (symbol1 != symbol2)
        {
            DBG printf ("\nsymbol2 - symbol1 = %d\n", symbol2- symbol1);
            //$$;
            return - (symbol2 - symbol1);
        }

        if (symbol1 == '\0' && symbol2 == '\0')
        {
            DBG printf ("\nsymbol1 = %d and symbol2 = %d\n", symbol1, symbol2);
            //$$;
            return 0;
        }
    }
    //$$;
 
    return 0;
}

int check_excess_forward (const char* pointer)
{
    int i = 0;
    while (pointer[i] != '\0')
    {
        int is_alpha1 = !isalpha (pointer [i]); 
        int is_alpha2 = !isalpha (pointer [i]); 
        DBG printf ( "__is_space1 = %d, is_punct1 = %d__", is_alpha1, is_alpha2);  

        if (!is_alpha1 &&
            !is_alpha2)
            break;   

        i++;
        //$$;
    }
    //$$;
    return i;
}

int check_excess_reverse(const char* pointer, int length)
{

    DBG printf("<<< length = %d, pointers = %s, pointer[length - 1] = %d\n", length, pointer, pointer[length]);
    while (1)
    {
        if (length == 0)
            break;
        //DBG printf ("\ni = %d, pointer = %s, pointer[%d]\n ", i, pointer, pointer[i] );
        int is_alpha1 = !isalpha (pointer [length]); 
        int is_alpha2 = !isalpha (pointer [length]);
        DBG printf ( "__is_space1 = %d, is_punct1 = %d__\n", is_alpha1, is_alpha2);  

        if (!is_alpha1 && !is_alpha2)
            break;   

        length--;
    }

    DBG printf(">>> length = %d, pointers = %s, pointer[length - 1] = %d\n", length, pointer, pointer[length]);
    return length;
}

int print (struct Text_info* onegin)
{
    printf ("THE TEXT:\n\n");

    for (int i = 0; i < onegin -> lines_number; i++) 
    {
        if ((onegin->data)[i].str    != NULL && 
            (onegin->data)[i].str[0] != '\0') 
        {
            printf ("<<< i = %02d: ", i);
            printf ("<%s>\n", (onegin->data)[i].str);

        //cod_sym(onegin->pointers[i]);
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

void check_strings_array(struct Text_info* onegin) 
{
    if (onegin == NULL || onegin->data == NULL) 
    {
        printf("The data array is NULL\n");
        return;
    }

    for (int i = 0; i < onegin->lines_number; i++) 
    {
        if (onegin->data[i].str == NULL) 
        {
            printf("Element |%d| in data is NULL.\n", i);
        } 
        else 
        {
            printf("Element |%d| in data is not NULL, length: %ld\n", i, (long)onegin->data[i].len_str);
        }
    }
}
// TODO написать через define переменную для qsort DATA1 DATA2
