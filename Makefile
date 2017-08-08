default: all

all:
	(cd src; make)
	(cp src/hasher.o .;)

test:
	(cd src; make test)
	(cp src/hasher .;)

clean:
	rm -rf hasher.o hasher
	(cd src; make clean)
