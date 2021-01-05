##############################################################################
# Flags and options for host and target.
##############################################################################

# You can override the following variables at the make command line:
#   HOST_CC	TARGET_ARCH	TARGET_SYS TARGET_SYSROOT TARGET_SYS_MIN MODE TYPE CROSS

MAJVER= 1
MINVER= 0
RELVER= 0

ABIVER=1.0

TARGET_CC=$(CROSS)cc
TARGET_CXX=$(CROSS)c++
TARGET_LD=$(CROSS)c++
TARGET_LIBTOOL=$(CROSS)libtool

TARGET_CFLAGS= -g -fPIC
TARGET_CXXFLAGS= -g -fPIC
TARGET_LDFLAGS=
TARGET_LIBTOOLFLAGS= -static

TARGET_ICFLAGS= -I.
TARGET_ICXXFLAGS= -I.

TARGET_GCFLAGS=
TARGET_GCXXFLAGS=
TARGET_GLDFLAGS= -L$(TARGET_PB_LITE_PATH) -l$(TARGET_PB_LITE_NAME)
TARGET_GLIBTOOLFLAGS= -L$(TARGET_PB_LITE_PATH) -l$(TARGET_PB_LITE_NAME)

TARGET_PB_LITE_NAME=protobuf-lite
TARGET_PB_LITE_PATH=../lib/pb_osx

TARGET_LUA_JIT_NAME=luajit
TARGET_LUA_JIT_PATH=../lib/luajit_osx

##############################################################################
# Sources
##############################################################################

GRAV_SRC=\
	Log.cpp Profiler.cpp Meta.cpp Random.cpp

GRAV_SRC+=\
	Battle.cpp BattleArea.cpp BattleField.cpp

GRAV_SRC+=\
	Actor.cpp ActorDatas.cpp ActorAttrs.cpp NewActorAttrs.cpp ActorModifier.cpp \
	ActorModifierSub.cpp ActorSkill.cpp ActorSkillSub.cpp

GRAV_SRC+=\
	EventData.cpp ConditionCheck.cpp SubEffectPerformer.cpp

GRAV_SRC+=\
	Calculator.cpp BaseCalc.cpp LuaCalc.cpp NativeCalc.cpp

GRAV_SRC+=\
	AffixMeta.cpp ValueMeta.cpp SkillMeta.cpp EffectMeta.cpp ActorMeta.cpp \
	ModifierMeta.cpp SubEffectsMeta.cpp CombatAttrMeta.cpp BattleModeMeta.cpp \
    TriggerMeta.cpp FightCondMeta.cpp PrivateChecker.cpp

GRAV_SRC+=\
	EffectUtil.cpp ModifierUtils.cpp AnalysisUtil.cpp TriggerAction.cpp BattleTrigger.cpp \
	TriggerCondition.cpp TriggerManager.cpp TriggerRelationCondition.cpp

SQLITE_SRC=\
	sqlite3.c

GRAV_G_SRC=\
	BattleReplay.pb.cc BattleParams.pb.cc BattleStatistics.pb.cc BattleTest.pb.cc BattleAnalysis.pb.cc BattleCommand.pb.cc

##############################################################################


##############################################################################
# Objects and Libraries
##############################################################################

GRAV_O= $(GRAV_SRC:.cpp=.o)
GRAV_D= $(GRAV_SRC:.cpp=.d)
GRAV_G_O= $(GRAV_G_SRC:.cc=.o)
GRAV_G_D= $(GRAV_G_SRC:.cc=.d)
SQLITE_O= $(SQLITE_SRC:.c=.o)
SQLITE_D= $(SQLITE_SRC:.c=.d)

ALL_O=$(GRAV_O) $(GRAV_G_O) $(SQLITE_O)

GRAV_A= libgrav.a
GRAV_SO= libgrav.so
GRAV_DYLIB=libgrav.dylib

##############################################################################


##############################################################################
# Build Type
##############################################################################

TYPE=standalone
ifeq (standalone,$(TYPE))
	ALL_O= $(GRAV_O) $(GRAV_G_O)
	TARGET_GCFLAGS= -DGRAV_STANDALONE
	TARGET_GCXXFLAGS= -DGRAV_STANDALONE
