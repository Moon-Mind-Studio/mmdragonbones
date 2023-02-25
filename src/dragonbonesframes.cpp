#include "dragonbonesframes.h"

#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/ref.hpp>
using namespace godot;

bool DragonBonesFrames::has_animation(const String &animation_name)
{
    return _animations.has(animation_name);
}

int DragonBonesFrames::count_animation_frames(const String &animation_name)
{
    ERR_FAIL_COND_V_MSG(!has_animation(animation_name), 1.0, "Animation '" + String(animation_name) + "' doesn't exists.");

    return _animations[animation_name].frames.size();
}

PackedStringArray DragonBonesFrames::get_animation_names()
{
    PackedStringArray animation_names;
	for (const KeyValue<StringName, Animation> &E : _animations) {
		animation_names.push_back(E.key);
	}
	animation_names.sort();
	return animation_names;
}

float DragonBonesFrames::get_frame_duration(const String &animation_name, int frame_index)
{
    ERR_FAIL_COND_V_MSG(!has_animation(animation_name), 1.0, "Animation '" + String(animation_name) + "' doesn't exists.");
    ERR_FAIL_COND_V_MSG(frame_index >= count_animation_frames(animation_name), 1.0, "Index out of value.");

    return _animations[animation_name].frames[frame_index].duration;
}

float DragonBonesFrames::get_animation_speed(const StringName &p_anim) const {
	HashMap<StringName, Animation>::ConstIterator E = _animations.find(p_anim);
	ERR_FAIL_COND_V_MSG(!E, 0, "Animation '" + String(p_anim) + "' doesn't exist.");
	return E->value.speed;
}

bool DragonBonesFrames::get_animation_loop(const StringName &p_anim) const {
	HashMap<StringName, Animation>::ConstIterator E = _animations.find(p_anim);
	ERR_FAIL_COND_V_MSG(!E, false, "Animation '" + String(p_anim) + "' doesn't exist.");
	return E->value.loop;
}

Ref<Texture2D> DragonBonesFrames::get_frame_texture(const StringName &p_anim, int p_idx) const {
		HashMap<StringName, Animation>::ConstIterator E = _animations.find(p_anim);
		ERR_FAIL_COND_V_MSG(!E, Ref<Texture2D>(), "Animation '" + String(p_anim) + "' doesn't exist.");
		ERR_FAIL_COND_V(p_idx < 0, Ref<Texture2D>());
		if (p_idx >= E->value.frames.size()) {
			return Ref<Texture2D>();
		}

		return E->value.frames[p_idx].texture;
}

void DragonBonesFrames::get_animation_list(List<StringName> *r_animations) const {
	for (const KeyValue<StringName, Animation> &E : _animations) {
		r_animations->push_back(E.key);
	}
}

void DragonBonesFrames::_bind_methods() {
}