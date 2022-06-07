// �ʿ��� ��� ���ϵ��� �ҷ��´� 
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <mmsystem.h>
#include <conio.h>

#include <time.h>
#include <string.h>

#include <tchar.h>

// ���� ���� ����ؼ� Ȧ���� ���� 
#define MAZE_WIDTH 131 // �̷��� ������ �ִ� ���� ���� 
#define MAZE_HEIGHT 41 // �̷��� ������ �ִ� ���� ���� 

#define ENTITY_COUNT 2 // �������� ��ü�� �� 

#define SUBMIT		101

// ������ ���� 
enum { LEFT, TOP, RIGHT, BOTTOM }; // ���⿡ ���� ������ 
enum { MWALL, MEMPTY, MEXIT }; // ��ǥ�� ����
enum unit { EMPTY, WALL, PLAYER, ENTITY, EXIT, ITEMHEAL }; // Ÿ���� ���¿� ���� ������ 
enum entityType { NORMAL, MORERANGE, MOREHEALTH, MOREDAMAGE }; // ������ ��ü�� ���¿� ���� ������ 
typedef enum { False, True } _boolean; // bool�� ���� 
typedef enum { WHITE, RED, GREEN, LIGHTBLUE, BLUE, ORANGE, PURPLE, BLACK } COLOR; // ���� ���� ������ 
typedef enum { MAINSCREEN, GAMESTART, GAMEEND, PROGRAMEND } _systemCode; // ���� ���� �÷��׿� ���� ������ 
typedef enum { MAINSTORY, GAMEOVER, BOSS1, GAMEFINAL } gameStory;

// ����ü ����
// �÷��̾� ����ü 
typedef struct _player {
	int x, y; // �÷��̾��� ��ġ 
	int health; // �÷��̾��� ü�� 
	// int attackDamage;
	
	_player() { // �ʱ� ��ġ, ü�� ���� 
		x = 1;
		y = 1;
		health = 10;
	}
} Player;

// �������� ��ü�� ���� ����ü 
typedef struct _entity {
	int x, y; // �������� ��ü�� ��ġ 
	int health; // �������� ��ü�� ü�� 
	int attackDamage; // �������� ��ü�� ���ݷ� 
	enum entityType type; // �������� ��ü�� ���� 
	
	_entity() { // �ʱ� ��ġ �� ���� ���� 
		x = 10;
		y = 10;
		health = 10;
		type = NORMAL;
	}
} Entity;

// ���� ���� 
typedef struct {
	int X, Y;
} MAPUNIT;

// ���� ���� ����
_systemCode SystemCODE = MAINSCREEN; // ���α׷��� ���¸� �����ϴ� ���� 

int maze[MAZE_HEIGHT][MAZE_WIDTH]; // �̷� ���� 

Entity entity[ENTITY_COUNT]; // �÷��̾ �������� ����ü ���� Entity��� ����ü�� �̷���� ���� 

const int direction[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}}; // ���� ���� 

int backScreenBuffer[MAZE_HEIGHT][MAZE_WIDTH]; // ���� ���۸��� �̿��ϱ� ���� �̷θ� ����ϴµ� �̿��� 

Player playerMain; // �÷��̾�  

int entityUpdateTime; // �÷��̾ ������ ��ü�� �ӵ��� ���� 

int timer; // Ÿ�̸� ���� 
int level; // ���� ���� 

int previousUnit; // ���� ���� ����(������ ��⿡ �̿��) 

const int Rdirect[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}}; // ��, ��, ��, �츦 �ǹ��ϸ� ���� ����Ͽ� 2ĭ�� �̵��Ѵ�. 
int _maze[MAZE_HEIGHT][MAZE_WIDTH]; // �̷θ� ������ �� �ӽ÷� �����س��� ���� 

int mazeWidth, mazeHeight; // �̷��� ����, ���� ũ�� 

// ���� ���� 
FILE* fp;
char location[] = "saves/savedata_"; // ������ �⺻ ��� 
char location2[] = "saves/scenedata_"; // ������ �⺻ ���2

char fileList[][30] = {"princess", "princess", "missedprincess", "mawang", "king", "soldiers", "nights", "title"};
char fileList2[][30] = {"", "", "", "", "", "", "", ""};
char sceneList[][30] = {"intro_", "boss1_"};

/* ���� �Լ� ��� */
// ���� ���� ó�� 
int gameMain(); // ������ ���������� ����Ǹ� �����ϴ� �Լ� 
int gameSetting();
void gameEnd(); // ������ ����Ǹ� �����ϴ� �Լ� 

// ��ü ó�� 
int entityUpdate(); // �÷��̾ ������ ��ü�� ���¸� �����ϴ� �Լ� 

// ���丮 ���
void story(gameStory);
void printText(const char*, int);
void nextDelay(int);

// ȭ�� ó��
void doubleBuffering(); // Double Buffering ����� �̿��ϴ� �Լ� 
void forceBuffering(int, int, int, int); // ������ Ư�� ȭ���� �����ϴ� �Լ� 
void printUnit(int, int, int); // ȭ���� ���ڿ� ���� ������ Ÿ���� ����ϴ� �Լ� 
void gotoxy(short, short); // ȭ���� ������ ���ڷ� �̵��ϴ� �Լ� 
int checkRange(int, int); // ���� ���� 
void backgroundColor(COLOR, _boolean); // ȭ�� �� ����

