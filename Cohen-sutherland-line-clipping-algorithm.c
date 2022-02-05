#include<stdio.h>
#include<GL/glu.h>
#include<GL/glut.h>

int iteration = 0, inputType;
int left = 1, right = 2, bottom = 4, top = 8, c1, c2;
float xMin = -0.5, xMax = 0.5, yMin = -0.5, yMax = 0.5;
float x1, y1, x2, y2;
int rejectFlag = 0;
FILE *fptr;

int getCode(float x, float y)
{
    int code = 0;
    if(x<xMin)
        code = code | left;
    if(x>xMax)
        code = code | right;
    if(y<yMin)
        code = code | bottom;
    if(y>yMax)
        code = code | top;
    return code;

}

void clip()
{
    int c;
    float x, y;
    if(c1)
        c = c1;
    else
        c = c2;
    if(c & left)
    {
        x = xMin;
        y = y1+(y2-y1)/(x2-x1)*(xMin-x1);
    }
    if(c & right)
    {
        x = xMax;
        y = y1+(y2-y1)/(x2-x1)*(xMax-x1);
    }
    if(c & bottom)
    {
        y = yMin;
        x = x1+(x2-x1)/(y2-y1)*(yMin-y1);
    }
    if(c & top)
    {
        y = yMax;
        x = x1+(x2-x1)/(y2-y1)*(yMax-y1);
    }
    if(c == c1)
    {
        x1 = x;
        y1 = y;
    }
    else
    {
        x2 = x;
        y2 = y;
    }
}

float randomNumber()
{
    return (rand()%(21))-10;
}
void inputRandom()
{
    x1= randomNumber()/10;
    y1= randomNumber()/10;
    x2= randomNumber()/10;
    x2= randomNumber()/10;
}
void inputFile()
{
    fscanf(fptr, "%f %f %f %f", &x1, &y1, &x2, &y2);
}

void display(void)
{
    if(inputType == 1)inputFile();
    else if(inputType == 2) inputRandom();

    printf("Input %d: (x1, y1) = (%f, %f)  (x2, y2) = (%f, %f)\n",++iteration, x1, y1, x2, y2);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1,1,1); /// show rectangle in white color
    glBegin(GL_LINE_LOOP);
    glVertex2f(xMin, yMin);
    glVertex2f(xMax, yMin);
    glVertex2f(xMax, yMax);
    glVertex2f(xMin, yMax);
    glEnd();

    glColor3f(1,0,0); // show input line in red color
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();

    while(1)
    {
        // get region code for two points
        c1 = getCode(x1, y1);
        c2 = getCode(x2, y2);
        if((c1|c2) == 0) //trivially Accept
        {
            break;
        }
        if((c1&c2) == 0) // Clipping Candidate
        {
            clip();
        }
        else //trivially reject
        {
            rejectFlag = 1;
            break;
        }

    }
    if(!rejectFlag)
    {
        glColor3f(1,1,1); // show result line in white color
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
        printf("Output: (x1, y1) = (%f, %f)  (x2, y2) = (%f, %f)\n\n", x1, y1, x2, y2);
    }
    else
        printf("Trivially rejected\n\n");
    glFlush();
    rejectFlag = 0;
}

int main (int argc, char** argv)
{
    printf("Select Input Type:\n");
    printf("1. Input File\n");
    printf("2. Random number\n");
    scanf("%d", &inputType);
    fptr = fopen("E:/Fall 2021/CSE 420/OpenGl/sample/input.txt","r");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Cohen-Sutherland");
    glutDisplayFunc(display);
    glutMainLoop();

    fclose(fptr);

}

