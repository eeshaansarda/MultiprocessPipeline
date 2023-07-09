CC = clang
RM = rm -f
DFLAG = -g
GFLAGS = -Wall -Wextra
CFLAGS = $(DFLAG) $(GFLAGS) -c
LFLAGS = $(DFLAG) $(GFLAGS)

all: SumSquaresPipeline SievePipeline TestPipeline

SumSquaresPipeline: SumSquaresPipeline.o Pipeline.o 
	$(CC) $(LFLAGS) SumSquaresPipeline.o Pipeline.o -o SumSquaresPipeline

SievePipeline: SievePipeline.o Pipeline.o 
	$(CC) $(LFLAGS) SievePipeline.o Pipeline.o -o SievePipeline

TestPipeline: TestPipeline.o Pipeline.o
	$(CC) $(LFLAGS) TestPipeline.o Pipeline.o -o TestPipeline

SievePipeline.o: SievePipeline.c
	$(CC) $(CFLAGS) SievePipeline.c

SumSquaresPipeline.o: SumSquaresPipeline.c
	$(CC) $(CFLAGS) SumSquaresPipeline.c

TestPipeline.o: TestPipeline.c Pipeline.h myassert.h
	$(CC) $(CFLAGS) TestPipeline.c


Pipeline.o: Pipeline.c Pipeline.h
	$(CC) $(CFLAGS) Pipeline.c


clean:
	$(RM) SumSquaresPipeline SievePipeline TestPipeline *.o
