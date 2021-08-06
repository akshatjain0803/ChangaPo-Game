/// header files
#include<iostream>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<dos.h>
#include <windows.h>
#include"mmsystem.h"
#include<thread>
#include<bits/stdc++.h>   
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
void color(int n){
	SetConsoleTextAttribute(hConsole, n);
}
int revert(int r){
	DEVMODE mode;
//first get setting for "current" screen
EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &mode);
if(r==0){
if (mode.dmFields | DM_DISPLAYORIENTATION){
    mode.dmDisplayOrientation = DMDO_180;
    LONG r;
    r = ChangeDisplaySettings(&mode, 0);
 //  std::cout << "result: " << r;
}
}
else{
	if (mode.dmFields | DM_DISPLAYORIENTATION)
{
    mode.dmDisplayOrientation = DMDO_DEFAULT;
    LONG r;
    r = ChangeDisplaySettings(&mode, 0);
}
	
}
}
// funtions to be used	
void copy(int *a,int *b){
		for(int i=0;i<25;i++)
		b[i]=a[i];
	}
	// functions to change the postion and get the position of the cursor on the console
void gotoxy(int x, int y);  //function for moving the cursor at the desired location
int wherex();  // function for getting the cursors current x coordinate
int wherey(); //function for getting the cursors cuurent y coordinate	
	// functions used by the dice
void dice(); //dice shows all the values of the steps for called by the player
int roll();  //  roll returns 1 to 8 except 7 values 
void show_dice(int d);
	//functions for the board
void set_all();
void display();

void gotoxy(int x, int y){  //function for moving the cursor at the desired location
	    COORD pos = { x, y };
	    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	}
int wherex(){  // function for getting the cursors current x coordinate
	    CONSOLE_SCREEN_BUFFER_INFO csbi;
	    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	    return csbi.dwCursorPosition.X;
	}
int wherey(){ //function for getting the cursors cuurent y coordinate
	    CONSOLE_SCREEN_BUFFER_INFO csbi;
	    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	    return csbi.dwCursorPosition.Y;
	}

int p_rem;
int win_j=0;
int win_arr[4] = {0,0,0,0};

	// classes and variables to be used
static int n;//  number of players
class player; //created a class player for storing all info about player and its katis
class kati; //created class kati for used by player class
class square; //created class square for every square on the board
int chance = 1;

int dir1[25]={2,1,0,5,10,15,20,21,22,23,24,19,14,9,4,3,8,13,18,17,16,11,6,7,12}; //  used 
int dir2[25]={10,15,20,21,22,23,24,19,14,9,4,3,2,1,0,5,6,7,8,13,18,17,16,11,12}; //  for the 
int dir3[25]={22,23,24,19,14,9,4,3,2,1,0,5,10,15,20,21,16,11,6,7,8,13,18,17,12}; // every player's
int dir4[25]={14,9,4,3,2,1,0,5,10,15,20,21,22,23,24,19,18,17,16,11,6,7,8,13,12}; // direction

int p1_c[8]={1,3,1,4,2,3,2,4};
int p2_c[8]={3,1,3,2,4,1,4,2};
int p3_c[8]={5,3,5,4,6,3,6,4};
int p4_c[8]={3,5,3,6,4,5,4,6};

int home_on_board[4] = {2,10,22,14};// first indexes of direction arrays

void clear_msg(){
	int oldx=wherex();
	int oldy=wherey();
	int x=101,y=29;
	for(int i=0;i<3;i++){
		for(int j=0;j<59;j++){
			gotoxy(x++,y);
			cout<<" ";
		}
		x=101;
		y++;
	}
	gotoxy(oldx,oldy);
}
	
void clear_line(){
	gotoxy(101,34);
	for(int i=0;i<59;i++)
	cout<<" ";
}

int *return_cod(int n){
	switch(n){
		case 1: return p1_c;
		break;
		case 2: return p2_c;
		break;
		case 3: return p3_c;
		break;
		case 4: return p4_c;
		break;
	}
}

int roll(){  // roll implementation
	int x = wherex();
	int r,j=0;
	char ch;
	srand(time(NULL));
	lable_repeat:
	clear_msg();
	gotoxy(102,29);
	cout<<"ROLL THE DICE BY PRESSING D(BEST OF LUCK) ";
	//PlaySound(TEXT("RTDPD.wav"),NULL,SND_SYNC);
	ch = getche();
//	Sleep(500);
	cout<<"\b \b";
	gotoxy(x,wherey());
		if(ch=='d'||ch=='D'){
			r=rand()%9+1;
			while(r==7||r==9)
			r=rand()%2+2;
			return r;
		}
		else{
			clear_msg();
			gotoxy(102,30);
			cout<<"PLEASE ROLL THE DICE PROPERLY ";
		//	PlaySound(TEXT("PRDP.wav"),NULL,SND_SYNC);
		//	cin.clear();
		//	cin.ignore(numeric_limits<streamsize>::max(),'\n');
			Sleep(300);
			gotoxy(x,wherey());
			goto lable_repeat;
		} 
}
	
