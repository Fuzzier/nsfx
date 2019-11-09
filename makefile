CC=cl.exe
LD=link.exe

# disable warning C4355: 'this' pointer is used in constructor.
# disable warning C4819: file contains non-ascii characters.

!IFDEF RELEASE
CFLAGS=/nologo
CPPFLAGS=/EHsc /W3 /MD /Ox /Zi /arch:AVX /wd4355 /wd4819 /Zm256 /I../.. /I. \
         /DBOOST_MATH_PROMOTE_DOUBLE_POLICY=false /DBOOST_DISABLE_ASSERTS
LDFLAGS=/NOLOGO /DEBUG /INCREMENTAL:NO
ARFLAGS=/NOLOGO
!ELSE
CFLAGS=/nologo
CPPFLAGS=/EHsc /W3 /MD /Od /Zi /DDEBUG /wd4355 /wd4819 /Zm256 /I../.. /I. \
         /DBOOST_MATH_PROMOTE_DOUBLE_POLICY=false
LDFLAGS=/NOLOGO /DEBUG /INCREMENTAL:NO
ARFLAGS=/NOLOGO
!ENDIF

all : echo nameable combination aggregation member-aggregation \
      event simulation statistics

clean :
    del /s /q *.pch 1>nul 2>nul
    del /s /q *.obj 1>nul 2>nul
    del /s /q *.pdb 1>nul 2>nul
    del /s /q *.ilk 1>nul 2>nul
    del /s /q *.exe 1>nul 2>nul
    del /s /q *.txt 1>nul 2>nul

!CMDSWITCHES +I
run :
    for /f "delims=" %d in ('dir /b *.exe 2^>nul') do @( \
        echo ====== %~d ====== & \
        "%~d" )
!CMDSWITCHES -I

################################################################################
# pch (pre-compiled header)
pch : config.pch

PCH=config.pch

PCHOBJ=config.obj

HEADERS=config.h

SOURCES=config.cpp

MAKEPCH = /Yc"config.h" /Fp"$(PCH)"
USEPCH  = /Yu"config.h" /Fp"$(PCH)"

config.pch : $(HEADERS) $(SOURCES) $(BUILD)
    $(CC) $(CFLAGS) $(CPPFLAGS) /c /Fo"$(PCHOBJ)" /Fd"./" $(MAKEPCH) $(SOURCES)

################################################################################
!CMDSWITCHES +I
# Clean all except for the pre-compiled header.
cl :
    for /f "delims=" %d in ('dir /s /b *.obj 2^>nul') do @( \
        if "%~nxd" neq "$(PCHOBJ)" del "%~d" 1>nul 2>nul )
    for /f "delims=" %d in ('dir /s /b *.pdb 2^>nul') do @( \
        echo %~nd| findstr "^vc100$$" >nul || \
        del "%~d" 1>nul 2>nul )
    del /s *.ilk 1>nul 2>nul
    del /s *.exe 1>nul 2>nul
    del /s *.txt 1>nul 2>nul
!CMDSWITCHES -I

################################################################################
{echo}.cpp{echo}.obj ::
    $(CC) /c $(CFLAGS) $(CPPFLAGS) $(USEPCH) /Fo"echo/" $<

{nameable}.cpp{nameable}.obj ::
    $(CC) /c $(CFLAGS) $(CPPFLAGS) $(USEPCH) /Fo"nameable/" $<

{combination}.cpp{combination}.obj ::
    $(CC) /c $(CFLAGS) $(CPPFLAGS) $(USEPCH) /Fo"combination/" $<

{aggregation}.cpp{aggregation}.obj ::
    $(CC) /c $(CFLAGS) $(CPPFLAGS) $(USEPCH) /Fo"aggregation/" $<

{member-aggregation}.cpp{member-aggregation}.obj ::
    $(CC) /c $(CFLAGS) $(CPPFLAGS) $(USEPCH) /Fo"member-aggregation/" $<

{event}.cpp{event}.obj ::
    $(CC) /c $(CFLAGS) $(CPPFLAGS) $(USEPCH) /Fo"event/" $<

{simulation}.cpp{simulation}.obj ::
    $(CC) /c $(CFLAGS) $(CPPFLAGS) $(USEPCH) /Fo"simulation/" $<

