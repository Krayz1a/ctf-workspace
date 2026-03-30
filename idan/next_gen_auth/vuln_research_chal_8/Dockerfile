# build vm
FROM ubuntu:22.04 AS builder

# # install needed resources for buliding
RUN apt-get update && apt-get install -y \
    build-essential cmake g++ libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# workdir for the build
WORKDIR /chal_srcs

# copy chal srcs
COPY . .

# build the executable
RUN chmod +x build_linux.sh && ./build_linux.sh


# runner vm
FROM ubuntu:22.04 AS runner

# install needed resources
RUN apt-get update && apt-get install -y \
    socat \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# create a user for the chal
RUN useradd -m -s /bin/bash ctf

# workdir for the chal
WORKDIR /app

# copy the built executable and the flag from the build vm
COPY --from=builder /chal_srcs/build/linux/vuln_research_chal_8 /app/chal
COPY --from=builder /chal_srcs/research_labs/flag.txt /app/flag.txt

# set proper permissions
RUN chown -R root:ctf /app && \
    chmod 750 /app/chal && \
    chmod 440 /app/flag.txt

# set the user
USER ctf

EXPOSE 1337

CMD ["socat", "-T60", "TCP-LISTEN:1337,reuseaddr,fork", "EXEC:/app/chal,stderr"]