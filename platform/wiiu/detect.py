
import os
import sys


def is_active():
    return True


def get_name():
    return "wiiu"


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

"""
def configure(env):
    arch = ['-march=armv6k', '-mtune=mpcore','-mfloat-abi=hard']

    env.Append(CPPPATH=['#platform/3ds'])
    env["CC"] = "arm-none-eabi-gcc"
    env["CXX"] = "arm-none-eabi-g++"
    env["LD"] = "arm-none-eabi-g++"
    env["AR"]="arm-none-eabi-ar"
    env["RANLIB"]="arm-none-eabi-ranlib"
    env["AS"]="arm-none-eabi-as"
    
    env["bits"] = "32"
    env.disabled_modules = ['openssl','webp']
    
    dev_path = os.environ["DEVKITARM"]

    env.Append(CCFLAGS=['-g','-Wall','-mword-relocations','-ffunction-sections', '-fno-rtti', '-fno-exceptions', '-std=gnu++11'])

    env.Append(CPPFLAGS=['-march=armv6k', '-mtune=mpcore', '-mfloat-abi=hard','-g','-Wall','-mword-relocations','-ffunction-sections', '-fno-rtti', '-fno-exceptions', '-std=gnu++11','-D__3DS__', '-DARM11','-DNEED_LONG_INT', '-DNO_STATVFS', '-DPTHREAD_ENABLED', '-DPTHREAD_NO_RENAME','-DNO_SAFE_CAST'])

    
    env.Append(CPPPATH=[dev_path+"/arm-none-eabi/include/", os.environ["CTRULIB"]+"/include"])
    env.Append(LIBPATH=[dev_path+"/arm-none-eabi/lib/", os.environ["DEVKITPRO"]+"/portlibs/3ds/lib/", os.environ["CTRULIB"]+"/lib/"])
    
    env.Append(LIBS=["picaGL", "ctru" , "png","z"])

    env.Append(LINKFLAGS=['-specs=3dsx.specs', '-g', '-march=armv6k', '-mtune=mpcore', '-mfloat-abi=hard'])

    # if (env["tools"]=="no"):
    #	#no tools suffix
    #	env['OBJSUFFIX'] = ".nt"+env['OBJSUFFIX']
    #	env['LIBSUFFIX'] = ".nt"+env['LIBSUFFIX']

    if (env["target"] == "release"):

        env.Append(CCFLAGS=['-O2', '-ffast-math', '-fomit-frame-pointer'] + arch)

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


    # env.Append(CPPFLAGS=['-D__3DS__', '-DARM11','-DNEED_LONG_INT', '-DNO_STATVFS', '-DPTHREAD_ENABLED', '-DPTHREAD_NO_RENAME','-DNO_SAFE_CAST'])#, '-DUNIX_ENABLED'])
    env.Append(LIBS=['pthread', 'z'])  # TODO detect linux/BSD!

    if (env["CXX"] == "clang++"):
        env.Append(CPPFLAGS=['-DTYPED_METHOD_BIND'])
        env["CC"] = "clang"
        env["LD"] = "clang++"
"""
def configure(env):
    env.disabled_modules = ['openssl','webp']

    env["bits"] = "32"
    devkitpro_path = os.environ["DEVKITPRO"]
    devkitarm_path = devkitpro_path + "/devkitPPC"
    wut_path = devkitpro_path + "/wut"
    
    env.Append(CPPPATH=['#platform/wiiu'])
    env["CC"] = devkitarm_path + "/bin/powerpc-eabi-gcc"
    env["CXX"] = devkitarm_path + "/bin/powerpc-eabi-g++"
    env["LD"] = devkitarm_path + "/bin/powerpc-eabi-g++"
    env["AR"] = devkitarm_path + "/bin/powerpc-eabi-ar"
    env["RANLIB"] = devkitarm_path + "/bin/powerpc-eabi-ranlib"
    env["AS"] = devkitarm_path + "/bin/powerpc-eabi-as"

    arch = ['-isystem', '-mcpu=750' '-meabi', '-mhard-float', '-ffunction-sections', '-fdata-sections']
    env.Append(CCFLAGS=['-g','-Wall', '-std=gnu++11'] + arch)
    env.Append(CCFLAGS=[ '-DESPRESSO', '-D__WIIU__', '-D__WUT__','-DNEED_LONG_INT', '-DLIBC_FILEIO_ENABLED','-DNO_SAFE_CAST', '-DPTHREAD_NO_RENAME'])
    
    env.Append(CPPPATH=[wut_path+"/include"])
    env.Append(LIBPATH=[wut_path+"/lib", devkitpro_path+"/portlibs/ppc/lib/"])
    # env.Append(LIBPATH=[devkitpro_path+"/portlibs/armv6k/lib", devkitpro_path +
               # "/portlibs/3ds/lib", ctrulib_path + "/lib", devkitarm_path + "/arm-none-eabi/lib/armv6k/fpu"])

    # env.Append(LINKFLAGS=['-specs=3dsx.specs', '-g'] + arch)
    env.Append(LIBS=["m","gx2gl", "wut"])
    env.Append(LIBS=["png", "z"])



    if (env["target"] == "release"):
        if (env["debug_release"] == "yes"):
            env.Append(CCFLAGS=['-g2'])
        else:
            env.Append(CCFLAGS=['-O3'])
    elif (env["target"] == "release_debug"):
        env.Append(CCFLAGS=['-O2', '-ffast-math', '-DDEBUG_ENABLED'])
        if (env["debug_release"] == "yes"):
            env.Append(CCFLAGS=['-g2'])
    elif (env["target"] == "debug"):
        env.Append(CCFLAGS=['-O3','-g2','-Wall',
                   '-DDEBUG_ENABLED', '-DDEBUG_MEMORY_ENABLED'])
