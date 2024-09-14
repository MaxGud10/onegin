#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <ctype.h> 

const int MAX_ROWS = 100;

enum OneginError
{
    FILE_NO_OPEN              = -1,
    ERROR_ALLOCATION_BUFFER   = -2,
    ERROR_ALLOCATION_POINTERS = -3,
    ERRO_R                    = -4,// TODO: ONEGIN_ERROR_NAME по name понятна суть должна быть
    STATUS_OK                 =  0
};

struct Text_t //Text
{
    char* buffer; //  массив для буфера
    int text_len;
    char** pointers;
    int lines_number;

};

int filling_buffer(struct Text_t* onegin);
OneginError read_file(struct Text_t* onegin);
int print_lines(const char* str);
int check_excess(char* pointer);
int bubble_sort(struct Text_t *onegin);
OneginError buffer_alloc(struct Text_t* onegin);
int str_case_cmp(char* pointer, char *pointer_1);
int print(struct Text_t* onegin);
OneginError pointers_alloc(struct Text_t* onegin);
int file_get_length(const char* str);

int main (void) // TODO: получать название файла через командную строку ./output "onegin.txt" <--- argv[1] 
{
    struct Text_t onegin = {};
    //enum OneginError error = filling_buffer(&onegin);
    printf(">>>> filling_buffer\n");
    if (filling_buffer(&onegin) != STATUS_OK) // TODO: process_file, принимает название файла
    {
        printf("Error while filling buffer\n"); // TODO: функция которая, которая будет через свич кейс выводить определённую строку об ошибке 
        return -1;
    }
    printf("\n<<<< filling_buffer\n");
    //error = filling_buffer(&onegin) ; // вызов функции которую мы передает адрес структуры переменная которой равна onegin
    //filling_buffer(&onegin);

    bubble_sort(&onegin);
     
    print(&onegin);

    free(onegin.buffer);
    free(onegin.pointers); // TODO: assert!
    return 0;
}

int filling_buffer(struct Text_t* onegin) // TODO: return value - enum OneginError
{
    //TODO; error = memory_allocation(onegin);
    /*if (error != ONEGIN_ERROR_NO)
    {
        return error;
    }*/
    assert (onegin != NULL);
    printf("\n<<<< read_file\n");
    enum OneginError error = read_file(onegin); 
    printf(">>> read_file\n");
    if (error != STATUS_OK)
    {
        return error; 
    }

    int lines = 0; 
    //for (; postion < text_len.st_size; postion++) что было 
    for (int postion = 0; postion < onegin->text_len; postion++) //  цикл по всем символам из буфера. если начало строки,
    {
        if ((onegin->buffer) [postion] == '\n')
        {
            printf("< <pointers(%p)> > = buffer(%s) + postion(%d)", onegin -> pointers, onegin->buffer, postion + 1);
            (onegin->pointers) [lines] = onegin->buffer + postion + 1; // присваивает адрес первого элемента из масcива buffer в массив postion  
            lines++; 
        } 


        if (onegin->buffer[postion] == '\r')
        {
            printf("\nonegin->buffer = %p\n",onegin->buffer);
            onegin->buffer[postion] = '\0';
            continue;
        }

        onegin->lines_number = lines;
         //\r\n
            //\n
        //return 0;
        //[a][n][d][\0][\n][B][][][] // TODO сделать if ++ и +1 (обработка \r\n)
    }

    for (int i = 0; i < lines; i++)
    {
       print_lines((onegin->pointers)[i]);
    }

    return 0;
}

enum OneginError read_file(struct Text_t* onegin) // TODO: разбить на более мелкие функции
{
    FILE* file = fopen("onegin.txt", "rb");
    printf("<<< file opens\n");
    if (file == NULL)
    {
        printf("File opening error: onegin.txt\n");
        return FILE_NO_OPEN;
    }

    enum OneginError error = buffer_alloc(onegin);
    if (error != STATUS_OK) 
    {
        fclose(file);
        return error;
    }

    fread(onegin->buffer, sizeof(char), onegin->text_len, file);
    fclose(file);

    printf("\n<<<fread\n");
    
    int count = 0; // TODO: переименовать (например, number_lines)
    for (int i = 0; i < onegin->text_len; i++) // цикл для прохождения по всем символам для проверки на \n  (сколько строк)
    {
        //printf("i = %d - buffer[i] = %d, buffer[i] ='%c'\n", i, (onegin -> buffer)[i], (onegin -> buffer)[i]);
        
        if ((onegin->buffer) [i] == '\n')
        {
            count++;
            printf("\nCOUNT = %d\n", count);
        } 
    }

    onegin->lines_number = count;

    error = pointers_alloc(onegin);
    if (error != STATUS_OK) 
    {
        return error;
    }

    return STATUS_OK;
}

//int buffer_alloc(struct Text_t* onegin)
enum OneginError buffer_alloc(struct Text_t* onegin)
{
    int size = file_get_length ("onegin.txt");
    if (size < 0) 
    {
        return ERROR_ALLOCATION_BUFFER;
    }

    char* tmp_buffer = (char*) calloc(size + 1, sizeof(char)); // выделяю память для буфера //TODO: +1 для \0

    if (tmp_buffer == NULL) 
    {
        printf("Incorrect memory allocation in tmp_buffer\n");
        return ERROR_ALLOCATION_BUFFER;
    }

    onegin->buffer   = tmp_buffer; 
    onegin->text_len = size;

    return STATUS_OK;
}

int file_get_length(const char* str)
{
    struct stat text_len  = {}; 

    stat(str, &text_len);  
    printf("text.st_size = %ld\n", text_len.st_size);

    return text_len.st_size;
}

OneginError pointers_alloc(struct Text_t* onegin)
{
    char** tmp_pointers = (char**) calloc(onegin->lines_number + 1, sizeof(char*));

    if (tmp_pointers == NULL)
    {
        printf("Incorrect memory allocation in tmp_pointers\n");
        return ERROR_ALLOCATION_POINTERS;
    }

    onegin->pointers = tmp_pointers;

    return STATUS_OK;
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

    for (int i = 0; i < onegin -> lines_number - 1; i++)
    {
        printf("\nSTARTING PASS i = %d\n", i);
        for (int j = 0; j < onegin -> lines_number - 1 - i; j++)
        {
            printf("j = %d...\n", j);
            printf("i = %d...\n", i);
            //assert();
            printf("<%s> vs <%s>...\n", onegin->pointers[j], onegin->pointers[j + 1]);
            if (str_case_cmp((onegin -> pointers)[j], (onegin -> pointers)[j + 1]) > 0)
            {
                ded                         = (onegin -> pointers)[j];
                (onegin -> pointers)[j]     = (onegin -> pointers)[j + 1];
                (onegin -> pointers)[j + 1] = ded;
            }
        }

        printf("END PASS %d\n", i);
    }
    return 0;
}

int str_case_cmp(char* pointer, char *pointer_1)
{
    int start_of_ptr   = check_excess(pointer);
    int start_of_ptr_1 = check_excess(pointer_1);
    
    int x = 0;
    for (; x < MAX_ROWS - 1; x++)
    {
        int symbol1 = tolower(pointer  [start_of_ptr   + x]);
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
        //printf("\ni = %d, pointer = %s, pointer[%d]\n ", i, pointer, pointer[i] );
        int is_space = isspace(pointer[i]); 
        int is_punct = ispunct(pointer[i]);
        printf( "__is_space1 = %d, is_punct1 = %d__", is_space, is_punct);  

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
 