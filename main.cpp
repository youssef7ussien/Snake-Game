#include <iostream> /* started on 15/8/2019 */
#include <fstream>
#include <conio.h>  /* 70 = UP / 80 = DOWN / 75 = LEFT / 77 = RIGHT  */
#include <windows.h>
#include <iomanip>
#include <ctime>

using namespace std;

enum directions {UP=1, DOWN, RIGHT, LEFT};
struct coordinates {int X, Y; };
struct colorGame{int border, snake, text; };

class snake
{
    int score,progressBar,tailLength,fruitNumber,repeatColors[2];
    float bonusProgressBar;
    directions direction;
    coordinates fruit,head,*tail,bonusFruit;
public:
    bool stopPlay,newGame;
    int level,speed,gameMode;
    snake()
    {
        tail=new coordinates[100];
        speed=55;
        gameMode=1;
        initialValueSnake();
    }
    void initialValueSnake()
    {
        newGame=true;
        stopPlay=true;
        score=0;
        repeatColors[0]=0; // For the last fruit campaign mode
        repeatColors[1]=0; // For the bonus fruit
        tailLength=2;
        fruitNumber=0;
        level=1;
        progressBar=1;
        bonusProgressBar=1;
        direction=RIGHT;
        fruit.X=0;      fruit.Y=0;
        bonusFruit.X=0; bonusFruit.Y=0;
        head.X=60;      head.Y=14;
        tail[0].X=59; tail[0].Y=14;
        tail[1].X=58; tail[1].Y=14;
    }
    void initialValueCampaignMode()
    {
        stopPlay=true;
        fruitNumber=0;
        repeatColors[0]=0; // For the last fruit campaign mode
        repeatColors[1]=0; // For the bonus fruit
        tailLength=2;
        progressBar=1;
        bonusProgressBar=1;
        direction=RIGHT;
        fruit.X=0; fruit.Y=0;
        bonusFruit.X=0; bonusFruit.X=0;
        head.X=60; head.Y=14;
        tail[0].X=59; tail[0].Y=14;
        tail[1].X=58; tail[1].Y=14;
    }
    void showData()
    {
    /*  1-newGame   2-gameMode   3-score   4-tailLength
        5-speed   6-fruitNumber   7-level   8-progressBar
        9-bonusProgressBar   10-head.X , head.X   11-fruit.X , fruit.Y
        12-bonusFruit.X , bonusFruit.Y   13-tail[i].X
    */
        cout<<"\n 1-newGame: "<<newGame;
        cout<<"\n 2-gameMode: "<<gameMode;
        cout<<"\n 3-score: "<<score;
        cout<<"\n 4-tailLength: "<<tailLength;
        cout<<"\n 5-speed: "<<speed;
        cout<<"\n 6-fruitNumber: "<<fruitNumber;
        cout<<"\n 7-level: "<<level;
        cout<<"\n 8-progressBar: "<<progressBar;
        cout<<"\n 9- bonusProgressBar: "<<bonusProgressBar;
        cout<<"\n 10-head.X: "<<head.X<<"\t, head.X: "<<head.Y;
        cout<<"\n 11-fruit.X: "<<fruit.X<<"\t, fruit.Y: "<<fruit.Y;
        cout<<"\n 12-bonusFruit.X: "<<bonusFruit.X<<"\t, bonusFruit.Y: "<<bonusFruit.Y;
        for(int i=0;i<tailLength;i++)
            cout<<"\n "<<i+13<<"-tail["<<i<<"].X: "<<tail[i].X<<"\t, tail["<<i<<"].Y: "<<tail[i].Y;
    }
    void saveData();
    bool callData();
    void drawSnake(int gameMode=1,int colorSnake=12,bool compaignMode=false);
    bool inputKeyboard(int colorBorder=7);
    void generateFruit(int gameMode=4);
    void generateBonusFruit(int gameMode=4);
    bool gameLogic(int gameMode=4);
    void campaignMode();
    ~snake()
    {
        delete[] tail;
    }
};

void setConsoleSize(short width, short height)
{
    _COORD coord={width,height};

    _SMALL_RECT Rect{};
    Rect.Top=0;
    Rect.Left=0;
    Rect.Bottom=height-1;
    Rect.Right=width-1;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(Handle, coord);
    SetConsoleWindowInfo(Handle, TRUE, &Rect);
}

void hideCursor()
{
   CONSOLE_CURSOR_INFO info;
   info.dwSize=1;
   info.bVisible=false;
   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
}

void setCursor(int x,int y)
{
    COORD coordinates;
    coordinates.X=x;
    coordinates.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coordinates);
}

int color(int numColor=7)
{
    return SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),numColor);
}

void initConsole()
{
    hideCursor();
    color();
    setConsoleSize(120,30);
    SetConsoleTitle(" SNAKE GAME ");
}

void logoSnakeGame(int coordX,int coordY)
{
    setCursor(coordX,coordY);
    cout<<char(218);
    for(int i=0;i<12;i++) cout<<char(196);
    cout<<char(191);
    setCursor(coordX,coordY+1);
    cout<<char(179)<<"   "; color(3);  cout<<char(220);  color();
    cout<<"  "<<char(220)<<char(220)<<char(220)
        <<char(220)<<char(220)<<" "<<char(179);
    setCursor(coordX,coordY+2);
    cout<<char(179)<<" "<<char(220)<<char(220)<<char(220)<<char(220)<<char(220)
        <<char(220)<<char(220)<<char(220)<<char(220)<<char(219)<<" "<<char(179);
    setCursor(coordX,coordY+3);
    cout<<char(179)<<" "<<char(219)<<char(220)<<char(220)<<char(220)<<char(220)
        <<char(220)<<char(220)<<char(220)<<char(220)<<char(220)<<" "<<char(179);
    setCursor(coordX,coordY+4);
    cout<<char(179)<<" "<<char(220)<<char(220)<<char(220)<<char(220)<<char(220)
        <<char(220)<<char(220)<<char(220)<<char(220)<<char(219)<<" "<<char(179);
    setCursor(coordX,coordY+5);
    cout<<char(179)<<" "<<char(219)<<char(220)<<char(220)<<char(220)<<char(220)<<"      "<<char(179);
    setCursor(coordX,coordY+6);
    cout<<char(192);
    for(int i=0;i<12;i++) cout<<char(196);
    cout<<char(217);

}

void rectangleInputInterface(int coordY,int coordX=48,int width=23,int length=1,bool temp=false)
{
    char mark1=196,mark2=218,mark3=191;
    if(temp)
    { mark1=205;  mark2=201;  mark3=187; }
    setCursor(coordX,coordY);
//*************************************** Top Border ************************************
    for(int i=0;i<width;i++)
    {
        if(i==0)  cout<<mark2;
        else if(i==width-1) cout<<mark3;
        else cout<<mark1;
    }
//***************************************************************************************
    mark1=196;  mark2=192;  mark3=217;
    if(temp)
    { mark1=205;  mark2=200;  mark3=188; }
    setCursor(coordX,coordY+length+1);
//************************************* Bottom Border ***********************************
    for(int i=0;i<width;i++)
    {
        if(i==0)  cout<<mark2;
        else if(i==width-1) cout<<mark3;
        else cout<<mark1;
    }
}

void interfaceStartGame()
{
    // 223 >> upMark , 220 >> downMark;
logoSnakeGame(53,8);
    setCursor(25,20); color(); cout<<char(220);
    for (int i=0 ; i<17 ; i++)
    {
        color(); Sleep(65); cout<<"\b"<<char(220)<<char(219);
        Sleep(25); color(9); cout<<char(223);
        if(i==2)
        {
            setCursor(37,18); color(8); cout<<"You can use "; color(12); cout<<"W";
            color(8); cout<<char(249); color(12); cout<<"S"; color(8); cout<<char(249);
            color(12); cout<<"A"; color(8); cout<<char(249); color(12); cout<<"D";
            color(8); cout<<" or "; color(12); cout<<"Directions";
            color(8); cout<<" to control .";  color(); setCursor(36,20);
        }
        else if(i==7)
        {
            setCursor(37,18); cout<<"                                              ";
            color(); setCursor(56,20);
        }
        else if(i==9)
        {
            setCursor(36,18); color(6);  cout<<"*"; color(8); cout<<"Recommend using ";
            color(12); cout<<"W"; color(8); cout<<char(249); color(12); cout<<"S";
            color(8); cout<<char(249); color(12); cout<<"A"; color(8); cout<<char(249);
            color(12); cout<<"D"; color(8); cout<<" to control the snake .";
            color(); setCursor(64,20);
        }
        else if(i==14)
        {
            setCursor(36,18); cout<<"                                               ";
            color(); setCursor(84,20);
        }
        else if(i==16)
        {
            setCursor(53,17); color(15); cout<<"LET'S GO "; color(3); cout<<"."; color(14);
            cout<<"."; color(11); cout<<"."; color(10);  cout<<"."; color(4);
            cout<<"."; color(); setCursor(92,20);
        }
        color(); Sleep(65); cout<<"\b"<<char(223)<<char(219);
        Sleep(25); color(9); cout<<char(220);
        if(i==16) { color(); cout<<"\b"<<char(220);  color(9); cout<<char(220); }
    } color(); Sleep(50);
}

