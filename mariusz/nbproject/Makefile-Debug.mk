#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/HandPointer/HandPointer.o \
	${OBJECTDIR}/src/HandPointer/Media.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`pkg-config opencv --cflags --libs` -lX11 -I/usr/local/include -L/usr/local/lib -lcvblob  
CXXFLAGS=`pkg-config opencv --cflags --libs` -lX11 -I/usr/local/include -L/usr/local/lib -lcvblob  

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mariusz

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mariusz: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mariusz ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/HandPointer/HandPointer.o: nbproject/Makefile-${CND_CONF}.mk src/HandPointer/HandPointer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/HandPointer
	${RM} $@.d
	$(COMPILE.cc) -g -Iinclude/cvBlob `pkg-config opencv --cflags --libs` -lX11 -I/usr/local/include -L/usr/local/lib -lcvblob -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HandPointer/HandPointer.o src/HandPointer/HandPointer.cpp

${OBJECTDIR}/src/HandPointer/Media.o: nbproject/Makefile-${CND_CONF}.mk src/HandPointer/Media.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/HandPointer
	${RM} $@.d
	$(COMPILE.cc) -g -Iinclude/cvBlob `pkg-config opencv --cflags --libs` -lX11 -I/usr/local/include -L/usr/local/lib -lcvblob -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HandPointer/Media.o src/HandPointer/Media.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mariusz

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
