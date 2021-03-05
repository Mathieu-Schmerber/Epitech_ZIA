
ifdef OS
	SHELL = CMD
	_OS = windows
	NAME = server.exe
	CLEAR = cls
else
	_OS = linux
	NAME = server
	CLEAR = clear
endif

ifdef OS
	RM = del /Q
	RMDIR = rmdir /Q /S
	FixPath = $(subst /,\,$1)
	deleteFolderIfExist = IF exist $1 ( $(RMDIR) $1 && echo $1 removed ) ELSE ( echo $1 "doesn't exist" )
else
	RM = rm -f
	RMDIR = rm -rf
	FixPath = $1
	deleteFolderIfExist = if [ -d $1 ]; then $(RMDIR) $1; fi
endif

all:
		$(CLEAR)
		$(MAKE) $(_OS)

run:	all execute

execute:
		$(call FixPath,build/bin/)$(NAME)

execute_debug:
		valgrind $(call FixPath,build/bin/)$(NAME)

windows:
		.\install_windows.bat

linux:
		./install_unix.sh

clean:
		@echo off
		$(call deleteFolderIfExist, $(call FixPath,build))

conan-clean:
		conan remove "*" -f

re:		clean all

rerun:	re execute

debug:	re execute_debug