int interfaceMenuGame(bool newGame=1)
{
    system("cls");
    char key=80;  int index=1;
    rectangleInputInterface(1,43,34,1,true);
    setCursor(43,2);
    cout<<char(186)<<" ";  color(116);
    cout<<"          SNAKE GAME          ";
    color();   cout<<" "<<char(186);
    for(int i=0;i<2;i++)
    {
        i==0?  setCursor(40,4) : setCursor(40,25);
        for(int i=0;i<40;i++)
            cout<<char(196);
    }
    rectangleInputInterface(6);
    rectangleInputInterface(9);
    rectangleInputInterface(12);
    rectangleInputInterface(15);
    rectangleInputInterface(18);
    rectangleInputInterface(21);
    while(key!=13)
    {
        if(newGame && index==1 && (key==80 || key=='s' || key=='S'))
            index=2;
        else if(newGame && index==1 && (key==72 || key=='w' || key=='W'))
            index=6;
        setCursor(48,7);
        cout<<char(179)<<" ";
        if(index==1)  color(192);
        else if(newGame) color(8);
        cout<<"   CONTINUE GAME   ";
        color();  cout<<" "<<char(179);
        setCursor(48,10);
        cout<<char(179)<<" ";
        if(index==2)  color(192);
        cout<<"     NEW GAME      ";
        color();  cout<<" "<<char(179);
        setCursor(48,13);
        cout<<char(179)<<" ";
        if(index==3)  color(192);
        cout<<"    SPEED SNAKE    ";
        color();  cout<<" "<<char(179);
        setCursor(48,16);
        cout<<char(179)<<" ";
        if(index==4)  color(192);
        cout<<"     GAME MODE     ";
        color();  cout<<" "<<char(179);
        setCursor(48,19);
        cout<<char(179)<<" ";
        if(index==5)  color(192);
        cout<<"       HELP        ";
        color();  cout<<" "<<char(179);
        setCursor(48,22);
        cout<<char(179)<<" ";
        if(index==6)  color(192);
        cout<<"       EXIT        ";
        color();  cout<<" "<<char(179);
        setCursor(0,28);
        while(1)
        {
            key=getch();
            if(key==80 || key=='s' || key=='S')
                { index++; break; }
            else if(key==72 || key=='w' || key=='W')
                { index--; break; }
            else if(key==13)
                break;
        }
        if(index==7)
            index=1;
        else if(index==0)
            index=6;
    }
    return index;
}

int interfaceSpeedSnake(int indexSpeed=55)
{
/* this function returns 8 numbers each number expresses a value for speed. */
    system("cls");
    char key='0'; int index=1,speedValue[8]={140,80,75,55,45,20,10,5};
    for(int i=0;i<8;i++)
    {
        if(indexSpeed==speedValue[i])
        { indexSpeed=i; break; }
    }
    rectangleInputInterface(1,46,29,1,true);
    setCursor(46,2);
    cout<<char(186)<<" ";  color(116);
    cout<<"       SPEED SNAKE       ";
    color();   cout<<" "<<char(186);
    for(int i=0;i<2;i++)
    {
        i==0?  setCursor(40,4) : setCursor(40,21);
        for(int i=0;i<41;i++)
            cout<<char(196);
    }
    rectangleInputInterface(6,52,17);   color(8);
//********************************** LEFT ***********************************************
    setCursor(46,6); cout<<char(218)<<char(196)<<char(196)<<char(196)<<char(191);
    setCursor(46,7); cout<<char(179)<<" < "<<char(179);
    setCursor(46,8); cout<<char(192)<<char(196)<<char(196)<<char(196)<<char(217);
//********************************** RIGHT **********************************************
    setCursor(70,6); cout<<char(218)<<char(196)<<char(196)<<char(196)<<char(191);
    setCursor(70,7); cout<<char(179)<<" > "<<char(179);
    setCursor(70,8); cout<<char(192)<<char(196)<<char(196)<<char(196)<<char(217);
    color();
    rectangleInputInterface(10,35,51);
    setCursor(35,11);  cout<<char(179);
    setCursor(85,11);  cout<<char(179);
    rectangleInputInterface(17,54,12);
    for(int i=0;i<=indexSpeed;i++)
    {
        setCursor(37+(i*6),11); color(4);
        cout<<char(219)<<char(219)<<char(219)<<char(219)<<char(219)<<" ";  color();
    }
    bool Down=false;
    while(key!=13)
    {
        setCursor(52,7);
        cout<<char(179)<<" ";
        if(index==1)  { color(192); Down=false; }
        if(indexSpeed==0)                        cout<<"  VERY SLOW  ";
        else if(indexSpeed==1 || indexSpeed==2)  cout<<"    SLOW     ";
        else if(indexSpeed==3 || indexSpeed==4)  cout<<"   MEDIUM    ";
        else if(indexSpeed==5 || indexSpeed==6)  cout<<"    FAST     ";
        else if(indexSpeed==7)                   cout<<"  VERY FAST  ";
        color();  cout<<" "<<char(179);
        if(key==77 || key=='d' || key=='D')
        {
            setCursor(37+(indexSpeed*6),11); color(4);
            cout<<char(219)<<char(219)<<char(219)<<char(219)<<char(219)<<" ";
            color();
        }
        else if(key==75 || key=='a' || key=='A')
        { setCursor(37+(indexSpeed+1)*6,11); cout<<"     "; }
        setCursor(54,18);
        cout<<char(179)<<" ";
        if(index==2)  color(192);
        cout<<"  BACK  ";
        color();  cout<<" "<<char(179);
        setCursor(0,24);
        while(1)
        {
            key=getch();
            if((key==77 || key=='d' || key=='D') && indexSpeed!=7 && !Down)
                { indexSpeed++; break; }
            else if((key==75 || key=='a' || key=='A') && indexSpeed!=0 && !Down)
                { indexSpeed--; break; }
            else if(key==72 || key=='w' || key=='W')
                { index++; break; }
            else if(key==80 || key=='s' || key=='S')
                { index--; Down=true; break; }
            else if(key==13 && index==2)
                break;
        }
        if(index>2)
            index=1;
        else if(index<1)
            index=2;
    }
    return speedValue[indexSpeed];
}

int interfaceGameMode()
{
/* this function return two number :
    * First number :
    {
        0 >> means no change has changed.
        1 >> it's that Classic Mode.
        3 >> it's that Campaign Mode.
    }
    * Second number :   form 4 to 9 >> it's Maps.
*/
    while(1)
    {
        system("cls");
        char key='0'; int gameMode=1;
        rectangleInputInterface(1,46,29,1,true);
        setCursor(46,2);
        cout<<char(186)<<" ";  color(116);
        cout<<"        GAME MODE        ";
        color();   cout<<" "<<char(186);
        for(int i=0;i<2;i++)
        {
            i==0?  setCursor(40,4) : setCursor(40,21);
            for(int i=0;i<41;i++)
                cout<<char(196);
        }
        rectangleInputInterface(5,50,21);
        rectangleInputInterface(8,50,21);
        rectangleInputInterface(11,50,21);
        rectangleInputInterface(17,54,12);
        while(key!=13)
        {
            setCursor(50,6);
            cout<<char(179)<<" ";
            if(gameMode==1)
            {
                setCursor(35,15);
                cout<<"                                                    ";
                setCursor(46,15); color(12); cout<<"*"; color(8);
                cout<<"Normal mode with map No Maze";
                setCursor(52,6); color(192);
            }
            cout<<"     CLASSIC     ";
            color();  cout<<" "<<char(179);
            setCursor(50,9);
            cout<<char(179)<<" ";
            if(gameMode==2)
            {
                setCursor(35,15);
                cout<<"                                                    ";
                setCursor(47,15); color(12); cout<<"*"; color(8);
                cout<<"Select one of the six maps";
                setCursor(52,9); color(192);
            }
            cout<<"     CUSTOM      ";
            color();  cout<<" "<<char(179);
            setCursor(50,12);
            cout<<char(179)<<" ";
            if(gameMode==3)
            {
                setCursor(35,15);
                cout<<"                                                    ";
               setCursor(35,15); color(12); cout<<"*"; color(8);
                cout<<"Campaign mode is a challenge consisting of 5 levels";
                setCursor(52,12); color(192);
            }
            cout<<"    CAMPAIGN     ";
            color();  cout<<" "<<char(179);
            setCursor(54,18);
            cout<<char(179)<<" ";
            if(gameMode==4)
            {
                setCursor(35,15);
                cout<<"                                                    ";
                setCursor(56,18); color(192);
            }
            cout<<"  BACK  ";
            color();  cout<<" "<<char(179);
            setCursor(0,20);
            while(1)
            {
                key=getch();
                if(key==80 || key=='s' || key=='S')
                    { gameMode++; break; }
                else if(key==72 || key=='w' || key=='W')
                    { gameMode--; break; }
                else if(key==13)
                    break;
            }
            if(gameMode==5)
                gameMode=1;
            else if(gameMode==0)
                gameMode=4;
        }
        if(gameMode==1 || gameMode==3)
            return gameMode;
        else if(gameMode==4)
            return 0;
        else if(gameMode==2)
        {
            rectangleInputInterface(6,31,59,11);
            for(int i=7;i<18;i++)
            {
                setCursor(31,i);
                cout<<char(179)<<"                                                         "<<char(179);
            }
            rectangleInputInterface(7,35,15);
            rectangleInputInterface(7,53,15);
            rectangleInputInterface(7,71,15);
            rectangleInputInterface(11,35,15);
            rectangleInputInterface(11,53,15);
            rectangleInputInterface(11,71,15);
            rectangleInputInterface(15,35,51);
            int numMap=4;    key='0';
            while(key!=13)
            {
                setCursor(35,8);
                cout<<char(179)<<" ";
                if(numMap==4)  color(192);
                cout<<" NO MAZE   ";
                color();  cout<<" "<<char(179);
                setCursor(53,8);
                cout<<char(179)<<" ";
                if(numMap==5)  color(192);
                cout<<" BOX       ";
                color();  cout<<" "<<char(179);
                setCursor(71,8);
                cout<<char(179)<<" ";
                if(numMap==6)  color(192);
                cout<<" TUNNEL    ";
                color();  cout<<" "<<char(179);
                setCursor(35,12);
                cout<<char(179)<<" ";
                if(numMap==7)  color(192);
                cout<<" MILL      ";
                color();  cout<<" "<<char(179);
                setCursor(53,12);
                cout<<char(179)<<" ";
                if(numMap==8)  color(192);
                cout<<" RAILS     ";
                color();  cout<<" "<<char(179);
                setCursor(71,12);
                cout<<char(179)<<" ";
                if(numMap==9)  color(192);
                cout<<" APARTMENT ";
                color();  cout<<" "<<char(179);
                setCursor(35,16);
                cout<<char(179)<<" ";
                if(numMap==10)  color(192);
                cout<<"                     BACK                      ";
                color();  cout<<" "<<char(179);
                setCursor(0,20);
                while(1)
                {
                    key=getch();
                    if(key==77 || key=='d' || key=='D')
                    { numMap++; break; }
                    else if(key==75 || key=='a' || key=='A')
                    { numMap--; break; }
                    else if(key==80 || key=='s' || key=='S')
                    {
                        if(numMap==8) numMap+=2;
                        else if(numMap==9) numMap+=1;
                        else numMap+=3;
                        break;
                    }
                    else if(key==72 || key=='w' || key=='W')
                    { numMap-=3; break; }
                    else if(key==13)
                        break;
                }
                if(numMap>10)
                    numMap=4;
                else if(numMap<4)
                    numMap=10;
            }
            if(numMap!=10)
                return numMap;
        }
    }
}

