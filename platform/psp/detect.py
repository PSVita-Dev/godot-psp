
import os
import sys


def is_active():
    return True


def get_name():
    return "PSP"


def can_build():

    if (os.name != "posix"):
        return False

    return True  # enabled


def get_opts():

    return [
        ('use_llvm', 'Use llvm compiler', 'no'),
        ('force_32_bits', 'Force 32 bits binary', 'yes'),
        ('openssl', 'openssl', 'no'),
        ('tools', 'Build the tools a.k.a. the Godot editor (yes/no)', 'no'),
        ('disable_3d','','no'),
        ('builtin_libwebp', '', 'no'),
    ]


def get_flags():

    return [
    ]


def configure(env):

    env.Append(CPPPATH=['#platform/psp'])
    env["CC"] = "psp-gcc"
    env["CXX"] = "psp-g++"
    env["LD"] = "psp-g++"
    
    env["bits"] = "32"
    env.disabled_modules = ['openssl','webp']
    
    psp_path = os.environ["PSPDEV"]
    
    env.Append(CPPPATH=[psp_path+"/psp/sdk/include/"])
    env.Append(LIBPATH=[psp_path+"/psp/sdk/lib"])
    
    env.Append(LIBS=["GL", "GLU", "glut", "m" ,"c","stdc++", "png","z","libc" , "pspdebug" ,"pspge","pspdisplay" ,"pspctrl" ,"pspsdk", "pspvfpu" ,"psppower", "pspaudiolib", "pspaudio", "pspgu", "pspgum"])

    # if (env["tools"]=="no"):
    #	#no tools suffix
    #	env['OBJSUFFIX'] = ".nt"+env['OBJSUFFIX']
    #	env['LIBSUFFIX'] = ".nt"+env['LIBSUFFIX']

    if (env["target"] == "release"):

        env.Append(CCFLAGS=['-O2', '-ffast-math', '-fomit-frame-pointer'])

    elif (env["target"] == "release_debug"):

        env.Append(CCFLAGS=['-O2', '-ffast-math', '-DDEBUG_ENABLED'])

    elif (env["target"] == "debug"):

        env.Append(CCFLAGS=['-g2', '-DDEBUG_ENABLED', '-DDEBUG_MEMORY_ENABLED'])


    # Shared libraries, when requested
    #env['builtin_openssl'] = 'no'
    #if (env['builtin_openssl'] == 'no'):
        #env.ParseConfig('pkg-config openssl --cflags --libs')

    #if (env['builtin_libwebp'] == 'no'):
        #env.ParseConfig('pkg-config libwebp --cflags --libs')

    if (env['builtin_freetype'] == 'no'):
        env.ParseConfig('pkg-config freetype2 --cflags --libs')

    if (env['builtin_libpng'] == 'no'):
        env.ParseConfig('pkg-config libpng --cflags --libs')

    # Sound and video libraries
    # Keep the order as it triggers chained dependencies (ogg needed by others, etc.)

    if (env['builtin_libtheora'] == 'no'):
        env['builtin_libogg'] = 'no'  # Needed to link against system libtheora
        env['builtin_libvorbis'] = 'no'  # Needed to link against system libtheora
        env.ParseConfig('pkg-config theora theoradec --cflags --libs')

    if (env['builtin_libvorbis'] == 'no'):
        env['builtin_libogg'] = 'no'  # Needed to link against system libvorbis
        env.ParseConfig('pkg-config vorbis vorbisfile --cflags --libs')

    if (env['builtin_opus'] == 'no'):
        env['builtin_libogg'] = 'no'  # Needed to link against system opus
        env.ParseConfig('pkg-config opus opusfile --cflags --libs')

    if (env['builtin_libogg'] == 'no'):
        env.ParseConfig('pkg-config ogg --cflags --libs')


    env.Append(CPPFLAGS=['-DPSP_ENABLED','-DNEED_LONG_INT', '-DNO_STATVFS', '-DPTHREAD_ENABLED', '-DPTHREAD_NO_RENAME'])#, '-DUNIX_ENABLED'])
    env.Append(LIBS=['pthread', 'z'])  # TODO detect linux/BSD!

    if (env["CXX"] == "clang++"):
        env.Append(CPPFLAGS=['-DTYPED_METHOD_BIND'])
        env["CC"] = "clang"
        env["LD"] = "clang++"