int top=-1;
int temp_top=-1;
int store[11];
int store_temp[11];

void push(int n){
	store[++top]=n;
}
void temp_push(int n){
	store_temp[++temp_top]=n;
}
int pop(){
	return store[top--];
}
int temp_pop(){
	return store_temp[temp_top--];
}
int remove(int n){
	if(n==5||n==1){
		while(top!=-1){
			if(store[top]==1||store[top]==5)
			break;
				temp_push(pop());
			}
		}
	else{
		while(store[top]!=n)
		temp_push(pop());
		}
	int temp = n;
	int get =0;
	if(top!=-1)
	get = pop();
	while(temp_top!=-1)
	push(temp_pop());
	return get;
}

class kati {//created class kati for used by player class 
	public:
	char kati_ch;
	int index_of_dir;
	int num;  //gives the kati its identity number;
	int player_no;
	int final_status;
	int h_no;
	kati(int n,int num,char ch,int home){
		this->final_status = 0;
		this->player_no = n;
		this->index_of_dir = 0; 
		this->num = num;
		this->kati_ch = ch;
		this->h_no = home;
	}
	kati(){
		
	}
	void back_to_home(){
		this->index_of_dir = 0;
	}
};

class player{   //created a class player for storing all info about player and its katis
	public:
	string name; //stores the name of the individual player
	char ch;  //stores the symbol for individual player
	kati* k; //assignes 4 katis for each player
	int is_enable;
	int tod;
	int final_destination;
	int no_of_enable_kati;
	int player_no;
	int home_no;
	int direction_arr[25];
	player(int home,char ch,int pno,string name_word){  //whenever a player is made its katis are initialised to their identity numbers
		this->final_destination = 15;
		this->home_no = home;
		this->name = name_word;
		this->no_of_enable_kati = 4;
		is_enable = 1;
		this->player_no = pno;
		k = new kati[4];
		this->ch = ch;
		for(int i=1;i<=4;i++){
		k[i-1]=kati(this->player_no,i,ch,home);
		}
		if(home==1)
			copy(dir1,this->direction_arr);
		else if(home==2)
			copy(dir2,this->direction_arr);
		else if(home==3)
			copy(dir3,this->direction_arr);
		else if(home==4)
			copy(dir4,this->direction_arr);
		else
			cout<<"WRONG HOME NUMBBER INPUT";
			
	}
	
	player(){
		
	}
	
	int make_final(kati *k){
		k->final_status = 1;
		this->no_of_enable_kati--;
		
		if(this->no_of_enable_kati==0){
		this->is_enable = 0;
		p_rem--;
		win_arr[win_j++]=this->player_no;
		PlaySound(TEXT("WIN.wav"),NULL,SND_SYNC);
		//chance--;
		return 2;
	}
	else{
	PlaySound(TEXT("LAL.wav"),NULL,SND_SYNC);
	return 1;
	}
	}
	void box(int ka[]){  // a box for individual map shows the cordinates of blocks helping player to enter proper cordinates
			int x,y;
			x=wherex();
			y=wherey();
			gotoxy(x,y++);
			int col;
			if(this->player_no==1)
			col=9;
			else if(this->player_no==2)
			col=10;
			else if(this->player_no==3)
			col=13;
			else
			col=11;
			color(14);
			cout<<"3 3 3 3 3 3 3";
			for(int i=0;i<4;i++){
					if(ka[i]!=0){
					gotoxy(x,y++);
					if(this->k[i].final_status==0)	{
						color(14);
					cout<<"3 ";
					color(col);
					cout<<" TOKEN "<<i+1<<":"<<" ";
							color(14);
					cout<<"3";
					}			
					else
					cout<<"3           3";
			}
			else{
					color(14);
					gotoxy(x,y++);
					cout<<"3           3";
			}
		}
			color(14);
			gotoxy(x,y++);
			cout<<"3 3 3 3 3 3 3";
			color(7);
	}
	
