#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "info.h"

#pragma warning(disable : 4996)

/**
* SHA1算法生成文件HASH
**/
char* generate_hash(char* file_path) {
	// 存储hash值
	char hash[21];

	// 读取文件
	FILE* fp;
	if ((fp = fopen(file_path, "rb+")) == NULL) {
		puts("Fail to open file!");
		exit(0);
	}

	// 获取文件大小
	int file_length = get_file_size(fp);

	// 分配内存空间
	char* file_buff = (char*)calloc(file_length, sizeof(char));

	// 读取文件到内存
	fread(file_buff, 1, file_length, fp);

	// 调用SHA1算法生成HASH
	SHA1(file_buff, file_length, hash);
	hash[20] = '\0';

	// 释放内存空间
	free(file_buff);
	fclose(fp);

	return hash;
}

/**
* 对比文件加密前后的hash
**/
int compare_hash(char* origin_file_hash, char* encrypt_file_hash) {
	int encrypt_result = 0;

	for (int i = 0; i < 20; i++) {
		/*printf("%02x 对比 %02x\n", origin_file_hash[i], encrypt_file_hash[i]);*/
		/*if (origin_file_hash[i] != encrypt_file_hash[i]) {
			encrypt_result = 1;
			break;
		}*/
		if (origin_file_hash != encrypt_file_hash) {
			encrypt_result = 1;
			break;
		}
	}

	return encrypt_result;
}