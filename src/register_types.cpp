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

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT mmdragonbones_library_init(const GDExtensionInterface *p_interface, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_initializer(initialize_mmdragonbones_module);
	init_obj.register_terminator(uninitialize_mmdragonbones_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}