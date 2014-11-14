REDHAT.ROOT = $(CURDIR)/../../

-include $(ROCKSROOT)/etc/Rules.mk
include Rules.mk
include $(SDSCDEVEL)/Rules.mk

$(VERSION_INC): $(VERSION_SRC)
	/bin/grep 'VERSION.*=' $(VERSION_SRC) > $@
 
build:

install:: typical-modulefile-install
	perl -pi -e 's#ROLLNETWORK#$(ROLLNETWORK)#g;' \
	  $(ROOT)/$(PKGROOT)/.version.$(VERSION) $(ROOT)/$(PKGROOT)/$(VERSION)

clean::
	rm -f $(VERSION_INC)
