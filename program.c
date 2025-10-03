#include <stdio.h>
#include <string.h>
#include <stdlib.h>

double evaluate_helper(double a, double b, char op);
double evaluate_expression(const char* expression);
int precedence_checker(char op);


int main()
{
    char expression[100] = "";            // declaring a char array 
    while(strlen(expression)==0)
    {
        printf("Enter the expression: ");
        fgets(expression, sizeof(expression), stdin);    // getting the expression from user
        expression[strlen(expression)-1]='\0';   // removing the newline character

    }
    double result = evaluate_expression(expression);   // calling the function to evaluate the expression
    if(result==-1)
    {
        printf("Please enter a valid expression.");
    }
    else
    {
        printf("%.2lf",result);
    }
    return 0;

}

double evaluate_expression(const char* expression)
{
    double operands[100];   // for storing operands
    char operators[100];   // for storing operators
    int i=0, len=strlen(expression);
    int operands_top=-1, operators_top=-1;

    while(i<len)
    {
        char ch = expression[i];

        if(ch==' ')
        {
            i++;
            continue;
        }
        
        // converting string to integer and pushing it to operands stack
        if(ch>='0' && ch<='9')
        {
            char numArr[50]="";
            int j=0;
            while(i<len && ((expression[i]>='0' && expression[i]<='9') || expression[i]=='.'))
            {
                numArr[j++] = expression[i++];
            }
            numArr[j]='\0';
            double num = atof(numArr);
            operands[++operands_top] = num;
        }
        else if(ch=='+' || ch=='-' || ch=='*' || ch=='/')
        {
            if(operands_top<0)
            {
                printf("Oops! The expression you have entered is invalid.\n");
                return -1;

            }
            while(operators_top>=0 && precedence_checker(operators[operators_top])>=precedence_checker(ch))
            {
                if(operands_top<1)
                {
                    printf("Oops! The expression you have entered is invalid.\n");
                    return -1;
                }
                double b = operands[operands_top--];
                double a = operands[operands_top--];
                char op = operators[operators_top--];
                double res = evaluate_helper(a,b,op);
                operands[++operands_top] = res;
            }

            // pushing operator to operators stack
            operators[++operators_top] = ch;
            i++;
        }
        else
        {
            printf("Oops! The expression you have entered is invalid.\n");
            return -1;
        }
    }

    while(operators_top>=0)
    {
         if(operands_top<1)
         {
             printf("Oops! The expression you have entered is invalid.\n");
             return -1;
         }
        double b = operands[operands_top--];
        double a = operands[operands_top--];
        char op = operators[operators_top--];
        double res = evaluate_helper(a, b, op);
        operands[++operands_top] = res;
    }

    return operands[operands_top];
}

int precedence_checker(char op)
{
    if(op == '/' || op == '*')
    {
        return 2;
    }
    if(op == '+' || op == '-')
    {
        return 1;
    }
    return 0;
}

double evaluate_helper(double a, double b, char op)
{
    switch (op)
    {
        case '+':
            return a+b;
        case '-':
            return a-b;
        case '*':
            return a*b;
        case '/':
            if(b==0)
            {
                printf("Cannot divide by zero");
                exit(1);
            }
            return a/b;
        default:
            break;
    }
    return 0;
}