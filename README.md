## Ransomware简介
通过 C 语言调用 Openssl API 模拟勒索软件加密算法，对特定文件夹下特定后缀文件进行加密

## Ransomware.exe加解密使用方式说明：

```shell
# 加密例子如下：
.\Ransomware.exe -mode encrypt -method 3 -path E:\\CProjects\\TestEncrypt\\ -extension *.txt
# 解密例子如下：
.\Ransomware.exe encrypt -mode decrypt -method 3 -path E:\\CProjects\\TestEncrypt\\ -extension *.txt

# 参数说明：
-mode		用法：-mode encrypt 或 -mode decrypt 
				说明：指定加密还是解密
-method		用法：-method 0 
				说明：有4个选项 -> 0，1，2，3（分别代表四种加解密方式）
				采用 128 位密钥，支持AES ECB模式、AES CBC模式、AES OFB模式、AES CFB模式
-path		用法：-path E:\\CProjects\\TestEncrypt\\ 
				说明：需要检索的路径
-extension 
			用法：-extension *.txt,calc.exe,*.docx,*.pdf 
				说明：需要加解密的后缀，用逗号分割
```

## 示例：

- 对E:\\CProjects\\TestEncrypt\\目录下所有后缀为.txt、calc.exe、.docx的文件进行加密：

```powershell
.\Ransomware.exe -mode encrypt -method 3 -path E:\\CProjects\\TestEncrypt\\ -extension *.txt,calc.exe
```

#### 加密前：

![](https://github.com/Hunter-0x07/Ransomware/blob/main/png/test_1.png)

#### 加密后：

![](https://github.com/Hunter-0x07/Ransomware/blob/main/png/test_2.png)

![](https://github.com/Hunter-0x07/Ransomware/blob/main/png/test_3.png)

#### 解密：

```powershell
.\Ransomware.exe -mode decrypt -method 3 -path E:\\CProjects\\TestEncrypt\\ -extension *.txt,calc.exe
```

![](https://github.com/Hunter-0x07/Ransomware/blob/main/png/test_4.png)

![](https://github.com/Hunter-0x07/Ransomware/blob/main/png/test_5.png)

## 后续想法
- 支持更多加密算法，以及改程序bug

## License & copyright

Licensed under the [MIT License](LICENSE).