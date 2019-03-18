BUILDDIR = build

DEVICE = stm32/device
CORE = stm32/core
PERIPH = stm32/periph
DISCOVERY = stm32/discovery
LDPATH = stm32/ldscripts
SYSTEM = stm32/system
STARTUP_PATH = stm32/startup
#USB = stm32/usb

#SOURCES += $(DISCOVERY)/src/stm32f3_discovery.c

SOURCES += $(PERIPH)/src/stm32l1xx_gpio.c \
		   $(PERIPH)/src/stm32l1xx_tim.c \
		   $(PERIPH)/src/misc.c \
		   $(PERIPH)/src/stm32l1xx_rcc.c 

SOURCES += $(STARTUP_PATH)/startup_stm32l1xx_mdp.s
SOURCES += $(PERIPH)/src/stm32l1xx_it.c
SOURCES += $(SYSTEM)/src/system_stm32l1xx.c

SOURCES += test.c

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))
#LDLIBS = STM32L152ZC_FLASH.ld

INCLUDES += -I$(DEVICE)/inc \
			-I$(CORE)/inc \
			-I$(PERIPH)/inc \
			-I$(DISCOVERY)/inc \
			-I$(SYSTEM)/inc \
			-I\

ELF = $(BUILDDIR)/program.elf
HEX = $(BUILDDIR)/program.hex
BIN = $(BUILDDIR)/program.bin

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
 	
CFLAGS  = -O0 -g -Wall -I.\
   -mcpu=cortex-m3 -mthumb \
   -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
   $(INCLUDES) -DUSE_STDPERIPH_DRIVER

LDSCRIPT = $(LDPATH)/STM32L152ZC_FLASH.ld
LDFLAGS += -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 -nostdlib

$(BIN): $(ELF)
	echo "5"
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	echo "4"
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJECTS)
	echo "3"
	echo $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	echo "2"
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	echo "1"
	$(CC) -c $(CFLAGS) $< -o $@

#flash: $(BIN)
#	st-flash write $(BIN) 0x8000000

clean:
	rm -rf build