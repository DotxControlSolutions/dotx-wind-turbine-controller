# ----------------------------------------------------------------------------------
#           file : make_mcu.mk
#    description : MINGW batch script (makefile) for Emergy Wind Technology (EWT)
#        toolbox : DotX Wind Turbine Control Software
#         author : DotX Control Solutions, www.dotxcontrol.com
# ----------------------------------------------------------------------------------

# Compilation Options --------------------------------------------------------------
# Only make changes in this section for different compilation options
# Comment or uncomment options as needed

# Wind turbine options
OPTIONS += -DNR_BLADES=3
OPTIONS += -DWINDDIRECTION=1

# Simulation software options
OPTIONS += -D_PHATAS_THROUGH_DISCON 
OPTIONS += -D_FAST_THROUGH_DISCON 

# Enable global debugging file
OPTIONS += -D_DOTX_GLOG_

# Enable (new) debugging
# OPTIONS += -DDOTXDEBUGGER

# Controller modules
MODULES  = -D_DTDAMP
MODULES += -D_FADAMP 
MODULES += -D_YAW 

 
# Enable/disable simulation module
#SIMULATIONMODULE = 1
#MODULES += -D_SIM


# Enable/disable supervisory control
SUPERVISORYCONTROL = 1
MODULES += -D_SUP

# Enable ASCII text logging or BINARY sqlite logging
TXTLOGGING = 1
#SQLLOGGING = 1


# Makefile definitions ---------------------------------------------------------------

CC      = gcc
F90     = gfortran
SRCDIR  = source
FLAGS   = -O
OUT     = build/DISCON.dll


F90SRC  = $(addprefix $(SRCDIR)/interface/, fastwrapper_fortran.f90)
F90OBJ  = fastwrapper_fortran.o
F90OPT  = -cpp -mrtd -fno-underscoring -fmessage-length=0 -ffree-line-length-none -fno-automatic -ffloat-store

mcu     = maincontrollerunit readconfiguration
SRC     = $(addprefix $(SRCDIR)/, $(mcu:%=%.c))
OBJ     = $(mcu:%=%.o)

support = matrix system filter pid par_readline par_readstruct bicubic hp_pid debugger
SRC     += $(addprefix $(SRCDIR)/suplib/, $(support:%=%.c))
OBJ     += $(support:%=%.o)

mcudata = mcudatastatic mcudatadynamic mcureadfile
SRC     += $(addprefix $(SRCDIR)/turbine/, $(mcudata:%=%.c))
OBJ     += $(mcudata:%=%.o)

ifdef SQLLOGGING
	logdata = logdata_sql sqlite3
	SRC     += $(addprefix $(SRCDIR)/logdata/, $(logdata:%=%.c))
	OBJ     += $(logdata:%=%.o)
endif

ifdef TXTLOGGING
	logdata = logdata
	SRC     += $(addprefix $(SRCDIR)/logdata/, $(logdata:%=%.c))
	OBJ     += $(logdata:%=%.o)
endif

interface = phataswrapper matlabwrapper bladedwrapper bladedlogsignals fastwrapper
SRC     += $(addprefix $(SRCDIR)/interface/, $(interface:%=%.c))
OBJ     += $(interface:%=%.o)

controller = basecontroller base_rotorspeed base_fa_damping base_dt_damping  base_yaw_control
SRC     += $(addprefix $(SRCDIR)/basecontrol/, $(controller:%=%.c))
OBJ     += $(controller:%=%.o)

operation = operation powerproduction fastshutdown
SRC     += $(addprefix $(SRCDIR)/operation/, $(operation:%=%.c))
OBJ     += $(operation:%=%.o)


ifdef SUPERVISORYCONTROL
	supervisor = supdatastatic supdatadynamic supreadfile supervisory triggerdetection postprocedure
	SRC     += $(addprefix $(SRCDIR)/supervisor/, $(supervisor:%=%.c))
	OBJ     += $(supervisor:%=%.o)
endif

ifdef SIMULATIONMODULE
	simulation = evmdatastatic evmdatadynamic evmreadfile simdatastatic simdatadynamic simreadfile simulation eventsmodule
	SRC     += $(addprefix $(SRCDIR)/simulation/, $(simulation:%=%.c))
	OBJ     += $(simulation:%=%.o)
endif



# Compile and Link

all: $(OUT)
	rm *.o

$(OBJ) :
	$(F90) -c  $(F90SRC) $(F90OPT) 
	$(CC) $(SRC) -c $(FLAGS) $(MODULES) $(OPTIONS)

$(OUT) : $(OBJ)
	$(F90) $(OBJ) $(F90OBJ) -shared -static-libgcc $(F90OPT) $(LIB) -o $@ -lws2_32 -lntdll 
#	$(CC) -Wall -shared $(OBJ) $(LIB) -o $@ -lws2_32 -lntdll 

	
