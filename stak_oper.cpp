 #include "stak_oper.h"

 eror Stack_increase(stack* stk) 
 {
    ON_DEBUG(if(IsEror(stk, "stack.oper", "Stack.increase", 7) != OK) {return stk->last_eror.eror_code;};);

    stk->data = (int*)((char *)realloc((char *)stk->data - sizeof(CANARY_OK), 2 * stk->capacity * sizeof(int) + 2 * sizeof(CANARY_OK)) + sizeof(CANARY_OK));
    assert(stk -> data);

    *((long long *)((char *)stk->data + stk->capacity * sizeof(int))) = CANARY_OK;
    *((long long *)((char *)stk->data - sizeof(CANARY_OK))) = CANARY_OK;

    stk->capacity *= 2;
    
    stk->hash_sum = Hash_count(*stk);

    ON_DEBUG(if(IsEror(stk, "stack.oper", "Stack.increase", 19) != OK) {return stk->last_eror.eror_code;};);

    return OK;
}

eror Stack_push(stack * stk, int a) 
{
    ON_DEBUG(if(IsEror(stk, "stack.oper", "Stack.push", 27) != OK) {return stk->last_eror.eror_code;};);    //TODO я правильно понимаю, что верификатор не должен прерывать выполнение функции

    if (stk->capacity <= stk->size) 
    {    
        Stack_increase(stk);
        fprintf(stk->Logfile,"Stack size increased to:%lu \n", stk->capacity);
    }

    stk->size += 1;
    *((int *)((char*)stk->data + (stk->size - 1)* sizeof(int))) = a;

    fprintf(stk->Logfile,"The element was successfully added to the stack!\n");

    stk->hash_sum = Hash_count(*stk);

    ON_DEBUG(if(IsEror(stk, "stack.oper", "Stack.push", 42) != OK) {return stk->last_eror.eror_code;};);

    return OK;
}

eror Stack_decrease(stack * stk) 
{
    ON_DEBUG(if(IsEror(stk, "stack.oper", "Stack.decrease", 50) != OK) {return stk->last_eror.eror_code;};);

    stk->capacity /= 2; 
    
    stk->data = (int*)((char *)realloc((char *)stk->data - sizeof(CANARY_OK), stk->capacity * sizeof(int) + 2 * sizeof(CANARY_OK)) + sizeof(CANARY_OK));
    assert(stk -> data);

    *((long long *)((char *)stk->data + stk->capacity * sizeof(int))) = CANARY_OK;
    *((long long *)((char *)stk->data - sizeof(CANARY_OK))) = CANARY_OK;
    
    stk->hash_sum = Hash_count(*stk);

    ON_DEBUG(if(IsEror(stk, "stack.oper", "Stack.decrease", 62) != OK) {return stk->last_eror.eror_code;};);
    return OK;
}

eror Stack_pop(stack * stk)
{
    ON_DEBUG(if(IsEror(stk, "stack.oper", "Stack_pop", 69) != OK) {return stk->last_eror.eror_code;};);
    
    if (stk->size <= 0) 
    {
        fprintf (stk->Logfile,"The stack is empty!\n");
        return CAPAS_LESS_DEFAULT;
    }
   
    if (stk->capacity >= stk->size * 3 && stk->capacity/2  > Default_capacity)  
    {
       
        if (Stack_decrease(stk) == OK ) 
        {
            fprintf(stk->Logfile,"Stack size decreased to:%lu \n", stk->capacity);

        } 

        else 
        {
            fprintf(stk->Logfile, "Error in decreasing stack`s size\n");
            return IsEror(stk, "stack.oper", "Stack_pop", 68);
        }
    }

    *((int *)((char *)stk->data + stk->size * sizeof(int))) = 0;      

    stk->size--;
    fprintf (stk->Logfile, "The element has left the stack!\n");
    stk->hash_sum = Hash_count(*stk);

    ON_DEBUG(if(IsEror(stk, "stack.oper", "Stack_pop", 99) != OK) {return stk->last_eror.eror_code;};);

    return OK;
}