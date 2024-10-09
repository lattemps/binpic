#    _   ,_,   _
#   / `'=) (='` \      binpic - Makefile.
#  /.-.-.\ /.-.-.\     Oct 9 2024
#         "      `
objs = binpic.o
flags = -Wall -Wextra -Wpedantic
exec = binpic

all: $(exec)

$(exec): $(objs)
	gcc	$(objs) -o $(exec)

%.o: %.c
	gcc	$(flags) -c $<

clean:
	rm	-f $(exec) $(objs) "\\"
