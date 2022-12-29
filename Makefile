#Master Makefile for root directory

COMMONPROJ=common
SERVERPROJ=server
CLIENTPROJ=client

export CC=gcc

#Headers and Objects
#TODO: find a way to not have to call them by name, and auto-detect instead
#==============================================================================
export _COMMONDEPS=config.h util.h
export _COMMONOBJ=util.o

export _SERVERDEPS=blacklist.h
export _SERVEROBJ=server.o blacklist.o

export _CLIENTDEPS=
export _CLIENTOBJ=client.o

#Rules
#==============================================================================
default: common server client

common:
	$(MAKE) -C $(COMMONPROJ) -f $(COMMONPROJ).mk
	
server:
	$(MAKE) -C $(SERVERPROJ) -f $(SERVERPROJ).mk
	
client:
	$(MAKE) -C $(CLIENTPROJ) -f $(CLIENTPROJ).mk

clean: clean-common clean-server clean-client	
	
clean-common:
	$(MAKE) -C $(COMMONPROJ) -f $(COMMONPROJ).mk clean

clean-server:
	$(MAKE) -C $(SERVERPROJ) -f $(SERVERPROJ).mk clean
	
clean-client:
	$(MAKE) -C $(CLIENTPROJ) -f $(CLIENTPROJ).mk clean
	
.PHONY: default common server client clean clean-common clean-server clean-client
