CXX = g++

build:
	$(CXX) src/* -I include/ -lpcap -lcrypto -o secret

debug:
	$(CXX) src/* -I include/ -D DEBUG -lpcap -g -lcrypto -o secret-debug

install: build
	cp secret /usr/local/bin

uninstall:
	rm -f /usr/local/bin/secret

clean:
	rm -f secret
	rm -f secret-debug
