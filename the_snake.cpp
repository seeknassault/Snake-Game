#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<fstream>
#include<conio.h>
#include<windows.h>


#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESCAPE 27
using namespace std;
struct snake{
    char part;
    int x;
    int y;
    struct snake* next;
};
struct food{
    char khana;
    int x;
    int y;
};
class GAME{
    public:
        int grab;
        char* myData;
        GAME(){
            grab=0;
            myData=new char[20];
        }
        void doubleSize();
        void halfSize();
        int getSize();
        struct snake* initializeSnake(struct snake* );   
        struct food* generate_food(struct snake*,char** );
        void displaySnake(struct snake*);
        void displayFood(struct food*);
        void print(char **);
        struct snake* moveAhead(struct snake*,struct  food*,int direction,int flag,char** ground);
        char** initPrint(char **mat,struct snake*);
        struct snake* khaLia(struct snake*,struct snake*,char**);
        struct snake* nhiKhaya(struct snake*,struct snake*,char**);
        void save(struct snake*,struct food*);
        struct snake* getStoredSnake(struct snake*);
        struct food* getStoredFood(struct food*);
        int hash(int );
        void terminate();
        bool checkCollision(struct snake*,struct snake*);
};
void GAME::doubleSize(){
	int size=getSize();
	char* toCopy=new char[2*size];
	for(int i=0;i<size;i++){
		toCopy[i]=myData[i];
	}
	delete myData;
	myData=toCopy;
}
void GAME::halfSize(){
	int size=getSize();
	char* toCopy=new char[size/2];
	for(int i=0;i<size/4;i++){
		toCopy[i]=myData[i];
	}
	delete myData;
	myData=toCopy;
}
int GAME::getSize(){
	int size=0;
	for(int i=0;myData[i]!='\0';i++)
	size++;
	return size;
}

