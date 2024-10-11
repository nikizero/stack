#include "stack_init.h"

static stack_valid_ctor *GetValidator() {
    static stack_valid_ctor valid = {.all_stack_validate_ctor = (stack**)calloc(2, sizeof(stack)), .count = 0};

    return &valid;
}

eror IsEror(stack* stk, const char* file, const char* funk, int num_of_line)
{
    if (stk == NULL) 
    {
        return NULL_PTR_STACK;
    }
    assert(stk->Logfile);

    mark_eror(file, funk, num_of_line, Verify(stk), stk);
    Dumper(*stk);                                                                                                            
    return stk -> last_eror.eror_code;
}

size_t Hash_count(stack stk) 
{
    // const size_t
    size_t HS1 = 0xDED;
    size_t HS2 = 0xAD;
    size_t total = 0;

    total = stk.capacity * HS1 + stk.size * HS1 + stk.Canary_1 * HS2 + 
            stk.Canary_2 * HS2 + (size_t)(stk.data) * (HS1 + HS2);
    return total;
}

void stk_default_set(stack* stk)
{
    stk->Canary_1 = CANARY_OK;
    stk->capacity = Default_capacity;
    stk->data = (StackELem_t*)((char*)stk + 1);
    stk->stack_name = "default stack name";
    stk->Canary_2 = CANARY_OK;
}

eror Stack_ctor(stack* stk, const char* stack_name)
{
    if (stk == NULL) 
    {
        return NULL_PTR_STACK;
    }
    
    stk_default_set(stk);
    stk->stack_name = stack_name;

    ON_HASH
    (
        stk->hash_sum = Hash_count(*stk);
    );

    Add_In_Valid(stk);
    ON_DEBUG
    (
        if(IsEror(stk, __FILE__, __func__, __LINE__) != OK)
        {
            return stk->last_eror.eror_code;
        };
    );
    fprintf(stk->Logfile, "stack created and Validate!\n");

    stk->data = (StackELem_t*)((char*)calloc(stk->capacity + 2 * sizeof(stk->Canary_1), 1) + sizeof(stk->Canary_1));

    ON_HASH
    (
        stk->hash_sum = Hash_count(*stk);
    );

    ON_DEBUG
    (
        if(IsEror(stk, __FILE__, __func__, __LINE__) != OK) 
        {
            return stk->last_eror.eror_code;
        };
    );
    return stk->last_eror.eror_code;
}

void Add_In_Valid(stack* stk)
{
    assert(stk);

    static const size_t CountOver = 1000;

    stack_valid_ctor* valid = GetValidator();

    if (valid->count == CountOver)
    {
        valid->count *= 2;
        valid->all_stack_validate_ctor = (stack**) realloc(valid->all_stack_validate_ctor, valid->count);
    }

    valid->all_stack_validate_ctor[valid->count] = stk;
    valid->count += 1;
}

valid_stat Stac_Is_Init(stack* stk)
{
    if (stk == NULL) 
    assert(stk);

    stack_valid_ctor* valid = GetValidator();

    stack** adr = valid->all_stack_validate_ctor;

    for (int i = 0; i < valid->count; i++)
    {
        if (adr[i] == stk)
        {
            return VALID;
        }
    }

    return NO_VALID;
}

eror Stack_dtor(stack* stk)
{
    ON_DEBUG
    (
        if(IsEror(stk, __FILE__, __func__, __LINE__) != OK) 
        {
            return stk->last_eror.eror_code;
        };
    );

    if (stk == NULL) 
    {
        return NULL_PTR_STACK;
    }
    
    free((char*)stk->data - sizeof(CANARY_OK));
    stk -> data = nullptr;

    stk->size = 0;
    stk->capacity = 0;

    fprintf(stk->Logfile, "stack determinate!\n");
    stk -> Logfile = stdout;
    return OK;
    
    ON_DEBUG
    (
        if(IsEror(stk, __FILE__, __func__, __LINE__) != OK) 
        {
            return stk->last_eror.eror_code;
        };
    );
}