	void ind_map_show(){ //individual map helping each player to guide regarding entries
		int x,y;
		int count=0;
		gotoxy(100,0);
		for(int i=0;i<5;i++)
		{
			for(int j=0;j<5;j++,count++)
			{
				x=wherex();
				y=wherey();
				int ka[8]={0,0,0,0,0,0,0,0};
				for(int l=0;l<4;l++){
					if(count==this->direction_arr[k[l].index_of_dir]){
						ka[l]++;
						ka[l+4]=count;
					}
				}
				box(ka);
				gotoxy(wherex()-1,wherey()-5);
			}
			gotoxy(100,wherey()+5);
		}
		gotoxy(100,wherey()+2);
	}
};
	kati demo_kati;

player *p;// player array for storing info of all players.
class square{
		public:
			char block[8][8];
			int block_no;// gives the numbber of block
			int is_home_of;// gives the block home no of player if any 
			int is_final; // checks that the square is final or not 
			int is_free;// cheks if the block is free for transaction
			int kati_count[4];
			int is_occupied_by;// gives the player no. the square is occupied by
			kati *stack[16];// keeps the katis in the stack
			kati *temp_stack[16];// keeps katis for temporary time
			int top;// tracks the top of stack 
			int temp_top;// tracks the top of temp stack	
	void fill_home(player *p){// fill the block and push all the katis in it
		this->is_home_of = p->player_no+1;
		for(int i=0;i<4;i++){
		this->push(&p->k[i]);
		}
		this->kati_count[p->player_no] = 4;
		this->is_free = 1;
	}
		
	kati *remove(kati *k){// remove kati from the block
		if(top==-1){
			cout<<"STACK IS EMPTY";
			return &demo_kati;
		}
		else{
			kati *temp_kati;
			while(this->stack[top]!=k&&top!=-1){
				this->push_temp(this->pop());
			}
			if(top!=-1){
			temp_kati = pop();
			}
			while(this->temp_top!=-1){
				this->push(this->pop_temp());
			}
				int *ptr;
				int hno=k->h_no;
				ptr = return_cod(hno);
				int x,y;
				x=wherex();
				y=wherey();
				gotoxy(this->block_no%5*14+ptr[(temp_kati->num-1)*2+1]*2,this->block_no/5*7+ptr[(temp_kati->num-1)*2]);
				cout<<" ";
				if(this->top==-1)
				this->is_free = 1;
				gotoxy(x,y);
			return temp_kati;
		}
	}
	
	void push_temp(kati *k){// temp push
			temp_stack[++this->temp_top] = k;
		}
		
	void push(kati *k){ //pushes kati into the stack
		int *ptr;
		int col;
		int hno=k->h_no;
		ptr = return_cod(hno);
		int x,y;
		x=wherex();
		y=wherey();
		gotoxy(this->block_no%5*14+ptr[(k->num-1)*2+1]*2,this->block_no/5*7+ptr[(k->num-1)*2]);
		if(k->player_no==1)
		col = 9;
		else if(k->player_no==2)
		col = 10;
		else if(k->player_no==3)
		col = 13;
		else
		col = 11;
		if(k->index_of_dir==24)
		color(15);
		else
		color(col);
		cout<<k->kati_ch;
		color(7);
	this->stack[++top] = k;
		this->is_free = 0;
		gotoxy(x,y);
		if(this->is_home_of){
			if(this->kati_count[k->player_no]==4)
			p[k->player_no].tod =0;
			p[k->player_no].final_destination==15;
		}
	}
		
		kati *pop_temp(){//temp pop
			return this->temp_stack[this->temp_top--];
		}
			
	kati *pop(){ // use * for returning a variable which stores address
		if(top==-1)
		cout<<"TOP is equal to -1/n";
		kati *k;
		k= stack[top--];
		stack[top+1]= NULL;
		return k;
	}
			
	void dis_block(){ // show individual block
		int n = this->block_no;
		if(n==12){
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
				if(i>0&&i<7&&j>0&&j<7)
				color(12);
				cout<<block[i][j]<<" ";
				color(7);
			}
				gotoxy(wherex()-16,wherey()+1);
		}
		gotoxy(wherex()+14,wherey()-8);
		}else if(n==2||n==10||n==14||n==22){
			for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
				if(i>0&&i<7&&j>0&&j<7)
				color(2);
				cout<<block[i][j]<<" ";
				color(7);
			}
				gotoxy(wherex()-16,wherey()+1);
		}
		gotoxy(wherex()+14,wherey()-8);
		}
		else{
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					cout<<block[i][j]<<" ";
				}
				gotoxy(wherex()-16,wherey()+1);
			}
			gotoxy(wherex()+14,wherey()-8);
		}
	}
		
	square(int n){ //constructor for initialization
		this->block_no = n;
		this->temp_top = top = -1;
		this->is_free = 1;
		this->is_final= 0;
		this->is_home_of = 0;
		for(int i=0;i<4;i++)
		this->kati_count[i] = 0;
		for(int i=0;i<8;i++){
			block[0][i]='3';
			block[7][i]='3';
			block[i][0]='3';
			block[i][7]='3';
		}
		for(int i=1;i<7;i++){
			for(int j=1;j<7;j++)
			block[i][j]=' ';
		}
	
	}
	
	square(){	
	}
		
	void set_as_home(int p){ //sets the diagonals to show the square as home
			int k=1;
		    this->is_home_of = p;
			for( int i=1,j=6; i<7;i++)
		{
	
			block[i][k]='\\';
			block[i][j]='/';
			j--;
			k++;
		}
	}
};