void GAME::terminate(){
	cout<<"bye"<<endl;
	exit(0);
}
int GAME::hash(int input){
	return input%10;
}
bool GAME::checkCollision(struct snake* head,struct snake* nextMove){
	struct snake* hashTable[10];
	for(int i=0;i<10;i++)
	hashTable[i]=NULL;
	struct snake* temp=head;
	while(temp!=NULL){
		struct snake* latest=new snake;
		latest->part=temp->part;
		latest->x=temp->x;
		latest->y=temp->y;
		int key=hash(latest->x+latest->y);
		latest->next=hashTable[key];
		hashTable[key]=latest;
		temp=temp->next;
	}
	for(int i=0;i<10;i++){
		struct snake* temp=hashTable[i];
		while(temp!=NULL){
			temp=temp->next;
		}
	}
	int key=hash(nextMove->x+nextMove->y);
	temp=hashTable[key];
	while(temp!=NULL){
		if(nextMove->x==temp->x&&temp->y==nextMove->y)
		return 1;
		temp=temp->next;
	}
	return 0;
}
struct food* GAME::getStoredFood(struct food* bookh){
        ifstream foodFile;
    foodFile.open("food.txt",ios::in|ios::binary);
    if(!foodFile){
        cout<<"Error in reading food"<<endl;
    }else{
        struct food* latest=new food;
        foodFile>>latest->khana>>latest->x>>latest->y;
        bookh=latest;
    }
    return bookh;
}
struct snake* GAME::getStoredSnake(struct snake* head){
    ifstream snakeFile;
    snakeFile.open("snake.txt",ios::in|ios::binary);
    if(!snakeFile){
        cout<<"Error in reading snake"<<endl;
    }else{
        while(!snakeFile.eof()){
            struct snake* latest=new snake;
            latest->next=NULL;
            snakeFile>>latest->part;
            snakeFile>>latest->x;
            snakeFile>>latest->y;
            cout<<latest->part<<latest->x<<latest->y<<endl;
            if(head==NULL)
            head=latest;
            else{
                struct snake* temp=head;
                while(temp->next!=NULL)
                temp=temp->next;
                temp->next=latest;
            }
        }
            struct snake* temp=head;
                while(temp->next->next!=NULL)
                temp=temp->next;
                temp->next=NULL;
    }
    return head;
}
void GAME::save(struct snake* head,struct food* bookh){
    ofstream snakeFile,foodFile;
    snakeFile.open("snake.txt",ios::out|ios::binary);
    struct snake* temp=head;
    if(!snakeFile){
        cout<<"Sorry no memory allocated for saving snake"<<endl;
    }else{
        while(temp!=NULL){
            snakeFile<<temp->part<<"\t";
            snakeFile<<temp->x<<"\t";
            snakeFile<<temp->y<<"\n";
            temp=temp->next;
        }
        snakeFile.close();
    }
    foodFile.open("food.txt",ios::out|ios::binary);
    if(!foodFile)
    cout<<"Sorry no memory allocated for saving food"<<endl;
    else{
        foodFile<<bookh->khana<<"\t";
        foodFile<<bookh->x<<"\t";
        foodFile<<bookh->y<<"\n";
        foodFile.close();   
    }

    cout<<"Game successfully saved see u next time!!!"<<endl<<"Bye"<<endl;
}
struct snake* GAME::nhiKhaya(struct snake* head,struct snake* latest,char** ground){
    grab=0;
    struct snake* checkGameOver=head;
    while(checkGameOver!=NULL){
        if(checkGameOver->x==latest->x&&checkGameOver->y==latest->y){
            cout<<"Game over"<<endl;
            exit(0);
        }
        checkGameOver=checkGameOver->next;
    }
  //  if(checkCollision(head,latest))
    //terminate();
    latest->next=head;
    head=latest;
    struct snake* temp=head;
    while(temp->next->next!=NULL)
        temp=temp->next;
    ground[temp->next->x][temp->next->y]=' ';
    ground[head->x][head->y]=head->part;
    ground[head->next->x][head->next->y]=head->next->part;
    struct snake* to_del=new snake;
    to_del=temp->next;
    temp->next=NULL;
    delete to_del;
    return head;
}
struct snake* GAME::khaLia(struct snake* head,struct snake* latest,char** ground){
    grab=1;
    latest->next=head;
    head=latest;
    ground[head->x][head->y]='@';
    ground[head->next->x][head->next->y]='#';
  /*  struct snake* copy=new snake;
    copy->part=head->part;
    copy->x=head->x;
    copy->y=head->y;
    copy->next=NULL;
    insertInHashTable(copy);*/
    return head;
}
struct snake* GAME::moveAhead(struct snake* head,struct food* bookh,int direction,int flag,char** ground){
    //Take flag for checking vertical and horizontal movements restrictioons
    //if flag ==1 check for vertical
    //if flag==0 check for horizontal
    if((flag==1&&head->next->x==direction)||(flag==0&&head->next->y==direction))
    return head;
    struct snake* latest=new snake;
    latest->next=NULL;
    latest->part=head->part;
    head->part=head->next->part;
    if(flag==1){
        latest->x=direction;
        latest->y=head->y;
    }else{
        latest->y=direction;
        latest->x=head->x;
    }
    if(latest->x==bookh->x&&latest->y==bookh->y){
    head=khaLia(head,latest,ground);
    return head;
    }else{
        head=nhiKhaya(head,latest,ground);
        //cout<<"hey";
        return head;
    }
   
   
}
char** GAME::initPrint(char **ground,struct snake* head){
    struct snake* travel=head;
    while(travel!=NULL){
        ground[travel->x][travel->y]=travel->part;
        travel=travel->next;
    }
    return ground;
}
void GAME::print(char **ground){
    for(int i=-1;i<=40;i++)
    cout<<'&';
    cout<<endl;
    for(int i=0;i<40;i++){
        for(int j=-1;j<=40;j++){
            if(j==-1||j==40)
            cout<<'&';
            else
            cout<<ground[i][j];
        }
        cout<<endl;
    }
    for(int i=-1;i<=40;i++)
    cout<<'&';
    cout<<endl;
}
void GAME::displayFood(struct food* bookh){
    cout<<bookh->khana<<"  x: "<<bookh->x<<" y: "<<bookh->y<<endl;
}
struct food* GAME::generate_food(struct snake* head,char** ground){
    int flag=1;
    int x,y;
    while(flag){
        flag=0;
        srand(time(NULL));
        x=rand()%40;
        y=rand()%40;
        struct snake* travel=head;
        while(travel!=NULL){
            if(travel->x==x&&travel->y==y){
                flag=1;
                break;
            }
            travel=travel->next;
        }
    }
    struct food* latest=new food;
    latest->x=x;
    latest->y=y;
    latest->khana='*';
    ground[x][y]='*';
    return latest;
}
void GAME::displaySnake(struct snake* head){
    struct snake* temp=head;
    while(temp!=NULL){
        cout<<temp->part<<" x: "<<temp->x<<" y: "<<temp->y<<endl;
        temp=temp->next;
    }
    cout<<endl;
}
struct snake* GAME::initializeSnake(struct snake* head){
    int init_size=3;
        srand(time(NULL));
        int x=rand()%30+4;
        int y=rand()%30+4;
    while(init_size--){
            struct snake* latest=new snake;
            latest->x=x;
            latest->y=y;
            latest->next=NULL;
        if(head==NULL){
            latest->part='@';
            head=latest;
        }else{
            latest->part='#';
            struct snake* travel=head;
            while(travel->next!=NULL)
                travel=travel->next;
            travel->next=latest;
        }
        ++y;
    }
    //Here we will initalize the myData String to store the inital data of the snake
    struct snake* temp=head;
    int i=0;
    while(temp!=NULL){
    	
	}
    return head;
}
void do_it(){
    //let's allocate the memory to the matrix first
    cout<<"Welcome to Snake game  Controls::->"<<endl;
    cout<<"up arrow for uppar"<<endl<<"left arrow for left"<<endl<<"right arrow for right"<<endl<<"down arrow for niche"<<endl<<"press escape any time to exit"<<endl;
    int rows=40, cols=40;
    char **ground=new char*[rows];
    for(int i=0;i<rows;i++)
        ground[i]=new char[cols];
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++)
            ground[i][j]=' ';   
        }
    GAME g;           
    struct snake* head=NULL;
    struct food* bookh=NULL;   
