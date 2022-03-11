#include "info.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#pragma warning(disable : 4996)

/**
* ���������ļ�
* @param
* @return
**/
void encrypt_file(char* file_path, int method) {
	FILE* fp;

	if ((fp = fopen(file_path, "rb+")) == NULL) {
		puts("Fail to open file!");
		exit(0);
	}

	// ��ȡ�����ļ���С����λ�ֽ�
	int file_length = get_file_size(fp);

	// ָ���������ݴ�С
	int encrypt_size = file_length;
	if (file_length < 16) {
		encrypt_size = 16;
		file_length = 16;
	}
	else {
		encrypt_size = (file_length / 16) * 16;
	}


	// Ϊ��ȡ�ļ����ݷ����ڴ�
	char* dPtr = calloc(file_length, sizeof(char));

	// ��ȡ�ļ�����
	fread(dPtr, 1, file_length, fp);
	rewind(fp);

	// ��ӡ�ļ�ԭʼ����
	/*printf_buff(dPtr, file_length);*/

	// �����ļ�
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

	// д��ԭ�ļ�
	fwrite(dPtr, 1, file_length, fp);
	rewind(fp);

	// �ļ��ر�
	fclose(fp);

	// �ڴ��ͷ�
	free(dPtr);
}

/**
* AES ECB��ʽ����
* @param
* @param
* @param
* @return
**/
void encrypt_0_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size) {
	// ����128λ��Կ
	AES_KEY aes_encrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_encrypt_key);
	while (encrypt_size) {
		AES_ecb_encrypt(dPtr_temp, dPtr_encrypt, &aes_encrypt_key, AES_ENCRYPT);
		encrypt_size -= AES_BLOCK_SIZE;
		dPtr_temp += AES_BLOCK_SIZE;
		dPtr_encrypt += AES_BLOCK_SIZE;
	}

	// �ͷ��ڴ�
	free(key);
}

/**
* AES CBC��ʽ����
* @param
* @param
* @param
* @return
**/
void encrypt_1_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size) {
	// ����128λ��Կ
	AES_KEY aes_encrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_encrypt_key);

	// ����iv��ʼ����
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");

	// ��������
	AES_cbc_encrypt(dPtr_temp, dPtr_encrypt, encrypt_size, &aes_encrypt_key, iv, AES_ENCRYPT);
	
	// �ڴ��ͷ�
	free(key);
	free(iv);
}

/**
* AES OFB��ʽ����
* @param
* @param
* @param
* @return
**/
void encrypt_2_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size) {
	// ����128λ��Կ
	AES_KEY aes_encrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_encrypt_key);

	// ����iv��ʼ����
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");

	// ����num����ΪAES_cfb128_encrypt����
	int encrypt_num = 0;

	// ��������
	AES_ofb128_encrypt(dPtr_temp, dPtr_encrypt, encrypt_size, &aes_encrypt_key, iv, &encrypt_num);

	// �ͷ��ڴ�
	free(key);
	free(iv);
}

/**
* AES CFB��ʽ����
* @param
* @param
* @param
* @return
**/
void encrypt_3_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size) {
	// ����128λ��Կ
	AES_KEY aes_encrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_encrypt_key);

	// ����iv��ʼ����
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");

	// ����num����ΪAES_cfb128_encrypt����
	int encrypt_num = 0;

	// ��������
	AES_cfb128_encrypt(dPtr_temp, dPtr_encrypt, encrypt_size, &aes_encrypt_key, iv, &encrypt_num, AES_ENCRYPT);

	// �ͷ��ڴ�
	free(key);
	free(iv);
}
