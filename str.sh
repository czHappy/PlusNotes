#!/bin/bash

# 使用重定向 < 将文件内容读入字符串
file_content=$(cat < file.txt)

# 输出字符串内容
echo "$file_content"

