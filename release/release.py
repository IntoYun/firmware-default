#!/usr/bin/env python
#coding=utf-8

import os
import sys
from os.path import dirname, join, exists, isfile
import json
import subprocess
from collections import OrderedDict
from threading import Thread

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

# ['board', 'board_full_name' 'mcu_sdk', 'core', 'board_id', 'default_project']
# default_project 如果未空，表示该设备不需要生成和发布默认程序包
# mcu_sdk 用于取platform sdk
# core 用于取build/linker 和 build/stratup
board_info_list = [
        ['neutron', '1', 'intorobot-neutron'],
        ['nut', '2', 'intorobot-nut'],
        ['fig', '3', 'intorobot-fig'],
        ['ant', '4', 'intorobot-ant'],
        ['fox', '5', 'intorobot-fox'],
        ['atom', '6', 'intorobot-atom']
        ]

def get_board_info_list():
    return board_info_list;

def get_board_index(board):
    index = -1
    for board_info in board_info_list:
        if board == board_info[0]:
            index = board_info_list.index(board_info)
    return index;

def get_board_full_name(board):
    index = get_board_index(board);
    board_full_name = ''
    if index >= 0 :
        board_full_name = board_info_list[index][2];
    return board_full_name;

def load_json(file_path):
    try:
        with open(file_path, "r") as f:
            return json.load(f, object_pairs_hook=OrderedDict)
    except ValueError:
        print "Could not load broken JSON: %s" % file_path

def get_package_version(board):
    file_path = join(get_release_dir(), 'packages-version.json')
    version = ''
    if isfile(file_path):
        data = load_json(file_path)
        version = data[get_board_full_name(board)]['version']
    return version

def get_project_dir():
    return dirname(os.getcwd())

def get_release_dir():
    return os.getcwd()

def get_release_common_board_dir(board):
    return join(get_release_dir(), 'common', board)

def get_release_packages_board_dir(board):
    return join(get_release_dir(), 'packages', board)

def get_release_temp_board_dir(board):
    new_dir = join(get_release_dir(), 'temp', board)
    if not exists(new_dir):
        mkdir(new_dir)
    return new_dir

def select_package_board():
    board_item_string = '-->请选择需发布默认程序包的开发板:\n 0.  所有\n'
    for board_info in get_board_info_list():
        board_item_string += '%02s. %s\n' %(board_info[1], board_info[2])
    board_item_string += '其他退出\n'

    board_item = raw_input(board_item_string);
    board = ''
    if board_item == '0':
        board = 'all'
    else:
        for board_info in get_board_info_list():
            if board_item == board_info[1]:
                board = board_info[0]
                break;
    if board == '':
        exit()
    return board;

