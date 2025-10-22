#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define TOTAL_SUBJECTS 3
const float total_subjects = 3.0;

typedef enum
{
    Grade_A = 85,
    Grade_B = 70,
    Grade_C = 50,
    Grade_D = 35,
    Grade_F = 0
} Grade;

typedef struct
{
    int rollNo;
    char name[50];
    double marks[3];
    double totalMarks;
    double averageMark;
    char grade;

} Student;

bool rollNoValidation(int rollNo);
bool nameValidation(char name[50]);
bool marksValidation(double marks[], int size);
double calculateTotalMarks(double marks[], int size);
double calculateAverageMark(double marks[], int size);
Grade calculateGrade(double totalMarks);
void sortByRollNo(Student student[], int noOfStudents);
void displayStudentsRecord(Student student[], int noOfStudents);
void displayPerformance(char grade);
void displayRollNumber(Student student[], int noOfStudents, int start);



int main()
{
    int noOfStudents;
    while(true)
    {
        printf("Enter the number of students: ");
        scanf("%d",&noOfStudents);

        if(noOfStudents < 1 || noOfStudents > 100)
        {
            printf("The students count should be between 1 to 100 only.\n");
            continue;
        }
        else
        {
            break;
        }
    }
    Student student[noOfStudents];
    
    for(int i = 0; i < noOfStudents; i++)
    {
        printf("Enter Roll No, Name, Mark 1, Mark 2, Mark 3: \n");
        scanf("%d %50s %lf %lf %lf", &student[i].rollNo, student[i].name, &student[i].marks[0], &student[i].marks[1], &student[i].marks[2]);
        bool rollNoPassed = rollNoValidation(student[i].rollNo);
        bool namePassed = nameValidation(student[i].name);
        bool marksPassed = marksValidation(student[i].marks, TOTAL_SUBJECTS);
        if(rollNoPassed && namePassed && marksPassed)
        {
            student[i].totalMarks = calculateTotalMarks(student[i].marks, TOTAL_SUBJECTS);
            student[i].averageMark = calculateAverageMark(student[i].marks, TOTAL_SUBJECTS);
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
        if(!((name[i]>='a' && name[i]<='z') || (name[i]>='A' && name[i]<='Z') || name[i]==' '))
        {
            printf("Please enter a valid name.\n");
            return false;
        }
    }
    return true;
}

bool marksValidation(double marks[], int totalSubjects)
{
    for (int i = 0; i < totalSubjects; i++)
    {
        if(marks[i] < 0 || marks[i] > 100)
        {
            printf("Marks should be between 0 to 100 only.\n");
            return false;
        }

    }
    return true;
}

double calculateTotalMarks(double marks[], int totalSubjects)
{
    double totalMark = 0;
    for (int i = 0; i < totalSubjects; i++)
    {
        totalMark += marks[i];
    }
    return totalMark;
}

double calculateAverageMark(double marks[], int totalSubjects)
{
    double totalMark = calculateTotalMarks(marks, totalSubjects);
    return totalMark / total_subjects;
}

Grade calculateGrade(double averageMark)
{
     if(averageMark >= Grade_A && averageMark <= 100)
     {
        return 'A';
     }
     else if(averageMark >= Grade_B && averageMark < Grade_A)
     {
        return 'B';
     }
     else if(averageMark >= Grade_C && averageMark < Grade_B)
     {
        return 'C';
     }
     else if(averageMark>= Grade_D && averageMark < Grade_C)
     {
        return 'D';
     }
     else if(averageMark >= Grade_F && averageMark < Grade_D)
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
    switch (grade)
    {
        case 'A':
            printf("Performance: *****\n");
            break;
        case 'B':
            printf("Performance: ****\n");
            break;
        case 'C':
            printf("Performance: ***\n");
            break;
        case 'D':
            printf("Performance: **\n");
            break;
        case 'F':
            printf("Performance: Grade is too low.\n");
            break;
        default:
            printf("Invalid grade.\n");
    }
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