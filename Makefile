#Master Makefile for root directory

#TODO: export all variables possible

SERVERDIR=server
CLIENTDIR=client
COMMONDIR=common

export CC=gcc

all:
	$(MAKE) -C $(COMMONDIR)
	$(MAKE) -C $(SERVERDIR)
	$(MAKE) -C $(CLIENTDIR)

clean:
	$(MAKE) -C $(SERVERDIR) clean
	$(MAKE) -C $(CLIENTDIR) clean
	$(MAKE) -C $(COMMONDIR) clean