def select_package_release_server():
    package_release_server_item_string = '''\
-->请选择发布平台:
============================================
1.  测试平台
2.  生成平台
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

def generate_package_impl(board):
    package_version = get_package_version(board)
    echo("-----------正在生成 %08s 默认程序包; 版本号 : %s---------------" % (board, package_version), 'red')
    board_full_name = get_board_full_name(board)
    project_dir = get_project_dir()
    release_temp_board_dir = get_release_temp_board_dir(board)
    release_temp_board_version_dir = join(release_temp_board_dir, board + '-' + package_version)
    release_temp_board_version_tools_dir = join(release_temp_board_version_dir, 'tools')
    release_temp_board_version_tar_path = release_temp_board_version_dir + '.tar.gz'
    release_common_board_dir = get_release_common_board_dir(board)
    release_tools_dir = join(get_release_dir(), 'tools')

    # 删除目录和文件
    rm(release_temp_board_version_dir)
    rm(release_temp_board_version_tar_path)

    # 编译默认工程
    cmd = "cd %s && intoyuniot run -s -e %s -t clean && intoyuniot run -s -e %s" % (project_dir, board_full_name, board_full_name)
    subprocess.call(cmd, shell=True)

    # 应用程序、公共文件
    copy_to_dir(join(release_common_board_dir, '*'), release_temp_board_version_dir)
    copy(join(project_dir, '.intoiotenvs/%s/firmware.bin' % board_full_name), join(release_temp_board_version_dir, 'firmware.bin'))

    # 编译默认工程
    if board == 'ant' or board == 'fox' or board == 'neutron':
        copy_to_dir(join(release_tools_dir, 'stlink'), release_temp_board_version_tools_dir)
        copy_to_dir(join(release_tools_dir, 'upload-reset'), release_temp_board_version_tools_dir)
        copy_to_dir(join(release_tools_dir, 'dfu-util'), release_temp_board_version_tools_dir)
    elif board == 'atom':
        copy_to_dir(join(release_tools_dir, 'stlink'), release_temp_board_version_tools_dir)
    elif board == 'nut':
        copy_to_dir(join(release_tools_dir, 'esp8266'), release_temp_board_version_tools_dir)
    elif board == 'fig':
        copy_to_dir(join(release_tools_dir, 'esp32'), release_temp_board_version_tools_dir)

def release_package_impl(board, server):
    package_version = get_package_version(board)
    echo("-----------正在上传 %s 默认程序包; 版本号: %s---------------" % (board, package_version), 'red')
    package_develop_dir = get_package_develop_dir(board)
    package_version_name = board + '-' + package_version
    package_version_tar_name = package_version_name + '.tar.gz'
    package_version_tar_path = join(package_develop_dir, package_version_tar_name)
    echo("1. 正在打包默认程序包(版本号: %s) ... " % package_version, 'blue')
    cmd = "cd %s && rm -rf %s && tar -czf %s %s && rm -rf %s" % (package_develop_dir, package_version_tar_name, package_version_tar_name, package_version_name, package_version_name)
    subprocess.call(cmd, shell=True)

    server_modules_packages_dir = join(server + ':/var/www/downloads/terminal/modules/package', board)
    local_release_modules_packages_dir = get_package_release_dir(board)
    if server != 'local':
        echo("2. 正在上传默认程序包 ... ", 'blue')
        cmd = "scp %s %s" % (package_version_tar_path, server_modules_packages_dir)
        subprocess.call(cmd, shell=True)

        echo("3. 复制到packages/release ... ", 'blue')
        copy(package_version_tar_path, local_release_modules_packages_dir)

        echo("4. 删除临时文件 ... ", 'blue')
        rm(package_version_tar_path)

def generate_package():
    echo("请确保firmware当前分支 非develop分支!!!", 'yellow')
    board = select_package_board()
    if board == 'all':   #发布所有 package
        for board_info in get_board_info_list():
            generate_package_impl(board_info[0])
    else:
        generate_package_impl(board)

def confirm_package_release_info(board, server):
    print('\033[1;31;40m')
    print "---------请确认上传默认程序信息-----------"
    print "开发板/模组:     %s" % board
    if board == 'all':   #发布所有 package
        for board_info in get_board_info_list():
            print "默认程序版本号:  %s: %s" % (board_info[0], get_package_version(board_info[0]))
    else:
        print "默认程序版本号:  %s: %s" % (board, get_package_version(board))
    print "上传服务器:      %s" % server
    print "-----确认继续, 其他退出-----"
    print('\033[0m')
    res = raw_input("")
    if res != '':
        exit()

def release_package():
    board = select_package_board()
    server = select_package_release_server()
    confirm_package_release_info(board, server)
    if board == 'all':   #发布所有 package
        for board_info in get_board_info_list():
            release_package_impl(board_info[0], server)
    else:
        release_package_impl(board, server)

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
1. 生成开发板默认程序包
2. 发布开发板默认程序包
3. 进入服务器
==========================================================
其他退出
'''
    item = raw_input(release_item_string);
    if item == '1':   #生成默认程序包
        generate_package()
    elif item == '2': #发布默认程序包
        release_package()
    elif item == '3': #进入服务器
        enter_server()
    else:
        exit()

