#!/usr/bin/env python3

import imghdr
import os
from typing import Optional, Tuple
import struct
import subprocess
import sys
import re


PROMPT = '... '
SHELL = os.environ.get(key='SHELL', default='sh')
USE_RLE = {True: True, False: True}
CONFIG_PATH = 'settings.conf'
QUANT_MATRIX_PATH = 'matrix.txt'
BUILD_DIR = 'cmake-build-debug'
TEST_DIR = 'test'
IN_EXTENSIONS = {'jpg', 'png'}
IN_EXTENSION = 'in'
OUT_EXTENSION = 'out'
DIFF_EXTENSION = 'diff'


def build_all() -> None:
    shell_run('cmake --build ' + BUILD_DIR + ' --target all -- -j 4')


def convert_to_image(in_path: str, out_path: str, dimensions: Tuple[int, int]) -> None:
    shell_run('convert -size %ix%i -depth 8 gray:%s.raw %s.png'
              % (dimensions[0], dimensions[1], in_path, out_path))


def convert_to_raw(in_path: str, out_path: str, dimensions: Tuple[int, int]) -> None:
    shell_run('convert %s -resize %ix%i! gray:%s.%s.raw'
              % (in_path, dimensions[0], dimensions[1], out_path, IN_EXTENSION))


def decode():
    shell_run('./%s/decoder %s' % (BUILD_DIR, CONFIG_PATH))


def encode():
    shell_run('./%s/encoder %s' % (BUILD_DIR, CONFIG_PATH))


def get_image_dimensions(file_path: str) -> Optional[Tuple[int, int]]:
    with open(file_path, 'rb') as f:
        head = f.read(24)

        if len(head) != 24:
            return

        if imghdr.what(file_path) == 'png':
            check = struct.unpack('>i', head[4:8])[0]

            if check != 0x0d0a1a0a:
                return

            width, height = struct.unpack('>ii', head[16:24])
        elif imghdr.what(file_path) == 'jpeg':
            try:
                f.seek(0)
                size = 2
                ftype = 0

                while not 0xc0 <= ftype <= 0xcf:
                    f.seek(size, 1)
                    byte = f.read(1)

                    while ord(byte) == 0xff:
                        byte = f.read(1)

                    ftype = ord(byte)
                    size = struct.unpack('>H', f.read(2))[0] - 2

                f.seek(1, 1)
                height, width = struct.unpack('>HH', f.read(4))
            except Exception:
                return
        else:
            return

        return width, height


def main() -> None:
    if not os.path.exists(TEST_DIR):
        os.makedirs(TEST_DIR)

    for f in os.listdir(TEST_DIR):
        if re.search('.*\.in\..*', f) or re.search('.*\.out\..*', f) or re.search('.*\.diff\..*', f) or re.search('.*\.raw', f) or re.search('.*\.enc', f) or re.search('.*\.log', f):
            os.remove(os.path.join(TEST_DIR, f))

    if len(sys.argv) >= 2 and sys.argv[1] == 'clear':   
        return

    build_all()

    for filename in os.listdir(TEST_DIR):
        for extension in IN_EXTENSIONS:
            if filename.endswith('.%s' % (extension)) and not filename.endswith('.%s.%s' % (IN_EXTENSION, extension)) and not filename.endswith('.%s.%s' % (OUT_EXTENSION, extension))and not filename.endswith('.%s.%s' % (DIFF_EXTENSION, extension)):
                file_path = TEST_DIR + '/' + filename
                base_path = file_path.rsplit('.', 1)[0]
                dimensions = get_image_dimensions(file_path)

                convert_to_raw(file_path, base_path, dimensions)
                convert_to_image(base_path + '.' + IN_EXTENSION, base_path +
                                '.' + IN_EXTENSION, dimensions)

                for use_rle in USE_RLE:
                    if not USE_RLE[use_rle]:
                        continue

                    config_file_contents = \
                        'rawfile=%s.%s.raw\n'\
                        'encfile=%s.enc\n'\
                        'decfile=\n'\
                        'width=%i\n'\
                        'height=%i\n'\
                        'rle=%s\n'\
                        'quantfile=%s\n'\
                        'logfile=%s.enc.log\n' % (
                            base_path, IN_EXTENSION,
                            base_path + ('.rle' if use_rle else ''),
                            dimensions[0],
                            dimensions[1],
                            '1' if use_rle else '0',
                            QUANT_MATRIX_PATH,
                            base_path + ('.rle' if use_rle else '')
                        )

                    with open(CONFIG_PATH, 'w') as f:
                        f.write(config_file_contents)

                    encode()

                    config_file_contents = \
                        'rawfile=\n'\
                        'encfile=%s.enc\n'\
                        'decfile=%s.%s.raw\n'\
                        'width=\n'\
                        'height=\n'\
                        'rle=\n'\
                        'quantfile=\n'\
                        'logfile=%s.dec.log\n' % (
                            base_path + ('.rle' if use_rle else ''),
                            base_path + ('.rle' if use_rle else ''), OUT_EXTENSION,
                            base_path + ('.rle' if use_rle else '')
                        )

                    with open(CONFIG_PATH, 'w') as f:
                        f.write(config_file_contents)

                    decode()

                    convert_to_image(base_path + ('.rle' if use_rle else '') + '.' + OUT_EXTENSION, base_path + ('.rle' if use_rle else '') + '.' +
                                    OUT_EXTENSION, dimensions)
                    
                    shell_run('blink-diff --output %s.%s.png %s.%s.png %s.%s.png' % (
                        base_path + ('.rle' if use_rle else ''), DIFF_EXTENSION,
                        base_path, IN_EXTENSION,
                        base_path + ('.rle' if use_rle else ''), OUT_EXTENSION,
                    ))

                    shell_run('eog %s.%s.png&' % (base_path + ('.rle' if use_rle else ''), DIFF_EXTENSION))


def shell_run(command: str) -> None:
    print(PROMPT + command)

    process = subprocess.Popen(SHELL, stdin=subprocess.PIPE)
    process.communicate((command + '\n').encode())


main()
