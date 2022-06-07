#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#define MAZE_WIDTH		100
#define MAZE_HEIGHT		50

char* getFilePath(char*);
int loadData(char*);

// ���� ���� ����
FILE* fp;
char location[] = "saves/savedata_";

char Data[100][5000]; // [MAZE_WIDTH * MAZE_HEIGHT]

char maze[MAZE_HEIGHT][MAZE_WIDTH];

int main(void) {
	
	loadData(getFilePath("nights"));
	
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

