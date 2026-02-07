cmake_minimum_required(VERSION 3.15)

project(Tarantism VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add VST3 SDK
set(SMTG_VSTGUI_ROOT "${VST3_SDK_ROOT}")
add_subdirectory(vst3sdk)

# Plugin target
smtg_add_vst3plugin(Tarantism
    source/pluginprocessor.cpp
    source/pluginprocessor.h
    source/plugincontroller.cpp
    source/plugincontroller.h
    source/pluginids.h
    source/version.h
    source/factory.cpp
)

# Set plugin properties
smtg_target_set_bundle(Tarantism
    BUNDLE_IDENTIFIER com.gnar.tarantism
    COMPANY_NAME "Gnar"
)

# Link VSTGUI for GUI
target_link_libraries(Tarantism
    PRIVATE
        sdk
        vstgui_support
)

# Set plugin category
smtg_target_add_plugin_snapshots(Tarantism
    RESOURCES
        resource/plugin.uidesc
)

if(SMTG_MAC)
    smtg_target_set_bundle(Tarantism
        BUNDLE_IDENTIFIER com.gnar.tarantism
        COMPANY_NAME "Gnar"
    )
elseif(SMTG_WIN)
    target_sources(Tarantism PRIVATE 
        resource/plugin.rc
    )
endif()
