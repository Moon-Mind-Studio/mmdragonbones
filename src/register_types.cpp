#include "register_types.h"
#include "mmdragonbones.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_mmdragonbones_module(ModuleInitializationLevel p_level) {
    GDREGISTER_CLASS(DragonBonesFrames)
    GDREGISTER_CLASS(MMDragonBones)
}

void uninitialize_mmdragonbones_module(ModuleInitializationLevel p_level) {

}