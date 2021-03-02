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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/thevenin_instrument.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/thevenin_instrument.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/main.c src/adc.c src/timers.c src/uart_debug.c "src/DEE Emulation 16-bit.c" "src/Flash Operations.s" src/i2c.c src/io.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/adc.o ${OBJECTDIR}/src/timers.o ${OBJECTDIR}/src/uart_debug.o "${OBJECTDIR}/src/DEE Emulation 16-bit.o" "${OBJECTDIR}/src/Flash Operations.o" ${OBJECTDIR}/src/i2c.o ${OBJECTDIR}/src/io.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/adc.o.d ${OBJECTDIR}/src/timers.o.d ${OBJECTDIR}/src/uart_debug.o.d "${OBJECTDIR}/src/DEE Emulation 16-bit.o.d" "${OBJECTDIR}/src/Flash Operations.o.d" ${OBJECTDIR}/src/i2c.o.d ${OBJECTDIR}/src/io.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/adc.o ${OBJECTDIR}/src/timers.o ${OBJECTDIR}/src/uart_debug.o ${OBJECTDIR}/src/DEE\ Emulation\ 16-bit.o ${OBJECTDIR}/src/Flash\ Operations.o ${OBJECTDIR}/src/i2c.o ${OBJECTDIR}/src/io.o

# Source Files
SOURCEFILES=src/main.c src/adc.c src/timers.c src/uart_debug.c src/DEE Emulation 16-bit.c src/Flash Operations.s src/i2c.c src/io.c



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
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/thevenin_instrument.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24HJ128GP206A
MP_LINKER_FILE_OPTION=,--script=p24HJ128GP206A.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/cff1bf4d2195e5dfe9e1d35f1835b02e023d163c.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/adc.o: src/adc.c  .generated_files/1d610ab3614b5a5681b4816cf21a87534c76856b.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/adc.o.d 
	@${RM} ${OBJECTDIR}/src/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/adc.c  -o ${OBJECTDIR}/src/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/adc.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/timers.o: src/timers.c  .generated_files/66eaca1a6c4fc063bfff0367b9e2b3894538c5a2.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/timers.o.d 
	@${RM} ${OBJECTDIR}/src/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/timers.c  -o ${OBJECTDIR}/src/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/timers.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/uart_debug.o: src/uart_debug.c  .generated_files/cee47c4fee98740e95a0168b53eea3e4c07119bf.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/uart_debug.o.d 
	@${RM} ${OBJECTDIR}/src/uart_debug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/uart_debug.c  -o ${OBJECTDIR}/src/uart_debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/uart_debug.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/DEE\ Emulation\ 16-bit.o: src/DEE\ Emulation\ 16-bit.c  .generated_files/46fcd9f00dc1d289b1ad7237a56894e76bd6f21e.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} "${OBJECTDIR}/src/DEE Emulation 16-bit.o".d 
	@${RM} "${OBJECTDIR}/src/DEE Emulation 16-bit.o" 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "src/DEE Emulation 16-bit.c"  -o "${OBJECTDIR}/src/DEE Emulation 16-bit.o"  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/DEE Emulation 16-bit.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/i2c.o: src/i2c.c  .generated_files/c0bf4bb26f49cef54c9263d1dbb5d3a6b5ed374d.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/i2c.o.d 
	@${RM} ${OBJECTDIR}/src/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/i2c.c  -o ${OBJECTDIR}/src/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/i2c.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/io.o: src/io.c  .generated_files/e9a247ada95bf2354d106ea5a513fee9751557be.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/io.o.d 
	@${RM} ${OBJECTDIR}/src/io.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/io.c  -o ${OBJECTDIR}/src/io.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/io.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/src/main.o: src/main.c  .generated_files/6ed5a43459ff48d2968b485a58d116dc83cb65a.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/main.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/adc.o: src/adc.c  .generated_files/948c8c133161c8d9919e7153d03d64cae1b0fe60.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/adc.o.d 
	@${RM} ${OBJECTDIR}/src/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/adc.c  -o ${OBJECTDIR}/src/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/adc.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/timers.o: src/timers.c  .generated_files/4cbf05675660f16707c89b72c2c85d2bb299968.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/timers.o.d 
	@${RM} ${OBJECTDIR}/src/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/timers.c  -o ${OBJECTDIR}/src/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/timers.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/uart_debug.o: src/uart_debug.c  .generated_files/bf6958cf357c754c4757367a6a388caa038bf0ae.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/uart_debug.o.d 
	@${RM} ${OBJECTDIR}/src/uart_debug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/uart_debug.c  -o ${OBJECTDIR}/src/uart_debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/uart_debug.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/DEE\ Emulation\ 16-bit.o: src/DEE\ Emulation\ 16-bit.c  .generated_files/f685e0cf88db14db462868f6358b75be964452fe.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} "${OBJECTDIR}/src/DEE Emulation 16-bit.o".d 
	@${RM} "${OBJECTDIR}/src/DEE Emulation 16-bit.o" 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "src/DEE Emulation 16-bit.c"  -o "${OBJECTDIR}/src/DEE Emulation 16-bit.o"  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/DEE Emulation 16-bit.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/i2c.o: src/i2c.c  .generated_files/e416bac81326934140fbe3835ca36a9c035d986a.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/i2c.o.d 
	@${RM} ${OBJECTDIR}/src/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/i2c.c  -o ${OBJECTDIR}/src/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/i2c.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/io.o: src/io.c  .generated_files/339fcd3ab615f6b7da2bcfaaf7d3c84dec56c128.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/io.o.d 
	@${RM} ${OBJECTDIR}/src/io.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/io.c  -o ${OBJECTDIR}/src/io.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/io.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -O0 -I"include" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/Flash\ Operations.o: src/Flash\ Operations.s  .generated_files/528881330084a7e2c6b4c1d0b58e88dad14d8d41.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} "${OBJECTDIR}/src/Flash Operations.o".d 
	@${RM} "${OBJECTDIR}/src/Flash Operations.o" 
	${MP_CC} $(MP_EXTRA_AS_PRE)  "src/Flash Operations.s"  -o "${OBJECTDIR}/src/Flash Operations.o"  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -I"include" -Wa,-MD,"${OBJECTDIR}/src/Flash Operations.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/src/Flash\ Operations.o: src/Flash\ Operations.s  .generated_files/7ed15ed3ab74397271abdaa7efc4084099e608a2.flag .generated_files/35713b39c1b4ae317f752aef53a0b681fac576c7.flag
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} "${OBJECTDIR}/src/Flash Operations.o".d 
	@${RM} "${OBJECTDIR}/src/Flash Operations.o" 
	${MP_CC} $(MP_EXTRA_AS_PRE)  "src/Flash Operations.s"  -o "${OBJECTDIR}/src/Flash Operations.o"  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -I"include" -Wa,-MD,"${OBJECTDIR}/src/Flash Operations.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/thevenin_instrument.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/thevenin_instrument.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include"     -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/thevenin_instrument.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/thevenin_instrument.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"include" -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/thevenin_instrument.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
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
