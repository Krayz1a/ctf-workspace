import hashlib

# The first 4 hex characters of your target hash
target_prefix = "537500" 

i = 0
while True:
    # Converting the number to a string to use as input
    guess = str(i).encode()
    # Calculating MD5
    h = hashlib.md5(guess).hexdigest()
    
    # Checking if the first 4 characters match
    if h.startswith(target_prefix):
        print(f"--- MATCH FOUND ---")
        print(f"Input string: {i}")
        print(f"MD5 Hash:     {h}")
        break
    
    i += 1
    # Safety break if it takes too long (though 4 hex chars is fast)
    if i > 1000000000:
        print("Increasing search range...")
