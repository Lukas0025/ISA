# BUT FIT ISA Project
###### transfer file with AES encription using ICMP echo packets (PING)

## Build and install install

```sh
make install
```

## Using

```sh
secret  -r <file> -s <ip|hostname> [-l] [-h]
```

## send file example

#### server (10.0.0.1)

```sh
sudo secret -s myfile -l
```

#### client

```sh
secret  -r /dir/to/file -s 10.0.0.1
```

## Makefile targets

`build` build whole project

`debug` build whole project with debug flags for debug

`install` copy binary to /usr/local/bin/secret

`uninstall` remove binary from /usr/local/bin/secret

`clean` clean repository
