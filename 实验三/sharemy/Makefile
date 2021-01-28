all: msender mreceiver
msender: msender.c
	gcc msender.c -o msender -pthread
mreceiver: mreceiver.c
	gcc mreceiver.c -o mreceiver -pthread
clean:
	rm msender mreceiver
