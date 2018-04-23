#!/usr/bin/env python
#coding=utf-8

import framework_imp
import package_imp
import firmware_imp
import platform_imp
import util
from os.path import join
import subprocess

util.set_encoding('utf-8')

def generate_framework():
    util.echo("请确保firmware当前分支 非develop分支!!!", 'yellow')
    board = util.select_framework_board()
    framework_version = util.input_framwork_version()
    if board == 'all':   #生成所有 framework
        for board_info in util.get_board_info_list():
            if framework_imp.is_allow_generate(framework_version):
                framework_imp.generate(board_info[0], framework_version)
            else:
                exit()
    else:
        framework_imp.generate(board, framework_version)

def confirm_framework_release_info(board, platform_version, framework_version, server):
    print('\033[1;31;40m')
    print "---------请确认当前发布信息-----------"
    print "开发板/模组:     %s" % board
    print "framework版本号: %s" % framework_version
    print "platform 版本号: %s" % platform_version
    print "上传服务器:      %s" % server
    print "-----确认继续, 其他退出-----"
    print('\033[0m')
    res = raw_input("")
    if res != '':
        exit()

def release_framework():
    board = util.select_framework_board()
    platform_version = util.input_platform_version()
    server = util.select_framework_release_server()
    framework_version = util.get_framework_version(board)
    confirm_framework_release_info(board, platform_version, framework_version, server)
    if board == 'all':   #生成所有 framework
        for board_info in util.get_board_info_list():
            if framework_imp.is_allow_release(board_info[0]):
                framework_imp.release(board_info[0], platform_version, server)
            else:
                exit()
    else:
        framework_imp.release(board, platform_version, server)

def generate_package():
    util.echo("请确保firmware当前分支 非develop分支!!!", 'yellow')
    board = util.select_package_board()
    if board == 'all':   #发布所有 package
        for board_info in util.get_board_info_list():
            if board_info[6] != '':
                package_imp.generate(board_info[0])
    else:
        package_imp.generate(board)

def confirm_package_release_info(board, server):
    print('\033[1;31;40m')
    print "---------请确认上传默认程序信息-----------"
    print "开发板/模组:     %s" % board
    if board == 'all':   #发布所有 package
        for board_info in util.get_board_info_list():
            if board_info[6] != '':
                print "默认程序版本号:  %s: %s" % (board_info[0], util.get_package_version(board_info[0]))
    else:
        print "默认程序版本号:  %s: %s" % (board, util.get_package_version(board))
    print "上传服务器:      %s" % server
    print "-----确认继续, 其他退出-----"
    print('\033[0m')
    res = raw_input("")
    if res != '':
        exit()

def release_package():
    board = util.select_package_board()
    server = util.select_package_release_server()
    confirm_package_release_info(board, server)
    if board == 'all':   #发布所有 package
        for board_info in util.get_board_info_list():
            if board_info[6] != '':
                package_imp.release(board_info[0], server)
    else:
        package_imp.release(board, server)

def enter_platform():
    server = util.select_enter_server()
    cmd = "ssh %s" % server
    subprocess.call(cmd, shell=True)

def release_firmware():
    util.echo("请确保firmware当前分支为待发布的release分支，并且为干净分支!!!", 'yellow')
    firmware_version = util.input_firmware_version()
    firmware_imp.release(firmware_version)

def remove_firmware():
    util.echo("请确认firmware release分支!!!", 'yellow')
    firmware_version = util.input_firmware_version()
    firmware_imp.remove(firmware_version)

def release_platform():
    board = util.select_package_board()
    if board == 'all':   #发布所有 platform
        for board_info in util.get_board_info_list():
            if platform_imp.is_allow_release(board_info[0]):
                platform_imp.release(board_info[0])
            else:
                exit()
    else:
        if platform_imp.is_allow_release(board):
            platform_imp.release(board)

def remove_platform():
    board = util.select_package_board()
    if board == 'all':   #发布所有 platform
        for board_info in util.get_board_info_list():
            if platform_imp.is_allow_remove(board_info[0]):
                platform_imp.remove(board_info[0])
            else:
                exit()
    else:
        if platform_imp.is_allow_remove(board):
            platform_imp.remove(board)

def mac_dir_clean():
    util.dir_clean(util.get_frameworks_dir())
    util.dir_clean(util.get_manifests_dir())
    util.dir_clean(util.get_platforms_dir())

if __name__ == '__main__':
    release_item_string = '''\
-->请选择:
===========================================================
1. 生成framework  (编译firmware并生成framework)
2. 发布framework  (上传framework/platform/manifest到平台)

3. 发布硬件平台(platform) -> github
4. 移除硬件平台(platform) -> github

5. 发布固件库  (firmware) -> github
6. 移除固件库  (firmware) -> github
==========================================================
7. 生成默认程序包 (生成开发板/模组 默认程序包)
8. 发布默认程序包 (上传开发板/模组 默认程序包)
==========================================================
9. 进入服务器
10. 清理文件(mac系统使用)
==========================================================
其他退出
'''
    item = raw_input(release_item_string);
    if item == '1':   #生成framework
        generate_framework()
    elif item == '2': #发布framework
        release_framework()
    elif item == '3': #发布硬件平台(platform) -> github
        release_platform()
    elif item == '4': #移除硬件平台(platform) -> github
        remove_platform()
    elif item == '5': #发布固件库  (firmware) -> github
        release_firmware()
    elif item == '6': #移除固件库  (firmware) -> github
        remove_firmware()
    elif item == '7': #生成默认程序包
        generate_package()
    elif item == '8': #发布默认程序包
        release_package()
    elif item == '9': #进入服务器
        enter_platform()
    elif item == '10': #清理文件(mac系统使用)
        mac_dir_clean()
    else:
        exit()

