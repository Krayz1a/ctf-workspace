FROM ubuntu:20.04

# Setup user
RUN useradd -d /home/professor/ -m -s /bin/bash professor

WORKDIR /home/professor

# Copy the specific challenge files
COPY professor .
COPY flag .
COPY libc.so.6 .
COPY ld-2.31.so .
COPY libpthread-2.31.so .
# If the challenge needs a wrapper like ynetd, copy it too
# COPY ynetd . 

# Ensure permissions are correct
RUN chown -R root:root /home/professor && chmod +x professor

# Set up the execution environment
USER professor
EXPOSE 1024

# We use the provided ld-linux to ensure it uses the provided libc
CMD ["./ld-2.31.so", "--library-path", ".", "./professor"]
