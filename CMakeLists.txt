
set(plug_sources
	include/BaseAPModule.h
	include/ConnectionMatrix.h
	include/EqualizerModule.h
	include/GainModule.h
	include/GuiBaseAPModule.h
	include/GuiCustomRowColumnView.h
	include/GuiCustomSplashScreen.h
	include/GuiCustomTextEdit.h
	include/GuiCustomTextLabel.h
	include/GuiCustomValueEdit.h
	include/GuiGraphicsConnections.h
	include/GuiGraphicsModule.h
	include/GuiGraphicsView.h
	include/GuiOptionMenuInputSelector.h
	include/GuiSignalGenerator.h
	include/GuiWorkspaceView.h
	include/Logging.h
	include/MixerModule.h
	include/PresetReadWrite.h
	include/QuantizerModule.h
	include/ReverbNetworkController.h
	include/ReverbNetworkDefines.h
	include/ReverbNetworkEditor.h
	include/ReverbNetworkEnums.h
	include/reverbnetworkids.h
	include/ReverbNetworkProcessor.h
	include/SchroederAllpass.h
	include/SignalGenerator.h
	include/ValueConversion.h
	include/version.h
	include/XmlPresetReadWrite.h
	pugixml/src/pugiconfig.hpp
	pugixml/src/pugixml.hpp
	src/BaseAPModule.cpp
	src/ConnectionMatrix.cpp
	src/EqualizerModule.cpp
	src/factory.cpp
	src/GainModule.cpp
	src/GuiBaseAPModule.cpp
	src/GuiCustomRowColumnView.cpp
	src/GuiCustomSplashScreen.cpp
	src/GuiCustomTextEdit.cpp
	src/GuiCustomTextLabel.cpp
	src/GuiCustomValueEdit.cpp
	src/GuiGraphicsConnections.cpp
	src/GuiGraphicsModule.cpp
	src/GuiGraphicsView.cpp
	src/GuiOptionMenuInputSelector.cpp
	src/GuiSignalGenerator.cpp
	src/GuiWorkspaceView.cpp
	src/Logging.cpp
	src/MixerModule.cpp
	src/PresetReadWrite.cpp
	src/QuantizerModule.cpp
	src/ReverbNetworkController.cpp
	src/ReverbNetworkEditor.cpp
	src/ReverbNetworkProcessor.cpp
	src/SchroederAllpass.cpp
	src/SignalGenerator.cpp
	src/ValueConversion.cpp
	src/VST2EntryPoint.cpp
	src/XmlPresetReadWrite.cpp
	pugixml/src/pugixml.cpp
	../../VST3_SDK/public.sdk/source/vst/vst2wrapper/vst2wrapper.sdk.cpp
)

#--- HERE change the target Name for your plug-in (for ex. set(target myDelay))-------
set(target reverbnetwork)

smtg_add_vst3plugin(${target} ${SDK_ROOT} ${plug_sources})
set_target_properties(${target} PROPERTIES ${SDK_IDE_MYPLUGINS_FOLDER})
target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
target_link_libraries(${target} PRIVATE base sdk vstgui_support)

smtg_add_vst3_resource(${target} "resource/plug.uidesc")
smtg_add_vst3_resource(${target} "resource/background.png")
smtg_add_vst3_resource(${target} "resource/animation_knob.png")
smtg_add_vst3_resource(${target} "resource/onoff_button.png")
smtg_add_vst3_resource(${target} "resource/background_2x.png")
smtg_add_vst3_resource(${target} "resource/animation_knob_2x.png")
smtg_add_vst3_resource(${target} "resource/onoff_button_2x.png")
smtg_add_vst3_resource(${target} "resource/background_3x.png")
smtg_add_vst3_resource(${target} "resource/animation_knob_3x.png")
smtg_add_vst3_resource(${target} "resource/onoff_button_3x.png")

if(MAC)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
elseif(WIN)
    target_sources(${target} PRIVATE resources/vstgui.rc)
endif()