// â ó�� 
void fullScreen(); // ��ü ȭ���� ���� �Լ� 

// �̷� ���� 
void mazeGen(MAPUNIT); // �̷� ������ ���� �Լ�, �̷� ����⸦ ������ ��ġ�� ������ �� ���� 
void mazeGenA(int, int); // ��������� �̷θ� �����ϱ� ���� �Լ� 
int checkRange(int, int); // ������ Ȯ���ϴ� �Լ�(�̷� ����, �÷��̾�, ��ü�� �������� ���ѿ� �̿��) 
void mixDirection(int*, int); // �̷θ� ������ ��, �����ϰ� ���ư��� ���� �Լ� 

// ���� ���� 
char* getFilePath(char*); // ������ ��θ� �ҷ����� �Լ�, ������ ���缺�� �˻��� 
int loadIMGData(char*); // ������ �̹��� �����͸� �ҷ����� �Լ� 
int loadTEXTData(char*, int); // ������ �ؽ�Ʈ �����͸� �ҷ����� �Լ� 

// �̺�Ʈ ó�� 
int eventHandler();
void bossFight(int);

// ���ڿ� ó��
char* textMerge(char*, int);

// ����ȭ�� ��� �� ���� 
int mainTitle();
int keyControl();
void init(char*);
int titleDraw(); 
int drawMenu(int);
int menuCode();
void infoDraw();

int main(void) {
	
	fullScreen(); // �켱 ȭ���� ��üȭ������ ����� 
	
	story(MAINSTORY);
	
startProgram: // ���α׷� ���� �޽������� �ƴϿ��� ������ �ٽ� ���ƿ��� ���� ���̺� 
	SystemCODE = MAINSCREEN; // ��ü���� �ý����� �����ϱ� ���� �÷��� 
	
	for(;SystemCODE != PROGRAMEND;) { // ���� �÷��װ� ���α׷��� ������ ������ �ƴϸ� 
		mainTitle(); // ���� ���� �� ����ȭ���� ��� �����ش�. 
	}
	
	// ���� ���Ḧ ���ýÿ� ������ Ȯ�� �޽��� ����
	
	int message = MessageBox(NULL, "������ �����Ͻðڽ��ϱ�?", "�˸�", MB_OKCANCEL | MB_TOPMOST | MB_ICONQUESTION); 
	if (message == IDCANCEL) goto startProgram; // ���� ������ �����ϰ� ���� �ʴٸ� 
	
	return 0; // ���α׷� ���� 
}