bool interfaceHelp()
{
    while(1)
    {
        system("cls");
        char key='0';  int index=1; bool doneBack=false;
        rectangleInputInterface(2,15,25,1,true);
        setCursor(15,3);
        cout<<char(186)<<" ";  color(116);
        cout<<"        HELP         ";
        color();   cout<<" "<<char(186);
        for(int i=0;i<2;i++)
        {
            i==0?  setCursor(15,5) : setCursor(15,21);
            for(int j=0;j<27;j++)
                cout<<char(196);
        }
        rectangleInputInterface(6,15,22);
        rectangleInputInterface(9,15,22);
        rectangleInputInterface(12,15,22);
        rectangleInputInterface(15,15,22);
        rectangleInputInterface(18,15,12);
         for(int i=1;i<24;i++)
            { setCursor(42,i); cout<<char(179); }
        while(key!=13)
        {
            setCursor(15,7);
            cout<<char(179)<<" ";
            if(index==1)  color(192);
            cout<<"  HOW TO PLAY     ";
            color();  cout<<" "<<char(179);
            setCursor(15,10);
            cout<<char(179)<<" ";
            if(index==2)  color(192);
            cout<<"  INSTRUCTIONS    ";
            color();  cout<<" "<<char(179);
            setCursor(15,13);
            cout<<char(179)<<" ";
            if(index==3)  color(192);
            cout<<"  CONNECT ME      ";
            color();  cout<<" "<<char(179);
            setCursor(15,16);
            cout<<char(179)<<" ";
            if(index==4)  color(192);
            cout<<"  GAME INFO       ";
            color();  cout<<" "<<char(179);
            setCursor(15,19);
            cout<<char(179)<<" ";
            if(index==5)  color(192);
            cout<<"  BACK  ";
            color();  cout<<" "<<char(179);
            setCursor(0,20);
            while(1)
            {
                key=getch();
                if(key==80 || key=='s' || key=='S')
                    { index++; break; }
                else if(key==72 || key=='w' || key=='W')
                    { index--; break; }
                else if(key==13)
                    break;
            }
            if(index==6)
                index=1;
            else if(index==0)
                index=5;
        }
        if(index==5) return 0;
        setCursor(43,20);
        for(int i=0;i<55;i++)
            cout<<char(196);
        switch(index)
        {
            case 1:
            {
            //*************** Use W,S,A,D or Directions to control the snake. ***************************
                setCursor(45,5); color(14); cout<<char(175); color(6); cout<<" Use "; color(12);
                cout<<"W"; color(); cout<<char(249); color(12); cout<<"S"; color(); cout<<char(249);
                color(12); cout<<"A"; color(); cout<<char(249); color(12); cout<<"D"; color();
                cout<<" or "; color(12); cout<<"Directions"; color(); cout<<" to control the snake.";
                for(int i=0;i<25;i+=24)
                {
                    color(8);
                //************************************ UP ***********************************************
                    setCursor(56+i,7);  cout<<char(218)<<char(196)<<char(196)<<char(196)<<char(191);
                    setCursor(56+i,8);  cout<<char(179); i==0? cout<<" W ":cout<<" ^ "; cout<<char(179);
                    setCursor(56+i,9);  cout<<char(192)<<char(196)<<char(196)<<char(196)<<char(217);
                //********************************** LEFT ***********************************************
                    setCursor(50+i,10);  cout<<char(218)<<char(196)<<char(196)<<char(196)<<char(191);
                    setCursor(50+i,11);  cout<<char(179); i==0? cout<<" A ":cout<<" < "; cout<<char(179);
                    setCursor(50+i,12); cout<<char(192)<<char(196)<<char(196)<<char(196)<<char(217);
                //********************************** DOWN ***********************************************
                    setCursor(56+i,10);  cout<<char(218)<<char(196)<<char(196)<<char(196)<<char(191);
                    setCursor(56+i,11);  cout<<char(179); i==0? cout<<" S ":cout<<" v "; cout<<char(179);
                    setCursor(56+i,12); cout<<char(192)<<char(196)<<char(196)<<char(196)<<char(217);
                //********************************** RIGHT **********************************************
                    setCursor(62+i,10);  cout<<char(218)<<char(196)<<char(196)<<char(196)<<char(191);
                    setCursor(62+i,11);  cout<<char(179); i==0? cout<<" D ":cout<<" > "; cout<<char(179);
                    setCursor(62+i,12); cout<<char(192)<<char(196)<<char(196)<<char(196)<<char(217);
                    color();
                }
            //*************** *Recommend using W,S,A,D to control the snake . ***************************
                setCursor(47,14); color(6);  cout<<"*"; color(8); cout<<"Recommend using ";
                color(12); cout<<"W"; color(8); cout<<char(249); color(12); cout<<"S"; color(8);
                cout<<char(249); color(12); cout<<"A"; color(8); cout<<char(249); color(12); cout<<"D";
                color(8); cout<<" to control the snake ."; color();
            //*************** Eat the food without colliding with any obstacles *************************
                setCursor(45,16); color(14); cout<<char(175); color(6); cout<<" Eat "; color();
                cout<<"the food without colliding with any obstacles";
            //**************  and Eat bonus food quickly to score extra points. *************************
                setCursor(45,18); cout<<" and "; color(6); cout<<"Eat "; color();
                cout<<"bonus food quickly to score extra points. ";
            } break;
            case 2:
            {
                setCursor(45,5); color(15);cout<<" "<<char(249); color(6); cout<<"FIRST";
                color(); cout<<": You must not delete or modify the file ";
                setCursor(55,6); cout<<"associated  with the game, which is ";
                setCursor(55,7); cout<<"named \""; color(12); cout<<"SnakeGame.DAT";
                color(); cout<<"\"."; setCursor(45,9); color(8);
                cout<<"      Because deleting or modifying it may lead        ";
                setCursor(45,10); cout<<"    to the deletion of your progress in the game.    ";
        //********************************************************************************************
                setCursor(45,12);
                for(int i=0;i<51;i++)
                    cout<<char(196);
        //********************************************************************************************
                setCursor(45,14); color(15);cout<<" "<<char(249); color(6); cout<<"SECOND";
                color(); cout<<": When you want to exit the game you have";
                setCursor(56,15); cout<<"to exit by going to \""; color(12); cout<<"EXIT";
                color(); cout<<"\" and "; setCursor(56,16); color(); cout<<"then \"";
                color(12); cout<<"SAVE | YES"; color(); cout<<"\".";
                setCursor(47,18); color(8);
                cout<<"So that you can save your progress in the game."; color();
            } break;
            case 3:
            {
                for(int i=0;i<4;i++)
                {
                    if(i==0)      setCursor(43,10);
                    else if(i==1)
                    {
                        setCursor(52,9);  cout<<char(179);
                        setCursor(52,8);  cout<<char(179);
                        setCursor(52,7);  cout<<char(179);
                        setCursor(52,6);  cout<<char(218);
                    }
                    else if(i==2)
                    {
                        setCursor(52,13);  cout<<char(179);
                        setCursor(52,12);  cout<<char(179);
                        setCursor(52,11);  cout<<char(179);
                        setCursor(52,10);  cout<<char(197);
                    }
                    else          { setCursor(52,14); cout<<char(192); }
                    for(int j=0;j<9;j++)
                        cout<<char(196);
                }
                setCursor(44,19); color(6); cout<<"*"; color(8); cout<<"Phone Number: ";
                color(12);  cout<<"01005492189"; color();
                rectangleInputInterface(5,62,16);
                rectangleInputInterface(9,62,16);
                rectangleInputInterface(13,62,16);
                rectangleInputInterface(21,85,12);
                int input=1;  key='0';
                while(key!=13)
                {
                    setCursor(62,6);
                    cout<<char(179)<<" ";
                    if(input==1)  color(192);
                    cout<<"  FACEBOOK  ";
                    color();  cout<<" "<<char(179);
                    setCursor(62,10);
                    cout<<char(179)<<" ";
                    if(input==2)  color(192);
                    cout<<"  WHATSAPP  ";
                    color();  cout<<" "<<char(179);
                    setCursor(62,14);
                    cout<<char(179)<<" ";
                    if(input==3)  color(192);
                    cout<<"  LINKEDIN  ";
                    color();  cout<<" "<<char(179);
                    setCursor(85,22);
                    cout<<char(179)<<" ";
                    if(input==4)  color(192);
                    cout<<"  BACK  ";
                    color();  cout<<" "<<char(179);
                    setCursor(0,24);
                    while(1)
                    {
                        key=getch();
                        if(key==80 || key=='s' || key=='S')
                            { input++; break; }
                        else if(key==72 || key=='w' || key=='W')
                            { input--; break; }
                        else if(key==13)
                            break;
                    }
                        if(input==5)
                            input=1;
                        else if(input==0)
                            input=4;
                }
                doneBack=true;
                switch(input)
                {
                    case 1:
                    {
                        ShellExecute(NULL,TEXT("open"),TEXT("https://www.facebook.com/youssef7ussien"),NULL,NULL,0);
                    } break;
                    case 2:
                    {
                        ShellExecute(NULL,TEXT("open"),TEXT("http://wa.me/201005492189"),NULL,NULL,0);
                    } break;
                    case 3:
                    {
                        ShellExecute(NULL,TEXT("open"),TEXT("https://www.linkedin.com/in/youssef7ussien"),NULL,NULL,0);
                    } break;
                }
            } break;
            case 4:
            {
                logoSnakeGame(46,3);
                setCursor(61,4);   color(116); cout<<" SNAKE GAME "; color(); cout<<".";
                setCursor(61,6);   cout<<" BY ";  color(12);  cout<<"Youssef Hussien Elsayed"; color();
                setCursor(61,8);   cout<<" Version: 2.1.4.34";
                setCursor(46,11);  color(8); cout<<" Copyright "; color(14);  cout<<"(C)"; color(8);
                cout<<" 2019 youssef7ussien.";
                setCursor(46,13);  cout<<" All rights reserved. "; color();
                setCursor(45,19); color(14); cout<<"*"; color(); cout<<" Using ";
                color(6); cout<<"C++"; color();  cout<<" Language. ";
                rectangleInputInterface(15,44,53);
                rectangleInputInterface(21,85,12);
                int input=0;  key='0';
                while(key!=13)
                {

                    setCursor(44,16);
                    cout<<char(179)<<" ";
                    if(input==1)  color(192);
                    cout<<"                  RATE THE GAME                  ";
                    color();  cout<<" "<<char(179);
                    setCursor(85,22);
                    cout<<char(179)<<" ";
                    if(input==2)  color(192);
                    cout<<"  BACK  ";
                    color();  cout<<" "<<char(179);
                    setCursor(0,24);
                    while(1)
                    {
                        key=getch();
                        if(key==80 || key=='s' || key=='S')
                            { input++; break; }
                        else if(key==72 || key=='w' || key=='W')
                            { input--; break; }
                        else if(key==13 && input!=0)
                            break;
                    }
                    if(input>=3)
                        input=1;
                    else if(input<=0)
                        input=2;
                }
                doneBack=true;
                if(input==1)
                        ShellExecute(NULL,TEXT("open"),TEXT("https://forms.gle/AxRk6bhGDQf37HjU6"),NULL,NULL,0);
            } break;
        }
        if(!doneBack)
        {
            rectangleInputInterface(21,85,12);
            key='0'; index=2;
            while(key!=13)
            {
                setCursor(85,22);
                cout<<char(179)<<" ";
                if(index==1)  color(192);
                cout<<"  BACK  ";
                color();  cout<<" "<<char(179);
                setCursor(0,24);
                while(1)
                {
                    key=getch();
                    if(key==80 || key=='s' || key=='S')
                        { index++; break; }
                    else if(key==72 || key=='w' || key=='W')
                        { index--; break; }
                    else if(key==13 && index==1)
                        break;
                }
                if(index==3)
                    index=1;
                else if(index==0)
                    index=2;
            }
        }
    }
}

