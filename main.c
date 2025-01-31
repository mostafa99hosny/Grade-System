#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#define PASSING_SCORE 60
COORD coord = {0, 0};

void gotoxy(int x, int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int ForgC)
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

struct Subject
{
    char name[40];
    int score;
    float averageScore;
};

typedef struct Subject Subject;

struct Student
{
    int id;
    char name[20];
    Subject *Sub;
    int subjectCount;
    char grade;
};

typedef struct Student Student;

Student* addStudent(Student* students, int* studentCount)
{
    int currentY = 5;
    int idExists;

    Student* newStudent = (Student*)malloc(sizeof(Student));

    do
    {
        idExists = 0;
        gotoxy(50, currentY);
        printf("Enter student ID: ");
        scanf("%d", &newStudent->id);
        currentY++;
        for (int i = 0; i < *studentCount; i++)
        {
            if (students[i].id == newStudent->id)
            {
                idExists = 1;
                gotoxy(50, currentY);
                printf("Error: Student ID %d already exists. Please enter a unique ID.\n", newStudent->id);
                currentY++;
                break;
            }
        }
    }
    while (idExists);

    gotoxy(50, currentY);
    printf("Enter student name: ");
    scanf("%s", newStudent->name);
    currentY++;

    gotoxy(50, currentY);
    printf("Enter the number of subjects: ");
    scanf("%d", &newStudent->subjectCount);
    currentY++;
    newStudent->Sub = (Subject*)malloc(newStudent->subjectCount * sizeof(Subject));

    for (int i = 0; i < newStudent->subjectCount; i++)
    {
        gotoxy(50, currentY);
        printf("Enter name of subject %d: ", i + 1);
        scanf("%s", newStudent->Sub[i].name);
        currentY++;
        gotoxy(50, currentY);
        printf("Enter score of subject %d: ", i + 1);
        scanf("%d", &newStudent->Sub[i].score);
        currentY++;
    }
    gotoxy(50, currentY);
    printf("Student and subjects added successfully.\n");
    currentY++;
    gotoxy(50, currentY);
    printf("Press any key to return to the menu...");
    getch();
    students = realloc(students, (*studentCount + 1) * sizeof(Student));
    students[*studentCount] = *newStudent;
    (*studentCount)++;
    free(newStudent);

    return students;
}

