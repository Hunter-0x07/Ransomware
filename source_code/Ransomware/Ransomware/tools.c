#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <assert.h>
#include <math.h>
#include "info.h"

// 解决visual studio对于不安全函数使用的报错
#pragma warning(disable : 4996)

/**
* 根据文件路径检索特定后缀文件
* @param	findExtension	指向要检索的文件路径数组的指针
* @param	findExtensionLength	要检索的文件路径数组的长度
* @return	检索到的所有文件信息的结构体数组
**/
ExistFile* retrieve(char** findExtension, int findExtensionLength) {
	// 计算需要为ExistFile结构体数组分配的内存空间
	size_t exist_files_size = sizeof(ExistFile) * findExtensionLength;

	// files结构体变量包含找到的文件的信息
	struct _finddata_t files;

	// 调用_findfirst函数返回的文件句柄
	intptr_t File_Handle;

	// 统计检索到的文件数量进而为ExistFile数组分配内存
	int file_num = 0;
	for (int j = 0; j < findExtensionLength; j++) {

		File_Handle = _findfirst(findExtension[j], &files);

		// 如果没有相关指定后缀文件，则继续下一个路径检索
		if (File_Handle == -1) {
			continue;
		}

		// 统计文件数量
		do {
			file_num++;

		} while (_findnext(File_Handle, &files) == 0);
	}

	// 定义指向ExistFile结构体数组的指针
	ExistFile* pexist_files = (ExistFile*)malloc(file_num * (sizeof(ExistFile)));

	// 循环检索文件
	for (int i = 0; i < findExtensionLength; i++) {

		File_Handle = _findfirst(findExtension[i], &files);

		// 如果没有相关指定后缀文件，则继续下一个路径检索
		if (File_Handle == -1) {
			continue;
		}

		do {
			// 保存文件名称
			pexist_files->file_name = (char*)malloc(strlen(files.name) + 1);
			strcpy(pexist_files->file_name, files.name);
			printf("文件名称: %s\n", pexist_files->file_name);

			// 保存文件路径
			char* file_path = get_file_path(findExtension[i], files.name);
			pexist_files->file_path = (char*)malloc(strlen(file_path) + 1);
			strcpy(pexist_files->file_path, file_path);
			printf("文件路径: %s\n", pexist_files->file_path);

			// 保存原始文件大小
			/*FILE* fp;
			if ((fp = fopen(pexist_files->file_path, "rb+")) == NULL) {
				puts("Fail to open file!\n");
				exit(0);
			}
			pexist_files->origin_file_length = get_file_size(fp);*/

			// 保存文件总数量
			pexist_files->file_num = file_num;
			pexist_files++;

			/*printf("file_name: %s\n", pexist_files->file_name);
			printf("file_path: %s\n", pexist_files->file_path);*/
			// 释放内存
			free(file_path);

		} while (_findnext(File_Handle, &files) == 0);
	}

	// 恢复指针初始位置
	pexist_files = pexist_files - file_num;

	return pexist_files;
}

///**
//* 获取文件绝对路径
//* @param	目录绝对路径
//* @param	文件名称
//* @return	文件绝对路径
//**/
//char* get_file_path(char* directory_path, char* file_name) {
//	char* pch;
//	int i = 0;
//
//	// 获取'\'字符最后一次出现的位置
//	pch = strrchr(directory_path, '\\');
//	int pos = pch - directory_path + 1;
//
//	// 文件绝对路径
//	char* file_path = (char*)calloc(pos, 1);
//
//	while (pos > 0) {
//		file_path[i] = directory_path[i];
//		pos -= 1;
//		i++;
//	}
//	/*strncpy(file_path, directory_path, pos);*/
//
//	// 拼接目录路径和文件路径进而获取文件绝对路径
//	strcat(file_path, file_name);
//
//	return file_path;
//}

/**
* 获取文件绝对路径
* @param	目录绝对路径
* @param	文件名称
* @return	文件绝对路径
**/
char* get_file_path(char* directory_path, char* file_name) {
	// 获取direcroty长度
	int directory_path_length = strlen(directory_path);

	// 获取'\'字符最后一次出现的位置
	char* last_pos = strrchr(directory_path, '\\');
	
	// 获取'\'字符出现的位置的索引
	int last_pos_index = 0;
	char* first_pos = directory_path;
	for (int i = 1; i <= directory_path_length; i++) {
		first_pos++;
		if (first_pos == last_pos) {
			last_pos_index = i + 1;
			/*printf("%c\n", directory_path[last_pos_index]);*/
		}
	}

	// 为最后拼接的文件路径分配内存
	int path_size = last_pos_index + strlen(file_name) + 3;
	char* file_path = calloc(path_size, sizeof(char));
	strncpy(file_path, directory_path, last_pos_index);
	strcat(file_path, file_name);

	return file_path;
}

/**
* 获取文件大小
* @param	File指针
* @return	以单位字节为单位的文件大小
**/
int get_file_size(FILE* fp) {
	// 用于保存文件大小，单位字节
	int n;
	// int fgetpos(FILE *fp, fpos_t *pos);
	fpos_t fpos;

	// 获取当前文件的指针所指的位置，并把该指针所指的位置信息存放在fpos变量中
	fgetpos(fp, &fpos);

	// 移动文件指针到文件末尾
	fseek(fp, 0, SEEK_END);

	// 获取当前指针距离文件开头的字节数
	n = ftell(fp);

	// 将指针恢复到初始位置，也就是文件开头
	fsetpos(fp, &fpos);

	return n;
}

