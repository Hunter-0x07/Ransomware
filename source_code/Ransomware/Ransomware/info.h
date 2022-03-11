#include <stdio.h>
/**
* 保存检索到的文件的信息
**/
typedef struct Exist_file {
	char* file_name;	// 文件名称
	char* file_path;	// 文件路径
	int file_num;	// 检索到的所有文件的总数量
	//int origin_file_length; // 原始文件大小
	char* origin_file_hash;	// 原始文件的hash
	char* encrypt_file_hash;	// 加密文件的hash
} ExistFile;

/**
* 根据文件路径检索特定后缀文件
* @param	findExtension	要检索的文件路径数组
* @param	findExtensionLength	要检索的文件路径数组的长度
* @return	检索到的指向所有文件信息的结构体数组的指针
**/
extern ExistFile* retrieve(char** findExtension, int findExtensionLength);

/**
* 获取文件绝对路径
* @param	目录绝对路径
* @param	文件名称
* @return	文件绝对路径
**/
extern char* get_file_path(char* directory_path, char* file_name);

/**
* 获取文件大小
* @param	File指针
* @return	以单位字节为单位的文件大小
**/
extern int get_file_size(FILE* fp);

/**
* 打印字节
* @param
* @param
* @return
**/
extern void printf_buff(char* buff, int size);

/**
* 加密所有文件
* @param
* @return
**/
extern void encrypt_file(char* file_path, int method);

/**
* 文件复制函数	如：fcopy(fSource, 0, -1, fTarget, 0)：将原文件全部内容复制到目标文件
* @param	fSource	要复制的原文件
* @param	offsetSource	原文件的位置偏移（相对文件开头），也就是从哪里开始复制
* @param	len	要复制的内容长度，小于0表示复制offsetSource后边的所有内容
* @param	fTarget	目标文件，也就是将文件复制到哪里
* @param	offsetTarget	目标文件的位置偏移，也就是复制到目标文件的什么位置
* @return 成功复制的字节数
**/
extern long fcopy(FILE* fSource, long offsetSource, long len, FILE* fTarget, long offsetTarget);

/**
* 删除距离文件开头offset偏移量后的所有内容
* @param
* @param
* @param
* @return
**/
extern int fdelete(char* file_path, FILE* fp, long offset);

/**
* 解密所有文件
* @param
* @return
**/
extern void decrypt_file(char* file_path, int method);

/**
* 字符串转Hex
* @param
* @return
**/
extern unsigned char* str2hex(char* str);

/**
* SHA1算法生成文件HASH
* @param
* @return
**/
extern char* generate_hash(char* file_path);

/**
* 对比文件加密前后的hash
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
* AES ECB方式加密
* @param
* @param
* @param
* @return
**/
extern void encrypt_0_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size);

/**
* AES CBC方式加密
* @param
* @param
* @param
* @return
**/
extern void encrypt_1_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size);

/**
* AES OFB方式加密
* @param
* @param
* @param
* @return
**/
extern void encrypt_2_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size);

/**
* AES CFB方式加密
* @param
* @param
* @param
* @return
**/
extern void encrypt_3_buff(char* dPtr_temp, char* dPtr_encrypt, int encrypt_size);

/**
* 解密文件内容 AES ECB方式解密
* @param
* @param
* @param
* @return
**/
extern void decrpyt_0_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp);

/**
* 解密文件内容 AES CBC方式解密
* @param
* @param
* @param
* @return
**/
extern void decrpyt_1_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp);

/**
* 解密文件内容 AES OFB方式解密（OFB模式需要用加密密钥来解密）
* @param
* @param
* @param
* @return
**/
extern void decrpyt_2_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp);

/**
* 解密文件内容 AES CFB方式解密（CFB模式需要用加密密钥来解密）
* @param
* @param
* @param
* @return
**/
extern void decrpyt_3_buff(char* dPtr_temp, char* dPtr_decrypt, int decrypt_size_temp);