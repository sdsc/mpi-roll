NAME    = openmpi_$(ROLLCOMPILER)_$(ROLLNETWORK)
VERSION = 1.6
RELEASE = 4
LOADMGR =
ifdef SGE
LOADMGR += --with-sge
endif
ifdef TORQUE
LOADMGR += --with-tm /opt/torque
endif
