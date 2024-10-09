#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>


enum valid_stat
{
    VALID    = 0,
    NO_VALID = 1
};

const long long CANARY_OK = 0XBADDED600DDED; 


#define Default_capacity 2
#define PRINTF_SPECIFIER "%d"                   //TODO WTF?

#ifdef BOTH
#define USE_CANARYS 
#define USE_HASH
#endif

#ifdef DEBUG
#define ON_DEBUG(code) do { code; } while (0)
#else
#define ON_DEBUG(code)
#endif

#ifdef USE_CANARYS
#define ON_CANARYS(code) do { code; } while (0)
#else
#define ON_CANARYS(code)
#endif

#ifdef USE_HASH
#define ON_HASH(code) do { code; } while (0)
#else
#define ON_HASH(code)
#endif

enum eror
{
    OK                                   = 0,
    STAK_OVERLOAD                        = 1,
    NULL_PTR_STACK                       = 2,
    NULL_PTR_LOGFILE                     = 3,
    Canary_Hack                          = 4,
    SIZE_LESS_NULL                       = 5,
    HASH_ERR                             = 6,
    CAPAS_LESS_DEFAULT                   = 7,
    NO_INIT_STK                          = 8,
    NULL_PTR_STRUCT                      = 9
};

struct eror_t
{
    char*   fauld_func_name;
    char*   fauld_file_name;
    int   fauld_num_of_line;
    eror  eror_code;
    FILE* Logfile;
};

struct stack
{
    long long Canary_1;                                     //TODO Почем бы просто не сделать, конст переменные, которые всё зарушат
    FILE* Logfile;
    size_t size;
    size_t capacity;
    size_t hash_sum;
    const char* stack_name;
    int* data;
    eror_t last_eror;
    long long Canary_2;
};

struct validator
{
    stack** validator;
    int count;
};

eror IsEror(stack* stk, const char* file, const char* funk, int num_of_line);
eror Stack_ctor(stack*stk, const char* stack_name);
eror Verify(stack* stk);
eror Stack_dtor(stack* stk);
eror_t mark_eror (const char* ffile, const char* ffunk, int num_of_line, eror eror_code, stack* stk);
size_t Hash_count(stack stk) ;
void Print_error(eror_t eror);
void Dumper(stack stk);
void stk_default_set(stack* stk);
void Add_In_Valid(stack* stk);
valid_stat Stac_Is_Init(stack* stk);