bool interfaceExitGame(bool exitGame=true,int colorBorder=7)
{
    char key='0';  int index=2;
    color(colorBorder);
    rectangleInputInterface(11,35,49,4);
    setCursor(35,12);
    cout<<char(179); color();
    cout<<"                 Are You Sure ?                ";
    color(colorBorder);
    cout<<char(179);
    setCursor(35,13);cout<<char(179)<<"   ";
    setCursor(35,14);cout<<char(179)<<"   ";
    setCursor(35,15);cout<<char(179)<<"   ";
    setCursor(50,13);cout<<"                    ";
    setCursor(50,14);cout<<"                    ";
    setCursor(50,15);cout<<"                    ";
    setCursor(80,13);cout<<"   "<<char(179);
    setCursor(80,14);cout<<"   "<<char(179);
    setCursor(80,15);cout<<"   "<<char(179);
    if(!exitGame) rectangleInputInterface(13,39,11);
    else rectangleInputInterface(13,39,16);
    rectangleInputInterface(13,70,10);
    while(key!=13)
        {
            setCursor(39,14);
            color(colorBorder); cout<<char(179)<<" "; color();
            if(index==1)  color(192);
            if(!exitGame) cout<<"  YES  ";
            else  cout<<" SAVE "<<char(179)<<" YES ";
            color(colorBorder); cout<<" "<<char(179); color();
            setCursor(70,14);
            color(colorBorder); cout<<char(179)<<" "; color();
            if(index==2)  color(192);
            cout<<"  NO  ";
            color(colorBorder); cout<<" "<<char(179); color();
            setCursor(0,26);
            while(1)
            {
                key=getch();
                if(key==77 || key=='d' || key=='D')
                    { index++; break; }
                else if(key==75 || key=='a' || key=='A')
                    { index--; break; }
                else if(key==13)
                    break;
            }
            if(index==3)
                index=1;
            else if(index==0)
                index=2;
        }
        if      (index==1 && exitGame) return 1;
        else if(index==1 && !exitGame) return 1;
        else
        {
            if(!exitGame)
                for(int i=0;i<6;i++)
                {
                    setCursor(35,11+i);
                    cout<<"                                                 ";
                }
            return 0;
        }
}

void interfaceLoginForFirstTime() //Login to the game for the first time
{
    color(11);
    system("cls");
    rectangleInputInterface(2,31,57,22);
    for(int i=0;i<22;i++)
    {
        setCursor(31,3+i);  cout<<char(179);
        setCursor(87,3+i);  cout<<char(179);
    }
    setCursor(32,4); color(15);
    cout<<"     Looks like it's your"; color(12); cout<<" FIRST TIME";
    color(15); cout<<" in the game.      ";
//**************************************************************************************
    setCursor(32,6);
    color(6); cout<<" *"; color(); cout<<" Here are some ";
    color(6); cout<<"Instructions:                        ";
//**************************************************************************************
    setCursor(32,8); color(15);cout<<"  "<<char(249); color(11); cout<<"First";
    color(); cout<<": You must not delete or modify the file     ";
    setCursor(32,9); cout<<"         associated with the game, which is named      ";
    setCursor(32,10); cout<<"         \""; color(10); cout<<"SnakeGame.DAT"; color();
    cout<<"\"."; setCursor(32,11); color(8);
    cout<<"      Because deleting or modifying it may lead        ";
    setCursor(32,12); cout<<"    to the deletion of your progress in the game.    ";
//**************************************************************************************
    setCursor(32,14); color(15);cout<<"  "<<char(249); color(11); cout<<"Second";
    color(); cout<<": If you encounter any problems in the       ";
    setCursor(32,15); cout<<"         game all you have to do is to communicate     ";
    setCursor(32,16); cout<<"         with me.                                      ";
    setCursor(32,17);
    color(6); cout<<"  *"; color(8); cout<<"To communicate go to \""; color(2);
    cout<<"HELP"; color(8); cout<<"\" and then \""; color(2);
    cout<<"CONTACT ME"; color(8); cout<<"\".  ";
    setCursor(32,19); color(15);cout<<"  "<<char(249);color(11); cout<<"Third";
    color(); cout<<": For more information go to \""; color(10); cout<<"HELP";
    color(); cout<<"\".          ";
//**************************************************************************************
    setCursor(33,21); color(11);
    for(int i=0;i<53;i++)
        cout<<char(196);
    rectangleInputInterface(22,43,33);
    char key='0'; int index=2;
    while(key!=13)
    {
        setCursor(43,23);
        color(11); cout<<char(179)<<" "; color();
        if(index==1)  color(192);
        cout<<"    I agree and continue.    ";
        color(11); cout<<" "<<char(179); color();
        setCursor(0,24);
        while(1)
        {
            key=getch();
            if(key==80 || key=='s' || key=='S')
                { index++; break; }
            else if(key==72 || key=='w' || key=='W')
                { index--; break; }
            else if(key==13 && index==1)
                break;
        }
        if(index==3)
            index=1;
        else if(index==0)
            index=2;
    }
}

bool interfaceGameOver(int score=1000)
{
    color(4);
    rectangleInputInterface(8,30,59,11);
    for(int i=9;i<20;i++)
    {
        setCursor(30,i);
        cout<<char(179)<<"                                                         "<<char(179);
    }
    rectangleInputInterface(9,51,17);
    setCursor(51,10); cout<<char(180)<<" ";
    color(71); cout<<" GAME OVER ! "; color(4); cout<<" "<<char(195);
    setCursor(44,10); cout<<char(218);
    for(int i=0;i<12;i++)
    {
        if(i==6) setCursor(68,10);
            cout<<char(196);
    }
    cout<<char(191);
    setCursor(44,11); cout<<char(179); setCursor(74,11); cout<<char(179);
    setCursor(44,12); cout<<char(179); setCursor(74,12); cout<<char(179);
    rectangleInputInterface(13,40,39);
    setCursor(40,14); cout<<char(179)<<" "; color(15);
    cout<<"  YOUR SCORE:               "<<setw(5)<<score<<"  ";
    color(4); cout<<" "<<char(179);
    setCursor(33,16); color(4);
    for(int i=0;i<53;i++)
        cout<<char(196);
    color(4);
    rectangleInputInterface(17,49,20);
    char key='0'; int index=2;
    while(key!=13)
    {
        setCursor(49,18);
        color(4); cout<<char(179)<<" "; color();
        if(index==1)  color(192);
        cout<<"      EXIT      ";
        color(4); cout<<" "<<char(179); color();
        setCursor(0,24);
        while(1)
        {
            key=getch();
            if(key==80 || key=='s' || key=='S')
                { index++; break; }
            else if(key==72 || key=='w' || key=='W')
                { index--; break; }
            else if(key==13 && index==1)
                break;
        }
        if(index==3)
            index=1;
        else if(index==0)
            index=2;
    }
    if(index==1)
        return 1;
    else
        return 1;
}

