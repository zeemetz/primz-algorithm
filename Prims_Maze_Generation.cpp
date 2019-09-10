#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>

#define finishx 80
#define finishy 24
#define wall 219
#define path 32

int map[finishy][finishx]={};

struct wall_list
{
	int x,y,index;
	struct wall_list * next, *prev;
}*head,*tail,*curr;

void gotoxy( int x, int y, int index )
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
	printf("%c",index);
}

void view_map()
{
	for(int i=0;i<finishy;i++)
	{
		for(int j=0;j<finishx;j++)
		{
			if(map[i][j]==0)
				printf("%c",wall);
		}
	}
}

void init_prims()
{
	curr = (struct wall_list*) malloc (sizeof(struct wall_list));
	curr->index = 1;
	curr->x = 0;
	curr->y = 0;
	curr->next = curr->prev = NULL;
	
	head = tail = curr;
}

struct wall_list* search_wall_from_list_by_xy(int x, int y)
{
	if( head->x == x && head->y == y)
	{
		return head;
	}
	else if(tail->x == x && tail->y == y)
	{
		return tail;
	}
	else
	{
		curr = head->next;
		while(curr!=NULL&&curr!=tail)
		{
			if(curr->x == x && curr->y == y)
				return curr;
			curr = curr->next;
		}
		return NULL;
	}
}

void push(int x,int y)
{
	curr = (struct wall_list*) malloc(sizeof(struct wall_list));
	curr->index = tail->index + 1;
	curr->x = x;
	curr->y = y;
	curr->next = curr->prev = NULL;

	tail->next = curr;
	curr->prev = tail;
	tail = curr;
}

void reorder()
{
	curr = head;
	while(curr!=NULL)
	{
		if(curr==head)
			curr->index=1;
		else
			curr->index = curr->prev->index+1;
		curr = curr->next;
	}
}

void pop(struct wall_list *deleted)
{
	if(head == tail)
	{
		free(head);
		head = tail = NULL;
	}
	else if(deleted == head)
	{
		curr = head;
		head = head->next;
		head->prev = NULL;
		free(curr);
	}
	else if( deleted == tail )
	{
		curr = tail;
		tail = tail->prev;
		tail->next  = NULL;
		free(curr);
	}
	else
	{
		curr = head->next;
		while(curr!=tail)
		{
			if(curr == deleted)
			{
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
				free(curr);
				break;
			}
			curr = curr->next;
		}
	}
	reorder();
}

void expand_wall(struct wall_list *expanded)
{
	map[expanded->x][expanded->y] = path;
	gotoxy(expanded->x,expanded->y,map[expanded->x][expanded->y]);

	if( expanded->x > 0 && map[expanded->x-1][expanded->y]==0)
	{
		if( search_wall_from_list_by_xy(expanded->x-1,expanded->y) == NULL )
			push(expanded->x-1,expanded->y);
		else
		{
			pop(search_wall_from_list_by_xy(expanded->x-1,expanded->y));
			map[expanded->x-1][expanded->y] = wall;
		}
	}

	if( expanded->y > 0 && map[expanded->x][expanded->y-1]==0)
	{
		if( search_wall_from_list_by_xy(expanded->x,expanded->y-1) == NULL )
			push(expanded->x,expanded->y-1);
		else
		{
			pop(search_wall_from_list_by_xy(expanded->x,expanded->y-1));
			map[expanded->x][expanded->y-1] = wall;
		}
	}

	if( expanded->x < finishx && map[expanded->x+1][expanded->y]==0)
	{
		if( search_wall_from_list_by_xy(expanded->x+1,expanded->y) == NULL )
			push(expanded->x+1,expanded->y);
		else
		{
			pop(search_wall_from_list_by_xy(expanded->x+1,expanded->y));
			map[expanded->x+1][expanded->y] = wall;
		}
	}

	if( expanded->y < finishy && map[expanded->x][expanded->y+1]==0)
	{
		if( search_wall_from_list_by_xy(expanded->x,expanded->y+1) == NULL )
			push(expanded->x,expanded->y+1);
		else
		{
			pop(search_wall_from_list_by_xy(expanded->x,expanded->y+1));
			map[expanded->x][expanded->y+1] = wall;
		}
	}
	pop(expanded);
}

struct wall_list* search_randomize_wall_from_list(int index)
{
	if( head->index == index )
	{
		return head;
	}
	else if(tail->index == index)
	{
		return tail;
	}
	else
	{
		curr = head->next;
		while(curr!=tail)
		{
			if(curr->index == index)
				return curr;
			curr = curr->next;
		}
		return NULL;
	}
}

void prims()
{
	int random;
	srand(time(0));

	view_map();
	init_prims();

	while(head!=NULL)
	{
		random = (rand()%tail->index) + 1;
		expand_wall( search_randomize_wall_from_list(random) );
		Sleep(5);
	}
}

int main()
{
	prims();
	getchar();
	return 0;
}