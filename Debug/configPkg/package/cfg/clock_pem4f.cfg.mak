# invoke SourceDir generated makefile for clock.pem4f
clock.pem4f: .libraries,clock.pem4f
.libraries,clock.pem4f: package/cfg/clock_pem4f.xdl
	$(MAKE) -f C:\Users\Andres\workspace_v7\clock_EK_TM4C123GXL_TI/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Andres\workspace_v7\clock_EK_TM4C123GXL_TI/src/makefile.libs clean