bool interfaceCongratulations(int score=1000)
{
    color(11);
    rectangleInputInterface(7,30,59,13);
    for(int i=8;i<21;i++)
    {
        setCursor(30,i);
        cout<<char(179)<<"                                                         "<<char(179);
    }
    rectangleInputInterface(8,49,21);
    setCursor(49,9); cout<<char(180)<<" ";
    color(176); cout<<" CONGRATULATIONS "; color(11); cout<<" "<<char(195);
    setCursor(42,9); cout<<char(218);
    for(int i=0;i<12;i++)
    {
        if(i==6) setCursor(70,9);
            cout<<char(196);
    } cout<<char(191);
    setCursor(42,10); cout<<char(179); setCursor(76,10); cout<<char(179);
    rectangleInputInterface(11,40,39);
    setCursor(40,12); cout<<char(179)<<" "; color(15);
    cout<<"    You have finished this mode    "; color(11); cout<<" "<<char(179);
    rectangleInputInterface(14,50,19);
    setCursor(50,15); cout<<char(180)<<" "; color(15);
    cout<<" SCORE: "<<setw(5)<<score; color(11); cout<<"  "<<char(195);
    setCursor(45,14); cout<<char(179); setCursor(73,14); cout<<char(179);
    setCursor(45,15);  cout<<char(192);
    for(int i=0;i<8;i++)
    {
        if(i==4) setCursor(69,15);
            cout<<char(196);
    } cout<<char(217);
    setCursor(33,17); color(11);
    for(int i=0;i<53;i++)
        cout<<char(196);
    color(11);
    rectangleInputInterface(18,49,21);
    char key='0'; int index=2;
    while(key!=13)
    {
        setCursor(49,19);
        color(11); cout<<char(179)<<" "; color();
        if(index==1)  color(192);
        cout<<"      EXIT.      ";
        color(11); cout<<" "<<char(179); color();
        setCursor(0,24);
        while(1)
        {
            key=getch();
            if(key==80 || key=='s' || key=='S')
                { index++; break; }
            else if(key==72 || key=='w' || key=='W')
                { index--; break; }
            else if(key==13 && index==1)
                break;
        }
        if(index==3)
            index=1;
        else if(index==0)
            index=2;
    }
    if(index==1)
        return 1;
    else
        return 1;
}

void moveBetweenMaps(int level=5)
{
    short colorText[7]={0,128,64,160,48,96},colorBorder[7]={0,7,12,10,3,14};
    Sleep(250);
    color(colorBorder[level]);
    rectangleInputInterface(13,49,15);
    setCursor(49,14); cout<<char(179)<<" ";
    color(colorText[level]); cout<<"   LEVEL   ";
    color(colorBorder[level]); cout<<" "<<char(179);
    rectangleInputInterface(13,64,7);
    setCursor(64,14); cout<<char(179)<<" ";
    color(colorText[level]); cout<<" "<<level<<" ";
    color(colorBorder[level]); cout<<" "<<char(179);

    Sleep(500);
    setCursor(49,13); cout<<"                      ";
    setCursor(49,14); cout<<"                      ";
    setCursor(49,15); cout<<"                      ";
    Sleep(250);
}

void displayBorder(int gameMode=1,int colorBorder=7,int colorText=116,bool compaignMode=false)
{
/* length border is 20 , width border is 80 */
    system("cls");
    color(colorBorder);
//***************************** small rectangles at the bottom **************************
    rectangleInputInterface(0,20,24);
    if(gameMode>4 && compaignMode)
    {
        rectangleInputInterface(0,44,32);
        setCursor(44,1);   cout<<char(179);
        setCursor(75,1);   cout<<char(179);
    }
    rectangleInputInterface(0,76,24);
//************************************ rectangles Borders *******************************
    for(int i=0;i<57;i+=56)
    {
        setCursor(20+i,1);   cout<<char(179);
        setCursor(43+i,1);   cout<<char(179);
    }
//******************************** Big Rectangle in middle  *****************************
    rectangleInputInterface(3,20,80,20);
//************************************* rectangle Borders *******************************
    for(int i=4;i<24;i++) // 20 is length
    {
        setCursor(20,i);  cout<<char(179);
        setCursor(99,i);  cout<<char(179);
    }
//***************************** small rectangles at the bottom **************************
    rectangleInputInterface(25,20,24);
    rectangleInputInterface(25,44,32);
    rectangleInputInterface(25,76,24);
//************************************ rectangles Borders *******************************
    for(int i=0;i<57;i+=56)
    {
        setCursor(20+i,26);   cout<<char(179);
        setCursor(43+i,26);   cout<<char(179);
    }
    setCursor(44,26);   cout<<char(179);
    setCursor(75,26);   cout<<char(179);
    //*************************************************************
    setCursor(20,29);  for(int i=0;i<80;i++)  cout<<char(196);
    color();
//***************************************************************************************
    setCursor(22,1);   color(112); cout<<"  MODE : "; color();
    setCursor(78,1);   color(112); cout<<"  MAP : ";   color();
    string mapsName[6]={" NO MAZE    "," BOX        "," TUNNEL     ",
                        " MILL       "," RAILS      "," APARTMENT  "};
    if(gameMode==1)
    {
        setCursor(31,1); color(colorText); cout<<" CLASSIC   ";
        setCursor(86,1); cout<<mapsName[0];   color();
    }
    else if(gameMode>4 && compaignMode)
    {
        setCursor(31,1); color(colorText); cout<<" CAMPAIGN  ";
        setCursor(86,1); cout<<mapsName[gameMode-4];   color();
    }
    else
    {
        setCursor(31,1); color(colorText); cout<<" CUSTOM    ";
        setCursor(86,1); cout<<mapsName[gameMode-4]; color();
    }
    setCursor(21,26);  cout<<"  STATUS : ";
    setCursor(77,26);  cout<<"    SCORE : ";
    setCursor(22,28);  color(8); cout<<"* ENTER "; color(6); cout<<"ECS"; color(8);
                       cout<<" OR '"; color(6); cout<<"X";  color(8); cout<<"' TO EXIT";
    setCursor(76,28);  cout<<"* ENTER "; color(6); cout<<"SPACE"; color(8); cout<<" TO PAUSE "; color();
}

void mapBox()
{
    setCursor(21,4);
//*************************************** Top Border ************************************
    for(int i=0 ;i<39;i++)
    { color(8);  cout<<char(178);  color(112); cout<<char(178); }
//************************************ rectangles Borders *******************************
    for(int i=5;i<23;i++)
    {
        setCursor(21,i); color(8); cout<<char(178); color(112); cout<<char(178);
        setCursor(97,i); color(8); cout<<char(178); color(112); cout<<char(178);
    }
//************************************** Bottom Border **********************************
    setCursor(21,23);
    for(int i=0;i<39;i++)
    { color(8);  cout<<char(178); color(112); cout<<char(178); }
    color();
}

void mapTunnel()
{
//*************************************** Top Border ************************************
    setCursor(21,4);
    for(int i=0 ;i<7;i++)
    {
        color(199);  cout<<char(178);
        color(196); cout<<char(178);
    }
    setCursor(85,4);
    for(int i=0 ;i<7;i++)
    { color(199);  cout<<char(178); color(196); cout<<char(178); }
//***************************************************************************************
    for(int i=10 ;i<18;i+=7)
    {
        setCursor(39,i);
        for(int j=0 ;j<21;j++)
        { color(199);  cout<<char(178); color(196); cout<<char(178); }
    }
//************************************ rectangles Borders *******************************
    for(int i=5;i<23;i++)
    {
        setCursor(21,i); color(199); cout<<char(178); color(196); cout<<char(178);
        setCursor(97,i); color(199); cout<<char(178); color(196); cout<<char(178);
        if(i==10) i=16;
    }
//************************************** Bottom Border **********************************
    setCursor(21,23);
    for(int i=0 ;i<14;i++)
    {
        if(i==7)  setCursor(85,23);
        color(199);  cout<<char(178); color(196); cout<<char(178);
    }
    color();
}

void mapMill()
{
//***************************************************************************************
    for(int i=4;i<19;i++)
    {
        setCursor(45,i);
        color(39); cout<<char(178); color(2); cout<<char(178);
    }
    for(int i=9;i<24;i++)
    {
        setCursor(73,i);
        color(39); cout<<char(178); color(2); cout<<char(178);
    }
//***************************************************************************************
    for(int i=0;i<2;i++)
    {
        i==0? setCursor(73,6):setCursor(21,21);
        for(int j=0 ;j<13;j++)
        { color(39);  cout<<char(178); color(2); cout<<char(178); }
    }
    color();
}

void mapRails()
{
//*************************************** Top Border ************************************
    setCursor(21,4);
    for(int i=0 ;i<39;i++)
    { color(151);  cout<<char(178); color(9); cout<<char(178); }
//***************************************************************************************
    for(int i=9;i<19;i+=9)
    {
        setCursor(31,i);
        for(int j=0 ;j<29;j++)
        { color(151);  cout<<char(178); color(9); cout<<char(178); }
    }
//************************************ rectangles Borders *******************************
    for(int i=5;i<23;i++)
    {
        setCursor(21,i); color(151); cout<<char(178); color(9); cout<<char(178);
        setCursor(97,i); color(151); cout<<char(178); color(9); cout<<char(178);
        if(i==9) i=17;
    }
//*************************************** Top Border ************************************
    setCursor(21,23);
    for(int i=0 ;i<39;i++)
    { color(151);  cout<<char(178); color(9); cout<<char(178); }
    color();
}

