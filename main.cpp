 #include "stak_oper.h"

int main(int argc, char const *argv[])
{
    FILE * LLogfile = fopen("report.txt", "w");

    stack stk = {.Logfile = LLogfile};

    Stack_ctor(&stk, "stack");

    for (int i = 13; i <= 35; i++)
    {
        Stack_push(&stk, i);
    }

    for (int i = 0; i <= 15; i++)
    {
        Stack_pop(&stk);
    }
 
    Stack_dtor(&stk);    //error
    
    fclose(LLogfile);
    return 0;
}