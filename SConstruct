#!/usr/bin/env python
import os
import sys
import fnmatch

env = SConscript("godot-cpp/SConstruct")

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/", "src/dragonBones/", "src/dragonBones/animation/", "src/dragonBones/armature/", "src/dragonBones/core/", "src/dragonBones/event/", "src/dragonBones/factory", "src/dragonBones/geom/", "src/dragonBones/model/", "src/dragonBones/parser/"])

sources = []
for root, dirnames, filenames in os.walk('src'):
  for filename in fnmatch.filter(filenames, '*.cpp'):
    sources.append(os.path.join(root, filename))

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "game/bin/dragonbones/libmmdragonbones.{}.{}.framework/libmmdragonbones.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "game/bin/dragonbones/libmmdragonbones{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)