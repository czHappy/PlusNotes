
## 2023.5.5
### Ubuntu 22.04 中文输入法
- Setting->Regin&Language->Manage Installed Languages->Install/Remove Languages... 勾选简体中文，下载
- sudo apt install ibus-pinyin 下载ibus拼音
- Keyboard->Input Sources 点击+，双击Chinese，选择Chinese(Intelliget Pinyin)
- Restart
- 切换中英文可以使用Win+whitespace

### Ubuntu 22.04 安装deb包
- sudo dpkg -i code_1.78.0-1683145611_amd64.deb
- code #启动vscode

### ubuntu界面美化
- 安装oh-my-sh
    ```bash
    sudo apt install git # 安装git
    sudo apt install zsh # 安装zsh
    sh -c "$(wget https://raw.github.com/ohmyzsh/ohmyzsh/master/tools/install.sh -O -)" # 安装ohmysh
    git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting # 安装语法高亮插件
    git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions # 安装自动补全插件
    plugins=(git z extract zsh-syntax-highlighting zsh-autosuggestions) # 添加插件到～/.zshrc
    source ~/.zshrc # zshrc生效
    ```
- 配置p10k主题
    ```bash
    git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ${ZSH_CUSTOM:-$HOME/.oh-my-zsh/custom}/themes/powerlevel10k
    ZSH_THEME="powerlevel10k/powerlevel10k" # 修改～/.zshrc
    ```
- 在vscode中设置terminal为zsh
  - 在设置里面搜索terminal.integrated，设置"terminal.integrated.defaultProfile.linux": "zsh"
- 一键配置脚本
```bash
#!/bin/bash

# 清理
sudo apt remove zsh -y
rm -rf ~/.zshrc ~/.zsh* ~/.oh-my-zsh*

# 安装Zsh和必要的依赖
sudo apt update -y
sudo apt install zsh git curl -y

# 安装Oh My Zsh
yes | sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
# 安装Powerlevel10k主题以及必要插件
git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ~/.oh-my-zsh/themes/powerlevel10k
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting # 安装语法高亮插件
git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions # 安装自动补全插件

# 更改zshrc配置
search_pattern='ZSH_THEME="robbyrussell"'
replace_string='ZSH_THEME="powerlevel10k/powerlevel10k"'
replace_string=$(echo $replace_string | sed -e 's/\//\\\//g')
sed -i "s/${search_pattern}/${replace_string}/g" ~/.zshrc

search_pattern='plugins=(git)'
replace_string='plugins=(git zsh-syntax-highlighting zsh-autosuggestions)'
sed -i "s/${search_pattern}/${replace_string}/g" ~/.zshrc

# 启动p10k配置
zsh
```
### /etc/host作用
- /etc/hosts 文件是一个计算机系统中的一个本地主机名解析文件。它通常被用于在计算机上手动指定 IP 地址和主机名的映射关系。当在计算机上访问一个网站时，计算机通常需要将主机名（如 www.example.com）转换为一个 IP 地址，以便能够与该网站建立连接。这通常通过DNS进行，但是在某些情况下，可能需要手动指定这些映射关系，以避免需要使用 DNS 进行解析，或者为了将某个主机名指向一个特定的 IP 地址。

- 在Linux和其他类Unix操作系统上，/etc/hosts 文件被用于这个目的。例如，可以在该文件中添加一行 127.0.0.1 localhost，以指定 localhost 主机名解析到本地回环地址 127.0.0.1。这样，在浏览器中输入 http://localhost 时，计算机会自动将其解析为 127.0.0.1，从而连接到本地主机。




## 2023.5.6 
### set指令
- set -x
  - 默认情况下，脚本执行后，屏幕只显示运行结果，没有其他内容。如果多个命令连续执行，它们的运行结果就会连续输出。有时会分不清，某一段内容是什么命令产生的。
- set -e
  - 如果脚本里面有运行失败的命令（返回值非0），Bash 默认会继续执行后面的命令。
  - set -e使得脚本只要发生错误，就终止执行。
- set -u
  - 执行脚本的时候，如果遇到不存在的变量，Bash 默认忽略它。
  - set -u就用来改变这种行为。脚本在头部加上它，遇到不存在的变量就会报错，并停止执行。


### bash -c
- -c
```
/bin/sh -c "./test.sh 1 2" # 使用sh启动一个由字符串表示的命令
CMD ["/bin/sh", "-c", "/start.sh ${SELECTIVE_FLAVOR}"] # dockerfile中使用CMD指令运行带参数的shell脚本
```

### 中间件
- 定义：中间件（英语：Middleware），又译中间件、中介层，是一类提供系统软件和应用软件之间连接、便于软件各部件之间的沟通的软件，应用软件可以借助中间件在不同的技术架构之间共享信息与资源。中间件位于客户机服务器的操作系统之上，管理着计算资源和网络通信。 -- 维基百科
- 性质：中间件是软件
- 作用层级：系统软件和应用软件之间、软件各部件之间；管理客户机与系统软件之间的计算资源和网络通信。
- 服务对象：中间件为应用软件服务，应用软件为最终用户服务，最终用户并不直接使用中间件
- 一些中间件
  - 消息中间件：支持在分布式系统之间发送和接收消息的软件。 如 Apache kafka, Apache RabbitMQ, NSQ, 阿里孵化开源的 Apache RocketMQ
  - 缓存服务中间件: 分布式的高速数据存储层，一般是内存存储。如 阿里 Tair，业界的 Redis
  - 任务调度：分布式环境下提供定时、任务编排、分布式跑批等功能的系统。如 阿里 SchedulerX
  - ...

### 消息队列
- MQ(Message Queue)
- 把数据放到消息队列叫做生产者，从消息队列里边取数据叫做消费者。生产者将数据放到消息队列中，消息队列有数据了，主动叫消费者去拿(俗称push)。消费者不断去轮询消息队列，看看有没有新的数据，如果有就消费(俗称pull)

