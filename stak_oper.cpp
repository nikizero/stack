 #include "stak_oper.h"

 eror Stack_increase(stack* stk) 
 {
    ON_DEBUG
    (
        if(IsEror(stk, __FILE__, __func__, __LINE__) != OK) 
        {
            return stk->last_eror.eror_code;
        };
    );

    stk->capacity *= 2;
    const size_t new_capacity = stk->capacity * sizeof(StackELem_t) + 2 * sizeof(CANARY_OK);

    stk->data = (StackELem_t*)((char *)realloc((char *)stk->data - sizeof(CANARY_OK), new_capacity) + sizeof(CANARY_OK));
    assert(stk -> data);

    *((canary_t *)((char *)stk->data + stk->capacity * sizeof(StackELem_t))) = CANARY_OK;
    *((canary_t *)((char *)stk->data - sizeof(CANARY_OK)))                   = CANARY_OK;
    
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

    return OK;
}

eror Stack_push(stack * stk, StackELem_t a) 
{
    ON_DEBUG
    (
        if(IsEror(stk, __FILE__, __func__, __LINE__) != OK) 
        {
            return stk->last_eror.eror_code;}
        ;
    );   

    if (stk->capacity <= stk->size) 
    {    
        Stack_increase(stk);
        fprintf(stk->Logfile,"Stack size increased to:%lu \n", stk->capacity);
    }

    stk->size += 1;
    *((StackELem_t *)((char*)stk->data + (stk->size - 1)* sizeof(StackELem_t))) = a;

    fprintf(stk->Logfile,"The element was successfully added to the stack!\n");

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

    return OK;
}

eror Stack_decrease(stack * stk) 
{
    ON_DEBUG
    (
        if(IsEror(stk, __FILE__, __func__, __LINE__) != OK) 
        {
            return stk->last_eror.eror_code;
        };
    );

    stk->capacity /= 2; 
    
    const size_t new_capacity = 2 * stk->capacity * sizeof(StackELem_t) + 2 * sizeof(CANARY_OK);
    stk->data = (StackELem_t*)((char *)realloc((char *)stk->data - sizeof(CANARY_OK), new_capacity) + sizeof(CANARY_OK));
    assert(stk -> data);

    *((canary_t *)((char *)stk->data + stk->capacity * sizeof(StackELem_t))) = CANARY_OK;
    *((canary_t *)((char *)stk->data - sizeof(CANARY_OK)))                   = CANARY_OK;
    
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
    return OK;
}

eror Stack_pop(stack * stk)
{
    ON_DEBUG
    (
        if(IsEror(stk, __FILE__, __func__, __LINE__) != OK) 
        {
            return stk->last_eror.eror_code;
        };
    );
    
    if (stk->size <= 0) 
    {
        fprintf (stk->Logfile,"The stack is empty!\n");
        return CAPAS_LESS_DEFAULT;
    }
   
   const size_t INCREASE_VALUE = 3;

    if (stk->capacity >= stk->size * INCREASE_VALUE && stk->capacity / 2  > Default_capacity)  
    {
       
        if (Stack_decrease(stk) == OK ) 
        {
            fprintf(stk->Logfile,"Stack size decreased to:%lu \n", stk->capacity);

        } 

        else 
        {
            fprintf(stk->Logfile, "Error in decreasing stack`s size\n");
            return IsEror(stk, __FILE__, "stack_pop", __LINE__);
        }
    }

    *((StackELem_t *)((char *)stk->data + stk->size * sizeof(StackELem_t))) = 0;      

    stk->size--;
    fprintf (stk->Logfile, "The element has left the stack!\n");
    
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

    return OK;
}