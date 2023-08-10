hello="/your/new/path"
old_string="/opt/plusai/run/recording"

# 使用sed替换字符串并将结果写回文件
sed -i "s|$old_string|$hello|g" configx.txt

