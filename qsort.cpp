//#define DEBUG

//#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
// TODO сделать чтобы pivot находился через rand
#include "color.h"

struct Interval
{
    int low; 
    int high; 
    //int pi;
};

struct Stack
{
    struct Interval* data; // массив который хранит интервалы, которые мы будет сортировать 
    int size;  // размер интервалов( которых мы будет сортировать )
};

const int ded_loh = 20;

#ifdef DEBUG
    #define dbg        if(1)
    #define DBG(...) __VA_ARGS__
#else
    #define dbg        if(0)
    #define DBG(code)
#endif

int  my_partition (int arr[], int low, int high);

void swap (int* a, int* b);

void my_qsort (struct Stack stack,  int arr[], int n);
void push     (struct Stack* stack, int low,   int high);
void pop      (struct Stack* stack, struct Interval* interval);

int  print_sort (int arr[], int n);

int main(void)
{
    // struct intervaluence arr[10] =  
    // {
    //     {9, 4, 8, 3, 7, 1, 6, 2, 5}
    // }; // 9, 10, 8, 10, 7, 10, 6, 10, 5
    struct Stack stack = {};

    int arr[] = {9, 10, 8, 10, 7, 10, 6, 10, 5}; // 9, 4, 8, 3, 7, 1, 6, 2, 5
    int n = sizeof(arr) / sizeof(arr[0]);

    //printf("sizeof(arr) = %d / sizeof(arr[0]) = %d\n", sizeof(arr), sizeof(arr[0]));

    my_qsort(stack, arr, n);

    //dbg(printf(GREEN "Sorted array:\n" RESET));

    print_sort(arr, n);

    
    struct Interval interval;

    // Пример извлечения интервала из стека
    pop(&stack, &interval);
    // Используйте interval здесь, если необходимо

    free(stack.data);

    return 0;
}

int my_partition(int arr[], int low, int high)
{

    int pivot = arr[high];
    int x = low - 1;

    dbg(printf("\nPartitioning with pivot =" RED " %d" WHITE " from" BLUE " %d" WHITE " to" BLUE " %d\n" RESET, pivot, low, high));

    for (int y = low; y <= high -1; y++)
    {
        if (arr[y] < pivot)
        {
            x++;

            swap(&arr[x], &arr[y]);

            dbg printf("\nSwapping elements: %d and %d\n", arr[x], arr[y]);
            dbg printf(GREEN "\nArray: " RESET);

            DBG(print_sort(arr, high - low + 1)); //  для вывода текущего состояние массива
        }
    }
    swap(&arr[x + 1], &arr[high]);

    dbg printf("\nThe pivot has moved to the position: Swapping %d with %d\n", arr[x + 1], arr[high]);
    dbg printf(BLUE "\nArray state after pivot swap: " RESET);
    DBG(print_sort(arr, high - low + 1)); // выводим текущее состояние массива после обмена с привязкой


    return (x + 1);
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void my_qsort(struct Stack stack, int arr[], int n)
{
    static int ded_loh = 0;
    ded_loh++;

    int low = 0;
    int high = n - 1;

    push(&stack, low, high); // добавляем начальный интервал (0, n - 1) 

    while (stack.size > 0) // продолжается до тех пор пока в стеке есть элементы 
    {
        struct Interval interval;
        pop(&stack, &interval); // извлекается интервал из стека 
        low = interval.low;   
        high = interval.high; 

        if (low < high) 
        {
            int pi = my_partition(arr, low, high);

            printf("[%2d] starting my_qsort with   (low =" YELLOW "%2d " WHITE "| pi-1=" YELLOW "%2d" WHITE ")\n" RESET, ded_loh, low, pi -1);
            printf("[%2d]    ...and storing: PUSH  (pi+1=" RED "%2d " WHITE "| high=" RED "%2d" WHITE ")\n" RESET,  ded_loh, pi + 1, high);

            // Сначала добавляем правую часть
            push(&stack, pi + 1, high); 

            printf("\n[%2d]       getting via POP:   (pi+1=" CYAN "%2d " WHITE "| high=" CYAN"%2d" WHITE") and running it\n" RESET, ded_loh, pi + 1, high);

            // Затем добавляем левую часть
            push(&stack, low, pi - 1);
        }
    ded_loh--;
    }

    free(stack.data);
}

void push (struct Stack* stack, int low, int high) 
{
    stack->size++;  // для добавления нового элемента  в стек 
    stack->data = (struct Interval*)realloc(stack->data, stack->size * sizeof(struct Interval)); // (stack->size * sizeof(struct Interval) мы умножаем size на размер одного элемента struct Interval 
    stack->data[stack->size - 1].low = low; // мы устанавливает новый уровень low для нового интервала 
    stack->data[stack->size - 1].high = high;
}

void pop(struct Stack* stack, struct Interval* interval)
{
    if (stack -> size > 0) // проверка на наличие элементов 
    { 
        *interval = stack -> data[stack -> size - 1]; // последний интервал находящийся на вершине стека, сохраняется в переменную, на котоую указывает *interval 
        stack -> size --; // уменьшаем размер стека после извлечения последнего элемента из стека 
        stack ->data = (struct Interval*) realloc (stack -> data, stack -> size * sizeof(struct Interval)); // память распределяется в соответствии с новым количеством элементов 
    }
}

int print_sort(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);  

    return 0;
}

/*#ifdef NDEBUG
    #define DBG            if (1) 
    #define ASSERT(cond)   ;
#else
    #define DBG            if (1) 
    #define ASSERT(cond)   if (! (cond)) { printf ("Your ASS in ERT\n"); abort(); }
#endif*/
