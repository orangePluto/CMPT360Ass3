########################################
#Damion Shillinglaw
#ID: 3069536
#Instructor: Cam Macdonell
########################################

CFLAGS = -Wall -std=c99
.PHONY: clean all

all: gentf-mr-1

gentf-mr-1: gentf-mr-1.c gentfCommon.h gentfCommon.c gentfThreads.h gentfThreads.c
	gcc $(CFLAGS)  -o gentf-mr-1 gentf-mr-1.c gentfCommon.c gentfThreads.c

clean:
	rm gentf-mr-1

