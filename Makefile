libfishield.so: *.cpp *.h *.proto
	g++ fishield.cpp -c -fPIC
	g++ file_transfer.pb.cc -c -fPIC
	g++ fs_callback.cpp -c -fPIC
	g++ fs_client.cpp -c -fPIC
	g++ fs_error.cpp -c -fPIC
	g++ fs_scheduler.cpp -c -fPIC
	g++ fs_server.cpp -c -fPIC
	g++ fs_task.cpp -c -fPIC
	g++ -shared *.o -o libfishield.so
	g++ main_server.cpp -lfishield -lboost_system -lboost_thread \
     -lpthread -lprotobuf -lssl -lcrypto -o server.out
	g++ main_client.cpp -lfishield -lboost_system -lboost_thread \
     -lpthread -lprotobuf -lssl -lcrypto -o client.out


clean:
	-rm *.o
	-rm libfishield.so
	-rm server.out
	-rm client.out
