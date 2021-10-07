CXX = g++

build:
	$(CXX) src/* -I include/ -o secret

install: build
	cp secret /usr/local/bin

uninstall:
	rm -f /usr/local/bin/secret

clean:
	rm -f secret
