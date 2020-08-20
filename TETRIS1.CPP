#include<stdio.h>
#include<conio.h>
#include<dos.h>

#include<stdlib.h>
#include<graphics.h>
#include<time.h>
#include<iostream.h>

//Function Prototype
int isAtBottom();
int *getFallingBlockArray();
int isGameOver();
void clearOldBlockVersion();
void CollapseFullRow();
void showScore();
void drawNewBlockVersion();
int isAtBottom ();
int isDrawable(int newrow,int newcol,int blockversion);
void GameOver();
void NextBlock();	/* For Generating New blocks*/

//The tetris matrix (2d array)
int board[25][13];

//Different versions of T block (rotated)
int t_[8]={1,0,1,1,1,2,2,1};
int t_90[8]={0,1,1,0,1,1,2,1};
int t_180[8]={0,1,1,0,1,1,1,2};
int t_270[8]={0,1,1,1,1,2,2,1};

//Different versions of L block (rotated)
int l_[8]={0,2,1,0,1,1,1,2};
int l_90[8]={0,1,1,1,2,1,2,2};
int l_180[8]={1,0,1,1,1,2,2,0};
int l_270[8]={0,0,0,1,1,1,2,1};

//S block
int s_[8]={0,0,0,1,1,0,1,1};

//Different versions of Z block
int z_[8]={1,1,1,2,2,0,2,1};
int z_90[8]={0,1,1,1,1,2,2,2};

//Different versions of I block
int i_ver[8]={0,1,1,1,2,1,3,1};
int i_hor[8]={1,0,1,1,1,2,1,3};

//score as string
char * scorestr;

/*
Numbering for type of blocks:
(values of fallingBlockNumber)
0=T
1=L
2=S
3=Z
4=I
*/

//pointer to current falling block type array
int *blockarray;
int fallingblockNum;
int fallingBlockVersion=0;	//values 0 to 4
int fallingBlockRow=0;	//the current row of falling block
int fallingBlockCol=0;	//the current column of falling block
int startdelay=200;
int motiondelay;
int scoreInc=5;		//score for each line clearance
int myscore=0;		//score

//spawning next falling block
void NextBlock()
{
fallingblockNum=rand()%5;	/* Value of fallingblockNum from 0 to 4*/
if(fallingblockNum==0||fallingblockNum==1)
 fallingBlockVersion=rand()%4;		/* FallingBlockVersion=0 or 1 or 2 or 3 and fallingblockNum=0 or 1*/
else if(fallingblockNum==4||fallingblockNum==3)
 fallingBlockVersion=rand()%2;		/* fallingblockversion=0 or 1 and fallingblockNum=4 or 3 */
else
 fallingBlockVersion=0;		/* fallingblockversion=0 and fallingblockNum=2*/
fallingBlockRow=0;	/*Spawns blocks from top of The Box*/
fallingBlockCol=5;	/*Spawns blocks from middle of The Box column wise.*/
}

//store pointer to current falling block's array in *blockarray;
int *getFallingBlockArray()
{
int a=fallingblockNum*10+fallingBlockVersion;	/* from NextBlock*/
switch(a)
    {
    case 0:return ((int*)&t_);
    case 1:return ((int*)&t_90);
    case 2:return ((int*)&t_180);
    case 3:return ((int*)&t_270);
    case 10:return ((int*)&l_);
    case 11:return ((int*)&l_90);
    case 12:return ((int*)&l_180);
    case 13:return ((int*)&l_270);
    case 20:return ((int*)&s_);
    case 30:return ((int*)&z_);
    case 31:return ((int*)&z_90);
    case 40:return ((int*)&i_hor);
    case 41:return ((int*)&i_ver);
    }
return ((int*)&i_ver);	/*Probably Default*/
}

/* To check if moving to left or right or rotation of falling block is
 feasible. i.e to check whether there is enough space around it.*/