void editScores(Student* students, int studentCount)
{
    int id;
    int found = 0;
    int currentY = 5;
    gotoxy(50, currentY);
    printf("Enter the ID of the student that his scores you want to edit: ");
    scanf("%d", &id);
    currentY++;
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].id == id)
        {
            found = 1;
            gotoxy(50, currentY);
            printf("Editing scores for student ID: %d, Name: %s\n", students[i].id, students[i].name);
            currentY++;
            for (int j = 0; j < students[i].subjectCount; j++)
            {
                gotoxy(50, currentY);
                printf("Current score for subject '%s': %d\n", students[i].Sub[j].name, students[i].Sub[j].score);
                currentY++;

                gotoxy(50, currentY);
                printf("Enter new score for '%s': ", students[i].Sub[j].name);
                scanf("%d", &students[i].Sub[j].score);
                currentY++;
            }
            gotoxy(50, currentY);
            printf("Scores updated successfully for student ID: %d, Name: %s\n", students[i].id, students[i].name);
            currentY++;
            break;
        }
    }

    if (!found)
    {
        gotoxy(50, currentY);

        printf("Student with ID %d not found.\n",id);
        currentY++;

    }
    gotoxy(50, currentY);
    printf("Press any key to return to the menu...");
    getch();
}
void calculateGrades(Student* students, int studentCount)
{
    int id, found = 0;
    int currentY = 5;

    gotoxy(50, currentY);
    printf("Enter the ID of the student to calculate grades for: ");
    scanf("%d", &id);
    currentY++;

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].id == id)
        {
            found = 1;

            gotoxy(50, currentY);
            printf("Calculating grades for student ID: %d, Name: %s\n", students[i].id, students[i].name);
            currentY++;

            int totalScore = 0;
            for (int j = 0; j < students[i].subjectCount; j++)
            {
                totalScore += students[i].Sub[j].score;
            }
            float averageScore = (float)totalScore / students[i].subjectCount;

            char grade;
            if (averageScore >= 90)
                grade = 'A';
            else if (averageScore >= 80)
                grade = 'B';
            else if (averageScore >= 70)
                grade = 'C';
            else if (averageScore >= 60)
                grade = 'D';
            else
                grade = 'F';

            students[i].grade = grade;

            gotoxy(50, currentY);
            printf("Average Score: %.2f, Grade: %c\n", averageScore, students[i].grade);
            currentY++;

            gotoxy(50, currentY);
            printf("Grade calculated successfully for student ID: %d.\n", students[i].id);
            currentY++;
            break;
        }
    }

    if (!found)
    {
        gotoxy(50, currentY);
        printf("No student found with ID: %d.\n", id);
        currentY++;
    }

    gotoxy(50, currentY);
    printf("Press any key to return to the menu...");
    getch();
}
void displayResults(Student* students, int studentCount)
{
    int currentY = 5;
    float totalAllScores = 0.0;
    int totalSubjects = 0;
    float highestAverage = 0.0;
    char highestAverageStudent[20];
    int totalStudentsPassed = 0;
    int totalStudentsFailed = 0;

    if (studentCount == 0)
    {
        gotoxy(50, currentY);
        printf("No students available to display results.\n");
        currentY++;
        gotoxy(50, currentY);
        printf("Press any key to return to the menu...");
        getch();
        return;
    }

    gotoxy(50, currentY);
    printf("Displaying results for all students:\n");
    currentY++;

    for (int i = 0; i < studentCount; i++)
    {
        int totalScore = 0;
        int passedSubjects = 0;
        int failedSubjects = 0;

        for (int j = 0; j < students[i].subjectCount; j++)
        {
            totalScore += students[i].Sub[j].score;
            totalAllScores += students[i].Sub[j].score;
            totalSubjects++;

            if (students[i].Sub[j].score >= PASSING_SCORE)
                passedSubjects++;
            else
                failedSubjects++;
        }

        float averageScore = (float)totalScore / students[i].subjectCount;

        if (averageScore > highestAverage)
        {
            highestAverage = averageScore;
            strcpy(highestAverageStudent, students[i].name);
        }

        gotoxy(50, currentY);
        printf("Student ID: %d, Name: %s\n", students[i].id, students[i].name);
        currentY++;
        gotoxy(50, currentY);
        printf("  Average Score: %.2f\n", averageScore);
        currentY++;
        gotoxy(50, currentY);
        printf("  Passed Subjects: %d, Failed Subjects: %d\n", passedSubjects, failedSubjects);
        currentY++;

        if (failedSubjects == 0)
            totalStudentsPassed++;
        else
            totalStudentsFailed++;
    }
    float overallAverage = totalAllScores / totalSubjects;

    gotoxy(50, currentY);
    printf("Summary:\n");
    currentY++;
    gotoxy(50, currentY);
    printf("  Total Students Passed: %d\n", totalStudentsPassed);
    currentY++;
    gotoxy(50, currentY);
    printf("  Total Students Failed: %d\n", totalStudentsFailed);
    currentY++;
    gotoxy(50, currentY);
    printf("  Overall Average Score: %.2f\n", overallAverage);
    currentY++;
    gotoxy(50, currentY);
    printf("  Highest Average: %.2f by %s\n", highestAverage, highestAverageStudent);
    currentY++;

    gotoxy(50, currentY);
    printf("Press any key to return to the menu...");
    getch();
}
int main()
{
    char StudentMenu[5][80] = {"Add Student","Update Student","Calculate Grades","Display Results", "EXIT"};
    int size = 5;
    int currentOption = 0;
    char ch;
    int choose = 0;
    Student* students = NULL;
    int studentCount = 0;
    system("COLOR 50");
    while (1)
    {
        while (!choose)
        {
            for (int i = 0; i < size; i++)
            {
                gotoxy(50, 8 + 4 * i);
                if (currentOption == i)
                {
                    SetColor(10);
                    printf("-->%s", StudentMenu[i]);
                }
                else
                {
                    SetColor(14);
                    printf("%s", StudentMenu[i]);
                }
            }
            ch = getch();
            if (ch == -32)
            {
                ch = getch();
                if (ch == 80)
                {
                    currentOption = (currentOption + 1) % size;
                }
                else if (ch == 72)
                {
                    currentOption = (currentOption - 1 + size) % size;
                }
            }
            else if (ch == 13)
            {
                choose = 1;

            }
            Sleep(50);
            system("CLS");
        }
        system("cls");
        switch (currentOption)
        {
        case 0:
            students = addStudent(students, &studentCount);
            choose = 0;
            system("cls");
            break;
        case 1:
            editScores(students, studentCount);
            choose = 0;
            system("cls");
            break;
        case 2:
            calculateGrades(students, studentCount);
            choose = 0;
            system("cls");
            break;
        case 3:
            displayResults(students, studentCount);
            choose = 0;
            system("cls");
            break;
        case 4:
            printf("Exiting the program.\n");
            free(students);
            exit(0);
            break;
        }
    }

    return 0;
}
