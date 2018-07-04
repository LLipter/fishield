project: libfishield.so
	mkdir build/server
	mkdir build/client
	g++ main_server.cpp -L./build/ -lfishield -lboost_system -lboost_thread \
     -lpthread -lprotobuf -lssl -lcrypto -o build/server/server.out
	g++ main_client.cpp -L./build/ -lfishield -lboost_system -lboost_thread \
     -lpthread -lprotobuf -lssl -lcrypto -o build/client/client.out
	rm build/*.o

libfishield.so: fs_protobuf.o fishield.o fs_callback.o fs_client.o \
 fs_error.o fs_scheduler.o fs_server.o fs_task.o
	g++ -shared build/fs_protobuf.o build/fishield.o build/fs_callback.o \
     build/fs_client.o build/fs_error.o build/fs_scheduler.o \
     build/fs_server.o build/fs_task.o -o build/libfishield.so

fishield.o: fishield.cpp fishield.h fs_callback.h fs_config.h fs_client.h \
 protobuf/file_transfer.pb.h fs_scheduler.h fs_task.h fs_task_info.h \
 fs_error.h fs_server.h
	g++ fishield.cpp -c -fPIC -o build/fishield.o

fs_protobuf.o: protobuf/file_transfer.proto
	protoc --cpp_out=. protobuf/file_transfer.proto
	g++ protobuf/file_transfer.pb.cc -I. -c -fPIC -o build/fs_protobuf.o

fs_callback.o: fs_callback.cpp fs_callback.h fs_config.h
	g++ fs_callback.cpp -c -fPIC -o build/fs_callback.o

fs_client.o: fs_client.cpp fs_client.h fs_config.h \
 protobuf/file_transfer.pb.h
	g++ fs_client.cpp -c -fPIC -o build/fs_client.o

fs_error.o: fs_error.cpp fs_error.h fs_config.h
	g++ fs_error.cpp -c -fPIC -o build/fs_error.o

fs_scheduler.o: fs_scheduler.cpp fs_scheduler.h fs_task.h fs_config.h \
 fs_task_info.h protobuf/file_transfer.pb.h
	g++ fs_scheduler.cpp -c -fPIC -o build/fs_scheduler.o

fs_server.o: fs_server.cpp fs_server.h fs_config.h \
 protobuf/file_transfer.pb.h fs_error.h
	g++ fs_server.cpp -c -fPIC -o build/fs_server.o

fs_task.o: fs_task.cpp fs_task.h fs_config.h fs_task_info.h \
 protobuf/file_transfer.pb.h fs_callback.h fs_client.h
	g++ fs_task.cpp -c -fPIC -o build/fs_task.o

clean:
	rm -rf build/*