int gameMain() {
	system("cls"); // ������ ������ �� �켱 �⺻������ ������ ȭ���� �ʱ�ȭ�� 
	
	level = 0; // ������ �ʱ�ȭ�� 
	
	int i; // �ӽ� ������ ������ 
	
	backgroundColor(WHITE, True); // ���ȭ���� ������ 
	 
	playerMain.health = 10; // ������ ������Ͽ��� ���� ����Ͽ� �÷��̾��� �⺻ ü���� �缳���Ѵ�
	playerMain.x = 1; // ������ ������Ͽ��� ���� ����Ͽ� �÷��̾��� �⺻ ��ġ�� �缳���Ѵ� 
	playerMain.y = 1; // ������ ������Ͽ��� ���� ����Ͽ� �÷��̾��� �⺻ ��ġ�� �缳���Ѵ�
	
	mazeWidth = 15; // ������ ������Ͽ��� ���� ����Ͽ� �̷��� �ʱ� ũ�⸦ �缳���Ѵ� 
	mazeHeight = 15; // ������ ������Ͽ��� ���� ����Ͽ� �̷��� �ʱ� ũ�⸦ �缳���Ѵ�
	
	timer = 600;
	
	forceBuffering(0, 0, mazeWidth, mazeHeight); // ������ ȭ���� �ѹ� �ʱ�ȭ�Ѵ�. 
	
	memset(maze, WALL, sizeof(maze)); // string.h
	memset(backScreenBuffer, WALL, sizeof(backScreenBuffer));
	
	mazeGen({1,1}); // �̷θ� �����Ѵ� 
	
	maze[2][2] = ITEMHEAL; // �׽�Ʈ, ������
	
	doubleBuffering(); // ���� ���۸����� �̷θ� �����Ѵ�. 
	
	Sleep(500); // �ᰣ�� �ð������� �д�. 
	
	while(1) { // ���ӿ����� ���� ������ ��� ���� 
		Sleep(50); // ������ �ӵ��� �����ϱ� ���� �ð����� �Լ� 
		
		// ���� ������ Ű�� DŰ�� ������ ���� �ȿ� �÷��̾ ���Դٸ� 
		if (((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState(0x44) & 0x8000)) && checkRange(playerMain.y, playerMain.x+1)) {
			if (maze[playerMain.y][playerMain.x+1] != WALL) { // ���� ������ ��ġ�� ���� ���ٸ� 
				maze[playerMain.y][playerMain.x] = EMPTY; // ���� ��ġ�� ���� 
				playerMain.x += 1; // ������ ����(������)���� ��ǥ�� �̵� 
				previousUnit = maze[playerMain.y][playerMain.x]; // ���� Ÿ���� ����(�������� �Ա� ����) 
				maze[playerMain.y][playerMain.x] = PLAYER; // ���ο� ��ġ�� �÷��̾ ǥ���ϱ� ���� �̷ο� ������ 
			}
		}
		
		// ���� ���� Ű�� AŰ�� ������ ���� �ȿ� �÷��̾ ���Դٸ� 
		if (((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState(0x41)) & 0x8000) && checkRange(playerMain.y, playerMain.x-1)) {
			if (maze[playerMain.y][playerMain.x-1] != WALL) { // ���� ������ ��ġ�� ���� ���ٸ� 
				maze[playerMain.y][playerMain.x] = EMPTY; // ���� ��ġ�� ���� 
				playerMain.x -= 1; // ������ ����(����)���� ��ǥ�� �̵� 
				previousUnit = maze[playerMain.y][playerMain.x]; // ���� Ÿ���� ����(�������� �Ա� ����) 
				maze[playerMain.y][playerMain.x] = PLAYER; // ���ο� ��ġ�� �÷��̾ ǥ���ϱ� ���� �̷ο� ������ 
			}
		}
		
		// ���� ���� Ű�� WŰ�� ������ ���� �ȿ� �÷��̾ ���Դٸ� 
		if (((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState(0x57) & 0x8000)) && checkRange(playerMain.y-1, playerMain.x)) {
			if (maze[playerMain.y-1][playerMain.x] != WALL) { // ���� ������ ��ġ�� ���� ���ٸ� 
				maze[playerMain.y][playerMain.x] = EMPTY; // ���� ��ġ�� ���� 
				playerMain.y -= 1; // ������ ����(����)���� ��ǥ�� �̵� 
				previousUnit = maze[playerMain.y][playerMain.x]; // ���� Ÿ���� ����(�������� �Ա� ����) 
				maze[playerMain.y][playerMain.x] = PLAYER; // ���ο� ��ġ�� �÷��̾ ǥ���ϱ� ���� �̷ο� ������ 
			}
		}
		
		// ���� �Ʒ��� Ű�� SŰ�� ������ ���� �ȿ� �÷��̾ ���Դٸ� 
		if (((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState(0x53) & 0x8000)) && checkRange(playerMain.y+1, playerMain.x)) {
			if (maze[playerMain.y+1][playerMain.x] != WALL) { // ���� ������ ��ġ�� ���� ���ٸ� 
				maze[playerMain.y][playerMain.x] = EMPTY; // ���� ��ġ�� ���� 
				playerMain.y += 1; // ������ ����(�Ʒ���)���� ��ǥ�� �̵� 
				previousUnit = maze[playerMain.y][playerMain.x]; // ���� Ÿ���� ����(�������� �Ա� ����) 
				maze[playerMain.y][playerMain.x] = PLAYER; // ���ο� ��ġ�� �÷��̾ ǥ���ϱ� ���� �̷ο� ������ 
			}
		}
		
		// ���� �÷��̾ �������� �����ߴٸ� 
		if ((playerMain.x == mazeWidth-2) && (playerMain.y == mazeHeight-2) && level%4 == 0) {
			level += 2; // ������ ������Ų�� 
			mazeGen({1,1}); // �̷θ� ���� ����� 
			backgroundColor(WHITE, True); // ���ȭ�� ���� �����Ѵ� 
			playerMain.x = mazeWidth-2; // �÷��̾��� ��ġ�� �缳���Ѵ� 
			playerMain.y = mazeHeight-2; // �÷��̾��� ��ġ�� �缳���Ѵ�
			timer += 200 + level*20;
			Sleep(10); // ��� ����
			eventHandler();
		} else if ((playerMain.x == 1) && (playerMain.y == 1) && level%4 == 2) {
			level += 2;
			mazeGen({mazeWidth-2, mazeHeight-2});
			backgroundColor(WHITE, True);
			timer += 200 + level*20;
			Sleep(10);
			eventHandler();
		}
		
		// ���� RŰ�� �����ٸ�(�׽�Ʈ�� ���� �Լ�)
		if (GetAsyncKeyState(0x52) & 0x8000) {
			level += 2;
			
			eventHandler();
			
			if (level%4 == 0) {
				mazeGen({1,1});
				
				playerMain.x = 1;
				playerMain.y = 1;
			} else {
				mazeGen({mazeWidth-2, mazeHeight-2});
				
				playerMain.x = mazeWidth-2;
				playerMain.y = mazeHeight-2;
			}
			
			timer = 600;
		}
		
		// ���� ESC�� �����ٸ� 
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			goto gameEND; // ������ ������ 
		}
		
		entityUpdateTime += 1; // ��ü ���� �ð� 
		
		if (entityUpdateTime%3 == 0) { // ��ü�� �÷��̾ 3�� ������ �� 1�� �����Ѵ� 
			entityUpdate(); // ��ü ���� �Լ� 
			entityUpdateTime = 0; // ���� �ð� �ʱ�ȭ 
		}
		
		// ���� ������ ��ü�� �÷��̾ �����Ͽ��ٸ� 
		for (i=0;i<ENTITY_COUNT;i++) {
			if (((playerMain.x-entity[i].x)*(playerMain.x-entity[i].x)
			+ (playerMain.y-entity[i].y) * (playerMain.y-entity[i].y)) <= 1.1) {
				playerMain.health -= 1;
			}
		}
		
		// ���� �÷��̾��� ü���� 0���϶��(����) 
		if (playerMain.health <= 0) {
			goto gameEND; // ������ ������ 
		}
		
		// ���� Ÿ���� ����������, � ���������� �˻� 
		switch (previousUnit) {
			case ITEMHEAL: // �� ȸ�� ������ 
				playerMain.health += 5; // �Ǹ� ��������ŭ ä��� 
				break;
			default: // ������ ��Ͽ� ������ �׳� �ѱ�� 
				break;
		}
		
		previousUnit = -1; // ���� ������ -1�� ���� �������� �ߺ��ؼ� ������ ���׸� ���� 
		
		timer -= 1; // Ÿ�̸� �������� 1�� ���� 
		
		if (timer < 0) { // ���� �ð� �ʰ��Ǿ��ٸ�? 
			goto gameEND; // ������ ������ 
		}
		
		doubleBuffering(); // ���� ���۸�, ������ ȭ���� ������ �����ϱ� 
		
		// �������� �����ֱ�(�ӽ�)
		backgroundColor(WHITE, False);
		gotoxy(0, MAZE_HEIGHT+2); 
		printf("Location: (%03d, %03d)\n\n", playerMain.x, playerMain.y);
		printf("Health: %03d\n", playerMain.health);
		printf("level: %03d\n", (int)(level/2));
		printf("time left: %03d\n", (int)(timer/20));
	}