int isDrawable(int newrow,int newcol,int blockversion)
{
int i,tempversion,flag=1;;
tempversion=fallingBlockVersion;
fallingBlockVersion=blockversion;
blockarray=getFallingBlockArray();
for(i=0;i<8;i+=2)
    {
//check if block goes out of the matrix (up or down)
if(newrow+blockarray[i]>24||newrow+blockarray[i]<0)
 {
 flag=0;
 break;
 }

//check if block goes out of the matrix (left or right)
if(newcol+blockarray[i+1]>12||newcol+blockarray[i+1]<0)
 {
 flag=0;
 break;
 }

//check if there is any obstacle on the proposed position
if(board[(newrow+blockarray[i])][(newcol+blockarray[i+1])]==2)
 {
 flag=0;
 break;
 }
    }
fallingBlockVersion=tempversion;
blockarray=getFallingBlockArray();
return flag;
}

/*To clear the old position of falling block after it moves to a new position
 (row/column change) or after it is rotated*/
void clearOldBlockVersion()
{
int i,r,c;
for(i=0;i<8;i+=2)
 {
 r=fallingBlockRow+blockarray[i];
 c=fallingBlockCol+blockarray[i+1];
 board[r][c]=0;	      /*MAKES PREVIOUS PALCE 0 */
 setfillstyle(EMPTY_FILL,BLACK);     /*MAKES PREVIOUS POSITION BLACK ie. NOT VISIBLE*/
 setcolor(BLACK);    /*BLACK IS NOT SEEN IN SCREEN*/
 bar(2+c*13,2+r*13,2+c*13+13,2+r*13+13);	/*MAILE BUJHNA SAKINA TARA BLOCK PRINT GARNE HOLA*/
 rectangle(2+c*13,2+r*13,2+c*13+13,2+r*13+13);
 }
}

/*To draw the new picture after it moves to a new position (row/column change)
 or after it is rotated */
void drawNewBlockVersion()
{
int i,r,c;
for(i=0;i<8;i+=2)
 {
 r=fallingBlockRow+blockarray[i];
 c=fallingBlockCol+blockarray[i+1];
 board[r][c]=1;
setcolor(MAGENTA);    /*BORDER OF EACH TETRIS BLOCK*/
  setfillstyle(SOLID_FILL,WHITE);	/* BLOCK IS WHITE*/
 bar(2+c*13,2+r*13,2+c*13+13,2+r*13+13);      /*PROBABLY BLOCK PRINT*/
 rectangle(2+c*13,2+r*13,2+c*13+13,2+r*13+13);
 }
}

/*To check whether game is over. called after every move. Game is over when
 tetris matrix is filled.*/
int isGameOver()
{
if(isDrawable(0,5,fallingBlockVersion)==0)    /*CALLS isDrawable,0 FOR ROW, 5 FOR COLUMN i.e PLACE WHERE BLOCK SPAWNS*/
    return 1;
drawNewBlockVersion();

if(isAtBottom())
    return 1;
return 0;
}

/*To check if falling block reached the bottom*/
int isAtBottom()
{
int i,max=0,ti,tj;
for(i=0;i<8;i+=2)
    if(*(blockarray+i)>max)
 max=*(blockarray+i);
if(fallingBlockRow+max>=24)
 return 1;
for(i=0;i<8;i+=2)
    {
    ti=*(blockarray+i)+fallingBlockRow;
    tj=*(blockarray+i+1)+fallingBlockCol;
    if(board[ti+1][tj]==2)
       return 1;
    }
return 0;
}

/*Draw score on screen*/
void showScore()
{
int left,top;
setcolor(BLACK); /*BOX APPEARS FOR SCREEN FOR OTHER COLOR*/
setfillstyle(EMPTY_FILL,BLACK);
left=getmaxx()-100;	/*initialization for coordinates*/
top=getmaxy()/2;	/*initialization for coordinates*/
bar3d(left,top,left+60,top+70,2,1);   /*3D SCORE*/
setcolor(YELLOW);      /* COLOR OF TEXT*/
settextstyle(TRIPLEX_FONT, HORIZ_DIR,3);    /*STYLE OF SCORE FONT,HOR/VERT,SIZE*/
outtextxy(getmaxx()-100,getmaxy()/2,"Score: ");	     /*X,Y COORDNATE,SCORE*/
outtextxy(getmaxx()-100,getmaxy()/2+20,itoa(myscore,scorestr,10));  /*X,Y COORDINATE, WHAT IS SCORE*/
}

