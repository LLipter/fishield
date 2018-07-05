objfile = file_transfer.pb.o fishield.o fs_callback.o fs_client.o \
          fs_error.o fs_scheduler.o fs_server.o fs_task.o

headerfile = file_transfer.pb.h fishield.h fs_callback.h fs_client.h \
             fs_error.h fs_scheduler.h fs_server.h fs_task.h

srcfile = file_transfer.pb.cc fishield.cpp fs_callback.cpp fs_client.cpp \
             fs_error.cpp fs_scheduler.cpp fs_server.cpp fs_task.cpp

libfishield.so: $(objfile) main_server.cpp main_client.cpp
	g++ -shared $(objfile) -o libfishield.so
	g++ main_server.cpp -L. -lfishield -lboost_system -lboost_thread \
     -lpthread -lprotobuf -lssl -lcrypto -o server.out
	g++ main_client.cpp -L. -lfishield -lboost_system -lboost_thread \
     -lpthread -lprotobuf -lssl -lcrypto -o client.out

fishield.o: fishield.cpp fishield.h fs_callback.h fs_config.h fs_client.h \
 file_transfer.pb.h fs_scheduler.h fs_task.h fs_task_info.h \
 fs_error.h fs_server.h
	g++ fishield.cpp -c -fPIC

file_transfer.pb.o: file_transfer.proto
	protoc --cpp_out=. file_transfer.proto
	g++ file_transfer.pb.cc -c -fPIC

fs_callback.o: fs_callback.cpp fs_callback.h fs_config.h
	g++ fs_callback.cpp -c -fPIC

fs_client.o: fs_client.cpp fs_client.h fs_config.h \
 file_transfer.pb.h
	g++ fs_client.cpp -c -fPIC

fs_error.o: fs_error.cpp fs_error.h fs_config.h
	g++ fs_error.cpp -c -fPIC

fs_scheduler.o: fs_scheduler.cpp fs_scheduler.h fs_task.h fs_config.h \
 fs_task_info.h file_transfer.pb.h
	g++ fs_scheduler.cpp -c -fPIC

fs_server.o: fs_server.cpp fs_server.h fs_config.h \
 file_transfer.pb.h fs_error.h
	g++ fs_server.cpp -c -fPIC

fs_task.o: fs_task.cpp fs_task.h fs_config.h fs_task_info.h \
 file_transfer.pb.h fs_callback.h fs_client.h
	g++ fs_task.cpp -c -fPIC

clean:
	-rm *.o
	-rm libfishield.so
	-rm server.out
	-rm client.out
