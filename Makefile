libfishield.so: fishield.cpp fishield.h fs_protobuf
	rm -rf build*
	mkdir build/
	g++ -c -fPIC fishield.cpp -o build/fishield.o
	g++ -shared build/fishield.o -o build/libfisheild.so
	g++ -L./build/ -lfishield -lboost_system main.cpp -o main.out

fs_protobuf:
	protoc --cpp_out=. protobuf/file_transfer.proto 


clean:
	rm -rf build*
	rm *.out