/*To clear any row that is fully filled and increment score.
 Also settles rows over the cleared row down.*/
void CollapseFullRow()
{
int i,j,k,sum,copyskipover=0,r;
for(i=24;i>=0;)
{
    sum=0;//full flag
    for(j=0;j<13;j++)
	sum+=board[i][j];      /*WHEN BLOCK REACHES BOTTOM THE 1 VALUE OF THEM IS CHAMGE TO 2 SO 2*COLUMN(13) MEANS A LINE IS FORMED*/
	if(sum==2*13)	//row full
	{
		myscore+=scoreInc;   /*SCORE INCREASE*/
		copyskipover++;
	}
    if(sum==0)
	break;
    i--;    /*MOVES UPWARD*/
    if(copyskipover>0)
    {
	for(j=0;j<13;j++)
	{
		r=i+copyskipover;
		board[r][j]=board[i][j];
		if(board[i][j]==0)
		{
			setfillstyle(EMPTY_FILL,RED);//empty yo hatch
			setcolor(MAGENTA);
			bar(2+j*13,2+r*13,2+j*13+13,2+r*13+13);
		}
		 else
		{
		setfillstyle(SOLID_FILL,WHITE);
		setcolor(MAGENTA);
		bar(2+j*13,2+r*13,2+j*13+13,2+r*13+13);
		rectangle(2+j*13,2+r*13,2+j*13+13,2+r*13+13);
		}
	}
    }
}
for(k=0;k<copyskipover;k++)
    {
    r=i+k;
    for(j=0;j<13;j++)
	{
		sound(300);
		board[r][j]=0;
		setfillstyle(SOLID_FILL,BLACK);
		setcolor(BLACK);
		bar(2+j*13,2+r*13,2+j*13+13,2+r*13+13);
		delay(10);
		nosound();
	}
    }
showScore();
}

