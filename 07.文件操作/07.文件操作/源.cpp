#include <Windows.h>
#include <iostream>
#include <string>

struct Student{
	int number;
	char name[MAX_PATH];
	char sex[MAX_PATH];
	int score;
};

void Test1() {
	Student s[4] = {
		{11111,"ylh","男",99},
		{22222,"lxy","女",90},
		{33333,"wjh","男",85},
		{44444,"cnm","女",95},
	};
	HANDLE hFile = CreateFile(
		L"test1.txt",
		GENERIC_ALL,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS, //始终打开一个文件，如果不存在则创建
		FILE_ATTRIBUTE_NORMAL, NULL
	);	
	if (hFile == INVALID_HANDLE_VALUE) {
		//打开文件失败
		MessageBox(NULL, L"打开文件失败!", L"警告!", MB_OK);
		exit(-1);
	}

	char buff[MAX_PATH]{};
	for (int i = 0; i < 4; i++) {
#if 0
		DWORD len = 0;
		sprintf(buff, "%d,%s,%s,%d", s[i].number, s[i].name.c_str(), s[i].sex.c_str(), s[i].score);
		if (!WriteFile(hFile, buff,
			strlen(buff),
			&len,
			NULL)) {
			MessageBox(NULL, L"写入文件失败!", L"警告!", MB_OK);
			exit(-1);
		}
		std::cout << "写入文件成功! 字节数: " << len << '\n';
#else
		DWORD len = 0;
		//写入文件
		if (!WriteFile(hFile, &s[i], sizeof(Student), &len, NULL)) {
			MessageBox(NULL, L"写入文件失败!", L"警告!", MB_OK);
			exit(-1);
		}
		std::cout << "写入文件成功! 字节数: " << len << '\n';
#endif
	}

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

	Student read_s{};

	//从文件读取
	while (true) {
		DWORD len = 0;
		auto p = ReadFile(hFile, &read_s, sizeof(Student), &len, NULL);
		if (!p) {
			MessageBox(NULL, L"读取文件失败!", L"警告!", MB_OK);
			exit(-1);
		}
		if (len == 0) {
			break;
		}
		std::cout << "读取的字节数: " << len << '\n';
		std::cout << read_s.number << ' ' << read_s.name << ' ' << read_s.sex << ' ' << read_s.score << '\n';
	}

	//关闭文件句柄
	CloseHandle(hFile);
	std::cout << "成功!" << '\n';

	if (!CopyFile(L"test1.txt", L"new_test1.txt", true)) { //已经存在则失败
		MessageBox(NULL, L"文件已经存在!", L"警告", MB_OK);
		exit(-1);
	}

	DeleteFile(L"test1.txt");
}
int main() {
	Test1();
	return 0;
}