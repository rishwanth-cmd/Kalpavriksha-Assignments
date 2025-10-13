#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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

bool rollNoValidation(int rollNo);
bool nameValidation(char name[50]);
bool marksValidation(double mark1, double mark2, double mark3);
double calculateTotalMarks(double mark1, double mark2, double mark3);
double calculateAverageMark(double totalMarks);
char calculateGrade(double totalMarks);
void sortByRollNo(Student student[], int noOfStudents);
void displayStudentsRecord(Student student[], int noOfStudents);
void displayPerformance(char grade);
void displayRollNumber(Student student[], int noOfStudents, int start);



int main()
{
    int noOfStudents;
    printf("Enter the number of students: ");
    scanf("%d",&noOfStudents);
    Student student[100];
    
    for(int i = 0; i < noOfStudents; i++)
    {
        printf("Enter Roll No, Name, Mark 1, Mark 2, Mark 3: \n");
        scanf("%d %50s %lf %lf %lf", &student[i].rollNo, student[i].name, &student[i].mark1, &student[i].mark2, &student[i].mark3);
        bool rollNoPassed = rollNoValidation(student[i].rollNo);
        bool namePassed = nameValidation(student[i].name);
        bool marksPassed = marksValidation(student[i].mark1, student[i].mark2, student[i].mark3);
        if(rollNoPassed && namePassed && marksPassed)
        {
            student[i].totalMarks = calculateTotalMarks(student[i].mark1, student[i].mark2, student[i].mark3);
            student[i].averageMark = calculateAverageMark(student[i].totalMarks);
            student[i].grade = calculateGrade(student[i].averageMark);
        }
        else
        {
            printf("Please enter valid details.\n");
            i--;
        }

    }
    sortByRollNo(student, noOfStudents);
    displayStudentsRecord(student, noOfStudents);
    printf("List of Roll Numbers (via recursion): ");
    displayRollNumber(student, noOfStudents, 0);

    return 0;

}


bool rollNoValidation(int rollNo)
{
    return rollNo > 0;
}

bool nameValidation(char name[50])
{
    for(int i = 0; name[i] != '\0'; i++)
    {
        if(!(name[i]>='a' && name[i]<='z'))
        {
            return false;
        }
    }
    return true;
}

bool marksValidation(double mark1, double mark2, double mark3)
{
    if(mark1 < 0 || mark1 > 100 ||
       mark2 < 0 || mark2 > 100 ||
       mark3 < 0 || mark3 > 100  )
    {
        return false;
    }
    return true;
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

void sortByRollNo(Student student[], int noOfStudents)
{
    bool swap = false;
    for(int i = 0; i < noOfStudents - 1; i++)
    {
        swap = false;
        for(int j = 0; j < noOfStudents - i - 1; j++)
        {
            if(student[j].rollNo > student[j+1].rollNo)
            {
                Student tempStudent = student[j];
                student[j] = student[j+1];
                student[j+1] = tempStudent;
                swap = true;
            }
        }
        if(swap == false)
        {
            break;
        }
    }
}

void displayStudentsRecord(Student student[], int noOfStudents)
{
    for(int i = 0; i < noOfStudents; i++)
    {
        printf("Roll Number: %d\n",student[i].rollNo);
        printf("Name: %s\n",student[i].name);
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

void displayRollNumber(Student student[], int noOfStudents, int start)
{
    if(noOfStudents == start)
    {
        return;
    }
    printf("%d ",student[start].rollNo);
    displayRollNumber(student, noOfStudents, start+1);
}