#include <stdio.h>

#include <time.h>
#include <string.h>

#include <windows.h>

// ���� ���� ����ؼ� Ȧ���� ���� 
#define MAZE_WIDTH 111
#define MAZE_HEIGHT 31

// ���� �����ϱ� ���� ������ ����
enum { LEFT, TOP, RIGHT, BOTTOM }; // ����
enum { MWALL, MEMPTY }; // ��ǥ�� ���� 

// ���� ���� 
typedef struct {
	int X, Y;
} MAPUNIT;

// �������� 
const int Rdirect[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}}; // ��, ��, ��, �츦 �ǹ��ϸ� ���� ����Ͽ� 2ĭ�� �̵��Ѵ�. 
int _maze[MAZE_HEIGHT][MAZE_WIDTH]; // �̰� [MAZE_HEIGHT][MAZE_WIDTH] �� �����ϱ� 

// �Լ� ���� 
void mazeGen();
void mazeGenA(int, int);
int checkRange(int, int);
void mixDirection(int*, int);

int main(void) {
	
	mazeGen();
	return 0;
}

void mazeGen() {
	int i, j;

	MAPUNIT startPoint = {1,1}; // ������ ����(���Ƿ� ���� ����) 
	
	srand((unsigned int)time(NULL)); // ���� �Լ� �õ� �ʱ�ȭ 
	// ���ϴ� ��(M_WALL)�� map�� �����ŭ map�� �޸� �Ҵ�, �� map�� ���� ������ ���� 
	memset(_maze, MWALL, sizeof(_maze)); // string.h
	
	// �̷� ����(�����) 
	mazeGenA(startPoint.Y, startPoint.X);
	
	// ���� ���� �ձ�
	
	// �̷� �����ϱ� 
	for (int i = 0;i < MAZE_HEIGHT;++i) {
		for (int j = 0;j < MAZE_WIDTH;++j) {
			if (_maze[i][j] == MWALL) printf("W");
			else printf(" ");
		}
		puts(""); 
	}

}

// �����ϴ� ������ ������ ���� �Լ�
void mixDirection(int* arr, int num) {
	// srand(time(NULL));
	
	int temp, i;
	int randomNum; // ������ ���� ���� ���� ���� 
	
	for (i=0;i<(num-1);i++) {
		randomNum = rand()%(num-i)+i;
		temp = arr[i];
		arr[i] = arr[randomNum];
		arr[randomNum] = temp;
	}
}

// ������ ���ο� �ִ��� üũ�ϴ� �Լ� 
int checkRange(int y, int x) {
	/*
	1. y�� ��ü ���̺��� �۰� 0���� ũ�� x�� ��ü ���̺��� �۰� 0���� Ŭ ��, 1�� ��ȯ
	2. �� �ܿ��� 0�� ��ȯ
	*/
	return (x < MAZE_WIDTH - 1 && x > 0) && (y < MAZE_HEIGHT - 1 && y > 0);
}
// �̷� ���� �˰���(Recursive Backtracking ���)
void mazeGenA(int y, int x) {
	int i = 0;
	int nx, ny;
	int direction[4] = { LEFT, RIGHT, TOP, BOTTOM };
	
	_maze[y][x] = MEMPTY; // ���� �ִ� ��ġ�� ��ĭ���� �����. 
	
	// ������ ������ ���´�. 
	mixDirection(direction, 4);
	
	// �� 4 ���⿡ ���� ��������� �����Ѵ�. 
	for (;i<4;i++) {
		// ���� �� ������ ���Ѵ�. 
		nx = x+Rdirect[direction[i]][0];
		ny = y+Rdirect[direction[i]][1];

		// ������ ����� ������ ������ ������ ���� ��(�������)�� �� 
		if (checkRange(ny, nx) && _maze[ny][nx] == MWALL) {
			mazeGenA(ny, nx);
			
			// ������ ��ġ ���̿� ������ �մ´�. 
			if (ny!=y) _maze[(ny+y)/2][x] = MEMPTY;// ���� �� �̵��� ���
			else _maze[y][(x+nx)/2] = MEMPTY; // ���� �� �̵��� ���
			
			// ������ ��ġ�� ������ �մ´�. 
			_maze[ny][nx] = MEMPTY;
		}
	}
}
