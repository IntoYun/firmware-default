# yunslave-wifi

## 1. 概述

yunslave-wifi 是WiFi从模式代码工程。支持模块包括W6/W7和W32/W33。

## 2. 工程结构

yunslave-wifi 工程结构如下：

```
|-- docs            : 工程文档
|-- lib             : 工程依赖库
|-- release         : 编译和发布脚本
|-- src             : 工程代码
|-- intoyuniot.ini  : 编译配置文件
 -- README.md
```

## 3. 编译与调试

yunslave-wifi 采取intoyuniot编译，具体如下：

```
intoyuniot run -t clean   # 清除临时文件

intoyuniot run -e intorobot-w67 -t upload  # 编译和下载w6/w7从模式程序

intoyuniot run -e intorobot-w323 -t upload # 编译和下载w32/w33从模式程序

```

## 4. 发布

进入release, 运行./release.sh

```
1. 生成产品软件包
2. 上传产品软件包
3. 进入生产平台

```




