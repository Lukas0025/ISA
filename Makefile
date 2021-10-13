CXX = g++

build:
	$(CXX) src/* -I include/ -o secret

debug:
	$(CXX) src/* -I include/ -D DEBUG -o secret-debug

install: build
	cp secret /usr/local/bin

uninstall:
	rm -f /usr/local/bin/secret

clean:
	rm -f secret
	rm -f secret-debug
