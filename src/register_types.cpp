#include "mmdragonbones.h"
#include "GDMesh.h"
#include "GDArmatureDisplay.h"

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_format_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <gdextension_interface.h>
#include <godot_cpp/classes/ref.hpp>

static Ref<ResourceFormatLoaderMMDB> resource_loader_mmdragonbones;

void register_mmdragonbones_types(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

    ClassDB::register_class<GDOwnerNode>(true);
    ClassDB::register_class<GDDisplay>(true);
    
	ClassDB::register_class<GDArmatureDisplay>(true);
	ClassDB::register_class<GDSlot>(true);
	ClassDB::register_class<GDBone2D>(true);
	ClassDB::register_class<GDMesh>(true);

    ClassDB::register_class<MMDragonBonesResource>();
    ClassDB::register_class<MMDragonBones>();

	resource_loader_mmdragonbones.instantiate();
	ResourceLoader::get_singleton()->add_resource_format_loader(resource_loader_mmdragonbones, true);
}

void unregister_mmdragonbones_types(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ResourceLoader::get_singleton()->remove_resource_format_loader(resource_loader_mmdragonbones);
	resource_loader_mmdragonbones.unref();
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT mmdragonbones_library_init(const GDExtensionInterface *p_interface, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

    init_obj.register_initializer(register_mmdragonbones_types);
	init_obj.register_terminator(unregister_mmdragonbones_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
	
    return init_obj.init();
}
}