gameEND: // ���� ���� 
	
	gameEnd(); // ������ ������ �Լ� 
	
	return 0;
}

// ��ü ���� �Լ� 
int entityUpdate() {
	int i, j; // �������� ���� 
	int random = 0; // ���� ���� ����(������ ���ϱ� ����) 
	
	for (i=0;i<ENTITY_COUNT;i++) {
		random = rand()%4; // ������ ���� �����Ѵ�.
		
		// ���� ������ ���⿡ �ⱸ, ������, ���� �ְų� �̷��� ������ �Ѿ�ٸ� 
		if (!checkRange(entity[i].y+direction[random][1], entity[i].x+direction[random][0])) {
			continue; // �������� �ʴ´�. 
		} else if (maze[entity[i].y+direction[random][1]][entity[i].x+direction[random][0]] == WALL) {
			continue; // �������� �ʴ´�.
		} else if (maze[entity[i].y+direction[random][1]][entity[i].x+direction[random][0]] == EXIT) {
			continue; // �������� �ʴ´�.
		} else if (maze[entity[i].y+direction[random][1]][entity[i].x+direction[random][0]] == ITEMHEAL) {
			continue; // �������� �ʴ´�.
		}
		
		// ���� ���ǿ� �������� �ʴ´ٸ� ������ �����Ѵ�. 
		maze[entity[i].y][entity[i].x] = EMPTY; // ���� ��ġ������ ��ü�� ���ش� 
		
		entity[i].x += direction[random][0]; // ��ǥ�� �����Ѵ� 
		entity[i].y += direction[random][1]; // ��ǥ�� �����Ѵ�
		
		maze[entity[i].y][entity[i].x] = ENTITY; // ���ο� ��ġ�� ��ü�� �߰��Ѵ�
	}
}

int eventHandler() {
	int num = (int)(level/2);
	
	switch (num) {
		case 10:
			bossFight(0);
			break;
		default:
			playerMain.health += 5;
			break;
	}
}

void bossFight(int bossLevel) {
	// system("cls");
	
	memset(maze, EMPTY, sizeof(maze));
	
	doubleBuffering();
	
	mazeWidth = 101;
	mazeHeight = 31;
	
	switch(bossLevel) {
		case 0:
			// exit(-1);
			break;
		default:
			break;
	}
	
	// TESTBOSS1
	backgroundColor(WHITE, False);
	// gotoxy(,);
	story(BOSS1);
	printText("", 30);
	
	while(1) {
		 
	}
}

