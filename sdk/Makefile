obj = 	file_transfer.pb.o fs_task.pb.o fishield.o \
	fs_client.o fs_server.o fs_task.o fs_scheduler.o fs_dbmanager.o
protohead = 	file_transfer.pb.cc file_transfer.pb.h \
		fs_task.pb.cc fs_task.pb.h
PWD := $(shell pwd)
linklib = 	-lboost_system -lboost_thread -lboost_filesystem \
     		-lpthread -lprotobuf -lmysqlcppconn

all: libfishield.so server.out client.out fsconfig.out

server.out: main_server.cpp libfishield.so
	g++ main_server.cpp -std=c++11 -L${PWD} -lfishield \
 	${linklib} -o server.out

client.out: main_client.cpp libfishield.so
	g++ main_client.cpp -std=c++11 -L${PWD} -lfishield \
	${linklib} -o client.out

fsconfig.out: admin_tool.cpp libfishield.so
	g++ admin_tool.cpp -std=c++11 -L${PWD} -lfishield \
	${linklib} -o client.out

libfishield.so: ${obj}
	g++ -shared ${obj} -o libfishield.so
	sudo cp libfishield.so /usr/lib

file_transfer.pb.cc file_transfer.pb.h: file_transfer.proto
	protoc --cpp_out=. file_transfer.proto
	
fs_task.pb.cc fs_task.pb.h: fs_task.proto
	protoc --cpp_out=. fs_task.proto
	
file_transfer.pb.o: file_transfer.pb.cc file_transfer.pb.h
	g++ file_transfer.pb.cc -std=c++11 	-c -fPIC

fs_task.pb.o: fs_task.pb.cc fs_task.pb.h
	g++ fs_task.pb.cc 	-std=c++11	-c -fPIC

fishield.o: fishield.cpp fishield.h fs_config.h file_transfer.pb.h \
 fs_task.pb.h fs_client.h fs_server.h fs_task.h fs_scheduler.h
	g++ fishield.cpp 	-std=c++11	-c -fPIC

fs_client.o: fs_client.cpp fs_client.h fs_config.h file_transfer.pb.h \
 fs_task.pb.h fs_scheduler.h fs_task.h
	g++ fs_client.cpp 	-std=c++11	-c -fPIC

fs_server.o: fs_server.cpp fs_server.h fs_config.h file_transfer.pb.h \
 fs_task.pb.h fs_task.h fs_dbmanager.h fs_client.h
	g++ fs_server.cpp 	-std=c++11	-c -fPIC

fs_task.o: fs_task.cpp fs_task.h fs_config.h file_transfer.pb.h \
 fs_task.pb.h fs_client.h fs_scheduler.h
	g++ fs_task.cpp		-std=c++11	-c -fPIC

fs_scheduler.o: fs_scheduler.cpp fs_scheduler.h fs_config.h \
 file_transfer.pb.h fs_task.pb.h fs_task.h fs_client.h
	g++ fs_scheduler.cpp 	-std=c++11	-c -fPIC

fs_dbmanager.o: fs_dbmanager.cpp fs_dbmanager.h fs_client.h fs_config.h \
 file_transfer.pb.h fs_task.pb.h
	g++ fs_dbmanager.cpp 	-std=c++11	-c -fPIC

clean:
	-rm ${obj}
	-rm ${protohead}