void mapApartment()
{
//***************************************************************************************
    setCursor(21,4);
    for(int i=0 ;i<24;i++)
    {
        if(i==5) setCursor(45,4);
        color(111);  cout<<char(178); color(6); cout<<char(178);
    }
//***************************************************************************************
    for(int i=5;i<16;i++)
    {
        if(i<=10)
        { setCursor(21,i); color(111); cout<<char(178); color(6); cout<<char(178); }
        setCursor(53,i); color(111); cout<<char(178); color(6); cout<<char(178);
    }
//***************************************************************************************
    setCursor(21,16);
    for(int i=0 ;i<31;i++)
    {
        if(i==17) setCursor(71,16);
        color(111);  cout<<char(178); color(6); cout<<char(178);
    }
//***************************************************************************************
    setCursor(21,19);
    for(int i=0 ;i<39;i++)
    { color(111);  cout<<char(178); color(6); cout<<char(178); }
    for(int i=20;i<24;i++)
    { setCursor(71,i); color(111); cout<<char(178); color(6); cout<<char(178); }
    color();
}

void snake::drawSnake(int gameMode,int colorSnake,bool compaignMode)
{
    setCursor(32,26);
    if(stopPlay) { cout<<" "; color(64); cout<<" PAUSE "; color(); cout<<" "; }
    else         { color(32); cout<<" PLAYING "; color(); }
    setCursor(90,26); color(11); cout<<score; color();
    if(direction==UP || direction==DOWN)//Without it ,going up/down is much faster than left/right
        Sleep(7);
    setCursor(fruit.X,fruit.Y);
    if(progressBar==14) // Last fruit in Campaign mode
    { repeatColors[0]++%3==0? color(15):color(8); }
    else                color(3);
    cout<<char(254); color();
//***************************** Bonus Fruit *******************************************************
    if(fruitNumber==5)
    {
        setCursor(bonusFruit.X,bonusFruit.Y);
        repeatColors[1]++%2==0? color(10):color(4); cout<<char(254); color(4);
        setCursor(46,26);
        for(int i=0;i<(int)bonusProgressBar;i++)
        { cout<<char(219)<<char(219); } color();
        bonusProgressBar+=0.3;
        if((int)bonusProgressBar==15)
        {
            setCursor(bonusFruit.X,bonusFruit.Y); cout<<" ";
            generateBonusFruit(gameMode);
            bonusProgressBar=1; fruitNumber=0; repeatColors[1]=0;
            setCursor(46,26); cout<<"                            ";
        }
    }
//*************************** Draw Snake - Head and Tail - ****************************************
    setCursor(head.X,head.Y);   color(4); cout<<char(254);  color();
    for(int i=0;i<tailLength-1;i++)
    {
        i%2==0?  color():color(colorSnake);
        setCursor(tail[i].X,tail[i].Y); cout<<char(219);
    }
    color();
    if(direction==UP || direction==DOWN)
    { setCursor(tail[tailLength-2].X,tail[tailLength-2].Y); cout<<char(186); }
    else
    { setCursor(tail[tailLength-2].X,tail[tailLength-2].Y); cout<<char(205); }
    setCursor(tail[tailLength-1].X,tail[tailLength-1].Y);  cout<<" ";
//*************************************************************************************************
    if(head.X==fruit.X && head.Y==fruit.Y) // If Snake eat Fruit
    {
        score+=5; tailLength++; generateFruit(gameMode);
        if(fruitNumber==5) fruitNumber=5;
        else fruitNumber++;
        repeatColors[0]=0;
        if(gameMode>4 && compaignMode)
        {
            setCursor(46,1); color(10);
            for(int i=0;i<progressBar;i++)
            { cout<<char(219)<<char(219); } color();
            progressBar++;
        }
    }
    if(head.X==bonusFruit.X && head.Y==bonusFruit.Y && fruitNumber==5) // If Snake eat Bonus Fruit
    {
        score+=abs(450-(int)bonusProgressBar*27); generateBonusFruit(gameMode);
        fruitNumber=0; bonusProgressBar=1; repeatColors[1]=0;
        setCursor(46,26); cout<<"                            ";
    }
}

bool snake::inputKeyboard(int colorBorder)
{
    setCursor(0,28);
    int enterNumber[5]={2,1,4,3};
    directions reverseDirection=direction;
    if(stopPlay)
    {
        while(stopPlay)
        {
            switch(getch())
            {
                case 'w': case 'W': case 72:
                    direction=UP;    stopPlay=false; break;
                case 's': case 'S': case 80:
                    direction=DOWN;  stopPlay=false; break;
                case 'd': case 'D': case 77:
                    direction=RIGHT; stopPlay=false; break;
                case 'a': case 'A': case 75:
                    direction=LEFT;  stopPlay=false; break;
                case 'x': case 'X': case 27:
                stopPlay=true;   return interfaceExitGame(0,colorBorder);
            }
            if(enterNumber[reverseDirection-1]==direction)
                direction=reverseDirection;
        }
        return 0;
    }
    if (_kbhit())
    {
        switch(getch())
        {
            case 'w': case 'W': case 72:
                direction=UP;           break;
            case 's': case 'S': case 80:
                direction=DOWN;         break;
            case 'd': case 'D': case 77:
                direction=RIGHT;        break;
            case 'a': case 'A': case 75:
                direction=LEFT;         break;
            case 32:
                stopPlay=true;          break;
            case 'x': case 'X': case 27:
                stopPlay=true;   return interfaceExitGame(0,colorBorder);
        }
        if(enterNumber[reverseDirection-1]==direction)
            direction=reverseDirection;
    }
    return 0;
}

void snake::generateFruit(int gameMode)
{
    bool busyPlace=true,notBorder=true;
    switch(gameMode)
    {
        case 1:case 4:
        {
            while(busyPlace)
            {
                fruit.X=rand()%(98-21+1)+21;
                fruit.Y=rand()%(23-4+1)+4;
                busyPlace=false;
                for(int i=0;i<tailLength;i++)
                {
                    if(fruit.X==tail[i].X && fruit.Y==tail[i].Y)
                    { busyPlace=true; break; }
                }
            }
        } break;
//*************************************** Map Box ********************************************
        case 5: /* Map Box */
        case 3: /* Mode Campaign ,because it start map box (5) */
        {
            while(busyPlace)
            {
                fruit.X=rand()%(96-23+1)+23;
                fruit.Y=rand()%(22-5+1)+5;
                busyPlace=false;
                for(int i=0;i<tailLength;i++)
                {
                    if(fruit.X==tail[i].X && fruit.Y==tail[i].Y)
                    { busyPlace=true; break; }
                }
            }
        } break;
//*************************************** Map Tunnel *****************************************
        case 6: /* Map Tunnel */
        {
            while(busyPlace)
            {
                while(notBorder)
                {
                    fruit.X=rand()%(98-21+1)+21;
                    fruit.Y=rand()%(23-4+1)+4;
                    notBorder=false;
                    if((fruit.Y==10 || fruit.Y==17) && (fruit.X>38 && fruit.X<81 ) )
                        notBorder=true;
                    else if(( fruit.Y==4 || fruit.Y==23 ) && ( fruit.X<35 || fruit.X>84 ))
                        notBorder=true;
                    else if(( fruit.X==21 || fruit.X==22 ) && (fruit.Y<11 || fruit.Y>16) )
                        notBorder=true;
                    else if(( fruit.X==97 || fruit.X==98 ) && (fruit.Y<11 || fruit.Y>16) )
                        notBorder=true;
                }
                notBorder=true;  busyPlace=false;
                for(int i=0;i<tailLength;i++)
                {
                    if(fruit.X==tail[i].X && fruit.Y==tail[i].Y)
                    { busyPlace=true; break; }
                }
            }
        } break;
//*************************************** Map Mill *******************************************
        case 7: /* Map Mill */
        {
            while(busyPlace)
            {
                while(notBorder)
                {
                    fruit.X=rand()%(98-21+1)+21;
                    fruit.Y=rand()%(23-4+1)+4;
                    notBorder=false;
                    if((fruit.Y==6 && fruit.X>72) || (fruit.Y==21 && fruit.X<47 ) )
                        notBorder=true;
                    else if(( fruit.X==45 || fruit.X==46 ) && fruit.Y<19 )
                        notBorder=true;
                    else if(( fruit.X==73 || fruit.X==74 ) && fruit.Y>8 )
                        notBorder=true;
                }
                notBorder=true;  busyPlace=false;
                for(int i=0;i<tailLength;i++)
                {
                    if(fruit.X==tail[i].X && fruit.Y==tail[i].Y)
                    { busyPlace=true; break; }
                }
            }
        } break;
//*************************************** Map Rails ******************************************
        case 8: /* Map Rails */
        {
            while(busyPlace)
            {
                while(notBorder)
                {
                    fruit.X=rand()%(98-21+1)+21;
                    fruit.Y=rand()%(22-5+1)+5;
                    notBorder=false;
                    if((fruit.Y==9 || fruit.Y==18 ) && (fruit.X>30 && fruit.X<89 ) )
                        notBorder=true;
                    else if((fruit.X==21 || fruit.X==22) && (fruit.Y<10 || fruit.Y>17))
                        notBorder=true;
                    else if((fruit.X==97 || fruit.X==98) && (fruit.Y<10 || fruit.Y>17))
                        notBorder=true;
                }
                notBorder=true;  busyPlace=false;
                for(int i=0;i<tailLength;i++)
                {
                    if(fruit.X==tail[i].X && fruit.Y==tail[i].Y)
                    { busyPlace=true; break; }
                }
            }
        } break;
//*************************************** Map Apartment **************************************
        case 9: /* Map Apartment */
        {
            while(busyPlace)
            {
                while(notBorder)
                {
                    fruit.X=rand()%(98-21+1)+21;
                    fruit.Y=rand()%(23-4+1)+4;
                    notBorder=false;
                    if(fruit.Y==4 && fruit.X<31)
                        notBorder=true;
                    else if(fruit.Y==4 && (fruit.X>45 && fruit.X<83))
                        notBorder=true;
                    else if(fruit.Y==16 && (fruit.X<56 || fruit.X>71))
                        notBorder=true;
                    else if((fruit.X==21 || fruit.X==22) && fruit.Y<11)
                        notBorder=true;
                    else if((fruit.X==53 || fruit.X==54) && fruit.Y<16)
                        notBorder=true;
                    else if((fruit.X==71 || fruit.X==72) && fruit.Y>19)
                        notBorder=true;
                    else if(fruit.Y==19)
                        notBorder=true;
                }
                notBorder=true;  busyPlace=false;
                for(int i=0;i<tailLength;i++)
                {
                    if(fruit.X==tail[i].X && fruit.Y==tail[i].Y)
                    { busyPlace=true; break; }
                }
            }
        } break;
    }
}