char name[30];
void main()
{
int scorespeedctrl=0;
char arrowpre,ch,timehalving=0;
int gd=DETECT,gm,i,j,k=1,spawn=1,tversion;
motiondelay=startdelay;
scorestr=(char*)malloc(sizeof(char)*10);
printf("Enter name:");
gets(name);
for(i=0;i<25;i++)
    for(j=0;j<13;j++)
 board[i][j]=0;

/*
to get the ascii characters for up arrow key.
When any of arrow key is pressed, two characters
are sent to buffer. The first one may be different
on different compilers and systems.
The second one is of value 72 for up arrow.
*/
while(k!=72)
{
printf("press 'up' arrow key to start\n");
arrowpre=getch();
if(kbhit())
 k=getch();
}
//
initgraph(&gd,&gm,"c:\\tc\\bgi");
cleardevice();
setbkcolor(LIGHTGRAY);    /*Color of Background*/
setcolor(YELLOW);	/*Color of Border of Board*/
rectangle(0,0,13*13+6,25*13+6);    /*PRINTING BOARD*/
srand(time(NULL));
showScore();
setcolor(YELLOW);	/*COLOR SET TO YELLOW*/
settextstyle(SMALL_FONT, HORIZ_DIR,6);      /*STYLE SET FOR DRAWING*/
outtextxy(getmaxx()-200,getmaxy()/2-100,"Press 'Q' to quit.");   /*PRINTING*/

while(ch!='q'&&ch!='Q'&&k!='q'&&k!='Q')
    {
    if(kbhit()&&spawn==0)
 {
 ch=getch();
 if(ch==arrowpre)//if an arrow key pressed
     {
     k=getch();
     if(k==72)//if up arrow key is pressed
  {
  if(fallingblockNum==0||fallingblockNum==1)
      tversion=(fallingBlockVersion+1)%4;
  else if(fallingblockNum==4||fallingblockNum==3)
      tversion=(fallingBlockVersion+1)%2;

  if(fallingblockNum!=2&&isDrawable(fallingBlockRow,fallingBlockCol,tversion))
      {
      clearOldBlockVersion();
      fallingBlockVersion=tversion;
      blockarray=getFallingBlockArray();
      drawNewBlockVersion();
      }
  }
     else if(k==75)//if left arrow key is pressed

  {
  if(isDrawable(fallingBlockRow,fallingBlockCol-1,fallingBlockVersion))
      {
      clearOldBlockVersion();
      fallingBlockCol--;
      drawNewBlockVersion();
      }
  }
     else if(k==77)//if right arrow key is pressed
  {
  if(isDrawable(fallingBlockRow,fallingBlockCol+1,fallingBlockVersion))
      {
      clearOldBlockVersion();
      fallingBlockCol++;
      drawNewBlockVersion();
      }
  }
     else if(k==80)//if down arrow key is pressed
  {
  if(isDrawable(fallingBlockRow+1,fallingBlockCol,fallingBlockVersion))
      {
      clearOldBlockVersion();
      fallingBlockRow++;
      drawNewBlockVersion();
      }
  }
     while(kbhit())
  getch();
     }
 }
    if(isAtBottom()&&spawn==0)
 {
 for(i=0;i<8;i+=2)
     {
     board[fallingBlockRow+blockarray[i]][fallingBlockCol+blockarray[i+1]]=2;
     }
 spawn=1;
 CollapseFullRow();
 }
    if(spawn)
 {
 NextBlock();
 blockarray=getFallingBlockArray();
 spawn=0;
 if(isGameOver())
     {
     GameOver();	/*calls game over function*/
	char plname[20],nplname[20],cha,c;
   int i,j,px;
   FILE *info;
   info=fopen("record.txt","a+");
   getch();
   system("cls");
   printf("Enter your name\n");
   scanf("%[^\n]",plname);
   for(j=0;plname[j]!='\0';j++){ //to convert the first letter after space to capital
//   nplname[0]=toupper(plname[0]);
   if(plname[j-1]==' '){
  //c nplname[j]=toupper(plname[j]);
   nplname[j-1]=plname[j-1];}
   else nplname[j]=plname[j];
   }
   nplname[j]='\0';
   fprintf(info,"Player Name :%s\n",nplname);

     fprintf(info,"Score:%d\n",myscore);
   for(i=0;i<=50;i++)
   fprintf(info,"%c",'_');
   fprintf(info,"\n");
   fclose(info);
   printf("wanna see past records press 'y'\n");
   cha=getche();
   system("cls");
   if(cha=='y'){
   info=fopen("record.txt","r");
   do{
       putchar(c=getc(info));

      }while(c!=EOF);
   }
    getch();
     fclose(info);
	return ;
     }
 }
    else
 {
 timehalving=(timehalving+1)%3;
 if(timehalving==2)
  {
  clearOldBlockVersion();
  fallingBlockRow++;
  drawNewBlockVersion();
  }
 }
    scorespeedctrl=(scorespeedctrl+1)%280;
    if(scorespeedctrl==0)
 {
 motiondelay-=8;
 //scoreInc++;
/* scoreInc++ increases score by 5++ for each line formed*/
 }
    delay(motiondelay);
    }
}

void GameOver()
{
static int style=TRIPLEX_FONT;	/*style=for font*/
static int size = 4;	/*font size*/
sound(500);	/*For Beep upon game over*/
delay(500);
setcolor(YELLOW);	/*color of Text*/
settextjustify(CENTER_TEXT, CENTER_TEXT);	/*Orientation of Text*/
settextstyle(style, HORIZ_DIR, size);	/*Text Format*/
outtextxy(getmaxx()/2-10,getmaxy()/3,"Game Over.\n");	/*Text Printing*/
outtextxy(getmaxx()/2-10,getmaxy()/2,"Score:\n");
outtextxy(getmaxx()/2+80,getmaxy()/2,itoa(myscore,scorestr,10));
nosound();	/*End of Sound*/
getch();
main();
}
