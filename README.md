# 安装mysql
---

[参考这篇指南](https://www.linode.com/docs/databases/mysql/how-to-install-mysql-on-centos-7/)

创建初始数据库

~~~
CREATE DATABASE fishield
~~~

创建初始用户

~~~
CREATE USER 'fishield'@'%' IDENTIFIED BY 'fishield'; 
GRANT ALL ON fishield.* TO 'fishield'@'%'; 
~~~

创建初始表格

~~~
USE fishield
CREATE TABLE user(
    username VARCHAR(100) PRIMARY KEY,
    password VARCHAR(100) NOT NULL,
    privilege int DEFAULT 0,
    create_time DATETIME DEFAULT NOW()
)default charset=utf8;

INSERT INTO user(username, password, privilege) VALUES('root', '123456', 128+2+1);
INSERT INTO user(username, password, privilege) VALUES('upload', '000000', 1);
INSERT INTO user(username, password, privilege) VALUES('download', '000000', 2);

CREATE TABLE task(
    task_id INT PRIMARY KEY,
    localbasepath VARCHAR(512),
    remotebasepath VARCHAR(512),
    filename VARCHAR(256),
    total_packet_no INT,
    task_status INT,
    received_packet_no INT,
    sent_packet_no INT,
    last_packet_time BIGINT
)default charset=utf8;



CREATE TABLE ip(
    address VARCHAR(200) PRIMARY KEY
)default charset=utf8;

INSERT INTO ip VALUES('127.0.0.1');
~~~


在安装的过程中，我把 mysql 的`root`用户密码设置为`whu2018`

# 安装mysql Connector
---

~~~
wget https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-8.0.11-linux-el7-x86-64bit.tar.gz
tar -xvf mysql-connector-c++-8.0.11-linux-el7-x86-64bit.tar.gz
cd mysql-connector-c++-8.0.11-linux-el7-x86-64bit
cd include/jdbc
cp -r * /usr/include
cd ../../lib64
cp -r * /usr/lib
~~~


# 安装gcc/g++
---

~~~
yum install gcc-c++
~~~

# 安装opengl
---

~~~
yum install mesa-libGL
yum install mesa-libGL-devel
~~~

# 安装boost
---

~~~
yum install boost-devel
~~~

# 安装protobuf
---

下载源代码

~~~
wget https://github.com/google/protobuf/releases/download/v3.6.0/protobuf-cpp-3.6.0.tar.gz
tar -xvf protobuf-cpp-3.6.0.tar.gz
cd protobuf-cpp-3.6.0
~~~

编译安装

~~~
./configure
make
make check
sudo make install
sudo ldconfig # refresh shared library cache.

cp /usr/local/lib/libprotobuf.so.16 /usr/lib
~~~


# 安装git
---

~~~
yum install git
~~~

# 获取项目代码
---

~~~
git clone http://github.com/LLipter/fishield.git
cd fishield
git submodule init
git submodule update
~~~


# 运行服务器端程序
---

~~~
cd sdk
make clean
make
make debug # optional
./server
~~~


# 安装Qt
---

[从官网下载](https://www.qt.io/download)

# 运行客户端程序
---
用Qt打开位于`fishield/client/client.pro`的项目

修改`backend.cpp` 中，函数`backend::login()` (大概在十二行的位置)

~~~
fs_client_startup("www.irran.top", 7614);
~~~

将服务器的地址作出相应的修改，比如说如果本机部署服务器的话，将地址修改为`127.0.0.1`，也可以直接将这条语句注释掉，注释后客户端会默认采用缺省值，缺省值就是`127.0.0.1`

然后即可点击运行

预先设置了一个`root`账户，密码是`123456`

# fsconfig小工具
---
在项目的`fishield/sdk`文件夹下，除了`server.out`程序外，还编译了一个叫做`fsconfig`的小程序，这个命令行小程序可以用来添加账户、删除账户、查看账户列表、修改允许访问的ip地址列表。这个小工具需要用`root`账户的密码登录
