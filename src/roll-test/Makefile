REDHAT.ROOT = $(CURDIR)/../../

-include $(ROCKSROOT)/etc/Rules.mk
include Rules.mk
include $(SDSCDEVEL)/Rules.mk

build:

install:: typical-roll-test-install
	perl -pi -e 's#ROLLNETWORK#$(ROLLNETWORK)#g;' $(ROOT)/$(PKGROOT)/*.t

clean::
