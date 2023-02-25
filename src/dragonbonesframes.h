#include "dragonBones/DragonBonesHeaders.h"

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/ref.hpp>
using namespace godot;

class DragonBonesFrames : public Resource
{
GDCLASS(DragonBonesFrames, Resource);

protected:
    struct Frame {
		Ref<Texture2D> texture;
		float duration = 1.0;
	};
    
    struct Animation
    {
        double speed = 5.0;
		bool loop = true;
		Vector<Frame> frames;
    };
    
    HashMap<StringName, Animation> _animations;
    static void _bind_methods();

public:
    bool has_animation(const String &animation_name);
    int count_animation_frames(const String &animation_name);
    PackedStringArray get_animation_names();
    float get_frame_duration(const String &animation_name, int frame_index);
	float get_animation_speed(const StringName &p_anim) const;
	bool get_animation_loop(const StringName &p_anim) const;
	Ref<Texture2D> get_frame_texture(const StringName &p_anim, int p_idx) const;
	void get_animation_list(List<StringName> *r_animations) const;
};