else
ifeq (unityplugin,$(TYPE))
	ALL_O= $(GRAV_O) $(GRAV_G_O) $(SQLITE_O)
	TARGET_GCFLAGS= -DGRAV_UNITY_PLUGIN
	TARGET_GCXXFLAGS= -DGRAV_UNITY_PLUGIN
else
	$(error Unsupported build type)
endif
endif

##############################################################################


##############################################################################
# Build Mode
##############################################################################

MODE=shared
ifeq (shared,$(MODE))
	TARGET_LIBRARY=$(GRAV_SO)
else
ifeq (static,$(MODE))
	TARGET_LIBRARY=$(GRAV_A)
else
ifeq (dynamic,$(MODE))
	TARGET_LIBRARY=$(GRAV_DYLIB)
else
	$(error Unsupported build mode)
endif
endif
endif

##############################################################################


##############################################################################
# Target System
##############################################################################

TARGET_SYS=OSX
ifeq (iOS,$(TARGET_SYS))
	TARGET_PB_LITE_PATH:=../lib/pb_ios

	TARGET_CFLAGS+= -arch $(TARGET_ARCH) -isysroot $(TARGET_SYSROOT) -std=gnu99
	TARGET_CFLAGS+= -Wall -fmessage-length=0 -fdiagnostics-show-note-include-stack
	TARGET_CFLAGS+= -fmacro-backtrace-limit=0 -fpascal-strings -O0 -fno-common

	TARGET_CXXFLAGS+= -arch $(TARGET_ARCH) -isysroot $(TARGET_SYSROOT) -std=gnu++11 -stdlib=libc++
	TARGET_CXXFLAGS+= -Wall -fmessage-length=0 -fdiagnostics-show-note-include-stack
	TARGET_CXXFLAGS+= -fmacro-backtrace-limit=0 -O0 -fpascal-strings -fno-common -fstrict-aliasing
	TARGET_CXXFLAGS+= -fembed-bitcode-marker -miphoneos-version-min=$(TARGET_SYS_MIN)

	TARGET_LIBTOOLFLAGS+= -arch_only $(TARGET_ARCH) -syslibroot $(TARGET_SYSROOT)
else
ifeq (OSX,$(TARGET_SYS))
	TARGET_PB_LITE_PATH:=../lib/pb_osx
	TARGET_LUA_JIT_PATH=../lib/luajit_osx

	TARGET_CFLAGS+= -arch $(TARGET_ARCH) -isysroot $(TARGET_SYSROOT) -std=gnu99
	TARGET_CFLAGS+= -Wall -fmessage-length=0 -fdiagnostics-show-note-include-stack
	TARGET_CFLAGS+= -fmacro-backtrace-limit=0 -fpascal-strings -O0 -fno-common -fasm-blocks -fstrict-aliasing

	TARGET_CXXFLAGS+= -arch $(TARGET_ARCH) -isysroot $(TARGET_SYSROOT) -std=gnu++11 -stdlib=libc++
	TARGET_CXXFLAGS+= -Wall -fmessage-length=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit=0
	TARGET_CXXFLAGS+= -fpascal-strings -O0 -fno-common -fasm-blocks -fstrict-aliasing -fvisibility-inlines-hidden
	TARGET_CXXFLAGS+= -mmacosx-version-min=$(TARGET_SYS_MIN)

	TARGET_LDFLAGS+= -arch $(TARGET_ARCH) -isysroot $(TARGET_SYSROOT) -stdlib=libc++
	TARGET_LDFLAGS+= -mmacosx-version-min=$(TARGET_SYS_MIN)

	TARGET_GLDFLAGS+= -L$(TARGET_LUA_JIT_PATH) -l$(TARGET_LUA_JIT_NAME)

	TARGET_LIBTOOLFLAGS+= -arch_only $(TARGET_ARCH) -syslibroot $(TARGET_SYSROOT)
