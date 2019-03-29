all:
	$(MAKE) -C src all
	$(MAKE) -C examples all

clean:
	$(MAKE) -C src clean
	$(MAKE) -C examples clean

compile:
	$(MAKE) -C src compile
	$(MAKE) -C examples compile

run:
	$(MAKE) -C examples run