/**
* 文件复制函数	如：fcopy(fSource, 0, -1, fTarget, 0)：将原文件全部内容复制到目标文件
* @param	fSource	要复制的原文件
* @param	offsetSource	原文件的位置偏移（相对文件开头），也就是从哪里开始复制
* @param	len	要复制的内容长度，小于0表示复制offsetSource后边的所有内容
* @param	fTarget	目标文件，也就是将文件复制到哪里
* @param	offsetTarget	目标文件的位置偏移，也就是复制到目标文件的什么位置
* @return 成功复制的字节数
**/
long fcopy(FILE* fSource, long offsetSource, long len, FILE* fTarget, long offsetTarget) {
	int bufferLen = 1024 * 4;
	char* buffer = (char*)malloc(bufferLen);
	int readCount;
	long nBytes = 0;
	int n = 0;
	int i;

	fseek(fSource, offsetSource, SEEK_SET);
	fseek(fTarget, offsetTarget, SEEK_SET);

	if (len < 0) {
		while ((readCount = fread(buffer, 1, bufferLen, fSource)) > 0) {
			nBytes += readCount;
			fwrite(buffer, readCount, 1, fTarget);
		}
	}
	else {
		n = (int)ceil((double)((double)len / bufferLen));
		for (i = 1; i <= n; i++) {
			if (len - nBytes < bufferLen) {
				bufferLen = len - nBytes;
			}
			readCount = fread(buffer, 1, bufferLen, fSource);
			fwrite(buffer, readCount, 1, fTarget);
			nBytes += readCount;
		}
	}
	fflush(fTarget);
	free(buffer);

	return nBytes;
}

/**
* 删除距离文件开头offset偏移量后的所有内容
* @param
* @param
* @param
* @return
**/
int fdelete(char* file_path, FILE* fp, long offset) {
	long fileSize = get_file_size(fp);
	FILE* fpTemp;

	if (offset > fileSize || offset < 0) {
		return -1;
	}

	fpTemp = tmpfile();
	fcopy(fp, 0, offset, fpTemp, 0); // 将前offset字节的数据复制到临时文件
	//fcopy(fp, offset + len, -1, fpTemp, offset); // 将offset + len之后的所有内容都复制到临时文件
	freopen(file_path, "wb+", fp);
	fcopy(fpTemp, 0, -1, fp, 0);

	fclose(fpTemp);

	return 0;
}

/**
* 打印字节
* @param
* @param
* @return
**/
void printf_buff(char* buff, int size) {
	int i = 0;
	for (i = 0; i < size; i++) {
		printf("%02X ", (unsigned char)buff[i]);
		if ((i + 1) % 8 == 0) {
			printf("\n");
		}
	}
	printf("\n\n\n\n");
}

/**
* 字符串转Hex
* @param
* @return
**/
unsigned char* str2hex(char* str) {
	unsigned char* ret = NULL;
	int str_len = strlen(str);
	int i = 0;
	assert((str_len % 2) == 0);
	ret = (char*)malloc(str_len / 2);
	for (i = 0; i < str_len; i = i + 2) {
		sscanf(str + i, "%2hhx", &ret[i / 2]);
	}
	return ret;
}

/**
* 拼接后缀名和文件目录路径
* @param
* @param
* @return
**/
char** path_concat_ext(char* extension, char* path) {
	/*char* extension = "*.txt,*.docx,calc.exe,*.pdf";
	char* path = "C:\\Users\\test\\";*/

	// 计算用户输入的目录大小便于后续的内存分配
	int path_size = strlen(path);

	// 计算用户输入的后缀字符串的长度
	int extension_length = strlen(extension);

	int flag = 0;
	char* p = extension;
	int extension_num = 0;

	// 统计后缀名个数
	for (int i = 0; i <= extension_length; i++) {
		if (extension[i] == ',' || extension[i] == '\0') {
			extension_num += 1;
		}
	}

	// 根据后缀名个数为指针数组分配内存空间
	char** save_extension = calloc(extension_num, sizeof(char*));

	int extension_index = 0;

	for (int i = 0; i <= extension_length; i++) {

		if (extension[i] == ',' || extension[i] == '\0') {
			// cut_num 为当前后缀的字符个数
			int cut_num = i - flag;

			// 根据当前后缀的字符个数分配内存空间
			save_extension[extension_index] = calloc(cut_num + 1, sizeof(char));

			// 将后缀名保存到内存空间中
			strncpy(save_extension[extension_index], p, cut_num);
			save_extension[extension_index][cut_num] = '\0';
			extension_index += 1;

			// 到达字符串末尾则不再调整指针
			if (extension[i] != '\0') {
				flag = i + 1;
				p += cut_num + 1;
			}
		}
	}

	// 打印保存后缀名的数组 save_extension
	/*for (int i = 0; i < extension_num; i++) {
		printf("extension: %s\n", save_extension[i]);
	}*/

	// 拼接目录和后缀名
	char** findExtension = calloc(extension_num, sizeof(char*));
	for (int i = 0; i < extension_num; i++) {
		int findExtensionSize = path_size + strlen(save_extension[i]) + 1;
		findExtension[i] = calloc(findExtensionSize, sizeof(char));
		strcpy(findExtension[i], path);
		strcat(findExtension[i], save_extension[i]);
	}

	// 打印最终拼接成功的数组
	/*for (int i = 0; i < extension_num; i++) {
		printf("extension path: %s\n", findExtension[i]);
	}*/

	// 释放内存空间
	for (int i = 0; i < extension_num; i++) {
		free(save_extension[i]);
	}
	free(save_extension);

	return findExtension;
}