// ������ ������ ��쿡 ���� �Լ� 
void gameEnd() {
	
	int i, j; // ������ �����Ѵ�. 
	
	// ȭ�� ���� �����Ѵ�. 
	backgroundColor(BLACK, True);
	backgroundColor(WHITE, False);
	gotoxy(3, mazeWidth/2-4); // Ư�� ��ġ�� �̵��Ѵ�
	for(i=0;i<50;i++) { // '='�� 50�� ����Ѵ� 
		printf("=");
	}
	
	for(j=0;j<5;j++) { // ��ĭ�� ���� �ٿ� ���� ����Ѵ� 
		gotoxy(3, mazeWidth/2-3+j);
		
		for(i=0;i<50;i++) {
			printf(" ");
		}
	}
	
	gotoxy(3, mazeWidth/2+2); // ��ĭ �Ʒ��ٷ� �̵��Ѵ� 
	for(i=0;i<50;i++) { // '='�� 50�� ����Ѵ� 
		printf("=");
	}
	
	// ���� ���� �����Ѵ�. 
	backgroundColor(WHITE, True);
	backgroundColor(WHITE, False);
	gotoxy(10, mazeWidth/2-1);
	printf("���� ������ ������ ��������\n"); // ��� ������ �����ش�. 
	
	// �ʴ� �ѹ��� ������ �����Ѵ�. 
	int secTimer = 2;
	int milsecTimer = 9;
	
	// �ð� 
	while (secTimer >= 0) {
		Sleep(99);
		
		gotoxy(10, mazeWidth/2);
		printf("�� %d.%d�� �ڿ� ����ȭ������ �����ϴ�", secTimer, milsecTimer);
		milsecTimer -= 1;
		
		if (milsecTimer < 0) {
			secTimer -= 1;
			milsecTimer = 9;
		}
	}
	
	fflush(stdin);
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

// Double Buffering�� ���� �Լ� 
void doubleBuffering() {
	int i, j; // ������ �����Ѵ� 
	
	// �̷��� �� ���ڿ� ���� ���� �۾��� �����Ѵ� 
	for (i=0;i<mazeHeight;i++) {
		for (j=0;j<mazeWidth;j++) {
			if (maze[i][j] != backScreenBuffer[i][j]) { // ���� ���� ������ ĭ�� ���� ������ ĭ�� �ٸ��� 
				printUnit(j, i, maze[i][j]); // �ٸ� ĭ�� ����Ѵ� 
				backScreenBuffer[i][j] = maze[i][j]; // �� ĭ�� �����Ѵ� 
			}
		}
	}
}

// ������ ȭ�� ������ ���� �Լ� 
void forceBuffering(int x, int y, int dx, int dy) {
	int i, j;
	
	// �Է¹��� ������ ���� ���� �۾��� �����Ѵ� 
	for (i=y;i<y+dy;i++) {
		for (j=x;j<x+dx;j++) {
			printUnit(j, i, maze[i][j]); // ���� ���� ĭ�� ����Ѵ� 
			backScreenBuffer[i][j] = maze[i][j]; // �� ĭ�� �����Ѵ� 
		}
	}
}

// ĭ�� ����ϱ� ���� �Լ� 
void printUnit(int x, int y, int chara) {
	gotoxy(x+1, y+1); // ȭ�� ����� ��¦ �̵��� 
	
	// backgroundColor(BLACK, True);
	
	switch(chara) { // ����ؾ� �� ĭ�� �ľ��Ѵ� 
		case PLAYER: // ���� ����ؾ� �ϴ� ĭ�� �÷��̾� ĭ�̶�� 
			backgroundColor(ORANGE, True); // ������ ������������ �����Ѵ� 
			break;
		case WALL: // ���� ����ؾ� �ϴ� ĭ�� ���� �ִ� ĭ�̶��
			backgroundColor(WHITE, True); // ������ ������� �����Ѵ� 
			break;
		case EMPTY: // ���� ����ؾ� �ϴ� ĭ�� ����ִ� ĭ�̶��
			backgroundColor(BLACK, True); // ������ ���������� �����Ѵ� 
			break;
		case ENTITY: // ���� ����ؾ� �ϴ� ĭ�� ��ü�� ���� ĭ�̶��
			backgroundColor(RED, True); // ������ ���������� �����Ѵ� 
			break;
		case EXIT: // ���� ����ؾ� �ϴ� ĭ�� �ⱸ ĭ�̶�� 
			backgroundColor(LIGHTBLUE, True); // ������ �ϴû����� �����Ѵ� 
			break;
		case ITEMHEAL: // ���� ����ؾ� �ϴ� ĭ�� �������̶�� 
			backgroundColor(GREEN, True); // ������ �ʷϻ����� �����Ѵ� 
			break;
		default: // ���� ����ؾ� �ϴ� ĭ�� ���ǵ��� ���� ĭ�̶�� 
			exit(-1); // ���α׷��� ������ �����Ѵ� 
			break;
	}
	
	printf(" ");
}

// �̷� ������ ���� �Լ� 
void mazeGen(MAPUNIT startPoint) {
	int i, j; // ������ �����Ѵ� 
	
	srand((unsigned int)time(NULL)); // ���� �Լ� �õ� �ʱ�ȭ 
	// ���ϴ� ��(M_WALL)�� map�� �����ŭ map�� �޸� �Ҵ�, �� map�� ���� ������ ���� 
	memset(_maze, MWALL, sizeof(_maze)); // string.h

	// �̷� ����(�����) 
	mazeGenA(startPoint.Y, startPoint.X);
	
	// �ⱸ �����
	if (level%4 == 0) _maze[mazeHeight-2][mazeWidth-2] = MEXIT;
	else _maze[1][1] = MEXIT;
	
	// �̷� �����ϱ� 
	for (i=0;i<MAZE_HEIGHT;i++) {
		for (j=0;j<MAZE_WIDTH;j++) {
			switch (_maze[i][j]) {
				case MWALL:
					maze[i][j] = WALL;
					break;
				case MEMPTY:
					maze[i][j] = EMPTY;
					break;
				case MEXIT:
					maze[i][j] = EXIT;
					break;
				default:
					break;
			}
		}
	}
}

// �����ϴ� ������ ������ ���� �Լ�
void mixDirection(int* arr, int num) {
	// srand(time(NULL));
	
	int temp, i; // �ӽ� �������� �����Ѵ� 
	int randomNum; // ������ ���� ���� ���� ���� 
	
	for (i=0;i<(num-1);i++) { // num-1���� ���� ������ ���´� 
		randomNum = rand()%(num-i)+i; // ������ ���� �ϳ� �����Ѵ� 
		// �� ĭ�� ���ڸ� �ٲ۴� 
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
	
	mazeWidth = level+15<MAZE_WIDTH?(15 + level):MAZE_WIDTH; // mazeWidth�� �����Ѵ� 
	mazeHeight = level+15<MAZE_HEIGHT?(15 + level):MAZE_HEIGHT; // mazeHeight�� �����Ѵ�
	// ��, �־��� ������ ������ ��, �ִ� ������ ĭ�� �Ѿ�� �ʱ� ���� ���� �����ڸ� �̿��Ѵ� 
	
	return (x < mazeWidth - 1 && x > 0) && (y < mazeHeight - 1 && y > 0); // ���� �Է¹��� ĭ�� ���� ����� 1�� ��ȯ 
}

// �̷� ���� �˰��� (Recursive Backtracking ���)
void mazeGenA(int y, int x) {
	int i = 0; // ������ �����Ѵ� 
	int nx, ny; // ���� ĭ���� ������ ���ư� ���ο� ĭ 
	int direction[4] = { LEFT, RIGHT, TOP, BOTTOM }; // ������ �����Ѵ� 
	
	_maze[y][x] = MEMPTY; // ���� �ִ� ��ġ�� ��ĭ���� �����
	
	// ������ ������ ���´�
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

// ���� ��� �ҷ����� �Լ�(���ڿ��� ��ȯ�ϱ� ���� char*�̴�)
char* getFilePath(char* filename) {
	char* Ifilename = (char*)calloc(100, sizeof(char)); // ���� �̸��� ������ ������ �����Ѵ� 
	
	strcat(Ifilename, location); // ���� �⺻ ��θ� ������ ��ģ�� 
	strcat(Ifilename, filename); // ������ ���� �̸��� ������ ��ģ�� 
	strcat(Ifilename, ".txt"); // Ȯ���ڸ� ������ ��ģ�� 
	
	// printf("%s\n\n", Ifilename);
	// free(Ifilename);
	
	return Ifilename; // ��θ� ������ ��ü �̸��� ��ȯ�Ѵ�
}

// �����͸� �ҷ����� �Լ� 
int loadIMGData(char* filename) {
	if((fp = fopen(filename, "rt")) == NULL) { // ���� ������ �������� ������ 
		return -1; // -1�� ��ȯ�ϸ� �Լ��� ������ 
	}
	
	// ������ ������ �������� �����Ѵ� 
	int i = 0; // ������ �����Ѵ� 
	char buffer[100]; // ������ �� ���� �޾ƿ� �����̴� 
	
	while(!feof(fp)) { // �� �̻� �޾ƿ� �����Ͱ� ���� ������ �Ʒ��� ������ �����Ѵ� 
		fgets(buffer, sizeof(buffer), fp); // buffer�� ũ�⸸ŭ ���پ� �ҷ��´� 
		
		printf("%s", buffer);
		// Data[i] = buffer; // �޾ƿ� ���� Data�� �����Ѵ� 
		// printf("%s", Data[i]); // �ӽ�(����Ͽ� �� �޾ƿ������� Ȯ���ϱ� ���� �뵵) 
		i += 1; // Data�� ���� �࿡ �����ϱ� ���� �ӽ� ������ �ϳ� �����ش� 
	}
	
	fclose(fp); // ���� fp�� �ݾƼ� �޸� ������ ���´� 
	return i;
}

int loadTEXTData(char* filename, int delay) {
	char* Ifilename = (char*)calloc(100, sizeof(char)); // ���� �̸��� ������ ������ �����Ѵ� 
	
	strcat(Ifilename, location2); // ���� �⺻ ��θ� ������ ��ģ�� 
	strcat(Ifilename, filename); // ������ ���� �̸��� ������ ��ģ�� 
	strcat(Ifilename, ".txt"); // Ȯ���ڸ� ������ ��ģ�� 
	
	if((fp = fopen(Ifilename, "rt")) == NULL) { // ���� ������ �������� ������ 
		return -1; // -1�� ��ȯ�ϸ� �Լ��� ������ 
	}
	
	// ������ ������ �������� �����Ѵ� 
	int i = 0; // ������ �����Ѵ� 
	char buffer[100]; // ������ �� ���� �޾ƿ� �����̴� 
	
	while(!feof(fp)) { // �� �̻� �޾ƿ� �����Ͱ� ���� ������ �Ʒ��� ������ �����Ѵ� 
		fgets(buffer, sizeof(buffer), fp); // buffer�� ũ�⸸ŭ ���پ� �ҷ��´� 
		
		printText(buffer, delay);
		// Data[i] = buffer; // �޾ƿ� ���� Data�� �����Ѵ� 
		// printf("%s", Data[i]); // �ӽ�(����Ͽ� �� �޾ƿ������� Ȯ���ϱ� ���� �뵵) 
		i += 1; // Data�� ���� �࿡ �����ϱ� ���� �ӽ� ������ �ϳ� �����ش�  
	}
	
	fclose(fp); // ���� fp�� �ݾƼ� �޸� ������ ���´� 
	return i;
}

// �Է¹��� ���ڷ� �̵��ϱ� ���� �Լ�(Windows Api�� �̿���) 
void gotoxy(short x, short y) {
	COORD pos = {x, y}; // ��ġ�� �޴´� 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); // Ŀ���� ��ġ�� �����ϴ� �Լ��� �̿��Ѵ� 
}

// ��üȭ���� ����� �Լ� 
void fullScreen() {
    SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0); // �ܼ��� ���â�� ��带 �����Ͽ� ��üȭ������ ����� 
}

// ������ �����ϴ� �Լ� 
void backgroundColor(COLOR c, _boolean isBackground) {
	WORD colorType; // ���� Ÿ���� �����Ѵ� 
	
	// �����Ϸ��� ���� �����̶�� 
	if (isBackground == True) {
		goto background; // ������ �����ϴ� ������� �̵��Ѵ� 
	}
	
	switch(c) { // �����Ϸ��� ���� �ľ��Ѵ� 
		case WHITE:
			colorType = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			break;
		case RED:
			colorType = FOREGROUND_RED;
			break;
		case ORANGE:
			colorType = FOREGROUND_RED | FOREGROUND_GREEN;
			break;
		case GREEN:
			colorType = FOREGROUND_GREEN;
			break;
		case LIGHTBLUE:
			colorType = FOREGROUND_GREEN | FOREGROUND_BLUE;
			break;
		case BLUE:
			colorType = FOREGROUND_BLUE;
			break;
		case PURPLE:
			colorType = FOREGROUND_RED | FOREGROUND_BLUE;
			break;
		default:
			colorType = NULL;
			break;
	}
	
	goto paintcolor; // ���� ������ �����ϱ� ���� ������ �̵��Ѵ� 
	
background:
	switch(c) { // �����Ϸ��� ���� �ľ��Ѵ� 
		case WHITE:
			colorType = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
			break;
		case RED:
			colorType = BACKGROUND_RED;
			break;
		case ORANGE:
			colorType = BACKGROUND_RED | BACKGROUND_GREEN;
			break;
		case GREEN:
			colorType = BACKGROUND_GREEN;
			break;
		case LIGHTBLUE:
			colorType = BACKGROUND_GREEN | BACKGROUND_BLUE;
			break;
		case BLUE:
			colorType = BACKGROUND_BLUE;
			break;
		case PURPLE:
			colorType = BACKGROUND_RED | BACKGROUND_BLUE;
			break;
		default:
			colorType = NULL;
			break;
	}
	
paintcolor:
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorType); // �ܼ��� �ؽ�Ʈ �Ӽ��� �����Ͽ� ���� �����Ѵ� 
}

char* textMerge(char* text, int num) {
	char strBuffer[30];
	sprintf(strBuffer, "%s%d", text, num);
	return strBuffer;
}

void story(gameStory type) {
	int sceneNum;
	
	switch(type) {
		case MAINSTORY:
			goto _mainstory;
			break;
		case BOSS1:
			goto _boss1;
			break;
		case GAMEOVER:
			break;
		case GAMEFINAL:
			break;
		default:
			break;
	}
	
_mainstory:
	for(sceneNum=0;sceneNum<8;sceneNum++) {
		system("cls");
		int imageLine = loadIMGData(getFilePath(fileList[sceneNum]));
		
		gotoxy(0, imageLine+1);
		loadTEXTData(textMerge(sceneList[0],(sceneNum+1)), 10);
		
		nextDelay(300);
		
		for(;;) {
			if(GetAsyncKeyState(VK_RETURN) & 0x8000) {
				break;
			} else if(GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				goto _endStory;
			}
		}
	}
	
	goto _endStory;
	
_boss1:
	for(sceneNum=0;sceneNum<10;sceneNum++) {
		int imageLine = loadIMGData(getFilePath(fileList2[sceneNum]));
		
		gotoxy(0, imageLine+1);
		loadTEXTData(sceneList[1], 10);
		
		nextDelay(300);
		
		for(;;) {
			if(GetAsyncKeyState(VK_RETURN) & 0x8000) {
				break;
			} else if(GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				goto _endStory;
			}
		}
	}

_endStory:
	return;
}

void nextDelay(int delayTime) {
	CONSOLE_SCREEN_BUFFER_INFO cursorData;
	
	Sleep(delayTime);
	
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorData);
	
	gotoxy(1, cursorData.dwCursorPosition.Y+2);
	printText("�� EnterŰ�� ���� ����", 10);
	gotoxy(1, cursorData.dwCursorPosition.Y+3);
	printText("�� EscŰ�� ���� ��ŵ", 10);
}