square board[25]; // creates a board

 
void show_steps(){
	int x=wherex();
	int y=wherey();
	int temp_y = 0;
	int temp_x = 83;
	int col;
	if(p[chance-1].player_no==1)
	col=9;
	else if(p[chance-1].player_no==2)
	col=10;
	else if(p[chance-1].player_no==3)
	col=13;
	else
	col=11;
	for(int i=0;i<40;i++){
			gotoxy(temp_x,temp_y++);
			for(int j=0;j<7;j++){
				cout<<" ";
			}
		}
	temp_y=0;
	color(col);
	for(int i=0;i<=top;i++){
		gotoxy(temp_x,temp_y++);
		cout<<"3 3 3 3";
		if(store[i]==1||store[i]==5){
			gotoxy(temp_x,temp_y++);
			cout<<"3 1 or3";
			gotoxy(temp_x,temp_y++);
			cout<<"3 5   3";
		}
		else{
			gotoxy(temp_x,temp_y++);
			cout<<"3     3";
			gotoxy(temp_x,temp_y++);
			cout<<"3 "<<setw(2)<<store[i]<<"  3";
		}
		gotoxy(temp_x,temp_y);
		cout<<"3 3 3 3";
	}
	color(7);
	gotoxy(x,y);
	Sleep(800);
}

void display(){  // display 
	system("cls"); // clears everything from the console
	for(int i=0;i<4;i++){
		board[home_on_board[i]].set_as_home(i+1);
	}
	board[12].set_as_home(5);
	board[12].is_final = 1;
	for(int i=0;i<25;i++){
		if(i%5==0&&i!=0)
		gotoxy(0,wherey()+7);
		board[i].dis_block();
	}
	for(int i=0;i<n;i++){
	board[p[i].direction_arr[p[i].k[0].index_of_dir]].fill_home(&p[i]);
	}
	gotoxy(100,0);
	p[chance-1].ind_map_show();
	int x=100;
	int y=28;
	for(int i=0;i<31;i++){
		gotoxy(x,y);
		cout<<"3";
		x=x+2;
	}
	for(int i=0;i<7;i++){
		gotoxy(100,++y);
		cout<<"3";
		gotoxy(160,y);
		cout<<"3";
	}
	x=100;
	for(int i=0;i<31;i++){
		gotoxy(x,35);
		cout<<"3";
		x=x+2;
	}
	x=100;
	for(int i=0;i<31;i++){
		gotoxy(x,33);
		cout<<"3";
		x=x+2;
	}
}

void flush(){
	for(int i=0;i<=top;i++)
	store[i]=0;
	while(top!=-1)
	pop();
}
int rem;
void dice(){
	rem = 3; 
	int temp = top;
		int y=wherey();
		int x=wherex();
		int r =  roll();
		push(r);
		rem--;
		PlaySound(TEXT("dice.wav"),NULL,SND_SYNC);
		show_steps();
		gotoxy(x,y);
		
			if(r==4||r==6||r==8){
				clear_msg();
				gotoxy(102,29);
				cout<<"YOU GOT ONE MORE CHANCE DICE AGAIN!!!";
				Sleep(1000);
				r= roll();
				push(r);
				rem--;
				PlaySound(TEXT("dice.wav"),NULL,SND_SYNC);
				show_steps();
					gotoxy(x,y);
						if(r==4||r==6||r==8){
							clear_msg();
							gotoxy(102,29);
							cout<<"YOU GOT ONE MORE CHANCE DICE AGAIN!!!";
							Sleep(1000);
							r= roll();
							push(r);
							rem--;
							PlaySound(TEXT("dice.wav"),NULL,SND_SYNC);
							show_steps();
							gotoxy(x,y);
									if(r==4||r==6||r==8||r==1||r==5){
										clear_msg();
										gotoxy(102,29);
										cout<<"YOU'VE LOST ALL THE CHANCES";
										Sleep(1500);
										while(top!=temp)
										pop();
										rem =3;
										show_steps();
										gotoxy(x,wherey());
										dice();
									}
						}	
			}
			
}

