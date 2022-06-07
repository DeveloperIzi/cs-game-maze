#include <stdio.h>

#include <time.h>
#include <string.h>

#include <windows.h>

// ���� ���� ����ؼ� Ȧ���� ���� 
#define MAZE_WIDTH 111
#define MAZE_HEIGHT 29

// ���� �����ϱ� ���� ������ ����
enum { LEFT, TOP, RIGHT, BOTTOM }; // ����
enum { WALL, EMPTY }; // ��ǥ�� ���� 

// ���� ���� 
typedef struct {
	int X, Y;
} MAPUNIT;

// �������� 
const int Rdirect[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}}; // ��, ��, ��, �츦 �ǹ��ϸ� ���� ����Ͽ� 2ĭ�� �̵��Ѵ�. 
int _maze[MAZE_WIDTH][MAZE_HEIGHT]; // �̰� [MAZE_HEIGHT][MAZE_WIDTH] �� �����ϱ� 

// �Լ� ���� 
void mazeGen(int, int);
int checkRange(int, int);
void mixDirection(int*, int);

int main(void) {
	MAPUNIT startPoint = {1,1}; // ������ ����(���Ƿ� ���� ����) 
	
	srand((unsigned int)time(NULL)); // ���� �Լ� �õ� �ʱ�ȭ 
	// ���ϴ� ��(M_WALL)�� map�� �����ŭ map�� �޸� �Ҵ�, �� map�� ���� ������ ���� 
	memset(_maze, WALL, sizeof(_maze)); // string.h
	// �̷� ����(�����) 
	mazeGen(startPoint.X, startPoint.Y);
	
	// ���� ���� �ձ�
	
	
	// �̷� ��� 
	for (int i = 0;i < MAZE_HEIGHT;++i) {
		for (int j = 0;j < MAZE_WIDTH;++j) {
			if (_maze[j][i] == WALL) printf("W");
			else printf(" ");
		}
		puts(""); 
	}

	return 0;
}

// �����ϴ� ������ ������ ���� �Լ�
void mixDirection(int* arr, int num) {
	// srand(time(NULL));
	
	int temp, i = 0;
	int randomNum; // ������ ���� ���� ���� ���� 
	
	for (;i<(num-1);i++) {
		randomNum = rand()%(num-i)+i;
		temp = arr[i];
		arr[i] = arr[randomNum];
		arr[randomNum] = temp;
	}
}

// ������ ���ο� �ִ��� üũ�ϴ� �Լ� 
int checkRange(int x, int y) {
	/*
	1. y�� ��ü ���̺��� �۰� 0���� ũ�� x�� ��ü ���̺��� �۰� 0���� Ŭ ��, 1�� ��ȯ
	2. �� �ܿ��� 0�� ��ȯ
	*/
	return (x < MAZE_WIDTH - 1 && x > 0) && (y < MAZE_HEIGHT - 1 && y > 0);
}

// �̷� ���� �˰���(Recursive Backtracking ���)
void mazeGen(int x, int y) {
	int i = 0;
	int nx, ny;
	int direction[4] = { LEFT, RIGHT, TOP, BOTTOM };
	
	_maze[x][y] = EMPTY; // ���� �ִ� ��ġ�� ��ĭ���� �����. 
	
	// ������ ������ ���´�. 
	mixDirection(direction, 4);
	
	// �� 4 ���⿡ ���� ��������� �����Ѵ�. 
	for (;i<4;i++) {
		// ���� �� ������ ���Ѵ�. 
		nx = x+Rdirect[direction[i]][0];
		ny = y+Rdirect[direction[i]][1];

		// ������ ����� ������ ������ ������ ���� ��(�������)�� �� 
		if (checkRange(nx, ny) && _maze[nx][ny] == WALL) {
			mazeGen(nx, ny);
			
			// ������ ��ġ ���̿� ������ �մ´�. 
			if (nx!=x) _maze[(nx+x)/2][y] = EMPTY;// ���� �� �̵��� ���
			else _maze[x][(y+ny)/2] = EMPTY; // ���� �� �̵��� ���
			
			// ������ ��ġ�� ������ �մ´�. 
			_maze[nx][ny] = EMPTY;
		}
	}
}

void nextRoad() {
	int i, random = 0;
	int direction[4] = { LEFT, RIGHT, TOP, BOTTOM };
	
	for (i=0;i<4;i++) {
		
	}
}
