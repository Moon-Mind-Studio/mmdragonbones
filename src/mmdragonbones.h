#include "dragonbonesframes.h"

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/ref.hpp>
using namespace godot;

class MMDragonBones : public AnimatedSprite2D {
    GDCLASS(MMDragonBones, AnimatedSprite2D);

protected:

    void _notification(int p_what);
	float _get_frame_duration();
	void _calc_frame_speed_scale();
	static void _bind_methods();
	void _stop_internal(bool p_reset);
	void _res_changed();

public:

    StringName autoplay;
    Point2 offset;
    bool centered;
    bool vflip;
    bool hflip;
    bool playing;
    String animation;
    float custom_speed_scale;
    float speed_scale;
    int frame;
    float frame_progress; 
    float frame_speed_scale;
    Ref<DragonBonesFrames> frames;
	void play(const StringName &p_name = StringName(), float p_custom_scale = 1.0, bool p_from_end = false);
	void set_frame_and_progress(int p_frame, real_t p_progress);
	void pause();
    void set_sprite_frames(const Ref<DragonBonesFrames> &p_frames);
	void set_animation(const StringName &p_name);
	void set_autoplay(const String &p_name);
	float get_playing_speed() const;
	void stop();
    void set_frame(int p_frame);
};