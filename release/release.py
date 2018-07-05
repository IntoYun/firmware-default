#!/usr/bin/env python
#coding=utf-8

import os
import sys
from os.path import dirname, join, exists, isfile
import json
import subprocess
from collections import OrderedDict

def set_encoding(encoding):
    if sys.getdefaultencoding() != encoding:
        reload(sys)
        sys.setdefaultencoding(encoding)

set_encoding('utf-8')

def mkdir(dir):
    cmd = "mkdir -p %s" % dir
    subprocess.call(cmd, shell=True)

def copy_to_dir(old, new_dir):
    if not exists(new_dir):
        mkdir(new_dir)
    cmd = "cp -rf %s %s" %(old, new_dir)
    subprocess.call(cmd, shell=True)

def copy(old, new):
    cmd = "cp -rf %s %s" %(old, new)
    subprocess.call(cmd, shell=True)

def rm(name):
    cmd = "rm -rf %s" % name
    subprocess.call(cmd, shell=True)

def echo(str, color):
    if color == 'red':
        font_color = 31
    elif color == 'green':
        font_color = 32
    elif color == 'yellow':
        font_color = 33
    elif color == 'blue':
        font_color = 34
    else:
        font_color = 37

    print "\033[1;31;%sm%s\033[0m" %(font_color, str)

def load_json(file_path):
    try:
        with open(file_path, "r") as f:
            return json.load(f, object_pairs_hook=OrderedDict)
    except ValueError:
        print "Could not load broken JSON: %s" % file_path

def get_product_info_list():
    file_path = join(get_release_dir(), 'product-info.json')
    if not isfile(file_path):
        print "%s not exist!" % file_path
        exit()
    return load_json(file_path);

def get_product_module(product_name):
    dist = get_product_info_list()
    return dist[product_name]['module']

def get_product_version(product_name):
    dist = get_product_info_list()
    return dist[product_name]['version']

def get_project_dir():
    return dirname(os.getcwd())

def get_release_dir():
    return os.getcwd()

def get_release_common_dir():
    return join(get_release_dir(), 'common')

def get_release_common_module_dir(product_name):
    return join(get_release_dir(), 'common', get_product_module(product_name))

def get_release_package_product_dir(product_name):
    new_dir = join(get_release_dir(), 'package', product_name)
    if not exists(new_dir):
        mkdir(new_dir)
    return new_dir

def get_release_temp_product_dir(product_name):
    new_dir = join(get_release_dir(), 'temp', product_name)
    if not exists(new_dir):
        mkdir(new_dir)
    return new_dir

def select_package_product():
    product_item_string = '-->请选择产品:\n'
    product_item_string += '============================================\n'
    product_item_string += '0. 所有\n'
    dist = get_product_info_list()
    for name in dist.keys():
        product_item_string += '%s. %s\n' %(dist[name]['index'], name)
    product_item_string += '============================================\n'
    product_item_string += '其他退出\n'

    index = raw_input(product_item_string);
    product_name = ''
    if index == '0':
        product_name = 'all'
    else:
        for name in dist.keys():
            if index == dist[name]['index']:
                product_name = name
                break;
    if product_name == '':
        exit()
    return product_name;

def select_package_release_server():
    package_release_server_item_string = '''\
-->请选择发布平台:
============================================
1.  测试平台
2.  生产平台
============================================
其他退出
'''
    item = raw_input(package_release_server_item_string);
    if item == '1':
        server_name = 'test-web1'
    elif item == '2':
        server_name = 'prod-yun'
    else:
        exit()
    return server_name

def generate_package_impl(product_name):
    package_version = get_product_version(product_name)
    echo("-----------正在生成 %16s 程序包; 版本号 : %s---------------" % (product_name, package_version), 'red')
    module = get_product_module(product_name)
    project_dir = get_project_dir()
    release_temp_product_dir = get_release_temp_product_dir(product_name)
    release_temp_product_version_dir = join(release_temp_product_dir, product_name + '-' + package_version)
    release_temp_product_version_tools_dir = join(release_temp_product_version_dir, 'tools')
    release_temp_product_version_tar_path = release_temp_product_version_dir + '.tar.gz'
    release_common_product_dir = get_release_common_module_dir(product_name)
    release_common_tools_dir = join(get_release_common_dir(), 'tools')

    # 删除目录和文件
    rm(release_temp_product_version_dir)
    rm(release_temp_product_version_tar_path)

    # 编译默认工程
    cmd = "cd %s && intoyuniot run -s -e %s -t clean && intoyuniot run -s -e %s" % (project_dir, module, module)
    subprocess.call(cmd, shell=True)

    # 应用程序、公共文件
    copy_to_dir(join(release_common_product_dir, '*'), release_temp_product_version_dir)
    copy(join(project_dir, '.intoiotenvs/%s/firmware.bin' % module), join(release_temp_product_version_dir, 'firmware.bin'))

    # 编译默认工程
    if module == 'intorobot-ant' or module == 'intorobot-fox' or module == 'intorobot-neutron' or module == 'intorobot-l6' or module == 'intorobot-atom' or module == 'intorobot-gl2000' or module == 'intorobot-gl2100':
        copy_to_dir(join(release_common_tools_dir, 'stlink'), release_temp_product_version_tools_dir)
    if module == 'intorobot-ant' or module == 'intorobot-fox' or module == 'intorobot-neutron' or module == 'intorobot-l6' or module == 'intorobot-gl2000' or module == 'intorobot-gl2100':
        copy_to_dir(join(release_common_tools_dir, 'dfu-util'), release_temp_product_version_tools_dir)
    if module == 'intorobot-gl2000' or module == 'intorobot-gl2100':
        copy_to_dir(join(release_common_tools_dir, 'upload-reset'), release_temp_product_version_tools_dir)
    if module == 'intorobot-nut' or module == 'intorobot-neutron' or module == 'intorobot-w67':
        copy_to_dir(join(release_common_tools_dir, 'esp8266'), release_temp_product_version_tools_dir)
    if module == 'intorobot-fig' or module == 'intorobot-w323':
        copy_to_dir(join(release_common_tools_dir, 'esp32'), release_temp_product_version_tools_dir)
    echo("-----------生成完毕---------------", 'red')