int move(int kati_num,int steps){
	int again=0;	
	int go=0;
	kati *k=&p[chance-1].k[kati_num-1];
	if(k->final_status){
		again =3;
		gotoxy(102,30);
		cout<<"THIS TOKEN IS DISABLED";
		Sleep(1000);
	}else{
	int f=p[chance-1].direction_arr[k->index_of_dir+steps];
	if((k->index_of_dir+steps == p[chance-1].final_destination&&board[f].top==-1)||f==12){
			remove(steps);
		int initial = p[chance-1].direction_arr[k->index_of_dir];
		int final = p[chance-1].direction_arr[k->index_of_dir+steps];
		k->index_of_dir+=steps;
		
		board[initial].remove(k);
		board[initial].kati_count[k->player_no]--;
		board[final].push(k);
		board[final].kati_count[k->player_no]++;
	/*	if(p[chance-1].final_destination == 24)
		if(p[chance-1].is_enable)
			again = 1;
		else
			again = 2;*/
		if(p[chance-1].final_destination == 24) {
 	   // PlaySound(TEXT("win.wav"),NULL,SND_SYNC);
		//	if(p[chance-1].make_final(k))
		//	again = 1;
		//	else
		//	again = 0;
		//	return again;
		again = p[chance-1].make_final(k);
		}
		else{
		PlaySound(TEXT("move.wav"),NULL,SND_SYNC);
		}
	}
	else if(k->index_of_dir+steps > p[chance-1].final_destination){
		clear_msg();
		gotoxy(102,31);
		cout<<"CAN'T MOVE THIS KATI";
		again = 3;
	}
	else if(k->index_of_dir+steps < p[chance-1].final_destination||p[chance-1].final_destination!=24){
	remove(steps);
		
	int initial = p[chance-1].direction_arr[k->index_of_dir];
	int final = p[chance-1].direction_arr[k->index_of_dir+steps];
	
			if(board[final].is_free){ //the block is totaly free u can move
				k->index_of_dir+=steps;
				board[initial].remove(k);
						board[initial].kati_count[k->player_no]--;
						if(board[initial].top==-1)
						board[initial].is_free = 1;
						board[final].push(k);
						PlaySound(TEXT("move.wav"),NULL,SND_SYNC);
						board[final].kati_count[k->player_no]++;
						if(board[final].is_home_of){
						board[final].is_free = 1;}
						else
						{
						board[final].is_free = 0;
						}
						again = 0;
			}else{ // the board is not free u have to check first whether u can move directly or not
				if(board[final].kati_count[k->player_no]!=0){//if the same player kati is already presenmt in the final block
					k->index_of_dir+=steps;
					board[initial].remove(k);
						board[initial].kati_count[k->player_no]--;
						if(board[initial].top==-1)
						board[initial].is_free = 1;	
						board[final].push(k);
						PlaySound(TEXT("move.wav"),NULL,SND_SYNC);
						board[final].kati_count[k->player_no]++;	
						if(!board[final].is_home_of)
						board[final].is_free = 0;
						else
						board[final].is_free = 1;
						again = 0;
				}
				else{//if the same player kati is not present in the final block
			
					int kati_type_count=0;
					int tot_count=0;
					for(int i=0;i<n;i++){
						if(i==k->player_no)
							continue;
						if(board[final].kati_count[i]!=0){
							tot_count+=board[final].kati_count[i];
							kati_type_count++;
						}
					}
					if((tot_count==1&&kati_type_count==1)||(tot_count==3&&kati_type_count==1)||((tot_count==2&&kati_type_count==1)&&p[board[final].stack[board[final].top]->player_no].tod==0)){//checking the other kati present is only of same type and alone
						k->index_of_dir+=steps;
						board[initial].remove(k); // removing the current kati from the players block
						if(board[initial].top==-1)
						board[initial].is_free = 1;
						board[initial].kati_count[k->player_no]--;
						kati *k_temp;
						p[chance-1].final_destination = 24;
						for(int i=board[final].top;i>=0;i--){//back to home;
						
						k_temp = board[final].remove(board[final].stack[i]);
						if(board[final].top == -1)
						board[final].is_free = 1;		
						board[final].kati_count[k_temp->player_no]--;
								
							k_temp->index_of_dir = 0;
							board[p[k_temp->player_no].direction_arr[0]].push(k_temp);
							board[p[k_temp->player_no].direction_arr[0]].is_free = 1;//as the are going to home so it must be free
							
							board[p[k_temp->player_no].direction_arr[0]].kati_count[k_temp->player_no]++;
	
						}
							if(board[p[k_temp->player_no].direction_arr[0]].kati_count[k_temp->player_no]==4)
							p[k_temp->player_no].final_destination = 15;
							
							board[final].push(k);
							PlaySound(TEXT("BTH.wav"),NULL,SND_SYNC);
							if(!board[final].is_home_of)
							board[final].is_free = 0;
							else
							board[final].is_free = 1;
							board[final].kati_count[k->player_no]++;
							p[k->player_no].tod=1;
							again = 1;
					}else{// someone is already present in pair or with some other one so just simply move that kati
						k->index_of_dir+=steps;
						board[initial].remove(k);
						if(board[initial].top==-1)
						board[initial].is_free = 1;
						
						board[initial].kati_count[k->player_no]--;
						board[final].push(k);
						PlaySound(TEXT("move.wav"),NULL,SND_SYNC);
						
						if(!board[final].is_home_of)
						board[final].is_free = 0;
						else
						board[final].is_free = 1;
						board[final].kati_count[k->player_no]++;
					
						again = 0;
					}
				}
				
			}
			if(board[initial].top==-1)
			board[initial].is_free = 1;
			if(board[initial].is_home_of)
			board[initial].is_free = 1;
	}
	else{
		clear_msg();
		gotoxy(102,31);
		cout<<"Exception";
	}
	}
	p[chance-1].ind_map_show();
	return again;
}