void printText(const char* text, int time) {
	int i;
	
	for(i=0;i<strlen(text);i++) {
		Sleep(time);
		printf("%c", text[i]);
	}
}

int gameSetting() {
	system("cls");
	
	int difficulty = 0;
	_boolean isUpdate;
	
	gotoxy(5, 5);
	backgroundColor(WHITE, False);
	printText("���̵�: ", 5);
	backgroundColor(GREEN, False);
	printText("EASY", 5);
	
	gotoxy(5, 7);
	backgroundColor(WHITE, False);
	printText("�� ���͸� ������ ����", 5);
	gotoxy(5, 8);
	printText("�� ", 5);
	backgroundColor(LIGHTBLUE, False);
	printText("�� ��", 5);
	backgroundColor(WHITE, False);
	printText("�� ������ ���̵� ����", 5);
	
	Sleep(500);
	
	while(1) {
		if(GetAsyncKeyState(VK_LEFT) & 0x8000) {
			if (difficulty > 0) {
				difficulty -= 1;
				isUpdate = True;
			}
		} else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			if (difficulty < 2) {
				difficulty += 1;
				isUpdate = True;
			}
		} else if(GetAsyncKeyState(VK_RETURN) & 0x8000) {
			break;
		}
		
		if (isUpdate == True) {
			gotoxy(5, 5);
			backgroundColor(WHITE, False);
			printf("���̵�: ");
			
			switch (difficulty) {
				case 0:
					backgroundColor(GREEN, False);
					printText("EASY  ", 5);
					break;
				case 1:
					backgroundColor(ORANGE, False);
					printText("NORMAL", 5);
					break;
				case 2:
					backgroundColor(RED, False);
					printText("HARD  ", 5);
					break;
				default:
					break;
			}
			
			Sleep(100);
			
			isUpdate = False;
		}
	}
	
	return difficulty;
}

