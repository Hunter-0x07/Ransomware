#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include "info.h"
#pragma warning(disable : 4996)

/**
* ���������ļ�
* @param
* @return
**/
void decrypt_file(char* file_path, int method) {
	FILE* fp;

	if ((fp = fopen(file_path, "rb+")) == NULL) {
		puts("Fail to open file!");
		exit(0);
	}

	// ��ȡ�ļ���С
	int file_length = get_file_size(fp);
	
	// ֻ��Ҫ��ȡ16�������������ݵ��ڴ�����������
	int decrypt_size = (file_length / 16) * 16;

	// ��ȡ��Ҫ���ܵ����ݴ�С���ڴ�
	char* dPtr = calloc(decrypt_size, sizeof(char));
	fread(dPtr, 1, decrypt_size, fp);
	rewind(fp);

	// �����ļ�����
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

	// ���ֽ���ʽ��ӡ���ܺ������
	/*printf_buff(decrypt_buf, buffer_size);*/

	// ����ļ���С�պ�Ϊ16�ֽڵģ�д��ǰҪ���⴦��
	if (decrypt_size == 16) {
		// ��ȡ�ļ�ԭʼ��С
		for (int i = 0; i < decrypt_size; i++) {
			if (dPtr[i] == 0) {
				decrypt_size = i;
				/*printf("origin_file_length: %d\n", decrypt_size);*/
				break;
			}
		}
	}

	// �����ܺ������д��ԭ�ļ�
	fwrite(dPtr, 1, decrypt_size, fp);
	rewind(fp);

	// ���ԭʼ�ļ���СС��16�ֽڵ����ݣ���Ҫɾ���ļ�ĩβ����ļ�������
	if (decrypt_size < 16) {
		fdelete(file_path, fp, decrypt_size);
		FILE* fpTemp = tmpfile();
		fcopy(fp, 0, decrypt_size, fpTemp, 0);
		freopen(file_path, "wb+", fp);
		fcopy(fpTemp, 0, -1, fp, 0);
		fclose(fpTemp);
	}

	// �ر��ļ�
	fclose(fp);

	// �ڴ��ͷ�
	free(dPtr);
}

/**
* �����ļ����� AES ECB��ʽ����
* @param
* @param
* @param
* @return
**/
void decrpyt_0_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp) {
	// ���ɽ�����Կ
	AES_KEY aes_decrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_decrypt_key(key, 128, &aes_decrypt_key);

	// ����
	while (decrypt_size_temp) {
		AES_ecb_encrypt(dPtr_temp, dPtr_decrypt, &aes_decrypt_key, AES_DECRYPT);
		decrypt_size_temp -= AES_BLOCK_SIZE;
		dPtr_temp += AES_BLOCK_SIZE;
		dPtr_decrypt += AES_BLOCK_SIZE;
	}
	
	// �ͷ��ڴ�ռ�
	free(key);
}

/**
* �����ļ����� AES CBC��ʽ����
* @param
* @param
* @param
* @return
**/
void decrpyt_1_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp) {
	// ����128λ������Կ
	AES_KEY aes_decrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");
	AES_set_decrypt_key(key, 128, &aes_decrypt_key);

	// ����
	AES_cbc_encrypt(dPtr_temp, dPtr_decrypt, decrypt_size_temp, &aes_decrypt_key, iv, AES_DECRYPT);

	// �ͷ��ڴ�ռ�
	free(key);
	free(iv);
}

/**
* �����ļ����� AES OFB��ʽ���ܣ�OFBģʽ��Ҫ�ü�����Կ�����ܣ�
* @param
* @param
* @param
* @return
**/
void decrpyt_2_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp) {
	// ���ɼ�����Կ
	AES_KEY aes_decrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_decrypt_key);

	// ����iv����
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");
	
	// ����num
	int encrypt_num = 0;

	// ����
	AES_ofb128_encrypt(dPtr_temp, dPtr_decrypt, decrypt_size_temp, &aes_decrypt_key, iv, &encrypt_num);

	// �ͷ��ڴ�ռ�
	free(key);
	free(iv);
}

/**
* �����ļ����� AES CFB��ʽ���ܣ�CFBģʽ��Ҫ�ü�����Կ�����ܣ�
* @param
* @param
* @param
* @return
**/
void decrpyt_3_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp) {
	// ���ɼ�����Կ
	AES_KEY aes_decrypt_key;
	unsigned char* key = str2hex("8cc72b05705d5c46f412af8cbed55aad");
	AES_set_encrypt_key(key, 128, &aes_decrypt_key);

	// ����iv��ʼ����
	unsigned char* iv = str2hex("667b02a85c61c786def4521b060265e8");

	// ����num����AES_cfb128_encrypt��Ĭ��Ϊ0
	int encrypt_num = 0;

	// ����
	AES_cfb128_encrypt(dPtr_temp, dPtr_decrypt, decrypt_size_temp, &aes_decrypt_key, iv, &encrypt_num, AES_DECRYPT);

	// �ͷ��ڴ�ռ�
	free(key);
	free(iv);
}