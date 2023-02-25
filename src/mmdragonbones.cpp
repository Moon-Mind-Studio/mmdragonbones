#include "mmdragonbones.h"

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/ref.hpp>
using namespace godot;

void MMDragonBones::play(const StringName &p_name, float p_custom_scale, bool p_from_end) {
	StringName name = p_name;

	if (name == StringName()) {
		name = animation;
	}

	ERR_FAIL_COND_MSG(frames == nullptr, vformat("There is no animation with name '%s'.", name));
	ERR_FAIL_COND_MSG(!frames->get_animation_names().has(name), vformat("There is no animation with name '%s'.", name));

	if (frames->count_animation_frames(name) == 0) {
		return;
	}	

	playing = true;
	custom_speed_scale = p_custom_scale;

	int end_frame = MAX(0, frames->count_animation_frames(animation) - 1);
	if (name != animation) {
		animation = name;
		if (p_from_end) {
			set_frame_and_progress(end_frame, 1.0);
		} else {
			set_frame_and_progress(0, 0.0);
		}
		emit_signal("animation_changed");
	} else {
		bool is_backward = signbit(speed_scale * custom_speed_scale);
		if (p_from_end && is_backward && frame == 0 && frame_progress <= 0.0) {
			set_frame_and_progress(end_frame, 1.0);
		} else if (!p_from_end && !is_backward && frame == end_frame && frame_progress >= 1.0) {
			set_frame_and_progress(0, 0.0);
		}
	}

	set_process_internal(true);
	notify_property_list_changed();
	queue_redraw();
}

void MMDragonBones::set_frame_and_progress(int p_frame, real_t p_progress) {
	if (frames.is_null()) {
		return;
	}

	bool has_animation = (frames->has_animation(animation));
	int end_frame = has_animation ? MAX(0, frames->count_animation_frames(animation) - 1) : 0;
	bool is_changed = frame != p_frame;

	if (p_frame < 0) {
		frame = 0;
	} else if (has_animation && p_frame > end_frame) {
		frame = end_frame;
	} else {
		frame = p_frame;
	}

	_calc_frame_speed_scale();
	frame_progress = p_progress;

	if (!is_changed) {
		return; // No change, don't redraw.
	}

	queue_redraw();
	emit_signal("frame_changed");
}

void MMDragonBones::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			if (!Engine::get_singleton()->is_editor_hint() && !frames.is_null() && frames->has_animation(autoplay)) {
				play(autoplay);
			}
		} break;

		case NOTIFICATION_INTERNAL_PROCESS: {
			if (frames.is_null() || !frames->has_animation(animation)) {
				return;
			}

			double remaining = get_process_delta_time();
			int i = 0;
			while (remaining) {
				// Animation speed may be changed by animation_finished or frame_changed signals.
				double speed = frames->get_animation_speed(animation) * speed_scale * custom_speed_scale * frame_speed_scale;
				double abs_speed = Math::abs(speed);

				if (speed == 0) {
					return; // Do nothing.
				}

				// Frame count may be changed by animation_finished or frame_changed signals.
				int fc = frames->count_animation_frames(animation);

				int last_frame = fc - 1;
				if (!signbit(speed)) {
					// Forwards.
					if (frame_progress >= 1.0) {
						if (frame >= last_frame) {
							if (frames->get_animation_loop(animation)) {
								frame = 0;
								emit_signal("animation_looped");
							} else {
								frame = last_frame;
								pause();
								emit_signal("animation_finished");
								return;
							}
						} else {
							frame++;
						}
						_calc_frame_speed_scale();
						frame_progress = 0.0;
						queue_redraw();
						emit_signal("frame_changed");
					}
					double to_process = MIN((1.0 - frame_progress) / abs_speed, remaining);
					frame_progress += to_process * abs_speed;
					remaining -= to_process;
				} else {
					// Backwards.
					if (frame_progress <= 0) {
						if (frame <= 0) {
							if (frames->get_animation_loop(animation)) {
								frame = last_frame;
								emit_signal("animation_looped");
							} else {
								frame = 0;
								pause();
								emit_signal("animation_finished");
								return;
							}
						} else {
							frame--;
						}
						_calc_frame_speed_scale();
						frame_progress = 1.0;
						queue_redraw();
						emit_signal("frame_changed");
					}
					double to_process = MIN(frame_progress / abs_speed, remaining);
					frame_progress -= to_process * abs_speed;
					remaining -= to_process;
				}

				i++;
				if (i > fc) {
					return; // Prevents freezing if to_process is each time much less than remaining.
				}
			}
		} break;

		case NOTIFICATION_DRAW: {
			if (frames.is_null() || !frames->has_animation(animation)) {
				return;
			}

			Ref<Texture2D> texture = frames->get_frame_texture(animation, frame);
			if (texture.is_null()) {
				return;
			}

			RID ci = get_canvas_item();

			Size2 s = texture->get_size();
			Point2 ofs = offset;
			if (centered) {
				ofs -= s / 2;
			}

			if (get_viewport() && get_viewport()->is_snap_2d_transforms_to_pixel_enabled()) {
				ofs = ofs.floor();
			}
			Rect2 dst_rect(ofs, s);

			if (hflip) {
				dst_rect.size.x = -dst_rect.size.x;
			}
			if (vflip) {
				dst_rect.size.y = -dst_rect.size.y;
			}

			texture->draw_rect_region(ci, dst_rect, Rect2(Vector2(), texture->get_size()), Color(1, 1, 1), false);
		} break;
	}
}

