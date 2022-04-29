#include <stdio.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define _size	19 // Ȧ�� ���� 

#define LEFT	1
#define RIGHT	2
#define TOP		3
#define BOTTOM	4
#define FILLED	5
#define EMPTY	6

typedef struct {
	int x, y;
} point;

int maze[_size][_size];
int mazeStack[(_size*_size)/4]; // �˳��ϰ� ���� 
// 1: left, 2: right, 3: top, 4: bottom
// 5: Ȯ����, 6: ��Ȯ���� 

void printMaze() {
	int i, j;
	
	// �ӽ� ��� 
	for (i=0;i<_size;i++) {
		for (j=0;j<_size;j++) {
			if (maze[j][i] == LEFT) printf("L");
			else if (maze[j][i] == RIGHT) printf("R");
			else if (maze[j][i] == TOP) printf("T");
			else if (maze[j][i] == BOTTOM) printf("B");
			else if (maze[j][i] == EMPTY) printf(" ");
			else if (maze[j][i] == FILLED) printf("F");
			
			// printf("%s", (maze[j][i]==5)?"��":" ");
		}
		puts("");
	}
}

int main(void) {
	
	int i, j;
	int random;
	
	srand(time(NULL));
	
	for (i=0;i<_size;i++) {
		for (j=0;j<_size;j++) {
			if (i == 0 || i == _size-1 || j == 0 || j == _size-1) {
				maze[i][j] = 5;
			} else {
				maze[i][j] = 6;
			}
		}
	}
	
	i = 1; // ��ǥ�� x�� (�������� +)
	j = 1; // ��ǥ�� y�� (�Ʒ��� +)
	
	int count = 0;
	int stackCount = 0;
	int blockCount = 0;
	
	while (1) {
		// ���� ����� �� �ѷ��ο��ٸ�
		if (i+2 < _size) {
			if (maze[i+2][j] != EMPTY) {
				blockCount++;
			}
		} else {
			blockCount++;
		}
		
		if (j+2 < _size) {
			if (maze[i][j+2] != EMPTY) {
				blockCount++;
			}
		} else {
			blockCount++;
		}
		
		if (i-2 < 0) {
			if (maze[i-2][j] != EMPTY) {
				blockCount++;
			}
		} else {
			blockCount++;
		}
		
		if (j-2 < 0) {
			if (maze[i][j-2] != EMPTY) {
				blockCount++;
			}
		} else {
			blockCount++;
		}
		
		if (blockCount == 4) {
			maze[i][j] = FILLED;
			stackCount--;
			
			// printf("%d, %d\n", stackCount, mazeStack[stackCount]);
			
			switch(mazeStack[stackCount]) { // �̸� ǥ���� ���� �̿��Ѵ�. 
				case LEFT:
					i += 2;
					break;
				case RIGHT:
					i -= 2;
					break;
				case TOP:
					j += 2;
					break;
				case BOTTOM:
					j -= 2;
					break;
				default:
					break;
			}
		} else {
			blockCount = 0;
			stackCount++;
		}
		
		/*
		if (( || i+2 >= _size) // ������ ���� �ҷ����� ������ �ǹ̰� ���� 
			&& (maze[i-2][j] != EMPTY || i-2 < 0)
			&& (maze[i][j+2] != EMPTY || j+2 >= _size)
			&& (maze[i][j-2] != EMPTY || j-2 < 0)) {
			
			
			}
		}
		*/
		
		random = rand()%4 + 1; // ������ ���� ����
		
		switch(random) { // ���⿡ ���� �� 
			case LEFT: // ���� ������ �����̶�� 
				if (i-2 >= 0 && maze[i-2][j] == EMPTY) { // ������ ���� ����ְ� �迭 ������ ���� �ʴ´ٸ� 
					maze[i][j] = LEFT; // ���� ��ġ�� ������ ����ϱ� 
					maze[i-1][j] = FILLED;
					i -= 2; // ���� ��ġ�� Ŀ���� �̵� 
					mazeStack[stackCount] = LEFT; // ���� ��ġ�� ������ ���� �ӽ÷� �����ϱ� 
				} else {
					stackCount--;
				}
				break;
			case RIGHT: // ���� ������ �������̶��
				if (i+2 < _size && maze[i+2][j] == EMPTY) {
					maze[i][j] = RIGHT;
					maze[i+1][j] = FILLED;
					i += 2;
					mazeStack[stackCount] = RIGHT;
				} else {
					stackCount--;
				}
				break;
			case TOP: // ���� ����
				if (j-2 < _size && maze[i][j-2] == EMPTY) {
					maze[i][j] = TOP;
					maze[i][j-1] = FILLED;
					j -= 2;
					mazeStack[stackCount] = TOP;
				} else {
					stackCount--;
				}
				break;
			case BOTTOM: // ���� �Ʒ���
				if (j+2 < _size && maze[i][j+2] == EMPTY) {
					maze[i][j] = BOTTOM;
					maze[i][j+1] = FILLED;
					j += 2; 
					mazeStack[stackCount] = BOTTOM;
				} else {
					stackCount--;
				}
				break;
		}
		
		if (stackCount < 0) break;
		
		// Sleep(100);
		
		count = 0;
		system("cls");
		printMaze();
		
		printf("%d: %d (%d, %d)\n", stackCount, mazeStack[stackCount], i, j);
		
		Sleep(25);
		
		while (1) {
			if (GetAsyncKeyState(VK_RETURN)) break;
		}
		
		count += 1;
		if (count == 100) {
			count = 0;
			system("cls");
			printMaze();
		}
	}
	
	printf("����");
	
	return 0;
}

/*
int main(void) {
	
	int i, j;
	int random;
	
	srand(time(NULL));
	
	// VERSION 1.0
	
	for (i=0;i<_size;i++) {
		for (j=0;j<_size;j++) {
			maze[i][j] = 1;
			
			if ((i+1)%2 == 0 && (j+1)%2 == 0) {
				maze[i][j] = 0;
			}
		}
	}
	
	for (i=1;i<_size;i+=2) {
		for (j=1;j<_size;j+=2) {
			if (rand()%2 == 1) {
				maze[i][j+1] = 0;
			} else {
				maze[i+1][j] = 0;
			}
		}
	}

	// VERSION 1.0
	
	for (i=0;i<_size;i++) {
		for (j=0;j<_size;j++) {
			printf("%s", (maze[i][j]==1)?"��":" ");
		}
		puts("");
	}
	
	return 0;
}
*/
