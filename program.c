#include <stdio.h>
#include <string.h>

int evaluate_expression(const char* expression);

int main()
{
    char expression[100] = "";            // declaring a char array 
    printf("Enter the expression: ");
    fgets(expression, sizeof(expression), stdin);    // getting the expression from user
    expression[strlen(expression)-1]='\0';   // removing the newline character
    int result = evaluate_expression(expression);   // calling the function to evaluate the expression
    return 0;
}

int evaluate_expression(const char* expression)
{
    int operands[100];   // for storing operands
    char operators[100];   // for storing operators
    int i=0, len=strlen(expression);
    int operands_top=-1, operators_top=-1;

    while(i<len)
    {
        char ch = expression[i];
        
        // converting string to integer and pushing it to operands stack
        if(ch>='0' && ch<='9')
        {
            int num=0;
            while(i<len && expression[i]>='0' && expression[i]<='9')
            {
                num = num*10 + (expression[i]-'0');
                i++;
            }
            operands[++operands_top] = num;
        }
        else if(ch=='+' || ch=='-' || ch=='*' || ch=='/')
        {
            // pushing operator to operators stack
            operators[++operators_top] = ch;
            i++;
        }
        else
        {
            i++;   
        }
    }
    for(int j=0; j<=operands_top; j++)
        printf("%d ", operands[j]);
    for(int j=0; j<=operators_top; j++)
        printf("%c ", operators[j]);
    return 0;
}
