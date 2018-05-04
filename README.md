# 开发板默认固件程序

## 1. 概述

firmware-default 属于开发板的默认出厂程序。

## 2. 工程结构

firmware-default 工程结构如下：

```
|-- docs            : 工程文档
|-- lib             : 工程依赖库
|-- release         : 编译和发布脚本
|-- src             : 工程代码
|-- intoyuniot.ini  : 编译配置文件
 -- README.md
```

## 3. 编译与调试

firmware-default 采取intoyuniot编译，具体如下：

```
intoyuniot run -t clean   # 清除临时文件
intoyuniot run -e intorobot-nut -t upload  # 编译和下载nut程序
intoyuniot run -e intorobot-fig -t upload  # 编译和下载fig程序
```

## 4. 发布

进入release, 运行./release.sh

```
1. 生成产品软件包
2. 上传产品软件包
3. 进入生产平台

```