else
ifeq (Android,$(TARGET_SYS))
	TARGET_CC=$(CROSS)gcc
	TARGET_CXX=$(CROSS)g++
	TARGET_LD=$(CROSS)g++

	ifeq (armv7-a,$(TARGET_ARCH))
		ANDROID_ABI=armeabi-v7a
		ANDROID_SYSROOT=$(TARGET_SYSROOT)/platforms/android-9/arch-arm
		
		TARGET_CFLAGS+= -march=$(TARGET_ARCH) -DNDEBUG -DANDROID -std=gnu99
		TARGET_CFLAGS+= -Wall -fomit-frame-pointer
		TARGET_CFLAGS+= -mfpu=vfpv3-d16 -mfloat-abi=softfp -mthumb -Os
		TARGET_CFLAGS+= -fno-strict-aliasing -finline-limit=64
		TARGET_CFLAGS+= -ffunction-sections -funwind-tables -fstack-protector -no-canonical-prefixes
		
		TARGET_CXXFLAGS+= --sysroot $(ANDROID_SYSROOT) -march=$(TARGET_ARCH) -DNDEBUG -DANDROID -std=gnu++11
		TARGET_CXXFLAGS+= -Wall -mfpu=vfpv3-d16 -mfloat-abi=softfp -mthumb -Os
		TARGET_CXXFLAGS+= -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64
		TARGET_CXXFLAGS+= -ffunction-sections -funwind-tables -fstack-protector -no-canonical-prefixes -fomit-frame-pointer
		
		TARGET_LDFLAGS+= -Wl,-soname,$(GRAV_SO) --sysroot $(ANDROID_SYSROOT) -march=$(TARGET_ARCH)

		TARGET_GLDFLAGS= $(TARGET_PB_LITE_PATH)/$(TARGET_PB_LITE_NAME) $(CXX_STL)
		TARGET_GLDFLAGS+= -lgcc -no-canonical-prefixes -Wl,--fix-cortex-a8 -Wl,--no-undefined
		TARGET_GLDFLAGS+= -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -mthumb -lc -lm
	else
	ifeq (x86,$(TARGET_ARCH))
		ANDROID_ABI=x86
		ANDROID_SYSROOT=$(TARGET_SYSROOT)/platforms/android-9/arch-x86

		TARGET_CFLAGS+= -ffunction-sections -funwind-tables -no-canonical-prefixes -fstack-protector
		TARGET_CFLAGS+= -O2 -DNDEBUG -DANDROID -fomit-frame-pointer -fstrict-aliasing -funswitch-loops
		TARGET_CFLAGS+= -finline-limit=300

		TARGET_GCFLAGS+= -Wall

		TARGET_CXXFLAGS+= -ffunction-sections -funwind-tables -no-canonical-prefixes -fstack-protector
		TARGET_CXXFLAGS+= -fno-exceptions -fno-rtti -O2 -DNDEBUG -DANDROID -fomit-frame-pointer
		TARGET_CXXFLAGS+= -fstrict-aliasing -funswitch-loops -finline-limit=300

		TARGET_GCXXFLAGS+= -Wall -std=c++11
		TARGET_GCXXFLAGS+= -I$(ANDROID_SYSROOT)/usr/include

		TARGET_LDFLAGS+= -Wl,-soname,$(GRAV_SO) --sysroot $(ANDROID_SYSROOT)

		TARGET_GLDFLAGS= -Wl,--whole-archive $(TARGET_PB_LITE_PATH)/$(TARGET_PB_LITE_NAME)
		TARGET_GLDFLAGS+= -Wl,--no-whole-archive $(CXX_STL)
		TARGET_GLDFLAGS+= -lgcc -no-canonical-prefixes  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -lc -lm
	else
		$(error Unsupported android arch)
	endif
	endif

	CXX_STL=$(TARGET_SYSROOT)/sources/cxx-stl/gnu-libstdc++/4.8/libs/$(ANDROID_ABI)/libgnustl_static.a

	TARGET_ICFLAGS+= -I$(TARGET_SYSROOT)/sources/cxx-stl/gnu-libstdc++/4.8/include
	TARGET_ICFLAGS+= -I$(TARGET_SYSROOT)/sources/cxx-stl/gnu-libstdc++/4.8/libs/$(ANDROID_ABI)/include
	TARGET_ICFLAGS+= -I$(TARGET_SYSROOT)/sources/cxx-stl/gnu-libstdc++/4.8/include/backward
	TARGET_ICFLAGS+= -I$(TARGET_SYSROOT)/platforms/android-9/arch-arm/usr/include

	TARGET_ICXXFLAGS+= -I$(TARGET_SYSROOT)/sources/cxx-stl/gnu-libstdc++/4.8/include
	TARGET_ICXXFLAGS+= -I$(TARGET_SYSROOT)/sources/cxx-stl/gnu-libstdc++/4.8/libs/$(ANDROID_ABI)/include
	TARGET_ICXXFLAGS+= -I$(TARGET_SYSROOT)/sources/cxx-stl/gnu-libstdc++/4.8/include/backward

	TARGET_PB_LITE_PATH=../lib/pb_android/$(ANDROID_ABI)
	TARGET_PB_LITE_NAME=libprotobuf-lite.a