/*
�������� ���� �ڵ� 
*/

// ���� ���� �� ���� ȭ�� 
int mainTitle() {
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	system("cls"); // ȭ���� �ѹ� �ʱ�ȭ�Ѵ�(�ٸ� ȭ�鿡�� ���ƿ� ���� ����) 
    init("�徾 �����"); // â�� �̸� ���� �⺻ ������ �����Ѵ� 
    
	while(1) {
    	int titleLength = titleDraw();
	    int menuCode = drawMenu(titleLength);
	    
	    switch(menuCode) {
	    	case 0:
	    		gameSetting();
	    		gameMain();
				break;
	    	case 1:
	    		infoDraw();
				break;
	    	case 2:
	    		SystemCODE = PROGRAMEND;
	    		break;
	    	default:
	    		break;
		}
	    
	    system("cls");
	}
	
	return 0;
}

void infoDraw() {
	system("cls");
	printf("\n\n");
	printf("\t\t\t[ ���۹� ]\n\n");
	printf("\t\t�̵�: W, A, S, D\n");
	printf("\t\t����: �����̽���\n\n\n\n\n\n\n");
	printf("\t������: ������, ��ȣâ, ������\n\n");
	printf("\t�����̽��ٸ� ������ ����ȭ������ �̵��մϴ�.");
	
	while(1) {
		if(keyControl() == SUBMIT){
			break;
		}
	}
} 

