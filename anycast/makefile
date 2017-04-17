# @Author: Michel Andy <andy>
# @Date:   2017-04-02T19:44:52-04:00
# @Email:  Andymic12@gmail.com
# @Project: Anycast
# @Filename: makefile
# @Last modified by:   andy
# @Last modified time: 2017-04-16T09:02:46-04:00



CC			= g++
CFLAGS		= -std=c++11 -g -c -Wall
LDFLAGS		=
SERV_SOURCES= server/Server.cc
CLI_SOURCES = client/Client.cc
BASE_SOURCES= common/TCPSocket.cc common/Packet.cc $(SERV_SOURCES) $(CLI_SOURCES)
JAP_SOURCES = server/JoinProxy.cc
IAP_SOURCES = server/IngressProxy.cc
RAP_SOURCES = server/RendezvousProxy.cc
TAP_SOURCES = server/Target.cc
AC_SOURCES	= client/AnycastClient.cc
INCLUDES	= -Icommon/ -Iserver/ -Iclient/
BASE_OBJECTS= $(BASE_SOURCES:.cc=.o)
SERV_OBJECTS= $(SERV_SOURCES:.cc=.o)
JAP_OBJECTS = $(JAP_SOURCES:.cc=.o)
RAP_OBJECTS = $(RAP_SOURCES:.cc=.o)
IAP_OBJECTS = $(IAP_SOURCES:.cc=.o)
TAP_OBJECTS = $(TAP_SOURCES:.cc=.o)
AC_OBJECTS  = $(AC_SOURCES:.cc=.o)
CLI_OBJECTS = $(CLI_SOURCES:.cc=.o)
SERVER		= serv
IAP			= ingressproxy
JAP			= joinproxy
RAP			= rendezvousproxy
TAP			= target
CLIENT		= cli


all: $(CLIENT) $(IAP) $(JAP) $(RAP) $(TAP)

$(SERVER): $(BASE_OBJECTS) $(SERV_OBJECTS)
	$(CC) $(LDFLAGS) $(BASE_OBJECTS) $(SERV_OBJECTS) -o $@

$(JAP): $(BASE_OBJECTS) $(JAP_OBJECTS)
	$(CC) $(LDFLAGS) $(BASE_OBJECTS) $(JAP_OBJECTS) -o $@

$(IAP): $(BASE_OBJECTS) $(IAP_OBJECTS)
	$(CC) $(LDFLAGS) $(BASE_OBJECTS) $(IAP_OBJECTS) -o $@

$(RAP): $(BASE_OBJECTS) $(RAP_OBJECTS)
	$(CC) $(LDFLAGS) $(BASE_OBJECTS) $(RAP_OBJECTS) -o $@

$(TAP): $(BASE_OBJECTS) $(TAP_OBJECTS)
	$(CC) $(LDFLAGS) $(BASE_OBJECTS) $(TAP_OBJECTS) -o $@

$(CLIENT): $(BASE_OBJECTS) $(AC_OBJECTS)
	$(CC) $(LDFLAGS) $(BASE_OBJECTS) $(AC_OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
clean:
	rm -rf $(BASE_OBJECTS) $(SERV_OBJECTS) $(CLI_OBJECTS)\
	       $(IAP_OBJECTS) $(RAP_OBJECTS) $(JAP_OBJECTS)\
		   $(TAP_OBJECTS) $(AC_OBJECTS) $(SERVER) $(CLIENT)\
		   $(JAP) $(IAP) $(RAP) $(TAP)