eror Verify(stack* stk)         
{
    if (stk == NULL)
    {
        printf("Verify Eror: stak struct is null \n");
        return NULL_PTR_STRUCT;
    }

    if (stk->data == NULL)
    {
        printf("Verify Eror: stak ptr is null \n");
        return NULL_PTR_STACK;
    }

    if (Stac_Is_Init(stk) != VALID)
    {
        printf("Verify Eror: attempt to use no init stack \n");
        return NO_INIT_STK;
    }

    if (stk->data == NULL && stk->size == 0 && stk->capacity == 0)
    {
        return OK;
    }

    ON_CANARYS
    (
        if (stk->Canary_1 != CANARY_OK || stk->Canary_2 != CANARY_OK)
        {
            printf("Verify Eror: Attempt to hack, its no so simple (Canary) \n");
            return Canary_Hack;
        }
    );

    ON_HASH
    (
        if (stk->hash_sum != Hash_count(*stk))
        {
            printf("Verify Eror: Attempt to hack, its no so simple (Hash) \n");
            return Canary_Hack;
        }
    );

    if (stk->capacity < Default_capacity)
    {
        printf("Verify Eror: capas < 0 \n");
        return CAPAS_LESS_DEFAULT;
    }

    if (stk->size > stk->capacity)
    {
        printf("Verify Eror: stak size > capacity, overload \n");
        return STAK_OVERLOAD;
    }

    if (stk->size > stk->capacity)
    {
        printf("Verify Eror: stak size > capacity, overload \n");
        return STAK_OVERLOAD;
    }

    if (stk->size < 0)
    {
        printf("Verify Eror: size less than defaukt \n");
        return SIZE_LESS_NULL;
    }

    else
    {
        return OK;
    }
}

StackError_t mark_eror (const char* ffile, const char* ffunk, int num_of_line, eror eror_code, stack* stk)
{
    assert(ffile);
    assert(ffunk);
    assert(stk->Logfile);

    char* file = (char*)ffile;
    char* funk = (char*)ffunk;

    StackError_t struct_eror = {.eror_code = eror_code, .fauld_file_name = file, .fauld_func_name = funk, .fauld_num_of_line = num_of_line,
    .Logfile = stk->Logfile};
    stk -> last_eror = struct_eror;
    return struct_eror;
}

void Print_error(StackError_t eror)
{
    assert(eror.Logfile);

    switch (eror.eror_code) {

    case OK:
        fprintf(eror.Logfile, "Error code: okey\n");
        break;                                                            

    case NULL_PTR_STRUCT:
        fprintf(eror.Logfile, "Error code: zero pointer on struct!\n");
        break;

    case NULL_PTR_STACK:
        fprintf(eror.Logfile, "Error code: zero pointer on stack!\n");
        break;
    
    case NO_INIT_STK:
        fprintf(eror.Logfile, "Error code: atempt to use no ctor stack!\n");
        break;

    case NULL_PTR_LOGFILE :
        printf("Error code: zero pointer on logfile!\n");
        break;

    case STAK_OVERLOAD:
        fprintf(eror.Logfile, "Error code: stack overload!\n");
        break;

    case Canary_Hack:
        fprintf(eror.Logfile, "Error code: data in struct become bad!\n");
        break;

    case SIZE_LESS_NULL:
        fprintf(eror.Logfile, "Error code: size < 0 \n");
        break;

    case CAPAS_LESS_DEFAULT:
        fprintf(eror.Logfile, "Error code: capacity < 0\n");
        break;
    default:
        fprintf(eror.Logfile, "Error code: error not recognized!\n");
    }
}
void Dumper(stack stk) 
{
    fprintf(stk.Logfile,"\n \n");

    fprintf(stk.Logfile, "stack, named $%s$ [%p] from function: %s()\n", stk.stack_name, &stk, stk.last_eror.fauld_func_name);

    ON_CANARYS
    (
        fprintf(stk.Logfile, "Curret Canary 1: %llX", stk.Canary_1);
        stk.Canary_1 == CANARY_OK ? fprintf(stk.Logfile, "[OK]\n") : fprintf(stk.Logfile, "[ERR]\n");
        fprintf(stk.Logfile, "Curret Canary 2: %llX", stk.Canary_2);
        stk.Canary_2 == CANARY_OK ? fprintf(stk.Logfile, "[OK]\n") : fprintf(stk.Logfile, "[ERR]\n");
    );

    fprintf(stk.Logfile, "   1. Capacity: %lu \n", stk.capacity);
    fprintf(stk.Logfile, "   2. Size: %lu \n", stk.size);
    
    ON_HASH
    (
        fprintf(stk.Logfile, "   3. Hash_sum: %lu right: %lu \n", stk.hash_sum, Hash_count(stk));
    );

    fprintf(stk.Logfile, "\n");

    fprintf(stk.Logfile, "   4. ");
    Print_error(stk.last_eror);

    if (stk.data != NULL) 
    {
        fprintf(stk.Logfile, "   5. Data[%p] \n", stk.data);
        for (size_t i = 0; i < stk.size; i ++) 
        {
            fprintf(stk.Logfile, "       [%lu] = ", i);
            fprintf(stk.Logfile, PRINTF_SPECIFIER, *((StackELem_t *)((char*)stk.data + i * sizeof(StackELem_t))));  
            fprintf(stk.Logfile, "\n");
        }

        fprintf(stk.Logfile,"\n \n");
    }
}