int keyControl() {
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	Sleep(200);
	
	while (1) {
		
		// ���� ������ Ű�� DŰ�� �����ٸ� 
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState(0x44) & 0x8000)) {
			return RIGHT;
		}
		// ���� ���� Ű�� AŰ�� �����ٸ�
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState(0x41) & 0x8000)) {
			return LEFT;
		}
		// ���� ���� Ű�� �����̽� Ű�� �����ٸ� 
		if ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000)) {
			return SUBMIT;
		}
	}
}

void init(char* gameTitle) {
	SetConsoleTitle(gameTitle);
	PlaySoundA((LPCSTR)"saves/test1.mp3", NULL, SND_FILENAME | SND_ASYNC);
}

int titleDraw() {
	printf("\n\n");
	return loadIMGData(getFilePath("title"));
}

int drawMenu(int titleLength) {
	
	titleLength += 2;
	int menuPointer = 0;
	
	while(1) {
		switch (menuPointer) {
			case 0:
				gotoxy(15, titleLength);
				backgroundColor(GREEN, False);
				printf("�� ���ӽ���  ");
				backgroundColor(WHITE, False);
				gotoxy(45, titleLength);
				printf("   ��������  ");
				gotoxy(75, titleLength);
				printf("   ����      ");
				break;
			case 1:
				gotoxy(15, titleLength);
				printf("   ���ӽ���  ");
				gotoxy(45, titleLength);
				backgroundColor(GREEN, False);
				printf("�� ��������  ");
				backgroundColor(WHITE, False);
				gotoxy(75, titleLength);
				printf("   ����      ");
				break;
			case 2:
				gotoxy(15, titleLength);
				printf("   ���ӽ���  ");
				gotoxy(45, titleLength);
				printf("   ��������  ");
				gotoxy(75, titleLength);
				backgroundColor(GREEN, False);
				printf("�� ����      ");
				backgroundColor(WHITE, False);
				break;
			default:
				break;
		}
		
		gotoxy(2, titleLength+3);
		backgroundColor(WHITE, False);
		printf("�� SpaceŰ�� ������ ����");
		gotoxy(2, titleLength+4);
		printf("�� ");
		backgroundColor(LIGHTBLUE, False);
		printf("�� ��");
		backgroundColor(WHITE, False);
		printf("�� ������ Ŀ�� �̵�");
		
		switch(keyControl()) {
			case LEFT:
				if (menuPointer > 0) menuPointer -= 1;
				break;
			case RIGHT:
				if (menuPointer < 2) menuPointer += 1;
				break;
		    case SUBMIT:
			    return menuPointer;
			default:
				break;
		}
	}
}

