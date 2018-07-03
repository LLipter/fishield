libfishield.so: fishield.cpp fishield.h
	mkdir build
	g++ -c -fPIC fishield.cpp -o build/fishield.o
	g++ -shared build/fishield.o -o build/libfisheild.so
	g++ -L./build/ -lfishield main.cpp -o main.out

clean:
	rm -rf build/
	rm main.out