def release_package_impl(product_name, server):
    package_version = get_product_version(product_name)
    echo("-----------正在上传 %16s 程序包; 版本号: %s---------------" % (product_name, package_version), 'red')
    release_temp_product_dir = get_release_temp_product_dir(product_name)
    package_version_name = product_name + '-' + package_version
    package_version_tar_name = package_version_name + '.tar.gz'
    release_temp_product_version_tar_path = join(release_temp_product_dir, package_version_tar_name)
    echo("1. 正在打包程序包(版本号: %s) ... " % package_version, 'blue')
    cmd = "cd %s && rm -rf %s && tar -czf %s %s && rm -rf %s" % (release_temp_product_dir, package_version_tar_name, package_version_tar_name, package_version_name, package_version_name)
    subprocess.call(cmd, shell=True)

    server_modules_packages_dir = join(server + ':/var/www/downloads/terminal/modules/package', product_name)
    local_release_modules_packages_dir = get_release_package_product_dir(product_name)
    echo("2. 正在上传程序包 ... ", 'blue')
    cmd = "scp %s %s" % (release_temp_product_version_tar_path, server_modules_packages_dir)
    subprocess.call(cmd, shell=True)

    echo("3. 复制到packages ... ", 'blue')
    copy(release_temp_product_version_tar_path, local_release_modules_packages_dir)

    echo("4. 删除临时文件 ... ", 'blue')
    rm(release_temp_product_version_tar_path)
    echo("-----------上传完毕---------------", 'red')

def generate_package():
    product_name = select_package_product()
    dist = get_product_info_list()
    if product_name == 'all':   #发布所有 package
        for name in dist.keys():
            generate_package_impl(name)
    else:
        generate_package_impl(product_name)

def confirm_package_release_info(product_name, server):
    print('\033[1;31;40m')
    print "---------请确认上传默认程序信息-----------"
    print "产品:     %s" % product_name
    dist = get_product_info_list()
    if product_name == 'all':   #发布所有 package
        for name in dist.keys():
            print "程序版本号:  %s: %s" % (name, get_product_version(name))
    else:
        print "程序版本号:  %s: %s" % (product_name, get_product_version(product_name))
    print "上传服务器:      %s" % server
    print "-----确认继续, 其他退出-----"
    print('\033[0m')
    res = raw_input("")
    if res != '':
        exit()

def release_package():
    product_name = select_package_product()
    server = select_package_release_server()
    confirm_package_release_info(product_name, server)
    dist = get_product_info_list()
    if product_name == 'all':   #发布所有 package
        for name in dist.keys():
            release_package_impl(name, server)
    else:
        release_package_impl(product_name, server)

def enter_server():
    enter_server_item_string = '''\
-->请选择进入的服务器类型:
============================================
1.  测试平台-文件服务器
2.  生产平台-文件服务器
============================================
其他退出
'''
    item = raw_input(enter_server_item_string);
    if item == '1':
        server_name = 'test-web1'
    elif item == '2':
        server_name = 'prod-yun'
    else:
        exit()

    cmd = "ssh %s" % server_name
    subprocess.call(cmd, shell=True)

if __name__ == '__main__':
    release_item_string = '''\
-->请选择:
===========================================================
1. 生成产品软件包
2. 上传产品软件包
3. 进入生成平台
==========================================================
其他退出
'''
    item = raw_input(release_item_string);
    if item == '1':   #生成产品软件包
        generate_package()
    elif item == '2': #上传产品软件包
        release_package()
    elif item == '3': #进入生成平台
        enter_server()
    else:
        exit()

