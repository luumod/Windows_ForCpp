#include <iostream>
#include <windows.h>

void travelPath(char* pathName);

int main() {
	//获取当前文件夹名
	char currentFilePath[MAX_PATH] = { 0 };

	GetCurrentDirectory(MAX_PATH, currentFilePath);


	travelPath(currentFilePath);
	system("pause");
	return 0;
}

void travelPath(char* pathName) {
	char filename[MAX_PATH]{};
	sprintf(filename, "%s\\*.*", pathName);

	WIN32_FIND_DATA find_data{};
	HANDLE hFile = FindFirstFile(filename, &find_data);  //find_data接受文件信息

	int ret = true;
	while (ret) {
		//pathname：文件路径前缀
		sprintf(filename, "%s\\%s", pathName, find_data.cFileName);
		//如果文件属性等于目录，则递归
		if (find_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			if (strcmp(find_data.cFileName, ".") != 0 && strcmp(find_data.cFileName, "..") != 0) {
				std::cout << "文件夹----------------: " << filename << '\n';
				travelPath(filename);
			}
		}
		else {
			std::cout << "文件: " << filename << '\n';
		}
		ret = FindNextFile(hFile, &find_data);
	}

}