#include <stdio.h>

#include <string.h>
#include <stdlib.h>

#include <windows.h>

#define MAZE_WIDTH		100
#define MAZE_HEIGHT		50

// ���� ���� ����
FILE* fp;
char location[] = "saves/savedata_";

char preData[9][20] = {"[data]", "author=", "mapname=", "wsize", "hsize", "xstartpoint", "ystartpoint", "", "[content]"};
char readPreData[6][100];

char Data[100][5000]; // [MAZE_WIDTH * MAZE_HEIGHT]

char maze[MAZE_HEIGHT][MAZE_WIDTH];

int wsize, hsize, xstartpoint, ystartpoint;
char* author, mapname;

// �Լ� ���� 
char* getFilePath(char*);

void saveData(char*, int, int);
int loadData(char*);
void deleteData(char *);
void modifyData();

void saveMazeData();

// ���� ���� �Լ� 
int main(void) {
	int i = 0;
	
	char data[100];
	
	for(;i<2;i++) {
		scanf("%s", data);
		strcat(preData[i+1], (const char*) data);
	}
	
	wsize = 1000;
	hsize = 1000;
	xstartpoint = 1;
	ystartpoint = 1;
	
	saveData(getFilePath(data), 12, 23); // �ӽ�, ���ϸ� �����ϱ� 
	// loadData(getFilePath(data));
	
	return 0;
}

// ���� ��� �ҷ����� �Լ� 
char* getFilePath(char* filename) {
	char* Ifilename = (char*)calloc(100, sizeof(char));
	
	strcat(Ifilename, location);
	strcat(Ifilename, filename);
	strcat(Ifilename, ".txt");
	
	printf("%s\n\n", Ifilename);
	
	// free(Ifilename);
	
	return Ifilename;	
}

// ���ο� ������ ����� �����͸� �����ϴ� �Լ� 
void saveData(char* filename, int words, int lines) {
	int i = 0;
	
	fp = fopen(filename, "wt");
	
	for(;i<9;i++) {
		fprintf(fp, "%s\n", preData[i]);
	}
	
	saveMazeData();
	
	fclose(fp);
}

void saveMazeData() {
	int i = 0;
	
	for(;i<MAZE_HEIGHT;i++) {
		fprintf(fp, "%s", maze[i]);
	}
}

// ������ �����͸� �ҷ����� �Լ� 
int loadData(char* filename) {
	if((fp = fopen(filename, "rt")) == NULL) {
		printf("������ �������� �ʽ��ϴ�.\n\n");
		
		return -1;
	} else {
		printf("���̺������� �߰��߽��ϴ�.\n\n");
	}
	
	int i = 0;
	char buffer[5000];
	
	// fscanf(fp, "%s=%s", readPreData[0], readPreData[1]);
	while(!feof(fp)) {
		fgets(buffer, sizeof(buffer), fp);
		
		strcpy(Data[i], buffer);
		printf("%s", Data[i]);
		i += 1;
	}
	
	fclose(fp);
	return 0;
}

// ���̺� ������ �����ϴ� �Լ� 
void deleteData(char* filename) {
	if((fp = fopen(filename, "rt")) == NULL) {
		printf("������ �������� �ʽ��ϴ�.");
	} else {
		printf("���� ������ �����մϴ�.");
		fclose(fp);
		remove(filename);
	}
}