cout<<"Do u want to continue previous seession y or n"<<endl;
char make_mind;
cin>>make_mind;
if(make_mind=='y'){
    head=g.getStoredSnake(head);
    bookh=g.getStoredFood(bookh);
    ground[bookh->x][bookh->y]='*';
}else{
    head=g.initializeSnake(head);
    bookh=g.generate_food(head,ground);
}
    ground=g.initPrint(ground,head);
    g.print(ground);
    int KB_code=75;   
    while(KB_code!=KB_ESCAPE){
    if(kbhit())
        KB_code=getch();
        
    switch(KB_code){
            case KB_UP:
                if(head->x>0)
                head=g.moveAhead(head,bookh,(head->x-1)%40,1,ground);
                else
                head=g.moveAhead(head,bookh,39,1,ground);
                break;
            case KB_LEFT:
                if(head->y>0)
                head=g.moveAhead(head,bookh,(head->y-1)%40,0,ground);
                else
                head=g.moveAhead(head,bookh,39,0,ground);
                break;
            case KB_RIGHT:
                head=g.moveAhead(head,bookh,(head->y+1)%40,0,ground);
                break;
            case KB_DOWN:
                head=g.moveAhead(head,bookh,(head->x+1)%40,1,ground);
        }
        if(g.grab==1){
            bookh=g.generate_food(head,ground);
            g.grab=0;
        }
        Sleep(200);
        system("cls");
        g.print(ground);
    }   
        g.save(head,bookh);
    //Deletion of memory of matrix
    for(int i=0;i<rows;i++)
    delete [] ground[i];
    delete [] ground;
   
}
int main(){
    do_it();   
}