{statistics}.cpp{statistics}.obj ::
    $(CC) /c $(CFLAGS) $(CPPFLAGS) $(USEPCH) /Fo"statistics/" $<

########################################
echo : echo.exe

SRC = echo/echo.cpp \
      echo/test-echo.cpp

HEADERS = echo/i-echo.h \
          echo/echo.h

OBJECTS = echo/echo.obj \
          echo/test-echo.obj

echo.exe : $(PCH) $(HEADERS) $(SRC) $(OBJECTS)
    $(LD) $(LDFLAGS) $(LIBS) $(PCHOBJ) $(OBJECTS) /OUT:$@

########################################
nameable : nameable.exe

SRC = nameable/nameable.cpp \
      nameable/test-nameable.cpp

HEADERS = nameable/i-nameable.h \
          nameable/nameable.h

OBJECTS = nameable/nameable.obj \
          nameable/test-nameable.obj

nameable.exe : $(PCH) $(HEADERS) $(SRC) $(OBJECTS)
    $(LD) $(LDFLAGS) $(LIBS) $(PCHOBJ) $(OBJECTS) /OUT:$@

########################################
combination : combination.exe

SRC = nameable/nameable.cpp \
      combination/combination.cpp \
      combination/test-combination.cpp

HEADERS = nameable/i-nameable.h \
          nameable/nameable.h \
          combination/combination.h

OBJECTS = nameable/nameable.obj \
          combination/combination.obj \
          combination/test-combination.obj

combination.exe : $(PCH) $(HEADERS) $(SRC) $(OBJECTS)
    $(LD) $(LDFLAGS) $(LIBS) $(PCHOBJ) $(OBJECTS) /OUT:$@

########################################
aggregation : aggregation.exe

SRC = nameable/nameable.cpp \
      aggregation/aggregation.cpp \
      aggregation/test-aggregation.cpp

HEADERS = nameable/i-nameable.h \
          nameable/nameable.h \
          aggregation/aggregation.h

OBJECTS = nameable/nameable.obj \
          aggregation/aggregation.obj \
          aggregation/test-aggregation.obj

aggregation.exe : $(PCH) $(HEADERS) $(SRC) $(OBJECTS)
    $(LD) $(LDFLAGS) $(LIBS) $(PCHOBJ) $(OBJECTS) /OUT:$@

########################################
member-aggregation : member-aggregation.exe

SRC = nameable/nameable.cpp \
      member-aggregation/member-aggregation.cpp \
      member-aggregation/test-member-aggregation.cpp

HEADERS = nameable/i-nameable.h \
          nameable/nameable.h \
          member-aggregation/member-aggregation.h

OBJECTS = nameable/nameable.obj \
          member-aggregation/member-aggregation.obj \
          member-aggregation/test-member-aggregation.obj

member-aggregation.exe : $(PCH) $(HEADERS) $(SRC) $(OBJECTS)
    $(LD) $(LDFLAGS) $(LIBS) $(PCHOBJ) $(OBJECTS) /OUT:$@

########################################
event : event.exe

SRC = event/countdown-alarm.cpp \
      event/alarm-listener.cpp  \
      event/test-event.cpp

HEADERS = event/i-alarm-event.h   \
          event/i-countdown.h     \
          event/countdown-alarm.h \
          event/alarm-listener.h

OBJECTS = event/countdown-alarm.obj \
          event/alarm-listener.obj  \
          event/test-event.obj

event.exe : $(PCH) $(HEADERS) $(OBJECTS)
    $(LD) $(LDFLAGS) $(LIBS) $(PCHOBJ) $(OBJECTS) /OUT:$@

########################################
simulation : simulation.exe

SRC = simulation/test-event.cpp

HEADERS =

OBJECTS = simulation/test-simulation.obj

simulation.exe : $(PCH) $(HEADERS) $(OBJECTS)
    $(LD) $(LDFLAGS) $(LIBS) $(PCHOBJ) $(OBJECTS) /OUT:$@

########################################
statistics : statistics.exe

SRC = statistics/test-statistics.cpp

HEADERS = statistics/statistics-provider.h

OBJECTS = statistics/statistics-provider.obj \
          statistics/test-statistics.obj

statistics.exe : $(PCH) $(HEADERS) $(OBJECTS)
    $(LD) $(LDFLAGS) $(LIBS) $(PCHOBJ) $(OBJECTS) /OUT:$@