int present(int n){
	int r;
	if(n!=1&&n!=5){
	for(int i=0;i<=top;i++)
		if(store[i]==n){	
		return 1;
		}
	}else{
		for(int i=0;i<=top;i++){
			if(store[i]==1||store[i]==5){
				return 1;
			}
		}
	}
	return 0;
}

void inst(){
	char ch;
	cout<<"YOU WANT INSTRUCTIONS ? PRESS Y OR N\n";
	ch = getche();
	if(ch=='Y'||ch=='y'){
	system("cls");
	cout<<"   SOME INSTRUCTIONS AND RULES\n\n";
	cout<<"   1. PRESS D TO ROLL THE DICE.\n";
	cout<<"   2. TO MOVE YOUR TOKEN ENTER TOKEN NUMBER AND STEPS.\n";
	cout<<"   3. YOU NEED A PAAS TO GET INTO THE INNER REGION.\n";
	cout<<"   4. PASS WILL AUTOMATICALLY ASSIGNED TO YOU AFTER YOU REPLACE OTHER PLAYER'S TOKEN.\n";
	cout<<"   5. IF THE OTHER PLAYER DOES HAVE A PASS THEN IF THEIR TOKEN ARE IN PAIR THEY ARE SAFE OTHERWISE THEY ARE UNSAFE.\n";
	cout<<"   6. YOU WILL WIN AFTER YOUR ALL FOUR TOKENS REACH TO THE MAIN BLOCK.\n";
	cout<<"   7. YOU WILL LOSE YOUR PASS IN CASE IF YOUR ALL FOUR TOKEN REACH BACT TO YOUR HOME.\n";
	cout<<"   8. ALL CROSSED BLOCKS SO-CALLED HOMES ARE SAFE FOR EVERYONE.\n";
	cout<<"   9. IF YOU DON'T WANT TO MOVE OR SAY YOU CAN'T MOVE WITH RESPECTIVE STEPS THEN PRESS -1 TO SKIP WHILE ENTERING YOUR TOKEN NUMBER.\n";
	cout<<"   10. 1 AND 5 ARE CONSIDERED AS SAME YOU CAN ENTER ANYONE AS YOU WISH FOR STEPS.\n";
	cout<<"   11. IF YOU GET 4   OR   6   OR   8 YOU GET ONE MORE CHANCE.\n";
	cout<<"   12. AFTER THREE TIMES REPEATING CHANCE YOU'LL LOOSE ALL YOUR GAINED STEPS.\n";
	cout<<"   13. IN THE THIRD REPEATED CHANCE 1 OR 5 IS ALSO RESPONSIBLE FOR LOSING YOUR STEPS\n\n";
	cout<<"   BEST OF LUCK HERE WE GO :)\n";
	PlaySound(TEXT("INST.wav"),NULL,SND_SYNC);
	cout<<"\n\nENTER ANY ALPHABET TO START THE GAME\n";
	cin>>ch;
	}
}
void win_display(){
	system("cls");
	for(int i=0;i<n;i++)
	cout<<win_arr[i]<<" ";
	cout<<"\n";
	string first="      ",second="    ",third="    ";
	if(win_arr[0]+1)
	first = p[win_arr[0]].name;
	if(win_arr[1]+1)
	second = p[win_arr[1]].name;
	if(win_arr[2]+1)
	third = p[win_arr[2]].name;
	if(n==2)
	third="         ";
	gotoxy(0,100);
	cout<<"                "<<setw(12)<<first<<"               \n";
	cout<<"                 3 3 3 3 3 3 3 3                \n";
	cout<<"                 3      3       3               \n";
	cout<<"                 3     33       3               \n";
	cout<<"                 3      3       3               \n";
	cout<<"                 3      3       3               \n";
	cout<<"  "<<setw(12)<<second<<"   3      3       3"<<setw(10)<<third<<"   \n";
	cout<<"  3 3 3 3 3 3 3 3     33333      3 3 3 3 3 3 3 3\n";
	cout<<"  3                                 3333       3\n";
	cout<<"  3     333                        3    3      3\n";
	cout<<"  3    3   3                            3      3\n";
	cout<<"  3       3                           33       3\n";
	cout<<"  3      3                              3      3\n";
	cout<<"  3    333333                      3    3      3\n";
	cout<<"  3                                 3333       3\n";
	cout<<"33333333333333333333333333333333333333333333333333\n";
	cout<<"33333333333333333333333333333333333333333333333333\n";
}
void play_music() {
  PlaySound("merikahani01_www.wav", NULL, SND_SYNC);
}
int chek;
int main(){
//	PlaySound(TEXT("ESP.wav"),NULL,SND_SYNC);
//	std::thread t(play_music);

//PlaySound("merikahani01_www.wav", NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
	PlaySound(TEXT("welcome.wav"),NULL,SND_SYNC);
	string name[4];
	for(int i=0;i<25;i++)
		board[i] = square(i);
	char chr[4];
	lable:	
	//PlaySound(TEXT("ENOP.wav"),NULL,SND_SYNC);
	cout<<"ENTER THE NUMBER OF PLAYERS\n"; //getting number of players
	n = getche();	
	cout<<"\n";
	while(n<50||n>52)
	{
	cout<<"NUMBER OF PLAYER INVALID\n";
	PlaySound(TEXT("INVALID.wav"),NULL,SND_SYNC);	
	n = getche();
	cout<<"\n";
	}
	n=n-48;

	p_rem = n;
	p =  new player[n];
	board[2].is_home_of = 1;
	if(n==2){
		board[22].is_home_of = 2;
		for(int i=0;i<n;i++){
			cout<<"ENTER THE NAME OF PLAYER "<<i+1<<endl;
	//		PlaySound(TEXT("ENP.wav"),NULL,SND_SYNC);
			cin>>name[i];
				while(1){
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					cout<<"THE INPUT IS WRONG"<<endl;
					PlaySound(TEXT("IIW.wav"),NULL,SND_SYNC);
					cin>>name[i];
				}else{
					break;
				}
			}
			cout<<"ENTER THE SYMBOL OF PLAYER "<<i+1<<endl;
		//	PlaySound(TEXT("ESP.wav"),NULL,SND_SYNC);
			cin>>chr[i];
				while(1){
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					cout<<"THE INPUT IS WRONG"<<endl;
					PlaySound(TEXT("IIW.wav"),NULL,SND_SYNC);
					cin>>chr[i];
				}else{
					break;
				}
			}
		}
		p[0] = player(1,chr[0],0,name[0]);
		p[1] = player(3,chr[1],1,name[1]);
	} 
	else{
		board[10].is_home_of = 2;
		board[22].is_home_of = 3;	
		for(int i=0;i<3;i++){
			cout<<"ENTER THE NAME OF PLAYER "<<i+1<<endl;
	//		PlaySound(TEXT("ENP.wav"),NULL,SND_SYNC);
			cin>>name[i];
				while(1){
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					cout<<"THE INPUT IS WRONG"<<endl;
					PlaySound(TEXT("IIW.wav"),NULL,SND_SYNC);
					cin>>name[i];
				}else{
					break;
				}
			}
			cout<<"ENTER THE SYMBOL OF PLAYER "<<i+1<<endl;
	//		PlaySound(TEXT("ESP.wav"),NULL,SND_SYNC);
			cin>>chr[i];
				while(1){
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					cout<<"THE INPUT IS WRONG"<<endl;
					PlaySound(TEXT("IIW.wav"),NULL,SND_SYNC);
					cin>>chr[i];
				}else{
					break;
				}
			}
			p[i] = player(i+1,chr[i],i,name[i]);
		}
		if(n==4){
			board[14].is_home_of = 4;
			cout<<"ENTER THE NAME OF PLAYER 4"<<endl;
	//		PlaySound(TEXT("ENP.wav"),NULL,SND_SYNC);
			cin>>name[3];
				while(1){
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					cout<<"THE INPUT IS WRONG"<<endl;
					PlaySound(TEXT("IIW.wav"),NULL,SND_SYNC);
					cin>>name[3];
				}else{
					break;
				}
			}
			cout<<"ENTER THE SYMBOL OF PLAYER 4"<<endl;
	//		PlaySound(TEXT("ESP.wav"),NULL,SND_SYNC);
			cin>>chr[3];
				while(1){
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					cout<<"THE INPUT IS WRONG"<<endl;
					PlaySound(TEXT("IIW.wav"),NULL,SND_SYNC);
					cin>>chr[3];
				}else{
					break;
				}
			}
			p[3] = player(4,chr[3],3,name[3]);
		}
}
int flag =0;
char ch;
cout<<"YOU WANT TO ROTATE DISPLAY FOR CONVENIENCE PRESS Y FOR YES OR ANY OTHER KEY FOR NO"<<endl;
ch = getche();
cout<<"\n";
if(ch=='Y'||ch=='y')
flag=1;
inst();
	Sleep(3000); // hold the game to read the info
	cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
	display();	
	
	int again;
	while(1){
		if(p_rem==1)
		{
			if(chance==n+1)
			chance=1;
			win_arr[win_j++]=p[chance-1].player_no;
			break;
		}
		clear_line();
		flush();
		show_steps();
		int k,no;
		if(chance==n+1)
			chance=1;
		if(flag)
		revert(p[chance-1].home_no-1);
		int xr=wherex();
		int yr=wherey();
		clear_line();
		gotoxy(125,34);
		cout<<p[p[chance-1].player_no].name<<"'s CHANCE";	
		gotoxy(xr,yr);
		if(!p[chance-1].is_enable){
			chance++;
			continue;
		}
		p[chance-1].ind_map_show();
		lableagain:
			again= 0;
		dice();
		while(top!=-1){
			show_steps();
			lableinvalid1:
			try{
			clear_msg();
			gotoxy(102,29);	
			cout<<"ENTER TOKEN NUMBER AND STEPS ";
		//	PlaySound(TEXT("EKS.wav"),NULL,SND_SYNC);
			cin>>k;
			while(1){
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					Sleep(500);
					clear_msg();
					gotoxy(102,31);
					cout<<"THE INPUT IS WRONG";
					PlaySound(TEXT("IIW.wav"),NULL,SND_SYNC);
					goto lableinvalid1;
				}else{
					break;
				}
			}
			if(k==-1){
				goto skip;
			}
		/*	no =  getche();
			no=-48;
			while(no<1||no>8){
				cout<<"\b \b";
				no =  getche();
				no-=48;
			}*/
			gotoxy(134,29);
			cin>>no;
			while(1){
				if(cin.fail()){
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					Sleep(500);
					clear_msg();
					gotoxy(102,31);
					cout<<"THE INPUT IS WRONG";
					PlaySound(TEXT("IIW.wav"),NULL,SND_SYNC);
					goto lableinvalid1;
				}else{
					break;
				}
			}
			if(no==-1){
				goto skip;
			}
			if(k<1||k>4)
			throw 404;
			}
			catch(...){
				clear_msg();
				gotoxy(102,32);
			}
			gotoxy(100,37);
			clear_line();
				if(!present(no)&&(k<1||k>4)){
					clear_msg();
					gotoxy(102,30);
					cout<<"TOKEN NUMBER AND STEPS ARE INVALID";
					PlaySound(TEXT("INVALID.wav"),NULL,SND_SYNC);
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					Sleep(500);
					goto lableinvalid1;
				}
				if(!present(no)){
					clear_msg();
					gotoxy(102,30);
					cout<<"STEPS ARE INAVLID";
					PlaySound(TEXT("INVALID.wav"),NULL,SND_SYNC);
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					Sleep(500);
					goto lableinvalid1;
				}
				if(k<1||k>4){
					clear_msg();
					gotoxy(102,30);
					cout<<"TOKEN NUMBER IS INVALID";
					PlaySound(TEXT("INVALID.wav"),NULL,SND_SYNC);
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					Sleep(500);
					goto lableinvalid1;
				}
			again = move(k,no);
			show_steps();
			if(again==1)
				goto lableagain;
			if(again==2)
				goto skip;	
			if(again==3)
				goto lableinvalid1;
		}
		skip:
	gotoxy(0,37);
	chance++;
	}
	revert(3);
	win_display();
}
