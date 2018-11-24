## Makefile to compile and load kernel loadable modules
## Author: Jonah Kubath
## Date: 11/22/2018
## Class: CS4540 Operating Systems

all: build_good build_bad

print:
	(cd "./module"; echo $(CURDIR);)

build_good:
	make -C /lib/modules/$(shell uname -r)/build M=$(CURDIR)/module modules

load_good: build_good
	sudo insmod ./module/jonah_module.ko

remove_good:
	sudo rmmod jonah_module

clean_good:
	(cd "./module"; rm -f .*.cmd .cache.mk jonah_module.ko jonah_module.*.* jonah_module.o modules.order Module.symvers; rm -f -d -r .tmp_versions)


build_bad:
	make -C /lib/modules/$(shell uname -r)/build M=$(CURDIR)/module_bad modules;)

load_bad: build_bad
	sudo insmod ./module_bad/jonah_module_bad.ko

remove_bad:
	sudo rmmod jonah_module_bad

clean_bad:
	(cd "./module_bad"; rm -f .*.cmd .cache.mk jonah_module_bad.ko jonah_module_bad.*.* jonah_module_bad.o modules.order Module.symvers; rm -f -d -r .tmp_versions)

