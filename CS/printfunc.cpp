#include <stdio.h>

#include <windows.h>
#include <string.h>

void printCut(char *, int);

int main(void) {
	printCut("���󿡼� ���� �Ƹ��ٿ� ���ִ��� ��ҽ��ϴ�.\n", 35);
	
	return 0;
}

void printCut(char* text, int time) {
	int i;
	
	for(i=0;i<strlen(text);i++) {
		Sleep(time);
		printf("%c", text[i]);
	}
}

