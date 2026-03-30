import socket
import time

HOST = "researchlabs.tech"
PORT = 1341

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.settimeout(0.5)  # short per-recv timeout; we implement our own overall timeout
sock.connect((HOST, PORT))

buf = b""

def write(s: str):
    sock.sendall(s.encode())

def sendline(s: str):
    write(s + "\n")

def read_until(token: bytes, overall_timeout=10.0, max_bytes=500_000) -> bytes:
    """
    Read until `token` appears. Returns data up to and including token.
    Keeps any extra bytes in `buf` for the next call.
    """
    global buf
    deadline = time.monotonic() + overall_timeout

    while True:
        # if token already in buffer, split and return
        idx = buf.find(token)
        if idx != -1:
            idx_end = idx + len(token)
            out = buf[:idx_end]
            buf = buf[idx_end:]
            return out

        if time.monotonic() > deadline:
            raise TimeoutError(f"Timed out waiting for {token!r}. Last bytes: {buf[-200:]!r}")

        try:
            chunk = sock.recv(4096)
        except socket.timeout:
            continue

        if not chunk:
            raise ConnectionError("Server closed the connection")

        buf += chunk
        if len(buf) > max_bytes:
            raise RuntimeError("too much data without seeing token")

# ----------------- your logic -----------------
k = 308

# Wait for first prompt (don’t pre-read/print; it can consume it)
print(read_until(b"choose group_sz").decode(errors="replace"), end="")
sendline(str(k))

for i in range(k):
    print(read_until(b"choose type").decode(errors="replace"), end="")
    if i == 0:
        sendline("0")
        print(read_until(b"zone").decode(errors="replace"), end="");     sendline("0")
        print(read_until(b"building").decode(errors="replace"), end=""); sendline("0")
        print(read_until(b"floor").decode(errors="replace"), end="");    sendline("0")
        print(read_until(b"room").decode(errors="replace"), end="");     sendline("10")
    else:
        sendline("2")
        print(read_until(b"other_person").decode(errors="replace"), end=""); sendline("0")
        # prompt text contains "gym" / "pool" in the sentence
        print(read_until(b"gym").decode(errors="replace"), end="");         sendline("pool")

for i in range(307):
    # print the room prompt line (we sync to the fixed substring)
    print(read_until(b"choose leisure activities for room").decode(errors="replace"), end="")

    if i == 306:
        print(read_until(b"spa").decode(errors="replace"), end="");     sendline("6")
        print(read_until(b"food").decode(errors="replace"), end="");    sendline("9")
        print(read_until(b"minibar").decode(errors="replace"), end=""); sendline("160")
        print(read_until(b"movie").decode(errors="replace"), end="");   sendline("3")
    else:
        print(read_until(b"spa").decode(errors="replace"), end="");     sendline("2")
        print(read_until(b"food").decode(errors="replace"), end="");    sendline("3")
        print(read_until(b"minibar").decode(errors="replace"), end=""); sendline("0")
        print(read_until(b"movie").decode(errors="replace"), end="");   sendline("1")

# budget
sendline("4000")

# read remaining output
sock.settimeout(1.0)
try:
    while True:
        data = sock.recv(4096)
        if not data:
            break
        print(data.decode(errors="replace"), end="")
except socket.timeout:
    pass

sock.close()

