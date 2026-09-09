TARGET      := MyReg
CC 			:= arm-none-eabi-gcc                   #biên dịch 
OBJCOPY 	:= arm-none-eabi-objcopy			   #đóng gói
OBJ_DIR		:= Build
OPENOCD		:= D:/xpack-openocd-0.12.0-7-win32-x64/xpack-openocd-0.12.0-7/bin/openocd

MCU			:= -mcpu=cortex-m3 -mthumb

INC_DIR		:= MCU_Driver/Inc 	 						\
			   Device_Driver/Inc 						\
			   User/Inc			 						\
			   FreeRTOS_Kernel/include  				\
			   FreeRTOS_Kernel/portable/GCC/ARM_CM3  	

CFLAG		:= $(foreach dir, $(INC_DIR), -I$(dir))
CFLAG		+= $(MCU) -c -Wall -Os -g3 
CFLAG		+= -ffunction-sections -fdata-sections

SRC_DIR		:= MCU_Driver/Src 	 						\
			   Device_Driver/Src 						\
			   User/Src			 						\
			   Startup									\
			   FreeRTOS_Kernel							\
			   FreeRTOS_Kernel/portable/GCC/ARM_CM3		\
			   FreeRTOS_Kernel/portable/MemMang

SRC_FILE	:= $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
OBJ_NPATH	:= $(notdir $(SRC_FILE))
OBJ_FILE	:= $(subst .c,.o,$(OBJ_NPATH)) 
PATH_OBJ	:= $(foreach dir, $(OBJ_FILE), $(OBJ_DIR)/$(dir))
vpath %.c $(SRC_DIR)

LSCRIPT		:= Linker/stm32ls.ld
LDFLAG		:= $(MCU) -T$(LSCRIPT) -Wl,-Map=$(OBJ_DIR)/$(TARGET).Map
LDFLAG		+= -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs -Wl,-u,_printf_float

$(OBJ_DIR)/%.o : %.c
	$(CC) $(CFLAG) -o $@ $<

$(OBJ_DIR)/$(TARGET).elf : $(PATH_OBJ)
	$(CC) $(LDFLAG) -o $@ $^ -lm

$(OBJ_DIR)/$(TARGET).hex : $(OBJ_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(OBJ_DIR)/$(TARGET).bin : $(OBJ_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

run : $(OBJ_DIR)/$(TARGET).hex $(OBJ_DIR)/$(TARGET).bin
	@echo "------------------------------------------------"
	@echo "BIÊN DỊCH THÀNH CÔNG!"
	@arm-none-eabi-size $(OBJ_DIR)/$(TARGET).elf
	@echo "------------------------------------------------"

clean :
	rm -rf $(OBJ_DIR)/*

check:
	@echo "Cac file .0 dang co: $(OBJ_FILE)"

flash: run
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32f1x.cfg \
	-c "program Build/MyReg.elf verify reset exit"