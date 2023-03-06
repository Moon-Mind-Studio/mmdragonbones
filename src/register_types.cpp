#include "register_types.h"
#include "mmdragonbones.h"
#include "GDMesh.h"
#include "GDArmatureDisplay.h"

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
	ResourceLoader::add_resource_format_loader(resource_loader_mmdragonbones, true);
}

void unregister_mmdragonbones_types(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ResourceLoader::remove_resource_format_loader(resource_loader_mmdragonbones);
	resource_loader_mmdragonbones.unref();
}