void MMDragonBones::set_sprite_frames(const Ref<DragonBonesFrames> &p_frames) {
	if (frames == p_frames) {
		return;
	}

	if (frames.is_valid()) {
		frames->disconnect("changed", Callable(this, "_res_changed"));
	}
	stop();
	frames = p_frames;
	if (frames.is_valid()) {
		frames->connect("changed", Callable(this, "_res_changed"));

		List<StringName> al;
		frames->get_animation_list(&al);
		if (al.size() == 0) {
			set_animation(StringName());
			set_autoplay(String());
		} else {
			if (!frames->has_animation(animation)) {
				set_animation(al[0]);
			}
			if (!frames->has_animation(autoplay)) {
				set_autoplay(String());
			}
		}
	}

	notify_property_list_changed();
	queue_redraw();
	update_configuration_warnings();
	emit_signal("sprite_frames_changed");
}

void MMDragonBones::set_animation(const StringName &p_name) {
	if (animation == p_name) {
		return;
	}

	animation = p_name;

	emit_signal("animation_changed");

	if (frames == nullptr) {
		animation = StringName();
		stop();
		ERR_FAIL_MSG(vformat("There is no animation with name '%s'.", p_name));
	}

	int frame_count = frames->count_animation_frames(animation);
	if (animation == StringName() || frame_count == 0) {
		stop();
		return;
	} else if (!frames->has_animation(animation)) {
		animation = StringName();
		stop();
		ERR_FAIL_MSG(vformat("There is no animation with name '%s'.", p_name));
	}

	if (signbit(get_playing_speed())) {
		set_frame_and_progress(frame_count - 1, 1.0);
	} else {
		set_frame_and_progress(0, 0.0);
	}

	notify_property_list_changed();
	queue_redraw();
}

void MMDragonBones::set_autoplay(const String &p_name) {
	if (is_inside_tree() && !Engine::get_singleton()->is_editor_hint()) {
		WARN_PRINT("Setting autoplay after the node has been added to the scene has no effect.");
	}

	autoplay = p_name;
}

float MMDragonBones::get_playing_speed() const {
	if (!playing) {
		return 0;
	}
	return speed_scale * custom_speed_scale;
}

void MMDragonBones::_res_changed() {
	set_frame_and_progress(frame, frame_progress);
	queue_redraw();
	notify_property_list_changed();
}

void MMDragonBones::stop() {
	_stop_internal(true);
}

void MMDragonBones::pause() {
	_stop_internal(false);
}

void MMDragonBones::_stop_internal(bool p_reset) {
	playing = false;
	if (p_reset) {
		custom_speed_scale = 1.0;
		set_frame_and_progress(0, 0.0);
	}
	notify_property_list_changed();
	set_process_internal(false);
}

float MMDragonBones::_get_frame_duration() {
	if (frames.is_valid() && frames->has_animation(animation)) {
		return frames->get_frame_duration(animation, frame);
	}
	return 1.0;
}

void MMDragonBones::_calc_frame_speed_scale() {
	frame_speed_scale = 1.0 / _get_frame_duration();
}

void MMDragonBones::set_frame(int p_frame) {
	set_frame_and_progress(p_frame, signbit(get_playing_speed()) ? 1.0 : 0.0);
}

void MMDragonBones::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_sprite_frames", "sprite_frames"), &MMDragonBones::set_sprite_frames);

	ClassDB::bind_method(D_METHOD("set_animation", "name"), &MMDragonBones::set_animation);

	ClassDB::bind_method(D_METHOD("set_autoplay", "name"), &MMDragonBones::set_autoplay);

	ClassDB::bind_method(D_METHOD("play", "name", "custom_speed", "from_end"), &MMDragonBones::play, DEFVAL(StringName()), DEFVAL(1.0), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("pause"), &MMDragonBones::pause);
	ClassDB::bind_method(D_METHOD("stop"), &MMDragonBones::stop);

	ClassDB::bind_method(D_METHOD("set_frame", "frame"), &MMDragonBones::set_frame);

	ClassDB::bind_method(D_METHOD("set_frame_and_progress", "frame", "progress"), &MMDragonBones::set_frame_and_progress);

	ClassDB::bind_method(D_METHOD("get_playing_speed"), &MMDragonBones::get_playing_speed);

	ADD_SIGNAL(MethodInfo("sprite_frames_changed"));
	ADD_SIGNAL(MethodInfo("animation_changed"));
	ADD_SIGNAL(MethodInfo("frame_changed"));
	ADD_SIGNAL(MethodInfo("animation_looped"));
	ADD_SIGNAL(MethodInfo("animation_finished"));

	// ADD_GROUP("Animation", "");
	// ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sprite_frames", PROPERTY_HINT_RESOURCE_TYPE, "SpriteFrames"), "set_sprite_frames", "get_sprite_frames");
	// ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "animation", PROPERTY_HINT_ENUM, ""), "set_animation", "get_animation");
	// ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "autoplay", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_autoplay", "get_autoplay");
	// ADD_PROPERTY(PropertyInfo(Variant::INT, "frame"), "set_frame", "get_frame");
	// ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "frame_progress", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_frame_progress", "get_frame_progress");
	// ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed_scale"), "set_speed_scale", "get_speed_scale");
	// ADD_GROUP("Offset", "");
	// ADD_PROPERTY(PropertyInfo(Variant::BOOL, "centered"), "set_centered", "is_centered");
	// ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset", PROPERTY_HINT_NONE, "suffix:px"), "set_offset", "get_offset");
	// ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_h"), "set_flip_h", "is_flipped_h");
	// ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_v"), "set_flip_v", "is_flipped_v");
}