##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## linux_exe_debug
ProjectName            :=SA_Toolkit
ConfigurationName      :=linux_exe_debug
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=/home/skei/Code/SA_Toolkit/build/codelite
ProjectPath            :=/home/skei/Code/SA_Toolkit/build/codelite
IntermediateDirectory  :=./build-$(ConfigurationName)/
OutDir                 :=./build-$(ConfigurationName)/
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=skei
Date                   :=06/03/23
CodeLitePath           :=/home/skei/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=./build-$(ConfigurationName)/bin/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)SAT_EXE $(PreprocessorSwitch)SAT_DEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../src $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)pthread $(LibrarySwitch)rt $(LibrarySwitch)dl $(LibrarySwitch)X11 $(LibrarySwitch)X11-xcb $(LibrarySwitch)xcb $(LibrarySwitch)xcb-util $(LibrarySwitch)xcb-image $(LibrarySwitch)xcb-cursor $(LibrarySwitch)xcb-keysyms $(LibrarySwitch)xkbcommon $(LibrarySwitch)GL $(LibrarySwitch)GLX $(LibrarySwitch)GLU 
ArLibs                 :=  "pthread" "rt" "dl" "X11" "X11-xcb" "xcb" "xcb-util" "xcb-image" "xcb-cursor" "xcb-keysyms" "xkbcommon" "GL" "GLX" "GLU" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS := -std=c++14 -Wl,--as-needed -faligned-new -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=./build-$(ConfigurationName)//up_build.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ./build-$(ConfigurationName)//.d $(Objects) 
	@mkdir -p "./build-$(ConfigurationName)/"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@mkdir -p "./build-$(ConfigurationName)/"
	@mkdir -p ""./build-$(ConfigurationName)/bin""

./build-$(ConfigurationName)//.d:
	@mkdir -p "./build-$(ConfigurationName)/"

PreBuild:


##
## Objects
##
./build-$(ConfigurationName)//up_build.cpp$(ObjectSuffix): ../build.cpp ./build-$(ConfigurationName)//up_build.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/skei/Code/SA_Toolkit/build/build.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_build.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//up_build.cpp$(DependSuffix): ../build.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//up_build.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//up_build.cpp$(DependSuffix) -MM ../build.cpp

./build-$(ConfigurationName)//up_build.cpp$(PreprocessSuffix): ../build.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//up_build.cpp$(PreprocessSuffix) ../build.cpp


-include ./build-$(ConfigurationName)///*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


