#!/usr/bin/env python
#coding=utf-8

from os.path import join
import sys
import json
import util
import subprocess

util.set_encoding('utf-8')

def generate(board):
    package_version = util.get_package_version(board)
    util.echo("-----------正在生成 %s 默认程序包; 版本号 : %s---------------" % (board, package_version), 'red')
    firmware_build_dir = util.get_firmware_build_dir()
    package_develop_dir = util.get_package_develop_dir(board)
    package_version_dir = join(package_develop_dir, board + '-' + package_version)
    package_version_tools_dir = join(package_version_dir, 'tools')
    package_version_tar_path = package_version_dir + '.tar.gz'
    package_common_dir = util.get_package_develop_common_dir(board)
    packages_tools_dir = join(util.get_packages_dir(), 'tools')
    board_id = util.get_board_id(board)
    board_core = util.get_board_core(board)

    # 删除目录和文件
    util.rm(package_version_dir)
    util.rm(package_version_tar_path)

    # 编译默认工程
    util.make_firmware(board, util.get_board_default_project(board))

    # 移动Bootloader、应用程序、公共文件
    util.copy_to_dir(join(package_common_dir, '*'), package_version_dir)
    util.copy(join(firmware_build_dir, 'target/main/platform-' + board_id, 'default' + '-' + board + '.bin'), join(package_version_dir, 'firmware.bin'))

    # 编译默认工程
    if board == 'neutron':
        util.copy_to_dir(join(packages_tools_dir, 'esp8266'), package_version_tools_dir)

    if board_core == 'arm':
        util.copy_to_dir(join(packages_tools_dir, 'stlink'), package_version_tools_dir)
        util.copy_to_dir(join(packages_tools_dir, 'upload-reset'), package_version_tools_dir)
        util.copy_to_dir(join(packages_tools_dir, 'dfu-util'), package_version_tools_dir)
        util.copy(join(package_version_dir, 'firmware.bin'), join(package_version_dir, 'firmware.dfu'))
        cmd = "%s/tools/dfu-util/dfu-suffix -v 0483 -p df11 -a %s 2 > /dev/null" % (util.get_firmware_dir(), join(package_version_dir, 'firmware.dfu'))
        subprocess.call(cmd, shell=True)
    elif board_core == 'esp8266':
        util.copy_to_dir(join(packages_tools_dir, 'esp8266'), package_version_tools_dir)
    elif board_core == 'esp32':
        util.copy_to_dir(join(packages_tools_dir, 'esp32'), package_version_tools_dir)

def release(board, server):
    package_version = util.get_package_version(board)
    util.echo("-----------正在上传 %s 默认程序包; 版本号: %s---------------" % (board, package_version), 'red')
    package_develop_dir = util.get_package_develop_dir(board)
    package_version_name = board + '-' + package_version
    package_version_tar_name = package_version_name + '.tar.gz'
    package_version_tar_path = join(package_develop_dir, package_version_tar_name)
    util.echo("1. 正在打包默认程序包(版本号: %s) ... " % package_version, 'blue')
    cmd = "cd %s && rm -rf %s && tar -czf %s %s && rm -rf %s" % (package_develop_dir, package_version_tar_name, package_version_tar_name, package_version_name, package_version_name)
    subprocess.call(cmd, shell=True)

    server_modules_packages_dir = join(server + ':/var/www/downloads/terminal/modules/package', board)
    local_release_modules_packages_dir = util.get_package_release_dir(board)
    if server != 'local':
        util.echo("2. 正在上传默认程序包 ... ", 'blue')
        cmd = "scp %s %s" % (package_version_tar_path, server_modules_packages_dir)
        subprocess.call(cmd, shell=True)

        util.echo("3. 复制到packages/release ... ", 'blue')
        util.copy(package_version_tar_path, local_release_modules_packages_dir)

        util.echo("4. 删除临时文件 ... ", 'blue')
        util.rm(package_version_tar_path)

