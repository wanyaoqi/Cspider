all :  spider

#-L /usr/lib/mysql -l mysqlclient -lz -lm
#-I /usr/include/mysql -L /usr/lib/mysql -l mysqlclient -lz -lm
#object1=$(mysql_config --cflags --libs)

spdier : crawler.o regex.o sqlspider.o spider.o
	g++ -o crawler.o regex.o sqlspider.o spider.o
crawler.o : crawler.cpp crawler.h
	g++ -c crawler.cpp
regex.o : regex.cpp regex.h
	g++ -c regex.cpp
sqlspider.o : sqlspider.cpp sqlspider.h
	g++ -c sqlspider.cpp -L /usr/lib/mysql -l mysqlclient -lz -lm  
spider.o : spider.cpp
	g++ -c spider.cpp
clean :
	remove *.o main

