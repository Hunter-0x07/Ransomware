#include <stdio.h>
/**
* ������������ļ�����Ϣ
**/
typedef struct Exist_file {
	char* file_name;	// �ļ�����
	char* file_path;	// �ļ�·��
	int file_num;	// �������������ļ���������
	//int origin_file_length; // ԭʼ�ļ���С
	char* origin_file_hash;	// ԭʼ�ļ���hash
	char* encrypt_file_hash;	// �����ļ���hash
} ExistFile;

/**
* �����ļ�·�������ض���׺�ļ�
* @param	findExtension	Ҫ�������ļ�·������
* @param	findExtensionLength	Ҫ�������ļ�·������ĳ���
* @return	��������ָ�������ļ���Ϣ�Ľṹ�������ָ��
**/
extern ExistFile* retrieve(char** findExtension, int findExtensionLength);

/**
* ��ȡ�ļ�����·��
* @param	Ŀ¼����·��
* @param	�ļ�����
* @return	�ļ�����·��
**/
extern char* get_file_path(char* directory_path, char* file_name);

/**
* ��ȡ�ļ���С
* @param	Fileָ��
* @return	�Ե�λ�ֽ�Ϊ��λ���ļ���С
**/
extern int get_file_size(FILE* fp);

/**
* ��ӡ�ֽ�
* @param
* @param
* @return
**/
extern void printf_buff(char* buff, int size);

/**
* ���������ļ�
* @param
* @return
**/
extern void encrypt_file(char* file_path, int method);

/**
* �ļ����ƺ���	�磺fcopy(fSource, 0, -1, fTarget, 0)����ԭ�ļ�ȫ�����ݸ��Ƶ�Ŀ���ļ�
* @param	fSource	Ҫ���Ƶ�ԭ�ļ�
* @param	offsetSource	ԭ�ļ���λ��ƫ�ƣ�����ļ���ͷ����Ҳ���Ǵ����￪ʼ����
* @param	len	Ҫ���Ƶ����ݳ��ȣ�С��0��ʾ����offsetSource��ߵ���������
* @param	fTarget	Ŀ���ļ���Ҳ���ǽ��ļ����Ƶ�����
* @param	offsetTarget	Ŀ���ļ���λ��ƫ�ƣ�Ҳ���Ǹ��Ƶ�Ŀ���ļ���ʲôλ��
* @return �ɹ����Ƶ��ֽ���
**/
extern long fcopy(FILE* fSource, long offsetSource, long len, FILE* fTarget, long offsetTarget);

/**
* ɾ�������ļ���ͷoffsetƫ���������������
* @param
* @param
* @param
* @return
**/
extern int fdelete(char* file_path, FILE* fp, long offset);

/**
* ���������ļ�
* @param
* @return
**/
extern void decrypt_file(char* file_path, int method);

/**
* �ַ���תHex
* @param
* @return
**/
extern unsigned char* str2hex(char* str);

/**
* SHA1�㷨�����ļ�HASH
* @param
* @return
**/
extern char* generate_hash(char* file_path);

/**
* �Ա��ļ�����ǰ���hash
* @param
* @param
* @return
**/
extern int compare_hash(char* origin_file_hash, char* encrypt_file_hash);

/**
*
**/
extern char** path_concat_ext(char* extension, char* path);

/**
* AES ECB��ʽ����
* @param
* @param
* @param
* @return
**/
extern void encrypt_0_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size);

/**
* AES CBC��ʽ����
* @param
* @param
* @param
* @return
**/
extern void encrypt_1_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size);

/**
* AES OFB��ʽ����
* @param
* @param
* @param
* @return
**/
extern void encrypt_2_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size);

/**
* AES CFB��ʽ����
* @param
* @param
* @param
* @return
**/
extern void encrypt_3_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size);

/**
* �����ļ����� AES ECB��ʽ����
* @param
* @param
* @param
* @return
**/
extern void decrpyt_0_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp);

/**
* �����ļ����� AES CBC��ʽ����
* @param
* @param
* @param
* @return
**/
extern void decrpyt_1_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp);

/**
* �����ļ����� AES OFB��ʽ���ܣ�OFBģʽ��Ҫ�ü�����Կ�����ܣ�
* @param
* @param
* @param
* @return
**/
extern void decrpyt_2_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp);

/**
* �����ļ����� AES CFB��ʽ���ܣ�CFBģʽ��Ҫ�ü�����Կ�����ܣ�
* @param
* @param
* @param
* @return
**/
extern void decrpyt_3_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp);