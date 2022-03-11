#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "info.h"

#pragma warning(disable : 4996)

/**
* SHA1�㷨�����ļ�HASH
**/
char* generate_hash(char* file_path) {
	// �洢hashֵ
	char hash[21];

	// ��ȡ�ļ�
	FILE* fp;
	if ((fp = fopen(file_path, "rb+")) == NULL) {
		puts("Fail to open file!");
		exit(0);
	}

	// ��ȡ�ļ���С
	int file_length = get_file_size(fp);

	// �����ڴ�ռ�
	char* file_buff = (char*)calloc(file_length, sizeof(char));

	// ��ȡ�ļ����ڴ�
	fread(file_buff, 1, file_length, fp);

	// ����SHA1�㷨����HASH
	SHA1(file_buff, file_length, hash);
	hash[20] = '\0';

	// �ͷ��ڴ�ռ�
	free(file_buff);
	fclose(fp);

	return hash;
}

/**
* �Ա��ļ�����ǰ���hash
**/
int compare_hash(char* origin_file_hash, char* encrypt_file_hash) {
	int encrypt_result = 0;

	for (int i = 0; i < 20; i++) {
		/*printf("%02x �Ա� %02x\n", origin_file_hash[i], encrypt_file_hash[i]);*/
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