### ZooKeeper
- ZooKeeper主要服务于分布式系统，可以用ZooKeeper来做：统一配置管理、统一命名服务、分布式锁、集群管理。
- 使用分布式系统就无法避免对节点管理的问题(需要实时感知节点的状态、对节点进行统一管理等等)，而由于这些问题处理起来可能相对麻烦和提高了系统的复杂性，ZooKeeper作为一个能够通用解决这些问题的中间件就应运而生了。
- ZooKeeper 是C/S结构(分成客户端和服务端),数据结构，跟Unix文件系统非常类似，可以看做是一颗树，每个节点叫做ZNode。每一个节点可以通过路径来标识.
- [参阅](https://mp.weixin.qq.com/s?__biz=MzI4Njg5MDA5NA==&mid=2247485115&idx=1&sn=5d269f40f820c82b460993669ca6242e&chksm=ebd747badca0ceac9953f82e08b1d1a49498ebd4af77ec5d628a0682bb9f0ac5ab347411f654&token=1741918942&lang=zh_CN#rd)

### Kafka
- 使用消息队列不可能是单机的（必然是分布式or集群）
  - Kafka天然是分布式的，往一个topic丢数据，实际上就是往多个broker的partition存储数据
- 数据写到消息队列，可能会存在数据丢失问题，数据在消息队列需要持久化(磁盘？数据库？Redis？分布式文件系统？)
  - Kafka会将partition以消息日志的方式(落磁盘)存储起来，通过 顺序访问IO和缓存(等到一定的量或时间)才真正把数据写到磁盘上，来提高速度。
- 想要保证消息（数据）是有序的，怎么做？
  - Kafka会将数据写到partition，单个partition的写入是有顺序的。如果要保证全局有序，那只能写入一个partition中。如果要消费也有序，消费者也只能有一个。
- 为什么在消息队列中重复消费了数据
  - 凡是分布式就无法避免网络抖动/机器宕机等问题的发生，很有可能消费者A读取了数据，还没来得及消费，就挂掉了。Zookeeper发现消费者A挂了，让消费者B去消费原本消费者A的分区，等消费者A重连的时候，发现已经重复消费同一条数据了。(各种各样的情况，消费者超时等等都有可能...)如果业务上不允许重复消费的问题，最好消费者那端做业务上的校验（如果已经消费过了，就不消费了）


## 2023.5.8
### Ubuntu设置快捷键
- Setting->Keyboard->Keyboard Shortcuts->view and customize shortcuts
- 可以设置系统内置的快捷键
- 最下面可以定制自己的快捷键
  
### no basic auth credential
- docker pull docker.plusai.co:5050/plusai/selective_data_monitor:209
  - Error response from daemon: Head "https://docker.plusai.co:5050/v2/plusai/selective_data_monitor/manifests/209": no basic auth credentials
- docker login 命令用于登陆到一个 Docker 镜像仓库，如果未指定镜像仓库地址，默认为官方仓库 Docker Hub
  - docker login -u username -p password server_name
  - Docker 会将 token 存储在 ~/.docker/config.json 文件中，从而作为拉取私有镜像的凭证。（也可以之直接将别人的config.json内容复制到自己的当中）
```json
{
	"auths": {
		"docker.plusai.co:5050": {
			"auth": "c3otZG9ja2VyOkFhMTIzNDU2"
		},
    "dist:5000": {
            "auth": "amVua2luczo0aXVzb2U2dno1MXR3NmJ0"
    },
    "dist-cn:5000": {
            "auth": "amVua2luczo0aXVzb2U2dno1MXR3NmJ0"
    },
    "bj-docker.plusai.co:5050": {
            "auth": "c3otZG9ja2VyOkFhMTIzNDU2"
    }
	}
}
```

### /etc/resolv.conf
- /etc/resolv.conf是DNS客户机的配置文件，用于设置DNS服务器的IP地址及DNS域名，还包含了主机的域名搜索顺序。
- 键字主要有4个，分别为：
  - nameserver：定义DNS服务器的IP地址
  - domain：定义本地域名
  - search：定义域名的搜索列表
  - sortlist：对返回的域名进行排序
- 先查看它是否是一个软链接，如果是的话修改其内容可以将其删掉重新创建一个同名文件然后再写入内容。

### POSIX
定义：
  - 可移植操作系统接口（Portable Operating System Interface of UNIX，缩写为 POSIX ）
  - POSIX是IEEE为要在各种UNIX操作系统上运行的软件而定义的一系列API标准的总称，其正式称呼为IEEE 1003，而国际标准名称为ISO/IEC 9945。
- 历史：
  - 1974年，贝尔实验室正式对外发布Unix。贝尔实验室以慷慨的条件向学校提供源代码，好些独立开发的与Unix基本兼容但又不完全兼容的OS，通称Unix-like OS。为了提高兼容性和应用程序的可移植性，阻止这种趋势， IEEE(电气和电子工程师协会)开始努力标准化Unix的开发，后来由 Richard Stallman命名为“Posix”。这套标准涵盖了很多方面，比如Unix系统调用的C语言接口、shell程序和工具、线程及网络编程。
- 遵从
  - Unix和Linux遵从这套标准
  - 苹果的操作系统也是Unix-based的。
  - Windows为了把Unix用户拉到Windows阵营，被迫支持POSIX。
- 可移植性
  - 系统调用是通向操作系统本身的接口，是面向底层硬件的。通过系统调用，可以使得用户态运行的进程与硬件设备(如CPU、磁盘、打印机等)进行交互，是操作系统留给应用程序的一个接口。
  - 库函数（Library function）是把函数放到库里，供别人使用的一种方式。方法是把一些常用到的函数编完放到一个文件里，供不同的人进行调用。一般放在.lib文件中。库函数调用则是面向应用开发的，库函数可分为两类，一类是C语言标准规定的库函数，一类是编译器特定的库函数。由于版权原因，库函数的源代码一般是不可见的，但在头文件中你可以看到它对外的接口。
    - glibc 是 Linux 下使用的开源的标准 C 库，它是 GNU 发布的 libc 库，即运行时库。这些基本函数都是被标准化了的，而且这些函数通常都是用汇编直接实现的。
    - glibc 为程序员提供丰富的 API（Application Programming Interface），这些API都是遵循POSIX标准的，API的函数名，返回值，参数类型等都必须按照POSIX标准来定义。
    -  库函数调用与系统无关，不同的系统，调用库函数，库函数会调用不同的底层函数实现，因此可移植性好。
  - 不能移植的
    - 基于各种操作系统平台不同，应用程序在二级制级别是不能直接移植的。
    - 在API层面上由于各个操作系统的命名规范、系统调用等自身原因，在API层面上实现可移植也是不大可能的
  - 可以移植的
    - 在各个平台下，我们默认C标准库中的函数都是一样的，这样基本可以实现可移植，C库封装了操作系统API在其内部的实现细节。因此，C语言提供了我们在代码级的可移植性，即这种可移植是通过C语言这个中间层来完成的。但是在不同的平台下，仍需要重新编译。
  - 操作系统为了考虑实现的难度和管理的方便，它只提供一少部分的系统调用，这些系统调用一般都是由C和汇编混合编写实现的，其接口用C来定义，而具体的实现则是汇编，这样的好处就是执行效率高，而且，极大的方便了上层调用
- printf函数执行过程
  - 当应用程序调用printf()函数时，printf函数会调用C库中的printf，继而调用C库中的write，C库最后调用内核的write()。
  - 程序状态切换：用户态–>系统调用–>内核态–>返回用户态

### RSA算法
- 对称加密算法（1976年以前）
  - 甲方选择某一种加密规则，对信息进行加密；
  - 乙方使用同一种规则，对信息进行解密。
  - 这种加密模式有一个最大弱点：甲方必须把加密规则告诉乙方，否则无法解密。保存和传递密钥，就成了最头疼的问题。

- 非对称加密算法
  - 加密和解密可以使用不同的规则，只要这两种规则之间存在某种对应关系即可，这样就避免了直接传递密钥。
    - 乙方生成两把密钥（公钥和私钥）。公钥是公开的，任何人都可以获得，私钥则是保密的。
    - 甲方获取乙方的公钥，然后用它对信息加密。
    - 乙方得到加密后的信息，用私钥解密。
- RSA算法
  - 获得公钥和私钥步骤
    - 选择两个大质数p,q, n = pq
    - e和欧拉函数$\phi_{n}$互质，计算e
    - 计算e对于$\phi_{n}$的模反元素d。
    - 将n和e封装成公钥，n和d封装成私钥。 
    - n和e封装成公钥，n和d封装成私钥。 
  - 只有知道e和φ(n)，才能算出d => 只有知道p和q，才能算出φ(n) => 只有将n因数分解，才能算出p和q。
  - 对极大整数n做因数分解的难度决定了RSA算法的可靠性


### GPG 
- GnuPG软件（简称GPG），它是目前最流行、最好用的加密工具之一，使用RSA对信息加密和解密。
- apt-key命令
  - apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
    - 用于将指定的公钥添加到系统的密钥环中，以便验证从相应的软件包源安装的软件包。
    - apt-key：这是一个命令行工具，用于管理软件包源的密钥。
    - adv：这是apt-key命令的选项之一，它表示执行高级操作，即添加公钥到密钥环中。
    - --keyserver hkp://keyserver.ubuntu.com:80：这是指定公钥服务器的选项。hkp是协议，keyserver.ubuntu.com是公钥服务器的地址，80是端口号。这个选项告诉apt-key从指定的公钥服务器下载公钥。
    - --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654：这是指定要添加到密钥环中的公钥的选项。C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654是公钥的ID。
 
  - 手动添加apt-key
    - 在官网根据公钥ID下载公钥文件：https://keyserver.ubuntu.com/
    - sudo apt-key add <path_to_key_file> # 添加本地公钥
    - apt-key list # 检查

- 验证软件包完整性：以kafka为例
  - https://dlcdn.apache.org/kafka/3.4.0/ 下载软件包和对应的SHA512文件
  ```sh
    gpg --print-md SHA256 kafka_2.13-3.4.0.tgz
    # 然后对比输出和HA512文件里的内容是否一致
  ```
## 2023.5.9
### QNX
- 内核
  - 定义：内核，是一个操作系统的核心。是基于硬件的第一层软件扩充，提供操作系统的最基本的功能，是操作系统工作的基础，它负责管理系统的进程、内存、设备驱动程序、文件和网络系统，决定着系统的性能和稳定性。从抽象的角度来看，内核其实就是计算机资源的管理者，资源包括软件资源和硬件资源。
  - 种类
    - 宏内核
      - 定义：宏内核简单理解就是把所有的基础功能都整合在一起。我们可以把进程管理、管理内存、管理硬盘、管理各种I/O设备……这些功能看作一个个模块。在宏内核中，这些模块都是集成在一起的，运行在内核进程中，只有处于内核态下才能运行。
      - 工作流程：函数=>系统调用=>切换到内核态=>执行内核函数=>返回结果=>切换用户态
      - 优缺点：性能十分好，像Linux就是传统的宏内核结构，其性能极高，但其缺点也很明显，就是其耦合度高，一旦其中一个模块出现问题，其他所有的模块都可能会受到影响。
    - 微内核
      - 定义：提倡内核中的功能模块尽可能的少。内核只提供最核心的功能，比如任务调度，中断处理等等。其他实际的模块功能如进程管理、存储器管理、文件管理……这些则被移出内核，变成一个个服务进程，和用户进程同等级，只是它们是一种特殊的用户进程。
      - 工作流程：以内存分配为例子。应用程序首先会发送内存分配的消息，这个发送消息的接口函数是由微内核提供的。此时CPU切换到内核态，开始执行该函数的代码，微内核的代码会使当前进程停止运行，并将消息发送给内存管理的服务进程。内存管理服务进程收到该消息后，就会分配一块内存，并且也会通过消息的形式将分配的内存块的地址返回给内核。微内核再将该消息返回给发送内存分配消息的应用程序。此时CPU切换到用户态，应用程序会得到返回的内存块首地址，并开始使用该内存。
      - 优缺点：对比宏内核中，微内核结构主要是多了接收和发送消息的这一过程，所以微内核结构的性能会差不少。但微内核降低了耦合度，模块移除内核后后使得即使某一个模块出现问题，只要重启这个模块的进程即可，不会影响到其他模块，更加的稳定。并且微内核有相当好的伸缩性、扩展性，因为模块功能只是一个进程，可以随时增加或减少系统功能。

- QNX
  - QNX是是基于POSIX规范的。
  - QNX的核心提供4种服务：进程调度/进程间通信/底层网络通信/中断处理。因此QNX内核非常的精致小巧，比传统的宏内核（Linux）系统可靠性更高。
  - QNX采用微内核架构
    ![](imgs/kernel.png)
  - QNX系统架构由微内核一组协作的系统服务进程组成
    ![](imgs/qnx_arch.png)
  - 进程管理
    - 在QNX Neutrino中，微内核与进程管理器一起组成procnto模块，所有运行时系统都需要这个模块。
      - 进程管理，管理进程的创建、销毁、属性处理（用户ID和组ID）等；
      - 内存管理，管理一系列的内存保护功能、共享库、进程间POSIX共享内存等；
      - 路径名管理，管理资源管理器可能附加到的路径名空间；
        ![](imgs/qnx_mm.png)  
  - 资源管理器
    ![](imgs/qnx_resource_m.png)
  - 线程调度
    - 线程状态
      ![](imgs/qnx_thread.png)
    - 抢占式调度
      ![](imgs/qnx_scheduler.png)
  - QNX和linux的区别
    ![](imgs/qnx_linux_diff.png)
  - QNX总结
    - 高效率：内核小巧，运行极快；可任意裁剪成适合自己的最小方案；
    - 易操作：应用程序接口完全符合 POSIX 标准，Linux用户可快速上手QNX。
    - 实时性：多种基于优先级的抢占式调度算法让QNX 能够实现实时任务调度和预测任务响应时间，确保不论系统负载如何，高优先级任务总是能按时完成。
    - 微内核：地址空间隔离，保证任何一个部分出了错误不会影响其他部分和内核， 并且可自动重启恢复。

### Linux后台执行
- nohup ./bin config/server.properties >/dev/null 2>&1 &
  ```sh
  bin/kafka-server-start.sh config/server.properties # 会阻塞住，不能继续在这个terminal里执行命令
  nohup bin/kafka-server-start.sh config/server.properties >/dev/null 2>&1 &
  ```

## 2023.5.10
### postgre数据库迁移
- 使用dbeaver-ce导出数据表，只能导出较小的，否则速度慢且容易出现内存不足

- pg_dump
```sh
# 导出sql
pg_dump testdb > /tmp/testdb.sql
# 迁移数据库
create 
```

## 2023.5.11
### git pull/push要求输入密码
- ssh-key配置正常，ssh方式的git clone正常
- 解决
```sh
# vi ~/.gitconf
[user]
        email = zhen.cheng@plus.ai
        name = chengzhen
将name改成和远程github用户名一致
```


## 2023.5.16
### git 配置多个账户
- 公司
  - github地址： github-cn.plus.ai/
  - 密钥：~/.ssh/id_ed25519
- 个人
  - github地址：github.com
  - 密钥：~/.ssh/id_rsa
- 修改~/.ssh/config
  ```sh
  Host github-cn.plus.ai
    HostName github-cn.plus.ai
    User chengzhen
    IdentityFile ~/.ssh/id_ed25519
  Host github.com
    HostName github.com
    User czHappy
    IdentityFile ~/.ssh/id_rsa
  ```
- 在不同账户的仓库下设置用户名和邮箱

```sh
# 针特定仓库局部设置 
git config user.name "Your Name"
git config user.email "your.email@example.com"

# 全局默认设置
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

## 2023.5.22
### codechecker
- codechecker
```
# 给定compile_commands.json， 使用clang-tidy进行静态代码检查
# 输出报告为reports，过滤规则遵照skipfile
CodeChecker analyze ./build/latest/compile_commands.json --enable sensitive --analyzers clang-tidy --output ./reports --skip skipfile

# 解析reports报告，导出html，文件夹名称为reports_html
CodeChecker parse --export html --output ./reports_html ./reports
```
- skipfile
  - 对每个compile_commands.json中的文件对skipfile中的规则进行匹配，按照从上到下的顺序，匹配成功立即返回
```sh
-*/build/* # 如果是build目录下的文件，忽略，不再往下匹配
+*/recorder/* # 如果是recorder目录下的文件，进行分析，不再往下匹配
-* # 忽略，不再往下匹配
```
## 2023.5.23
### tr
```sh
tr '[:upper:]' '[:lower:]' #把大写转成小写
echo "AAA" | tr '[:upper:]' '[:lower:]' # aaa
```
### /usr/bin/time + command
```sh
/usr/bin/time --format "buildtime: real=%e user=%U sys=%S [ %C ]" echo "hello" 
# hello
# buildtime: real=0.00 user=0.00 sys=0.00 [ echo hello ]
```
### tee
```
$(TIME_COMMAND) $(BUILD_COMMAND) -C "${BUILD_DIR}/${DEBUG_DIR}" 2>&1 && (echo "debug build SUCCEEDED") || (echo "debug build FAILED"; exit 1) | tee ${BUILD_DIR}/${DEBUG_DIR}/build-`date +%Y%m%dT%H%M%S`.log
```

## 2023.5.56
### git 同步上游
```sh
# 首先fork 仓库
# 添加源分支 URL
git remote add upstream [源项目 URL]
# 查看是否关联上上游仓库
git remote -v
# 从上游仓库源分支获取最新的代码
git fetch upstream
# 当前分支切换到本地主分支
git checkout master
# 合并上游master分支到当前分支
git merge upstream/master
# Push 到 Fork 分支
git push
```

### git ssh密钥
- ls -al ~/.ssh 检查密钥是否存在
- ssh-keygen -t rsa -C "271xxxxxx@qq.com" 生成密钥对，包括私钥 公钥 密码可以输入也可以不输入
- eval $(ssh-agent -s) 首先确保ssh-agent正常工作
- ssh-add ~/.ssh/id_rsa 直接将私钥id_rsa添加到ssh代理中，跟windows不同的是不需要修改后缀为.ppk
- vim /root/.ssh/id_rsa.pub 打开公钥文件复制全文将公钥id_rsa.pub添加到你的github或者gitlab等仓库中
- 登录仓库，用户setting -> SSH key 将公钥粘贴进去，起个容易识别的名字 title

## Jenkinsfile
### pipeline
- agent: 定义了pipeline或者stage内执行环境
  - any: 任何一个可用的执行器
  - none: 无需分配
  - label 'xxx': 以xxx为标签的执行器
- environment: 定义一些自定义环境变量
  - ${env.WORKSPACE} 内置环境变量 当前工作空间目录
- post: 在 Pipeline 结束的时候运行， 所以我们可以添加通知或者其他的步骤去完成清理、通知或者其他的 Pipeline 结束任务。
- options
```Jenkinsfile
pipeline {
    agent any
    environment {
        DISABLE_AUTH = 'true'
        DB_ENGINE    = 'sqlite'
    }
    options {
      buildDiscarder(logRotator(daysToKeepStr: '180')) # 设置构建过程中保留的构建记录数量
      skipDefaultCheckout() # 跳过默认的代码检出步骤。
      timestamps() # 在构建输出中添加时间戳。用于记录构建的开始和结束时间。
      parallelsAlwaysFailFast() # 一旦有一个步骤失败，整个并行块将不再等待其他步骤的完成，而是立即停止执行。
    }
    parameters {
      string(name: 'USERNAME', defaultValue: 'guest', description: 'Enter your username')
      booleanParam(name: 'DEBUG_MODE', defaultValue: false, description: 'Enable debug mode')
    }
    stages {
      stage('Test') {
          steps {
              echo "Username: ${params.USERNAME}"
              echo "Debug mode enabled: ${params.DEBUG_MODE}"
              sh 'printenv'
              sh 'echo "Fail!"; exit 1'
          }
      }
    }
    post {
        always {
            echo 'This will always run'
        }
        success {
            echo 'This will run only if successful'
        }
        failure {
            echo 'This will run only if failed'
        }
        unstable {
            echo 'This will run only if the run was marked as unstable'
        }
        changed {
            echo 'This will run only if the state of the Pipeline has changed'
            echo 'For example, if the Pipeline was previously failing but is now successful'
        }
    }
}
```
### stage
- stage之间是隔离的，可以指定不同的执行器
- 上一个stage在WORKSPACE中所产生的文件在下一个stage中是可见的
- 上一个stage所build的docker镜像在下一个stage中是可见的 
  - runWithImage(img_name, command)
```
stages{
  stage("Checkout Code") {
      steps {
          checkoutScm() # 检出代码，即将代码拉到workspace下
      }
  }
  stage("Static code check"){
      when{
          environment name: 'OS_TYPE_ID', value: 'x86'
      }
      steps{
          script {
              def img_name = getDockerImageTag("event_recorder")
              def command = """
                  sudo apt-get update && sudo apt-get -y install clang clang-tidy cppcheck build-essential curl gcc-multilib \
                  git python3.8 python3-dev python3-venv python3-setuptools
                  virtualenv -p /usr/bin/python3.8 ${env.WORKSPACE}/.venv38
                  . ${env.WORKSPACE}/.venv38/bin/activate
                  pip3 install codechecker
                  CodeChecker analyze ./build/latest/compile_commands.json --enable sensitive --analyzers clang-tidy --output ./reports --skip skipfile
                  echo "Going to run CodeChecker parse..."
                  CodeChecker parse --export html --output ./reports_html ./reports || echo "Static code analysis complete!"
                  deactivate
              """
              runWithImage(img_name, command)
              publishHTML([allowMissing: true, reportDir: "reports_html",
                                          reportFiles: 'index.html',
                                          reportName: "Static Code Check Report"])
          }
      }
  }
}
```


### 内置函数
- populateEnv
  - 在 Jenkins Pipeline 中，环境变量是通过 environment 部分定义的全局变量，可以在 Pipeline 的任何步骤中使用。但在某些情况下，需要在构建过程中动态加载当前构建的环境变量，并使其在 Pipeline 的后续步骤中可用。这时就可以使用 populateEnv 步骤来实现
- publishHTML
  - 用于将HTML报告或文档发布到Jenkins构建的页面,在左侧导航栏可以找到名为reportName的链接
```
  publishHTML(target: [
      allowMissing: true, # 如果设置为true，则允许在构建期间找不到HTML文件时继续构建，默认为false。
      alwaysLinkToLastBuild: true, # 如果设置为true，则在构建页面上的侧边栏中始终显示链接到最后一次构建的报告，默认为false
      keepAll: true, # 如果设置为true，则会保留构建历史中所有构建的报告，默认为false，只保留最后一次构建的报告
      reportDir: 'path/to/html/files', # HTML报告文件所在的目录路径
      reportFiles: 'index.html', # 要发布的HTML文件的名称或匹配模式，支持通配符
      reportName: 'My HTML Report'# HTML报告在Jenkins构建页面上显示的名称
  ])

```

### PR merge追踪
- 一个job完成后会有触发这个job的commit记录,可以与repo中的commit 哈希值进行比对。

```
# http://jenkins-cn/job/edr_monitor/job/master/386/
Revision: a027b2713c7f7c4bbe44a37d816eaa32298573e7
Repository: git@github-cn.plus.ai:plusai/edr_monitor.git
master
# see https://github-cn.plus.ai/PlusAI/edr_monitor/commits/master
```
## docker
- 参阅https://yeasy.gitbook.io/docker_practice/
- dockerfile
- docker-compose

### docker example
```sh
# download project
git clone https://github.com/jakewright/tutorials.git
cd docker
cd 01-getting-started
# make image
sudo docker build -t hello-world .
# run container
sudo docker run -p 80:80 -v /home/plusai/Documents/tutorials/docker/01-getting-started/src:/var/www/html hello-world
# install docker-compose
docker-compose -v
sudo apt  install docker-compose
docker-compose -version

# cd 02-docker-compose, run docker-compose
sudo docker-compose up
# get two runing containers,ctrl-c后停止

sudo docker ps -a
sudo docker container start 7d64073e69ea
sudo docker container start 9051b76dd5a3
sudo docker ps
sudo docker stop 7d64073e69ea
sudo docker stop 9051b76dd5a3
sudo docker ps

# 镜像推送到仓库
docker tag IMAGE_ID docker.plusai.co:5050/plusai-l4e-phase1-p1.1/selective_data_monitor:latest # 给IMAGE_ID镜像打个tag
docker push docker.plusai.co:5050/plusai-l4e-phase1-p1.1/selective_data_monitor:latest # 推送镜像
docker pull docker.plusai.co:5050/plusai-l4e-phase1-p1.1/selective_data_monitor:latest # 拉取镜像
```

### 配置docker用户组
```bash
cat /etc/group | grep docker # 查找 docker 组，确认其是否存在
groups # 列出自己的用户组，确认自己在不在 docker 组中

# 如果 docker 组不存在，则添加之：
sudo groupadd docker

# 将当前用户添加到 docker 组
sudo gpasswd -a ${USER} docker

# 重启服务
sudo service docker restart

# 切换一下用户组（刷新缓存）
newgrp - docker;
newgrp - `groups ${USER} | cut -d' ' -f1`; # TODO：必须逐行执行，不知道为什么，批量执行时第二条不会生效
# 或者，注销并重新登录
pkill X
```

### 复用host ssh key
```
docker run -itd --name drive-cz -w /home/plusai/workspace -v /home/plusai/workspace:/home/plusai/workspace:rw,z -v /home/plusai/.ssh:/home/plusai/.ssh docker.plusai.co:5050/plusai/drive:latest bash

# 在docker中执行
eval $(ssh-agent -s)
ssh-add id_rsa
ssh-add id_ed25519
```

## event-recorder
### brain
- 以mgmt_data为例
- ROSEventRecorderProgram
  - init
    - init_subscribers(_brain, subs, *_node_handle)
      - ingest
      - doIngest
        - processMessage
          - processMessageForDestination
            - fc->collect(topic, last)

## ibox-service
- ServiceWorker Run()
  - connection_handler_ InitScheduler InitStorage
  - socket_server_ AsyncAccept(connection_handler_)
    - async_accept -> HandleAccept ->SessionStart(while true)
      - socket_->async_read_some ->OnSockRead
        - command_handler.Process
  

## ROS Plusai
### ROSProgram plusai
- common/common/src/base/program.cpp
- run()
  - init_ros
  - set_gflags_from_rosparams
  - init_gflags
  - setup_glog_logging
  - init_ros_logging
  - setup_metrics
  - _scheduler
  - init_ipc
  - **init** need override
  - _run_publisher ipc::advertise
  - **go()** need override
  - cleanup()
  - gameover
## protobuf

### 定义
- 序列化和反序列化的message

### demo
- person.proto
```protobuf
syntax = "proto3";

message Person {
    string name = 1;
    int32 age = 2;
}

```
- main.cpp

```c++
#include <iostream>
#include <fstream>
#include "person.pb.h"

void WritePersonToFile(const Person& person, const std::string& filename) {
    std::ofstream output(filename, std::ios::binary | std::ios::trunc);
    if (!person.SerializeToOstream(&output)) {
        std::cerr << "Failed to write person to file." << std::endl;
    }
}

void ReadPersonFromFile(Person& person, const std::string& filename) {
    std::ifstream input(filename, std::ios::binary);
    if (!person.ParseFromIstream(&input)) {
        std::cerr << "Failed to read person from file." << std::endl;
    }
}

int main() {
    Person person;
    person.set_name("John Doe");
    person.set_age(30);

    // 将消息序列化并写入文件
    WritePersonToFile(person, "person.dat");

    // 从文件中读取并反序列化消息
    Person loadedPerson;
    ReadPersonFromFile(loadedPerson, "person.dat");

    // 打印读取到的消息
    std::cout << "Name: " << loadedPerson.name() << std::endl;
    std::cout << "Age: " << loadedPerson.age() << std::endl;

    return 0;
}

```
- CMakeLists.txt
```makefile
cmake_minimum_required(VERSION 3.10)
project(protobuf_example)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 寻找 Protocol Buffers 包
find_package(Protobuf REQUIRED)

# 生成 Protobuf C++ 代码
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS person.proto)

# 添加可执行文件
add_executable(main main.cpp ${PROTO_SRCS} ${PROTO_HDRS})

# 链接 Protocol Buffers 库
target_link_libraries(main PRIVATE ${Protobuf_LIBRARIES})

```
- 编译构建
```
apt-get install libprotobuf-dev protobuf-compiler #安装protobuf
mkdir build
cmake ..
make
```

### python proto
- selective server生成的proto文件
```sh
find /opt/plusai -name "*.py" | grep pb | grep mgmt
vim /opt/plusai/lib/python/event_recorder/mgmt_message_pb2.py # 查看是否包含字段
dpkg -l | grep plusai-common-pro # 查看版本 然后进入common_protobuf里的master ci对比版本号

```

### clang-format
- vscode安装C++ 扩展插件
- 打开首选项设置 Ctrl + ,
- 搜索format 勾选format on save，之后每次保存文件之后会自动进行格式刷

### kafka
### systemd

### core dump
- 修改limit
  - ulimit -c 查询
  - ulimit -c unlimited
- 修改默认路径z`
  - 临时修改：sudo echo ‘/var/log/%e.core.%p’ > /proc/sys/kernel/core_pattern
  - 永久修改：sudo /sbin/sysctl -w kernel.core_pattern=/var/log/%e.core.%p
- docker上生成core文件
  - 首先按照上面方法在宿主机上设置core文件配置
  - docker run时加上参数 --ulimit core=-1 --security-opt seccomp=unconfined
    - 前者就是把 Core Dump 文件的大小设置为无限制，后者是为了开放 ptrace 系列高权限的系统调用，这样我们才可以在 Docker 里面使用 GDB
## PLUSAI常用
### 车端环境变量
- hamlaunch 环境变量
  - /home/plusai/.config/systemd/user、event_recorder.service 表示event-recorder hamlaunch 启动的动作
    - /opt/plusai/config/systemd/pdb-l4e-lab001/event_recorder.env 写入了需要的环境变量

- 某个节点的环境变量查询
  - pidof 节点
  - cat cat /proc/<PID>/environ

### drive镜像
- proto路径： /opt/plusai/common/include/event_recorder

### 服务器
- sz1
  - baseline路径： /data4/gtx5/mirror/gtx5/dists/single_node_perf_test
### ADU测试
- 重启ADU
  - common_if_testapp -tegrareset 重启ADU
- 动态查看文件末尾新增
  - tail -f system_metrics_collector.localhost.root.log.ERROR.19700101-000043.1830988

- 环境变量设置
  - rosparam set /use_sim_time false
  - cat /data/VIN 
  - . launch/pdb-l4e-b0007/setup.sh 
- 查日志
  - /tmp/ham.log
### 路测
- 路测分支
```sh
# 新建本地分支 同步上游最新路测分支
git checkout -b master-20230616 --track upstream/master-20230616
# 将改动的commit cherry-pick到路测分支
git cherry-pick 1311102303e7ede88ee5a5914b1893253371ae5d
# 解决冲突
...
# push
git push upstream master-20230616
```
- 跳板机： ssh chengzhen@192.168.10.241 密码chengzhen
- 登陆路测工程师机器： ssh plusai@192.168.15.126 密码plusai  运营车密码：5uY^M!7d
hamlaunch stop --modules event_recorder
hamlaunch start --modules event_recorder
vi /opt/plusai/conf/event_recorder/modules-j7-l4e/common_config.prototxt
- 登陆ADU
  - 192.168.11.100 root PLAV2021! or plusai plusai
- 查看某个进程所占用的资源
  - pidof PROCESS_NAME
  - top -p PID
- 根据进程名称杀死所有这个名字的进程
  - kill -9 \$(pgrep -f "/usr/bin/python /opt/ros/melodic/bin/rosmaster --core")
## ssh相关
- 登陆脚本
  - vi go 输入以下内容 chmod +x go 然后添加脚本所在位置到环境变量
  - go 112即可ssh到112上

### 请使用AD账号登陆192.168.11.230

Plusai98
```sh
#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Usage: $0 <parameter>"
  exit 1
fi

case "$1" in
  112)
    USER="root"
    IP="192.168.2.112"
    PASSWORD="PLAV2021!"
    ;;
  125)
    USER="root"
    IP="192.168.11.125"
    PASSWORD="PLAV2021!"
    ;;
  118)
    USER="plusai"
    IP="192.168.2.118"
    PASSWD="plusai"
    ;;
  *)
    echo "Invalid parameter: $1"
    exit 1
    ;;
esac

sshpass -p "$PASSWORD" ssh $USER@$IP
```
### jenkins slaves
- 192.168.11.246 slave2

- 192.168.11.247 slave3

- 172.16.100.187 slave4

- 172.16.100.217 slave5

- 172.16.100.41 slave6

- 172.16.100.135 slave7

- 172.16.100.76 salve8



yonghui.rao
  12:56 PM
172.16.100.76 salve8
### gdb 调试
- 编译时工具链由mk指定
```bash
vi /home/workspace/v3na_linux_bsp/make/plus-defs.mk
# contents
BSP_TOP_DIR = /home/workspace/v3na_linux_bsp
TOOLCHAIN_ROOT = $(BSP_TOP_DIR)/toolchains/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu
TOOLCHAIN_SYS_ROOT = $(TOOLCHAIN_ROOT)/aarch64-linux-gnu/libc
CROSSBIN = $(TOOLCHAIN_ROOT)/bin/aarch64-linux-gnu-
...
#CXX
ifeq ($(CXX),g++)
CXX    = $(CROSSBIN)g++ # $(TOOLCHAIN_ROOT)/bin/aarch64-linux-gnu-g++
endif
```

- example
```bash
/home/workspace/v3na_linux_bsp/toolchains/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++ test.cpp other.cpp -o test -g -I .
```

## POSTDB数据库
### 基础命令
```
\q 结束
\l 打印所有数据库
\c exampledb 连接上exampledb数据库
\dt 打印所有表
```
### local登陆
```bash
# psql -U user-name -h localhost # 本地登录
su - postgres
CREATEUSER root WITH PASSWORD '*****';  # 新建一个用户root 注意命令以;结尾表示结束
CREATE DATABASE exampledb OWNER root;  # 创建数据库 属主root
GRANT ALL PRIVILEGES ON DATABASE exampledb TO root;  # 将exampledb数据库的所有权限都赋予root
psql --username=root exampledb < exampledb.sql  # 对数据库执行vehicle_management_db_schema.sql中的命令
```

### 远程连接
```bash
sudo apt install postgresql postgresql-contrib # 安装psql命令
psql -h 172.16.100.17 -p 5432 -U root -d vehicle_management_db -W  # 登陆数据库 后续输入密码 e7zYehLG#
```

### 远程执行命令
- pg_dump
```bash
 pg_dump -S root  -U root -h 172.16.100.17 -p 5432  --schema-only vehicle_management_db  -f vehicle_management_db_schema.sql

 pg_dump -S root  -U root -h 172.16.100.178 -p 45432  --schema-only --table map_localization  vehicle_management_db_test   -f vehicle_management_db_map_localization.sql 

 psql -U root -h 172.16.100.178 -p 45432 -d vehicle_management_db_test -f vehicle_management_db_schema-vehicle_iccid.sql
```
- sql
```sql
select pg_table_size('map_localization')
select pg_database_size('vehicle_management_db');
delete from tablename where ...

SELECT EXISTS (
   SELECT *
   FROM information_schema.columns
   WHERE table_name = 'management_data'
     AND column_name = 'safety_barrier_compensation1'
) AS column_exists;

SELECT super_pilot_engage_button_pressed
FROM management_data
WHERE super_pilot_engage_button_pressed IS NOT null
	AND vehicle_name = 'pdb-l4e-b0007'
 	AND vehicle_timestamp > '2024-12-30 12:38:24.628';

CREATE TABLE driver_behavior (
    vehicle_name character varying,
    vehicle_timestamp timestamp without time zone,
    super_pilot_engage_button_pressed boolean,
    gear_engaged boolean,
    aeb_soft_switch boolean,
    ldw_soft_switch boolean,
    steering_report_override boolean,
    safety_strategy real,
    safety_strategy_trajectory real,
    retarder_level_selected real
);

ALTER TABLE driver_behavior OWNER TO root;
CREATE INDEX driver_behavior_idx ON driver_behavior USING btree (vehicle_name, vehicle_timestamp);
GRANT SELECT ON TABLE public.driver_behavior TO viewer;
```

### 创建只读用户

```
CREATE USER reader WITH ENCRYPTED PASSWORD 'zhijianiubiPLUS+';
ALTER USER reader SET default_transaction_read_only=on;
GRANT USAGE ON SCHEMA public to reader;
GRANT SELECT ON ALL TABLES IN SCHEMA public to reader;
```
## 自动驾驶全栈
### ROS
- rostopic hz /vehicle/control_cmd
- ipc_topic hz /side_left_camera/image_color/compressed
- rosnode list
### 节点含义
- vehicle_can
- app_watchdog
- gmsl_cam
- sense_dms_camera
- rear_radar
- bumper_radar
- side_radar
- rs_lidar
- inno_lidar
- hirain_dms_camera
- obstacle
- lane
- prediction
- planning
- control
- event_recorder
- system_metrics
- map_sensor
- localization
- auto_calibration
- uds_server_node
- ublox
- dispatcher_server

### BBOX
- a black box project for l4e trucks data collection

### 编译
- make package 安装包 
- dpkg -i xxx.deb 安装包到机器
- 一些repo依赖common 先更新common的deb包，然后重新编译这个repo

## 车端配置
- 环境变量初始化
  - /opt/plusai/launch/pdb-l4e-b0001/setup.sh
    - /opt/plusai/launch/pdb-l4e/setup.sh
      - /opt/plusai/launch/l4e-common/setup.sh
- hamlaunch启动进程
  - systemd启动流程： ~/.config/systemd/user/event_recorder.service
  - 环境变量和参数： /opt/plusai/config/hamlaunch.prototxt.pdb-l4e-lab001
  - 

## Other
### 常用命令
```sql

```
### Questions
- 为什么Failed to connect via socket_fd 16 to '192.168.2.14' on port 13006: 'Operation already in progress'
- 为什么Failed to connect via socket_fd

### 小工具
- 把文件内容复制到粘贴板
  ```
  xclip -sel clip file_name
  ```
- 把公钥传送到别的服务器
```
ssh-copy-id -i ~/.ssh/my_public_key.pub user@hostname
```
- iperf带宽测试
```
sudo apt update
sudo apt install iperf
# 在节点1(IP：192.168.10.184)上起iperf服务器，端口号设置19989, 使用MB/s作为单位
iperf -s -p 19989 -f M 
# 在节点2上起iperf客户端，指定服务器的IP地址和端口
iperf -c 192.168.10.184 -p 19989 
```


- 打印机
  - 477打印机
  - Ctrl + P
/data/plusai/DoNotUseThisDirectory/20230710/1.6.1667/plusai



## proto yonghui

- /opt/plusai/conf/event_recorder/recorder_cfg.prototxt.pdb-l4e-lab001
  - inherit: "recorder_cfg.prototxt.pdb-l4e-production"
- /opt/plusai/conf/event_recorder/modules-j7-l4e/common_config.prototxt
  - sync_config: url删掉
- modules-j7-l4e/destinations_pdb_production.prototxt
  - ST_EDR => ST_IBOX
- modules-j7-l4e/destinations_pdb_production.prototxt
  - 4g路由检查设置成false
- transfer_channels_pdb_production.prototxt
  - transfer_channel:{
      url: "ibox://192.168.2.14:5000" # 修改Ip
      name: "ibox_channel"
      protocol: IBOX
      timeout_ms: 3000
      connect_timeout_ms: 500
      nodelay: false
      max_data_waiting_seconds: 1
      expected_block_bytes: 1048576
      queue: {
        max_queue_size : 104857600
        type : FIFO
        full_threshold : 0.95
      }
    }


- ibox端启动
  - ./ibox_service --db_path=/home/chengzhen/workspace/ibox_service/ibox_service/build/db_path --edr_chunk_size=102 --edr_chunk_count=2 --aeb_chunk_size=300 --aeb_chunk_count=3 --log_dir=/home/chengzhen/workspace/ibox_service/ibox_service/build/log_path
  - python3 read-proto.py /home/chengzhen/workspace/ibox_service/ibox_service/build/db_path/edr_5000_1 > logx
- ibox-service test
  - LC_ALL=C TMPDIR=/home/chengzhen/workspace/ibox_service/ibox_service/tmp_build make -j -C build  clean check package

## 打包
```
{ "repositories": [
  {
    "url": "git@github-cn.plus.ai:chengzhen/event_recorder.git",
    "depth": 10,
    "destination_dir": "event_recorder/event_recorder",
    "branch": "ibox-message",
    "dependencies": [
        "common",
        "fastbag"
    ],
    "name": "event_recorder"
  }
  ]
}
```


## gtest
### subprocess abort
- 安装改repo的所有依赖repo生成的package

turn_lever,super_pilot_engage_button_pressed,gear_engaged,aeb_soft_switch,ldw_soft_switch,steering_report_override,retarder_level_selected,
                        safety_strategy,
                        safety_strategy_trajectory




### 书签
http://tt.pluscn.cn/
https://bagdb.pluscn.cn/
https://grafana.pluscn.cn/d/llS0Mw6nk/real-time-system-metrics?orgId=1
https://github-cn.plus.ai/PlusAI/v3na_linux_bsp
http://jenkins-cn/job/v3na_linux_pack_perf/
https://www.processon.io/diagraming/65a78e8ee4b056be76f594bc chengzhen@smartxtruck.com
http://sz1/gtx5/dists/nfs/v3na_linux/test_package/
https://note.youdao.com/web/#/file/recent/markdown/WEBbb497eb00014ae67f1767f3391370c59/
http://jenkins-cn/job/edr_monitor/job/master/
https://github.com/


jdbc:postgresql://172.16.100.17:5432/vehicle_management_db
172.16.100.17 5432 vehicle_management_db

selective 
jdbc:postgresql://172.16.100.56:5432/selective_record

jdbc:postgresql://172.16.100.178:45432/vehicle_management_db_test


SELECT column_name
FROM information_schema.columns
WHERE table_name = 'your_table_name'
  AND column_name = 'your_column_name';


### vescode 

- 改键
{
  "key": "ctrl+v",
  "command": "workbench.action.terminal.paste",
  "when": "terminalFocus && terminalHasBeenCreated || terminalFocus && terminalProcessSupported"
}
{
  "key": "ctrl+c",
  "command": "workbench.action.terminal.copySelection",
  "when": "terminalTextSelectedInFocused || terminalFocus && terminalHasBeenCreated && terminalTextSelected || terminalFocus && terminalProcessSupported && terminalTextSelected || terminalFocus && terminalTextSelected && terminalTextSelectedInFocused || terminalHasBeenCreated && terminalTextSelected && terminalTextSelectedInFocused || terminalProcessSupported && terminalTextSelected && terminalTextSelectedInFocused"
}


## runtime环境变量

### Makefile
```Makefile
# STAGING_ROOT=$(NV_PLATFORM_DIR)/targetfs
.PHONY: hamlaunch_config
hamlaunch_config:
	bash runtime/runtime/hamlaunch/tools/auto_config_l4e.sh $(STAGING_ROOT)/opt j7-l4e
	bash runtime/runtime/hamlaunch/tools/auto_config_l4e.sh $(STAGING_ROOT)/opt pdb-l4e
```

### auto_config_l4e.sh
```sh
bash hamlaunch_config.sh ${opt_path} ${vehicle_type} ${vehicle_name} ${tmp_folder}
...
bash systemd_config.sh ${opt_path} ${vehicle_type} ${vehicle_name}
```

### hamlaunch_config.sh
```sh
for component in ${PLUSAI_COMPONENTS}
    ...
    python ${main_path}/dump_ros_params.py "${file_path}" --dump_file ${tmp_folder}/rosparam.dump
    dump_result=$?
python convert_rosdump_to_hamlaunch_prototxt.py --dump_files ${tmp_folder}/rosparam.dump --vehicle "${vehicle}" --opt_path "${opt_path}" --drive_os "${DRIVE_OS}"
......
mv ${tmp_folder}/rosparam.dump.prototxt  ${tmp_folder}/hamlaunch.prototxt.${vehicle}
cp ${tmp_folder}/hamlaunch.prototxt.${vehicle} ${opt_path}/plusai/config/
```
- dump_ros_params.py
  - 
- convert_rosdump_to_hamlaunch_prototxt.py
  - 把roslaunch 的xml参数文件转化成hamlaunch所需要的hamlaunch.prototxt
  - 以pdb-l4e-b0002为例子，生成/opt/plusai/config/hamlaunch.prototxt.pdb-l4e-b0002,这个配置文件供hamlaunch使用，例如hamlaunch start --modules event_recorder，那么首先会根据vehile_name找到这个配置文件，读入event_recorder这个module，包含了可执行文件的路径bin_path，需要的ros_params等。
    - 特别是event_recorder_config这个参数，指示了/opt/plusai/conf/event_recorder/recorder_cfg.prototxt.pdb-l4e-b0002，会在event_recorder启动后的init函数中进一步读取程序运行时的参数。即/opt/plusai/config/hamlaunch.prototxt.pdb-l4e-b0002是节点的启动时参数，/opt/plusai/conf/event_recorder/recorder_cfg.prototxt.pdb-l4e-b0002是节点的运行时参数
```
    # many modules
    modules {
    name: "event_recorder"
    bin_path: "lib/event_recorder/event_recorder"
    extra_gflags_path: "/opt/plusai/config/event_recorder.gflags"
    ros_args: {key: "__name", value: "event_recorder"}
    gflags: {}
    starting_timeout: 100
    ros_params:{key: "event_recorder_config", value: "/opt/plusai/conf/event_recorder/recorder_cfg.prototxt.pdb-l4e-b0002"}
    ros_params:{key: "latency_aggregator_print_abnormal_latency", value: "false"}
    ros_params:{key: "logbuflevel", value: "2"}
    ros_params:{key: "metrics_targets", value: "latency_aggregator"}
    ros_params:{key: "watchdog_report_topic", value: "/event_recorder/status_report"}
    roles: REGULAR
    roles: CALIB
    roles: CHECK
    roles: VEH_PERF_CHECK
}
```
### systemd_config.sh
```sh
python convert_hamlaunch_to_systemd.py --hamlaunch_config ${hamlaunch_config_file} \
    --target_path ${systemd_config_path} \
    --runtime_path ${runtime_path} \
    --common_proto_path ${common_proto_path} \
    --drive_root ${opt_path}/plusai \
    --skip_env "${skip_env}" \
    --env_file_install_path "${env_file_install_path}"
```
- convert_hamlaunch_to_systemd.py
  - 生成/opt/plusai/config/systemd/pdb-l4e-b0007/event_recorder.env，即systemd维护event_recorder节点时所需要的EnvironmentFile，其中注明了车上runtime的配置，包括IPC，ROS等一些基础公共配置
  - 生成event_recorder.service，即systemd所需要的配置文件
```systemd
[Unit]
Description=event_recorder
StartLimitIntervalSec=20
[Service]
ExecStartPre=/bin/bash -c 'if [ -f /tmp/ham.systemd.event_recorder.log ];then mv /tmp/ham.systemd.event_recorder.log /tmp/ham.systemd.event_recorder.$$(date +%%Y_%%m_%%d.%%H_%%M_%%S).log';fi
ExecStart=/bin/bash -c 'export VEHICLE_NAME=`cat /data/BRAND`-`cat /data/VIN` &&  /opt/plusai/lib/event_recorder/event_recorder __name:=event_recorder `cat /opt/plusai/config/event_recorder.gflags`'
StandardOutput=file:/tmp/ham.systemd.event_recorder.log
StandardError=file:/tmp/ham.systemd.event_recorder.log
LimitNOFILE=4096
Restart=always
RestartSec=0.1
StartLimitBurst=3
Type=simple
EnvironmentFile=/opt/plusai/config/systemd/pdb-l4e-b0007/event_recorder.env
KillSignal=2
RestartKillSignal=2
[Install]
WantedBy=multi-user.target
```
  - 注意到vehicle_can的service配置文件，指明了--flagfile=/opt/plusai/conf/vehicle_can_node/pdb-l4e-b0007.flags和/opt/plusai/config/vehicle_can_node.gflags
  - /opt/plusai/conf/vehicle_can_node/pdb-l4e-b0007.flags指明zf_steer_communication_error_threshold=5，而vehicle can中src/vehicle_can/include/vehicle_can_node/common/dbw_flags.h指定了PLUSAI_DECLARE_double(zf_steer_communication_error_threshold);即这个.flags文件可以指定vehile_can中的PLUSAI环境变量的赋值
  - 问题： .flags和.gflags文件有什么区别？
    - /bin/bash -c 'export VEHICLE_NAME=`cat /data/BRAND`-`cat /data/VIN` &&  /opt/plusai/lib/event_recorder/event_recorder --flagfile=/opt/plusai/conf/event_recorder/pdb-l4e-lab001.flags __name:=event_recorder `cat /opt/plusai/config/event_recorder.gflags`, --flagfile=/opt/plusai/conf/event_recorder/pdb-l4e-lab001.flags里写明了--upload_dms_to_mgmt=true，则FLAGS_upload_dms_to_mgmt变量就注册进了event_recorder runtime
    - .gflags中同样写入--upload_dms_to_mgmt=true，hamlaunch start节点不起作用，并且gflags文件被自动删除

### systemd的常用命令


## bag

### 查看bag信息
```
# 本地查看bag信息
fastbag info -i china-aeb.db
# 查看bag中的某一个topic数据
plusecho -b china-aeb.db /vehicle/dbw_reports > dbw 2>&1
# 车端echo 指定 topic
plusecho2 -topic /vehicle/dbw_reports
# grep 筛选
cat cs | grep POSSIBLE_COLLISION_DETECTED_BASED_ON_DELTA_V -A 1 | grep "value: 0" | wc -l
```

### 分析bag latency
```
# 登录 241
docker exec -it wenjun-20240227T114103 bash
source /opt/plusai/setup-plusai-common.bash 
source /opt/ros/noetic/setup.bash 
python ~/github/tools/common/python/aggregate_latency_reports_from_bags.py --bags path_to_bag --abnormal-threshold -1 > analysis.txt 2>&1
vi analysis.txt
```


## bbox
- ssh plusai@192.168.46.90 auto+ai!
- df -h查看空间最大的盘 /media/sda3/2024-04-09
- ls -lat | head -n 10 看看是否在写入
- 或者iostat -x 1对比前后两帧的写入速率 w/s



## drive工具
### 安装
```sh
# 参阅 https://github-cn.plus.ai/PlusAI/drive/blob/master/setup/ubuntu22_04_setup_guide.md


# 如果在docker内无法sudo
sudo vi /etc/sudoers
#@includedir /etc/sudoers.d

```

### 启动
```sh
drive -i 2955a706326d --name latest-drive-cmd-docker
# 如果编译时找不到环境变量，可能是文件属主的问题
sudo chown plusai:plusai drive
cd drive
make clean
make pacakge
```

## event_recorder配置同步
### 路径


## 远程诊断
### 功能
- crash / eol / 按下按钮(disengage状态)， 会上传日志
- core文件基本不会上传 除非很小

### 路径
- /sdata/selective/remote_diagnosis # sfs-nas2.cn-jssz1.internal.ctclouds.com:/share-56f841bd

## tensorRT
### 安装
```
# 安装 cuda 11.8 for x86_84 ubunut 22.04
wget https://developer.download.nvidia.com/compute/cuda/11.8.0/local_installers/cuda_11.8.0_520.61.05_linux.run
sudo bash cuda_11.8.0_520.61.05_linux.run # continue， 如果已经安装Driver就不需要安装driver
# 添加环境变量到bashrc
export PATH=$PATH:/usr/local/cuda/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib64
export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/cuda/lib64
nvcc -V # 查看cuda版本

# 安装cudnn 8.x for cuda 11.x
wget https://developer.nvidia.com/downloads/compute/cudnn/secure/8.9.7/local_installers/11.x/cudnn-linux-x86_64-8.9.7.29_cuda11-archive.tar.xz
tar -xvf  cudnn-linux-x86_64-8.9.7.29_cuda11-archive.tar.xz
sudo cp cudnn-linux-x86_64-8.9.7.29_cuda11-archive/lib/*  /usr/local/cuda-11.8/lib64
sudo cp cudnn-linux-x86_64-8.9.7.29_cuda11-archive/include/*  /usr/local/cuda-11.8/include
cat /usr/local/cuda/include/cudnn_version.h | grep CUDNN_MAJOR -A 2 # 查看版本

#  安装tensorRT 用tar包最好控制 避免版本问题
wget https://developer.nvidia.com/downloads/compute/machine-learning/tensorrt/secure/8.6.1/tars/TensorRT-8.6.1.6.Linux.x86_64-gnu.cuda-11.8.tar.gz
tar -xzvf TensorRT-8.6.1.6.Linux.x86_64-gnu.cuda-11.8.tar.gz
mv TensorRT-8.6.1.6 /usr/src/
# 添加环境变量到bashrc
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/src/TensorRT-8.6.1.6/lib
export PATH=$PATH:/usr/src/TensorRT-8.6.1.6/bin
```

### 测试
```
cd TensorRT-8.6.1.6/samples/sampleOnnxMNIST
sudo make -j8
cd ../../bin
./sample_onnx_mnist
```