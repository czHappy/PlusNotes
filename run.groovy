import groovy.lang.GroovyShell

// 创建 GroovyShell 对象
GroovyShell shell = new GroovyShell()

// 使用 parse() 方法解析 Groovy 脚本文件

def code = shell.parse(new File('script.groovy'))
def result = code("/opt/plusai/setup.sh")
println result
assert result == "/opt/plusai"