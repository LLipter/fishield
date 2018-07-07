libfishield.so: *.cpp *.h *.proto
	protoc --cpp_out=. file_transfer.proto
	g++ file_transfer.pb.cc -c -fPIC
	g++ fishield.cpp	-c -fPIC
	g++ fs_client.cpp	-c -fPIC
	g++ fs_server.cpp	-c -fPIC
	g++ -shared *.o -o libfishield.so
	g++ main_server.cpp -lfishield -lboost_system -lboost_thread \
     -lpthread -lprotobuf -o server.out
	g++ main_client.cpp -lfishield -lboost_system -lboost_thread \
     -lpthread -lprotobuf -o client.out


clean:
	-rm *.o
	-rm libfishield.so
	-rm server.out
	-rm client.out
