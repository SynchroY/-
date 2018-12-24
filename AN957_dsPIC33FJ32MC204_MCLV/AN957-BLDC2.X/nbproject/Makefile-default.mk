#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AN957-BLDC2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AN957-BLDC2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../Init.c ../Interrupts.c ../SensoredBLDC.c ../sci_buffer.c ../BLDC_command.c ../cmdline.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/Init.o ${OBJECTDIR}/_ext/1472/Interrupts.o ${OBJECTDIR}/_ext/1472/SensoredBLDC.o ${OBJECTDIR}/_ext/1472/sci_buffer.o ${OBJECTDIR}/_ext/1472/BLDC_command.o ${OBJECTDIR}/_ext/1472/cmdline.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/Init.o.d ${OBJECTDIR}/_ext/1472/Interrupts.o.d ${OBJECTDIR}/_ext/1472/SensoredBLDC.o.d ${OBJECTDIR}/_ext/1472/sci_buffer.o.d ${OBJECTDIR}/_ext/1472/BLDC_command.o.d ${OBJECTDIR}/_ext/1472/cmdline.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/Init.o ${OBJECTDIR}/_ext/1472/Interrupts.o ${OBJECTDIR}/_ext/1472/SensoredBLDC.o ${OBJECTDIR}/_ext/1472/sci_buffer.o ${OBJECTDIR}/_ext/1472/BLDC_command.o ${OBJECTDIR}/_ext/1472/cmdline.o

# Source Files
SOURCEFILES=../Init.c ../Interrupts.c ../SensoredBLDC.c ../sci_buffer.c ../BLDC_command.c ../cmdline.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/AN957-BLDC2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ32MC204
MP_LINKER_FILE_OPTION=,--script="..\p33FJ32MC204.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/Init.o: ../Init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Init.o.ok ${OBJECTDIR}/_ext/1472/Init.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/Init.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/Init.o.d" -o ${OBJECTDIR}/_ext/1472/Init.o ../Init.c    
	
${OBJECTDIR}/_ext/1472/Interrupts.o: ../Interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Interrupts.o.ok ${OBJECTDIR}/_ext/1472/Interrupts.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/Interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/Interrupts.o.d" -o ${OBJECTDIR}/_ext/1472/Interrupts.o ../Interrupts.c    
	
${OBJECTDIR}/_ext/1472/SensoredBLDC.o: ../SensoredBLDC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SensoredBLDC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SensoredBLDC.o.ok ${OBJECTDIR}/_ext/1472/SensoredBLDC.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/SensoredBLDC.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/SensoredBLDC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/SensoredBLDC.o.d" -o ${OBJECTDIR}/_ext/1472/SensoredBLDC.o ../SensoredBLDC.c    
	
${OBJECTDIR}/_ext/1472/sci_buffer.o: ../sci_buffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/sci_buffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sci_buffer.o.ok ${OBJECTDIR}/_ext/1472/sci_buffer.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/sci_buffer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/sci_buffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/sci_buffer.o.d" -o ${OBJECTDIR}/_ext/1472/sci_buffer.o ../sci_buffer.c    
	
${OBJECTDIR}/_ext/1472/BLDC_command.o: ../BLDC_command.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/BLDC_command.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/BLDC_command.o.ok ${OBJECTDIR}/_ext/1472/BLDC_command.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/BLDC_command.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/BLDC_command.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/BLDC_command.o.d" -o ${OBJECTDIR}/_ext/1472/BLDC_command.o ../BLDC_command.c    
	
${OBJECTDIR}/_ext/1472/cmdline.o: ../cmdline.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/cmdline.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/cmdline.o.ok ${OBJECTDIR}/_ext/1472/cmdline.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/cmdline.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/cmdline.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/cmdline.o.d" -o ${OBJECTDIR}/_ext/1472/cmdline.o ../cmdline.c    
	
else
${OBJECTDIR}/_ext/1472/Init.o: ../Init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Init.o.ok ${OBJECTDIR}/_ext/1472/Init.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/Init.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/Init.o.d" -o ${OBJECTDIR}/_ext/1472/Init.o ../Init.c    
	
${OBJECTDIR}/_ext/1472/Interrupts.o: ../Interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Interrupts.o.ok ${OBJECTDIR}/_ext/1472/Interrupts.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/Interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/Interrupts.o.d" -o ${OBJECTDIR}/_ext/1472/Interrupts.o ../Interrupts.c    
	
${OBJECTDIR}/_ext/1472/SensoredBLDC.o: ../SensoredBLDC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SensoredBLDC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SensoredBLDC.o.ok ${OBJECTDIR}/_ext/1472/SensoredBLDC.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/SensoredBLDC.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/SensoredBLDC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/SensoredBLDC.o.d" -o ${OBJECTDIR}/_ext/1472/SensoredBLDC.o ../SensoredBLDC.c    
	
${OBJECTDIR}/_ext/1472/sci_buffer.o: ../sci_buffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/sci_buffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sci_buffer.o.ok ${OBJECTDIR}/_ext/1472/sci_buffer.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/sci_buffer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/sci_buffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/sci_buffer.o.d" -o ${OBJECTDIR}/_ext/1472/sci_buffer.o ../sci_buffer.c    
	
${OBJECTDIR}/_ext/1472/BLDC_command.o: ../BLDC_command.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/BLDC_command.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/BLDC_command.o.ok ${OBJECTDIR}/_ext/1472/BLDC_command.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/BLDC_command.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/BLDC_command.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/BLDC_command.o.d" -o ${OBJECTDIR}/_ext/1472/BLDC_command.o ../BLDC_command.c    
	
${OBJECTDIR}/_ext/1472/cmdline.o: ../cmdline.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/cmdline.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/cmdline.o.ok ${OBJECTDIR}/_ext/1472/cmdline.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/cmdline.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/cmdline.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/h" -MMD -MF "${OBJECTDIR}/_ext/1472/cmdline.o.d" -o ${OBJECTDIR}/_ext/1472/cmdline.o ../cmdline.c    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/AN957-BLDC2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../p33FJ32MC204.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -o dist/${CND_CONF}/${IMAGE_TYPE}/AN957-BLDC2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,-L"..",-L"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/gld",-Map="${DISTDIR}/AN957-BLDC2.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/AN957-BLDC2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../p33FJ32MC204.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/AN957-BLDC2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,-L"..",-L"../../../../../Program Files/Microchip/MPLAB C30/support/dsPIC33F/gld",-Map="${DISTDIR}/AN957-BLDC2.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/AN957-BLDC2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=elf
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