void snake::generateBonusFruit(int gameMode)
{
    bool busyPlace=true,notBorder=true;
    coordinates previousFruit;
    previousFruit.X=bonusFruit.X;
    previousFruit.Y=bonusFruit.Y;
    switch(gameMode)
    {
        case 1: case 4:
        {
            while(busyPlace)
            {
                bonusFruit.X=rand()%(98-21+1)+21;
                bonusFruit.Y=rand()%(23-4+1)+4;
                busyPlace=false;
                if( previousFruit.X==bonusFruit.X && previousFruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else if( fruit.X==bonusFruit.X && fruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else
                {
                    for(int i=0;i<tailLength;i++)
                    {
                        if(bonusFruit.X==tail[i].X && bonusFruit.Y==tail[i].Y)
                        { busyPlace=true; break; }
                    }
                }
            }
        } break;
//*************************************** Map Box ********************************************
        case 5: /* Map Box */
        case 3: /* Mode Campaign ,because it start map box (5) */
        {
            while(busyPlace)
            {
                bonusFruit.X=rand()%(96-23+1)+23;
                bonusFruit.Y=rand()%(22-5+1)+5;
                busyPlace=false;
                if( previousFruit.X==bonusFruit.X && previousFruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else if( fruit.X==bonusFruit.X && fruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else
                {
                    for(int i=0;i<tailLength;i++)
                    {
                        if(bonusFruit.X==tail[i].X && bonusFruit.Y==tail[i].Y)
                        { busyPlace=true; break; }
                    }
                }
            }
        } break;
//*************************************** Map Tunnel *****************************************
        case 6: /* Map Tunnel */
        {
            while(busyPlace)
            {
                while(notBorder)
                {
                    bonusFruit.X=rand()%(98-21+1)+21;
                    bonusFruit.Y=rand()%(23-4+1)+4;
                    notBorder=false;
                    if((bonusFruit.Y==10 || bonusFruit.Y==17) && (bonusFruit.X>38 && bonusFruit.X<81 ) )
                        notBorder=true;
                    else if(( bonusFruit.Y==4 || bonusFruit.Y==23 ) && ( bonusFruit.X<35 || bonusFruit.X>84 ))
                        notBorder=true;
                    else if(( bonusFruit.X==21 || bonusFruit.X==22 ) && (bonusFruit.Y<11 || bonusFruit.Y>16) )
                        notBorder=true;
                    else if(( bonusFruit.X==97 || bonusFruit.X==98 ) && (bonusFruit.Y<11 || bonusFruit.Y>16) )
                        notBorder=true;
                }
                notBorder=true;  busyPlace=false;
                if( previousFruit.X==bonusFruit.X && previousFruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else if( fruit.X==bonusFruit.X && fruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else
                {
                    for(int i=0;i<tailLength;i++)
                    {
                        if(bonusFruit.X==tail[i].X && bonusFruit.Y==tail[i].Y)
                        { busyPlace=true; break; }
                    }
                }
            }
        } break;
//*************************************** Map Mill *******************************************
        case 7: /* Map Mill */
        {
            while(busyPlace)
            {
                while(notBorder)
                {
                    bonusFruit.X=rand()%(98-21+1)+21;
                    bonusFruit.Y=rand()%(23-4+1)+4;
                    notBorder=false;
                    if((bonusFruit.Y==6 && bonusFruit.X>72) || (bonusFruit.Y==21 && bonusFruit.X<47 ) )
                        notBorder=true;
                    else if(( bonusFruit.X==45 || bonusFruit.X==46 ) && bonusFruit.Y<19 )
                        notBorder=true;
                    else if(( bonusFruit.X==73 || bonusFruit.X==74 ) && bonusFruit.Y>8 )
                        notBorder=true;
                }
                notBorder=true;  busyPlace=false;
                if( previousFruit.X==bonusFruit.X && previousFruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else if( fruit.X==bonusFruit.X && fruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else
                {
                    for(int i=0;i<tailLength;i++)
                    {
                        if(bonusFruit.X==tail[i].X && bonusFruit.Y==tail[i].Y)
                        { busyPlace=true; break; }
                    }
                }
            }
        } break;
//*************************************** Map Rails ******************************************
        case 8: /* Map Rails */
        {
            while(busyPlace)
            {
                while(notBorder)
                {
                    bonusFruit.X=rand()%(98-21+1)+21;
                    bonusFruit.Y=rand()%(22-5+1)+5;
                    notBorder=false;
                    if((bonusFruit.Y==9 || bonusFruit.Y==18 ) && (bonusFruit.X>30 && bonusFruit.X<89 ) )
                        notBorder=true;
                    else if((bonusFruit.X==21 || bonusFruit.X==22) && (bonusFruit.Y<10 || bonusFruit.Y>17))
                        notBorder=true;
                    else if((bonusFruit.X==97 || bonusFruit.X==98) && (bonusFruit.Y<10 || bonusFruit.Y>17))
                        notBorder=true;
                }
                notBorder=true;  busyPlace=false;
                if( previousFruit.X==bonusFruit.X && previousFruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else if( fruit.X==bonusFruit.X && fruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else
                {
                    for(int i=0;i<tailLength;i++)
                    {
                        if(bonusFruit.X==tail[i].X && bonusFruit.Y==tail[i].Y)
                        { busyPlace=true; break; }
                    }
                }
            }
        } break;
//*************************************** Map Apartment **************************************
        case 9: /* Map Apartment */
        {
            while(busyPlace)
            {
                while(notBorder)
                {
                    bonusFruit.X=rand()%(98-21+1)+21;
                    bonusFruit.Y=rand()%(23-4+1)+4;
                    notBorder=false;
                    if(bonusFruit.Y==4 && bonusFruit.X<31)
                        notBorder=true;
                    else if(bonusFruit.Y==4 && (bonusFruit.X>45 && bonusFruit.X<83))
                        notBorder=true;
                    else if(bonusFruit.Y==16 && (bonusFruit.X<56 || bonusFruit.X>71))
                        notBorder=true;
                    else if((bonusFruit.X==21 || bonusFruit.X==22) && bonusFruit.Y<11)
                        notBorder=true;
                    else if((bonusFruit.X==53 || bonusFruit.X==54) && bonusFruit.Y<16)
                        notBorder=true;
                    else if((bonusFruit.X==71 || bonusFruit.X==72) && bonusFruit.Y>19)
                        notBorder=true;
                    else if(bonusFruit.Y==19)
                        notBorder=true;
                }
                notBorder=true;  busyPlace=false;
                if( previousFruit.X==bonusFruit.X && previousFruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else if( fruit.X==bonusFruit.X && fruit.Y==bonusFruit.Y)
                    busyPlace=true;
                else
                {
                    for(int i=0;i<tailLength;i++)
                    {
                        if(bonusFruit.X==tail[i].X && bonusFruit.Y==tail[i].Y)
                        { busyPlace=true; break; }
                    }
                }
            }
        } break;
    }
}

bool snake::gameLogic(int gameMode)
{
    if(!stopPlay)
    {
        if(tailLength>=99) // Stop playing when the tail is longer than 99
        { stopPlay=true; Sleep(700); return interfaceCongratulations(score); }
//*************************************************************************************************
        coordinates previousTail,previousTail_2;
        previousTail=tail[0];
        tail[0]=head;
        for (int i=1 ; i<tailLength ; i++)  // Move the tail once to the right
        {
            previousTail_2=tail[i];
            tail[i]=previousTail;
            previousTail=previousTail_2;
        }
        switch (direction)
        {
            case UP:    head.Y--; break;
            case DOWN:  head.Y++; break;
            case RIGHT: head.X++; break;
            case LEFT:  head.X--; break;
            default: break;
        }
//************************ Move the snake to the opposite area ************************************
        if     (head.X>98) head.X=21;  /* 21 = the start of the border on the left */
        else if(head.X<21) head.X=98;  /* 98 = the end of the border on the right */

        if    (head.Y>23) head.Y=4;    /*  4 = the start of the border in the top  */
        else if(head.Y<4) head.Y=23;   /* 23 = the end of the border at the bottom */
//****************** If the snake completed the level in campaign mode ****************************
        if(progressBar==15)
        {
            stopPlay=true; level++; Sleep(700); initialValueCampaignMode();
            if(level>=6)
                return interfaceCongratulations(score);
        }
//************************* If snake hits border **************************************************
        switch(gameMode)
        {
            case 5: /* Map Box */
            {
                if( head.X<23 || head.X>96)
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if(head.Y>22 || head.Y<5)
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
            } break;
            case 6: /* Map Tunnel */
            {
                if((head.Y==10 || head.Y==17) && (head.X>38 && head.X<81 ) )
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if(( head.Y==4 || head.Y==23 ) && ( head.X<35 || head.X>84 ))
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if(( head.X==21 || head.X==22 ) && (head.Y<11 || head.Y>16) )
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if(( head.X==97 || head.X==98 ) && (head.Y<11 || head.Y>16) )
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }

            } break;
            case 7: /* Map Mill */
            {
                if((head.Y==6 && head.X>72) || (head.Y==21 && head.X<47 ) )
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if(( head.X==45 || head.X==46 ) && head.Y<19 )
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if(( head.X==73 || head.X==74 ) && head.Y>8 )
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
            } break;
            case 8: /* Map Rails */
            {
                if((head.Y==9 || head.Y==18 ) && (head.X>30 && head.X<89 ) )
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if((head.X==21 || head.X==22) && (head.Y<10 || head.Y>17))
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if((head.X==97 || head.X==98) && (head.Y<10 || head.Y>17))
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if(head.Y>22 || head.Y<5)
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
            } break;
            case 9: /* Map Apartment */
            {
                if(head.Y==4 && head.X<31)
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if(head.Y==4 && (head.X>45 && head.X<83))
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if(head.Y==16 && (head.X<56 || head.X>71))
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if((head.X==21 || head.X==22) && head.Y<11)
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if((head.X==53 || head.X==54) && head.Y<16)
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if((head.X==71 || head.X==72) && head.Y>19)
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
                else if(head.Y==19)
                { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
            } break;
        }
//************************* If snake hits herself *************************************************
        for (int i=3; i<tailLength ; i++)
            if(tail[i].X==head.X && tail[i].Y==head.Y)
            { stopPlay=true; Sleep(500); return interfaceGameOver(score); }
    }
    return 0;
}

void snake::saveData()
{
/*  1-newGame   2-gameMode   3-score   4-tailLength
    5-speed   6-fruitNumber   7-level   8-progressBar
    9-bonusProgressBar   10-head.X , head.X   11-fruit.X , fruit.Y
    12-bonusFruit.X , bonusFruit.Y   13-tail[i].X
*/
    ofstream writeToFile("SnakeGame.DAT");
    if(writeToFile.is_open())
    {
        writeToFile<<" This file is important do not delete or modify it. "<<endl
                   <<"****************************************************\n"<<endl;
        writeToFile<<newGame<<endl<<gameMode<<endl<<score<<endl
                   <<tailLength<<endl<<speed<<endl<<fruitNumber<<endl
                   <<level<<endl<<progressBar<<endl<<bonusProgressBar<<endl
                   <<head.X<<"   "<<head.Y<<endl<<fruit.X<<"   "<<fruit.Y<<endl
                   <<bonusFruit.X<<"   "<<bonusFruit.Y<<endl;
        for(int i=0;i<tailLength;i++)
            writeToFile<<tail[i].X<<"   "<<tail[i].Y<<endl;
    }
    else
    {
        cout<<" \n\t\t\a Error Write . "<<endl; system("pause");
    }
    writeToFile.close();
}

bool snake::callData()
{
/*  1-newGame   2-gameMode   3-score   4-tailLength
    5-speed   6-fruitNumber   7-level   8-progressBar
    9-bonusProgressBar   10-head.X , head.X   11-fruit.X , fruit.Y
    12-bonusFruit.X , bonusFruit.Y   13-tail[i].X
*/
    ifstream readFromFile("SnakeGame.DAT");
    readFromFile.seekg(106,ios::beg);
    if(readFromFile.is_open())
    {
        readFromFile>>newGame>>gameMode>>score>>tailLength>>speed>>fruitNumber>>level
                    >>progressBar>>bonusProgressBar>>head.X>>head.Y>>fruit.X>>fruit.Y
                    >>bonusFruit.X>>bonusFruit.Y;
        for(int i=0;i<tailLength;i++)
            readFromFile>>tail[i].X>>tail[i].Y;
        readFromFile.close();
        return 1;
    }
    else
    { readFromFile.close(); return 0; }
}

int main()
{
    initConsole();
    srand(time(0));
    color();
    snake MOMO;
    colorGame colorMOMO[6]={{7,12,116},{7,12,116},{12,6,116},{10,6,114},{3,1,113},{14,2,118}};
    int choose=0,colorMode;
   interfaceStartGame();
    if(!MOMO.callData())
    {
        MOMO.initialValueSnake();
        MOMO.generateFruit(MOMO.gameMode);
        MOMO.generateBonusFruit(MOMO.gameMode);
        interfaceLoginForFirstTime();
    }
    if(MOMO.gameMode==1 || MOMO.gameMode==3) colorMode=0;
    else colorMode=MOMO.gameMode-4;
    while(1)
    {
        if(MOMO.newGame && choose==2){choose=1; MOMO.newGame=false; }
        else choose=interfaceMenuGame(MOMO.newGame);
        switch(choose)
        {
            case 1:
            {
                bool exit=false,exitMode=false,exitGameOver=false;
//********************************* Campaign Mode *************************************************
                if(MOMO.gameMode==3)
                {
                    while(!exitMode)
                    {
                        short currentlevel=MOMO.level;
                        displayBorder(MOMO.level+4,colorMOMO[colorMode+MOMO.level].border
                                      ,colorMOMO[colorMode+MOMO.level].text,true);
                        if(MOMO.newGame) moveBetweenMaps(MOMO.level);
                        switch(MOMO.level)
                        {
                            case 1:
                                mapBox();       break;
                            case 2:
                                mapTunnel();    break;
                            case 3:
                                mapMill();      break;
                            case 4:
                                mapRails();     break;
                            case 5:
                                mapApartment(); break;
                        }
                        while(!exit && !exitGameOver)
                        {
                            if(MOMO.stopPlay)
                            {
                                if(currentlevel!=MOMO.level) break;
                                switch(MOMO.level)
                                {
                                    case 1:
                                        mapBox();       break;
                                    case 2:
                                        mapTunnel();    break;
                                    case 3:
                                        mapMill();      break;
                                    case 4:
                                        mapRails();     break;
                                    case 5:
                                        mapApartment(); break;
                                }
                            }
                            MOMO.drawSnake(MOMO.level+4,colorMOMO[colorMode+MOMO.level].snake,true);
                            exit=MOMO.inputKeyboard(colorMOMO[colorMode+MOMO.level].border);
                            exitGameOver=MOMO.gameLogic(MOMO.level+4);
                            Sleep(MOMO.speed);
                        }
                        if(exit) exitMode=true;
                        else if(exitGameOver){ exitMode=true; MOMO.newGame=true; }
                        else
                        {
                            MOMO.generateFruit(MOMO.level+4);
                            MOMO.generateBonusFruit(MOMO.level+4);
                        }
                    }
                }
//***************************** Classic and Custom Mode *******************************************
                else
                {
                    displayBorder(MOMO.gameMode,colorMOMO[colorMode].border,colorMOMO[colorMode].text);
                    switch(MOMO.gameMode)
                    {
                        case 5:
                            mapBox();       break;
                        case 6:
                            mapTunnel();    break;
                        case 7:
                            mapMill();      break;
                        case 8:
                            mapRails();     break;
                        case 9:
                            mapApartment(); break;
                    }
                    while(!exit && !exitGameOver)
                    {
                        MOMO.drawSnake(MOMO.gameMode,colorMOMO[colorMode].snake);
                        if(MOMO.stopPlay)
                        {
                            switch(MOMO.gameMode)
                            {
                                case 5:
                                    mapBox();       break;
                                case 6:
                                    mapTunnel();    break;
                                case 7:
                                    mapMill();      break;
                                case 8:
                                    mapRails();     break;
                                case 9:
                                    mapApartment(); break;
                            }
                        }
                        exit=MOMO.inputKeyboard(colorMOMO[colorMode].border);
                        exitGameOver=MOMO.gameLogic(MOMO.gameMode);
                        Sleep(MOMO.speed);
                    }
                    if(exitGameOver) MOMO.newGame=true;
                }
            } break;
//*************************************** New Game ************************************************
            case 2:
            {
                MOMO.initialValueSnake();
                MOMO.newGame=true;
                MOMO.generateFruit(MOMO.gameMode);
                MOMO.generateBonusFruit(MOMO.gameMode);
            } break;
//*************************************** Speed Snake *********************************************
            case 3:
                MOMO.speed=interfaceSpeedSnake(MOMO.speed); break;
//*************************************** Game Mode ***********************************************
            case 4:
                {
                    int currentMode=MOMO.gameMode;
                    MOMO.gameMode=interfaceGameMode();
                    if(MOMO.gameMode==0)
                        MOMO.gameMode=currentMode;
                    else
                    {
                        if(MOMO.gameMode==1)
                        { colorMode=0; MOMO.initialValueSnake(); }
                        else if(MOMO.gameMode==3)
                        { colorMode=0; MOMO.initialValueSnake(); }
                        else
                        { colorMode=MOMO.gameMode-4; MOMO.initialValueSnake(); }
                        MOMO.newGame=true;
                        MOMO.generateFruit(MOMO.gameMode);
                        MOMO.generateBonusFruit(MOMO.gameMode);
                    }

                } break;
//******************************************** Help ***********************************************
            case 5:
                interfaceHelp();                  break;
//******************************************** Exit ***********************************************
            case 6:
            {
                bool exitGame=false;
                exitGame=interfaceExitGame();
                if(exitGame)
                {
                    MOMO.saveData();
                    return 0;
                }
            }break;
        }
    }
    return 0;
}
