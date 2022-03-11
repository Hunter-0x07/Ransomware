#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include "info.h"
#pragma warning(disable : 4996)

/**
* 解密所有文件
* @param
* @return
**/
void decrypt_file(char* file_path, int method) {
	FILE* fp;

	if ((fp = fopen(file_path, "rb+")) == NULL) {
		puts("Fail to open file!");
		exit(0);
	}

	// 获取文件大小
	int file_length = get_file_size(fp);
	
	// 只需要读取16的整数倍的数据到内存里来做解密
	int decrypt_size = (file_length / 16) * 16;

	// 读取需要解密的数据大小到内存
	char* dPtr = calloc(decrypt_size, sizeof(char));
	fread(dPtr, 1, decrypt_size, fp);
	rewind(fp);

	// 解密文件内容
	char* dPtr_temp = dPtr;
	char* dPtr_decrypt = dPtr;
	switch (method)
	{
	case 0:
		decrpyt_0_buff(dPtr_temp, dPtr_decrypt, decrypt_size);
		break;
	case 1:
		decrpyt_1_buff(dPtr_temp, dPtr_decrypt, decrypt_size);
		break;
	case 2:
		decrpyt_2_buff(dPtr_temp, dPtr_decrypt, decrypt_size);
		break;
	case 3:
		decrpyt_3_buff(dPtr_temp, dPtr_decrypt, decrypt_size);
		break;
	default:
		decrpyt_0_buff(dPtr_temp, dPtr_decrypt, decrypt_size);
	}

	// 以字节形式打印解密后的内容
	/*printf_buff(decrypt_buf, buffer_size);*/

	// 针对文件大小刚好为16字节的，写入前要特殊处理
	if (decrypt_size == 16) {
		// 获取文件原始大小
		for (int i = 0; i < decrypt_size; i++) {
			if (dPtr[i] == 0) {
				decrypt_size = i;
				/*printf("origin_file_length: %d\n", decrypt_size);*/
				break;
			}
		}
	}

	// 将解密后的内容写入原文件
	fwrite(dPtr, 1, decrypt_size, fp);
	rewind(fp);

	// 针对原始文件大小小于16字节的数据，需要删除文件末尾多余的加密数据
	if (decrypt_size < 16) {
		fdelete(file_path, fp, decrypt_size);
		FILE* fpTemp = tmpfile();
		fcopy(fp, 0, decrypt_size, fpTemp, 0);
		freopen(file_path, "wb+", fp);
		fcopy(fpTemp, 0, -1, fp, 0);
		fclose(fpTemp);
	}

	// 关闭文件
	fclose(fp);

	// 内存释放
	free(dPtr);
}

/**
* 解密文件内容 AES ECB方式解密
* @param
* @param
* @param
* @return
**/
void decrpyt_0_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp) {
	// 生成解密密钥
	AES_KEY aes_decrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_decrypt_key(key, 128, &aes_decrypt_key);

	// 解密
	while (decrypt_size_temp) {
		AES_ecb_encrypt(dPtr_temp, dPtr_decrypt, &aes_decrypt_key, AES_DECRYPT);
		decrypt_size_temp -= AES_BLOCK_SIZE;
		dPtr_temp += AES_BLOCK_SIZE;
		dPtr_decrypt += AES_BLOCK_SIZE;
	}
	
	// 释放内存空间
	free(key);
}

/**
* 解密文件内容 AES CBC方式解密
* @param
* @param
* @param
* @return
**/
void decrpyt_1_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp) {
	// 生成128位解密密钥
	AES_KEY aes_decrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");
	AES_set_decrypt_key(key, 128, &aes_decrypt_key);

	// 解密
	AES_cbc_encrypt(dPtr_temp, dPtr_decrypt, decrypt_size_temp, &aes_decrypt_key, iv, AES_DECRYPT);

	// 释放内存空间
	free(key);
	free(iv);
}

/**
* 解密文件内容 AES OFB方式解密（OFB模式需要用加密密钥来解密）
* @param
* @param
* @param
* @return
**/
void decrpyt_2_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp) {
	// 生成加密密钥
	AES_KEY aes_decrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_decrypt_key);

	// 生成iv向量
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");
	
	// 定义num
	int encrypt_num = 0;

	// 解密
	AES_ofb128_encrypt(dPtr_temp, dPtr_decrypt, decrypt_size_temp, &aes_decrypt_key, iv, &encrypt_num);

	// 释放内存空间
	free(key);
	free(iv);
}

/**
* 解密文件内容 AES CFB方式解密（CFB模式需要用加密密钥来解密）
* @param
* @param
* @param
* @return
**/
void decrpyt_3_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp) {
	// 生成加密密钥
	AES_KEY aes_decrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_decrypt_key);

	// 生成iv初始向量
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");

	// 定义num接收AES_cfb128_encrypt，默认为0
	int encrypt_num = 0;

	// 解密
	AES_cfb128_encrypt(dPtr_temp, dPtr_decrypt, decrypt_size_temp, &aes_decrypt_key, iv, &encrypt_num, AES_DECRYPT);

	// 释放内存空间
	free(key);
	free(iv);
}