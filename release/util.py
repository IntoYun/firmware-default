#!/usr/bin/env python
#coding=utf-8

import os
import sys
from os.path import dirname, join, exists, isfile
import json
import subprocess
import platform
from collections import OrderedDict
from threading import Thread

class AsyncPipe(Thread):

    def __init__(self, outcallback=None):
        Thread.__init__(self)
        self.outcallback = outcallback

        self._fd_read, self._fd_write = os.pipe()
        self._pipe_reader = os.fdopen(self._fd_read)
        self._buffer = []

        self.start()

    def get_buffer(self):
        return self._buffer

    def fileno(self):
        return self._fd_write

    def run(self):
        for line in iter(self._pipe_reader.readline, ""):
            line = line.strip()
            self._buffer.append(line)
            if self.outcallback:
                self.outcallback(line)
            else:
                print line
        self._pipe_reader.close()

    def close(self):
        os.close(self._fd_write)
        self.join()

def set_encoding(encoding):
    if sys.getdefaultencoding() != encoding:
        reload(sys)
        sys.setdefaultencoding(encoding)

# ['board', 'board_full_name' 'mcu_sdk', 'core', 'board_id', 'default_project']
# default_project 如果未空，表示该设备不需要生成和发布默认程序包
# mcu_sdk 用于取platform sdk
# core 用于取build/linker 和 build/stratup
board_info_list = [
        ['neutron', '1', 'intorobot-neutron', 'STM32F4xx', 'arm', '888002', 'smartLight-default'],
        ['nut', '2', 'intorobot-nut', 'ESP8266-Arduino', 'esp8266', '888003', 'smartLight-default'],
        ['fig', '3', 'intorobot-fig', 'ESP32-Arduino', 'esp32', '888005', 'smartLight-default'],
        ['ant', '4', 'intorobot-ant', 'STM32L1xx', 'arm', '868009', 'blank'],
        ['fox', '5', 'intorobot-fox', 'STM32F4xx', 'arm', '878008', 'smartLight-default'],
        ['w67', '6', 'intorobot-w67', 'ESP8266-Arduino', 'esp8266', '888006', 'smartLight-module'],
        ['w323', '7', 'intorobot-w323', 'ESP32-Arduino', 'esp32', '888007', 'smartLight-module'],
        ['l6', '8', 'intorobot-l6', 'STM32L1xx', 'arm', '868010', 'blank'],
        ['atom', '9', 'intorobot-atom', 'STM32F1xx', 'arm', '888001', ''],
        ['gl2000', '10', 'intorobot-gl2000', 'STM32F4xx', 'arm', '188002', ''],
        ['gl2100', '11', 'intorobot-gl2100', 'STM32F4xx', 'arm', '178003', '']
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

def get_board_mcu_sdk(board):
    index = get_board_index(board);
    board_mcu_sdk = ''
    if index >= 0 :
        board_mcu_sdk = board_info_list[index][3];
    return board_mcu_sdk;

def get_board_core(board):
    index = get_board_index(board);
    board_core = ''
    if index >= 0 :
        board_core = board_info_list[index][4];
    return board_core;

def get_board_id(board):
    index = get_board_index(board);
    board_id = ''
    if index >= 0 :
        board_id = board_info_list[index][5];
    return board_id;

def get_board_default_project(board):
    index = get_board_index(board);
    project = ''
    if index >= 0 :
        project = board_info_list[index][6];
    return project;

def get_package_version(board):
    file_path = join(get_release_dir(), 'packages-version.json')
    version = ''
    if isfile(file_path):
        data = load_json(file_path)
        version = data[get_board_full_name(board)]['version']
    return version

def select_framework_board():
    board_item_string = '-->请选择需发布framework的开发板/模组:\n 0.  所有\n'
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

def select_package_board():
    board_item_string = '-->请选择需发布默认程序包的开发板/模组:\n 0.  所有\n'
    for board_info in get_board_info_list():
        if board_info[6] != '':
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

def select_framework_release_server():
    framework_release_server_item_string = '''\
-->请选择发布平台:
1.  测试平台
2.  发布平台
3.  本地机器(替换.intoyuniot)
其他退出
'''
    item = raw_input(framework_release_server_item_string);
    if item == '1':
        server_name = 'test-web1'
    elif item == '2':
        server_name = 'prod-yun'
    elif item == '3':
        server_name = 'local'
    else:
        exit()
    return server_name;

def select_package_release_server():
    package_release_server_item_string = '''\
-->请选择发布平台:
1.  测试平台
2.  发布平台
其他退出
'''
    item = raw_input(package_release_server_item_string);
    if item == '1':
        server_name = 'test-web1'
    elif item == '2':
        server_name = 'prod-yun'
    else:
        exit()
    return server_name;

def select_enter_server():
    enter_server_item_string = '''\
-->请选择进入的服务器类型:
1.  测试平台-文件服务器
2.  测试平台-编译服务器
3.  生产平台-文件服务器
4.  生产平台-编译服务器
其他退出
'''
    item = raw_input(enter_server_item_string);
    if item == '1':
        server_name = 'test-web1'
    elif item == '2':
        server_name = 'test-web1'
    elif item == '3':
        server_name = 'prod-yun'
    elif item == '4':
        server_name = 'prod-program'
    else:
        exit()
    return server_name;

def input_framwork_version():
    version = raw_input("-->请输入framework版本号(例如: 1.6.9) : ")
    if version == '':
        exit()
    return version

def input_platform_version():
    version = raw_input("-->请输入platform版本号(例如: 1.0.2) : ")
    if version == '':
        exit()
    return version

def input_firmware_version():
    version = raw_input("-->请输入firmware版本号(例如: 1.6.9) : ")
    if version == '':
        exit()
    return version

def get_firmware_dir():
    return dirname(os.getcwd())

def get_firmware_build_dir():
    return join(get_firmware_dir(), 'build')

def get_release_dir():
    return os.getcwd()

def get_frameworks_dir():
    return join(get_release_dir(), 'intoyuniot/frameworks')

def get_framework_dir(board):
    return join(get_release_dir(), 'intoyuniot/frameworks', 'framework-intorobot-' + board)

def get_framework_cores_dir(board):
    framework_dir = get_framework_dir(board)
    if board == 'atom':
        framework_dir = join(framework_dir, 'STM32F1')
    return join(framework_dir, 'cores')

def get_framework_variants_dir(board):
    framework_dir = get_framework_dir(board)
    if board == 'atom':
        framework_dir = join(framework_dir, 'STM32F1')
    return join(framework_dir, 'variants')

def get_platform_dir(board):
    return join(get_release_dir(), 'intoyuniot/platforms', 'platform-intorobot-' + board)

def get_platforms_dir():
    return join(get_release_dir(), 'intoyuniot/platforms')

def get_manifests_dir():
    return join(get_release_dir(), 'intoyuniot/manifests')

def get_frameworks_manifest_path():
    return join(get_release_dir(), 'intoyuniot/manifests/frameworks/manifest.json')

def get_platforms_manifest_path():
    return join(get_release_dir(), 'intoyuniot/manifests/platforms/manifest.json')

def get_packages_dir():
    return join(get_release_dir(), 'packages')

def get_package_develop_dir(board):
    return join(get_release_dir(), 'packages/develop', board)

def get_package_develop_common_dir(board):
    return join(get_release_dir(), 'packages/develop', board, 'common')

def get_package_release_dir(board):
    return join(get_release_dir(), 'packages/release', board)

def exec_command(*args, **kwargs):
    result = {"out": None, "err": None, "returncode": None}

    default = dict(stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    default.update(kwargs)
    kwargs = default

    p = subprocess.Popen(*args, **kwargs)
    try:
        result['out'], result['err'] = p.communicate()
        result['returncode'] = p.returncode
    except KeyboardInterrupt:
        raise exception.AbortedByUser()
    finally:
        for s in ("stdout", "stderr"):
            if isinstance(kwargs[s], AsyncPipe):
                kwargs[s].close()

    for s in ("stdout", "stderr"):
        if isinstance(kwargs[s], AsyncPipe):
            result[s[3:]] = "\n".join(kwargs[s].get_buffer())

    for k, v in result.iteritems():
        if v and isinstance(v, basestring):
            result[k].strip()

    return result

def mkdir(dir):
    cmd = "mkdir -p %s" % dir
    subprocess.call(cmd, shell=True)

def copy_to_dir(old, new_dir):
    if not exists(new_dir):
        mkdir(new_dir);
    cmd = "cp -rf %s %s" %(old, new_dir)
    subprocess.call(cmd, shell=True)

def copy(old, new):
    cmd = "cp -rf %s %s" %(old, new)
    subprocess.call(cmd, shell=True)

def rm(name):
    cmd = "rm -rf %s" % name
    subprocess.call(cmd, shell=True)

def make_firmware(board, project):
    firmware_dir = get_firmware_dir()
    if board == 'neutron-net':
        cmd = "cd %s && make -s PLATFORM=neutron-net clean all COMPILE_LTO=n" % firmware_dir
    else:
        cmd = "cd %s && make -s PLATFORM=%s clean all COMPILE_LTO=n APP=%s" %(firmware_dir, board, project)
    subprocess.call(cmd, shell=True)

def calc_shasum(file_path):
    dlsha1 = None
    if exists(file_path):
        try:
            result = exec_command('sha1sum ' + file_path, shell=True)
            dlsha1 = result['out']
        except (OSError, ValueError):
            try:
                result = exec_command('shasum -a 1 ' + file_path, shell=True)
                dlsha1 = result['out']
            except (OSError, ValueError):
                pass
    if dlsha1:
        dlsha1 = dlsha1[1:41] if dlsha1.startswith("\\") else dlsha1[:40]

    return dlsha1

def load_json(file_path):
    try:
        with open(file_path, "r") as f:
            return json.load(f, object_pairs_hook=OrderedDict)
    except ValueError:
        print "Could not load broken JSON: %s" % file_path

def update_framework_json(board, version):
    file_path = join(get_framework_dir(board), 'package.json')
    if isfile(file_path):
        data = load_json(file_path)
        data['version'] = version

    with open(file_path, "w") as fp:
        json.dump(data, fp, indent=4, separators=(',', ': '), ensure_ascii=False)

def get_framework_version(board):
    if board == 'all':
        board = 'ant'
    file_path = join(get_framework_dir(board), 'package.json')
    version = ''
    if isfile(file_path):
        version = load_json(file_path)['version']
    return version

def update_platform_json(board, platform_version, framework_version):
    file_path = join(get_platform_dir(board), 'platform.json')
    if isfile(file_path):
        data = load_json(file_path)
        data['version'] = platform_version
        data['packages']['framework-intorobot-' + board]['version'] = '~' + framework_version

    with open(file_path, "w") as fp:
        json.dump(data, fp, indent=4, separators=(',', ': '), ensure_ascii=False)

def get_platform_version(board):
    file_path = join(get_platform_dir(board), 'platform.json')
    framework_version = ''
    platform_version = ''
    if isfile(file_path):
        data = load_json(file_path)
        platform_version = data['version']
        framework_version = data['packages']['framework-intorobot-' + board]['version'].replace('~', '')
    return platform_version, framework_version

def update_frameworks_manifest(board):
    version = get_framework_version(board)
    framework_name = 'framework-intorobot-' + board
    framework_tar_name = 'framework-intorobot-' + board + '-' + version + '.tar.gz'
    sha1 = calc_shasum(join(get_frameworks_dir(), framework_tar_name))
    file_path = get_frameworks_manifest_path()
    data = None
    if isfile(file_path):
        data = load_json(file_path)
        data[framework_name][0]['sha1'] = sha1
        data[framework_name][0]['url'] = join('http://dl.intoyun.com/intoyuniot/packages', framework_name, framework_tar_name)
        data[framework_name][0]['version'] = version

    with open(file_path, "w") as fp:
        json.dump(data, fp, indent=4, separators=(',', ': '), ensure_ascii=False)

def update_platforms_manifest(board):
    platform_vesion, framework_version = get_platform_version(board)
    platform_name = 'intorobot-' + board
    platform_full_name = 'platform-intorobot-' + board
    platform_tar_full_name = 'platform-intorobot-' + board + '-' + platform_vesion + '.tar.gz'
    file_path = get_platforms_manifest_path()
    if isfile(file_path):
        data = load_json(file_path)
        data[platform_name][0]['url'] = join('http://dl.intoyun.com/intoyuniot/platforms', platform_full_name, platform_tar_full_name)
        data[platform_name][0]['version'] = platform_vesion

    with open(file_path, "w") as fp:
        json.dump(data, fp, indent=4, separators=(',', ': '), ensure_ascii=False)

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

# 文件清理
def dir_clean(dir_path):
    sys_name = platform.system()
    if 'Darwin' in sys_name:
        # 删除.DS_Store临时文件
        cmd = "find %s -name \".DS_Store\" -delete" % dir_path
        subprocess.call(cmd, shell=True)

        # 修改文件属性，否则intoyuniot使用有问题
        cmd = "find %s -type f -exec xattr -c {} \\;" % dir_path
        subprocess.call(cmd, shell=True)
        cmd = "find %s -type d -exec xattr -c {} \\;" % dir_path
        subprocess.call(cmd, shell=True)

