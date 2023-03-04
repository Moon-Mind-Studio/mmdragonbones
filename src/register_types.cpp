#include "mmdragonbones.h"
#include "GDArmatureDisplay.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_format_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <gdextension_interface.h>
#include <godot_cpp/classes/ref.hpp>

class ResourceFormatLoaderMMDragonBones : public ResourceFormatLoader {
public:
	    virtual Ref<Resource> load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, bool p_no_cache) {

		MMDragonBones::MMDragonBonesResource* __p_res = memnew(MMDragonBones::MMDragonBonesResource);
		Ref<MMDragonBones::MMDragonBonesResource> __p_ref(__p_res);
		
		String __str_path_base = p_path.get_basename();

        __str_path_base.trim_suffix("_ske");

        // texture path
        __p_ref->set_def_texture_path(__str_path_base + "_tex.png");

        // loading atlas data
        bool __bret = __p_ref->load_texture_atlas_data(String(__str_path_base + "_tex.json").ascii().get_data());
        ERR_FAIL_COND_V(!__bret, 0);

        // loading bones data
        __bret = __p_ref->load_bones_data(p_path.ascii().get_data());
        ERR_FAIL_COND_V(!__bret, 0);

	    __p_res->set_def_texture_path(p_path);

		return __p_ref;
	}


    virtual void get_recognized_extensions(List<String> *p_extensions) const
    {
		p_extensions->push_back("dbbin");
		p_extensions->push_back("json");
	}

    virtual bool handles_type(const String& p_type) const
    {
		return p_type=="mmdragonbonesResource";
	}

    virtual String get_resource_type(const String &p_path) const
    {
		String el = p_path.get_extension().to_lower();

        if ((el == "json" || el == "dbbin") && p_path.get_basename().to_lower().ends_with("_ske"))
            return "mmdragonbonesResource";
        return "";
    }
};

static Ref<ResourceFormatLoaderMMDragonBones> resource_loader_mmdragonbones;

void register_mmdragonbones_types(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

    GDREGISTER_VIRTUAL_CLASS(GDOwnerNode);
    GDREGISTER_VIRTUAL_CLASS(GDDisplay);
    
	GDREGISTER_VIRTUAL_CLASS(GDArmatureDisplay);
	GDREGISTER_VIRTUAL_CLASS(GDSlot);
	GDREGISTER_VIRTUAL_CLASS(GDBone2D);

    GDREGISTER_CLASS(MMDragonBones);

	resource_loader_mmdragonbones.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_mmdragonbones);
}

void unregister_mmdragonbones_types(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ResourceLoader::remove_resource_format_loader(resource_loader_mmdragonbones);
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