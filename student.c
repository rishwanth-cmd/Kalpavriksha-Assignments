#include <stdio.h>
#include <string.h>

typedef struct
{
    int rollNo;
    char name[50];
    double mark1;
    double mark2;
    double mark3;
    double totalMarks;
    double averageMark;
    char grade;

} Student;

double calculateTotalMarks(double mark1, double mark2, double mark3);
double calculateAverageMark(double totalMarks);
char calculateGrade(double totalMarks);
void displayStudentsRecord(Student student[], int n);
void displayPerformance(char grade);
void displayRollNumber(Student student[], int n, int start);



int main()
{
    int n;
    printf("Enter the number of students: ");
    scanf("%d",&n);
    Student student[100];
    

    
    for(int i = 0; i < n; i++)
    {
        printf("Enter Roll No, Name, Mark 1, Mark 2, Mark 3: \n");
        scanf("%d %50s %lf %lf %lf", &student[i].rollNo, student[i].name, &student[i].mark1, &student[i].mark2, &student[i].mark3);
        student[i].totalMarks = calculateTotalMarks(student[i].mark1, student[i].mark2, student[i].mark3);
        student[i].averageMark = calculateAverageMark(student[i].totalMarks);
        student[i].grade = calculateGrade(student[i].averageMark);

    }
    displayStudentsRecord(student, n);
    printf("Roll Numbers: ");
    displayRollNumber(student, n, 0);
}

double calculateTotalMarks(double mark1, double mark2, double mark3)
{
    return mark1 + mark2 + mark3;
}

double calculateAverageMark(double totalMarks)
{
    return totalMarks/3.0;
}

char calculateGrade(double averageMark)
{
     if(averageMark >= 85 && averageMark <= 100)
     {
        return 'A';
     }
     else if(averageMark >= 70 && averageMark < 85)
     {
        return 'B';
     }
     else if(averageMark >= 50 && averageMark < 70)
     {
        return 'C';
     }
     else if(averageMark>= 35 && averageMark < 50)
     {
        return 'D';
     }
     else if(averageMark >= 0 && averageMark < 35)
     {
        return 'F';
     }
}

void displayStudentsRecord(Student student[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("Roll Number: %d\n",student[i].rollNo);
        printf("Name: %s\n",student[i].name);
        for(int j = 0; j < 3; j++)
        {

        }
        printf("Total Mark: %.2lf\n", student[i].totalMarks);
        printf("Average Mark: %.2lf\n", student[i].averageMark);
        char grade = student[i].grade;
        printf("Grade: %c\n", grade);
        displayPerformance(grade);

    }
}

void displayPerformance(char grade)
{
    if(grade == 'A')
    {
        printf("Performance: *****\n");
    }
    else if(grade == 'B')
    {
        printf("Performance: ****\n");
    }
    else if(grade == 'C')
    {
        printf("Performance: ***\n");
    }
    else if(grade == 'D')
    {
        printf("Performance: **\n");
    }
    else
    {
        printf("Performance: Grade is too low.\n");
        return;
    }
    return;
}

void displayRollNumber(Student student[], int n, int start)
{
    if(n == start)
    {
        return;
    }
    printf("%d ",student[start].rollNo);
    displayRollNumber(student, n, start+1);
}