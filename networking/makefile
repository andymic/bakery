CC		= g++
CFLAGS		= -std=c++11 -g -c -Wall 
LDFLAGS		= 
BASE_SOURCES= common/TCPConnector.cc common/TCPSocket.cc common/Packet.cc
SERV_SOURCES= server/Server.cc
CLI_SOURCES = client/Client.cc
INCLUDES	= -Icommon/ -Iserver/ -Iclient/ 
BASE_OBJECTS= $(BASE_SOURCES:.cc=.o)
SERV_OBJECTS= $(SERV_SOURCES:.cc=.o)
CLI_OBJECTS = $(CLI_SOURCES:.cc=.o)
SERVER		= serv
CLIENT		= cli

all: $(SERVER) $(CLIENT)

$(SERVER): $(BASE_OBJECTS) $(SERV_OBJECTS)
	$(CC) $(LDFLAGS) $(BASE_OBJECTS) $(SERV_OBJECTS) -o $@

$(CLIENT): $(BASE_OBJECTS) $(CLI_OBJECTS)
	$(CC) $(LDFLAGS) $(BASE_OBJECTS) $(CLI_OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
clean:
	rm -rf $(BASE_OBJECTS) $(SERV_OBJECTS) $(CLI_OBJECTS) $(SERVER) $(CLIENT)