else
ifeq (Linux,$(TARGET_SYS))
	TARGET_CC=$(CROSS)gcc
	TARGET_CXX=$(CROSS)g++

	TARGET_LIBTOOL=$(CROSS)ar

	TARGET_CFLAGS+= -Wall -std=c99

	TARGET_CXXFLAGS+= -Wall -std=c++0x

	TARGET_LIBTOOLFLAGS= cr

	TARGET_GLIBTOOLFLAGS=

	TARGET_PB_LITE_PATH:=../lib/pb_linux
	TARGET_LUA_JIT_PATH=../lib/luajit_linux
else
	$(error Unsupported system)
endif
endif
endif
endif

##############################################################################


##############################################################################
# Host System
##############################################################################

HOST_SYS= $(shell uname -s)
ifeq (Darwin,$(HOST_SYS))
else
ifeq (Linux,$(HOST_SYS))
else
endif
endif

##############################################################################


HOST_CC= cc
HOST_RM= rm -rf
ALL_RM= $(GRAV_O) $(GRAV_G_O) $(SQLITE_O) $(GRAV_A) $(GRAV_SO) $(GRAV_DYLIB) $(GRAV_D) $(GRAV_G_D) $(SQLITE_D)

CORE_O= $(GRAV_O) $(GRAV_G_O)

Q= @
E= @echo

all: $(TARGET_LIBRARY)

clean:
	$(HOST_RM) $(ALL_RM)

.PHONY: all clean depend

##############################################################################
# Rules for generated files.
##############################################################################

$(GRAV_A): $(ALL_O)
	$(E) "LIBTOOL 	$@"
	$(TARGET_LIBTOOL) $(TARGET_LIBTOOLFLAGS) $(TARGET_GLIBTOOLFLAGS) -o $@ $(ALL_O)

$(GRAV_SO): $(ALL_O)
	$(E) "LD		$@"
	$(TARGET_LD) -shared $(TARGET_LDFLAGS) $(ALL_O) $(TARGET_GLDFLAGS) -o $@

$(GRAV_DYLIB): $(ALL_O)
	$(E) "LD        $@"
	$(TARGET_LD) -dynamiclib $(TARGET_LDFLAGS) $(TARGET_GLDFLAGS) -o $@ $(ALL_O)

%.o: %.c
	$(E) "CC 		$@"
	$(TARGET_CC) -c $(TARGET_CFLAGS) $(TARGET_ICFLAGS) $(TARGET_GCFLAGS) -MMD $< -o $@
%.o: %.cpp
	$(E) "CXX 		$@"
	$(TARGET_CXX) -c $(TARGET_CXXFLAGS) $(TARGET_ICXXFLAGS) $(TARGET_GCXXFLAGS) -MMD $< -o $@
%.o: %.cc
	$(E) "CXX 		$@"
	$(TARGET_CXX) -c $(TARGET_CXXFLAGS) $(TARGET_ICXXFLAGS) $(TARGET_GCXXFLAGS) -MMD $< -o $@