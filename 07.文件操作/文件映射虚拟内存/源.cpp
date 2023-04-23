#include <iostream>
#include <windows.h>

struct Student {
	int number;
	char name[MAX_PATH];
	char sex[MAX_PATH];
	int score;
};

int main() {

	Student s[4] = {
		{11111,"ylh","男",99},
		{22222,"lxy","女",90},
		{33333,"wjh","男",85},
		{44444,"cnm","女",95},
	};
	//1. 打开文件
	HANDLE hFile = CreateFile(L"test.txt",
		GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	//2. 创建文件映射
	HANDLE hFileMap = CreateFileMapping(hFile, NULL,
		PAGE_EXECUTE_READWRITE,
		0, 4 * sizeof(Student), L"Mapping1");
	//3. 加载文件映射
	Student* pfile = (Student*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS,
		0, 0, 4 * sizeof(Student));
	//4. 操作
	memcpy(pfile, s, 4 * sizeof(Student));
	//5. 卸载文件映射
	UnmapViewOfFile(pfile);
	//6. 删除文件映射
	CloseHandle(hFileMap);
	//7. 关闭文件
	CloseHandle(hFile);
}