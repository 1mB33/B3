import io
import os
import sys
import time as t
import zlib


DEBUG = True
ENDIAN = 'little'
TYPE_MESH = b'mesh'


def print_debug(s: str):
    if (DEBUG):
        print('[DEBUG TRACE]: ', end='')
        print(s)
    pass


def print_help() -> None:
    print("""
          TODO: Write help message
    """)


def validate_type(type: bytes) -> bool:
    if (type != TYPE_MESH):
        return False
    return True


def validadte_and_process_file_type_postfix(fileType: str) -> str:
    if fileType.lower() == "mesh":
        return "_Mesh"

    raise Exception(f"Unkonwn fileType: {fileType}")


def construct_file_name(name: str, fileType: str) -> str:
    return name + validadte_and_process_file_type_postfix(fileType) + ".b33asset"


def generate_header(file: io.BufferedWriter, type: bytes) -> bytes:
    result: bytes
    result = bytes()

    if (len(type) != 4 or not validate_type(type)):
        raise Exception(f"Bad type on generate_header: {type}")

    creation_time = int(t.time())
    file_beg = file.tell()
    file.seek(0, os.SEEK_END)
    file_len = file.tell() - file_beg
    file.seek(0, os.SEEK_DATA)

    print_debug(f'creation_time = {creation_time}')

    result = result.__add__(bytes.fromhex('00'))
    result = result.__add__(b"B33")
    result = result.__add__(b"----")
    result = result.__add__(creation_time.to_bytes(8, ENDIAN))
    result = result.__add__(type)
    result = result.__add__(b"----")
    result = result.__add__(file_len.to_bytes(8, ENDIAN))

    return result


def process_obj_file(file: io.BufferedWriter) -> bytes:
    result: bytes
    result = bytes()
    result = result.__add__(generate_header(file, TYPE_MESH))
    result = result.__add__(file.read())

    # length = len(result)
    # if (length % 4 != 0):
    #     raise Exception(f"Bad aligment: {length}")

    return result


def process_file(file: io.BufferedWriter, fileType: str, ext: str) -> bytes:
    if fileType == "mesh":
        if ext != ".obj":
            raise Exception(f"Extention doesn't match the requested file type, or isn't supported: {ext}, {fileType}")
        return process_obj_file(file)
    raise Exception(f"Unkonwn extention: {ext}")


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Not enough arguments provided")
        print_help()
        exit(-1)
    try:
        sourceFilePath = sys.argv[1]
        sourceFileName = os.path.splitext(sys.argv[1])[0]
        sourceFileExt = os.path.splitext(sys.argv[1])[1]
        destFileType = sys.argv[2]

        with open(sourceFilePath, "rb") as sourceFile:
            with open(construct_file_name(sourceFileName, destFileType), "wb") as destFile:
                compressed = process_file(sourceFile, destFileType, sourceFileExt)
                #compressed = zlib.compress(process_file(sourceFile, destFileType, sourceFileExt), level=zlib.Z_BEST_COMPRESSION, wbits=-15)
                destFile.write(compressed)

        with open(construct_file_name(sourceFileName, destFileType), "rb") as destFile:
            compressed = destFile.read()
            print_debug(f"String: {compressed}")
    except Exception as e:
        print(str(e))
