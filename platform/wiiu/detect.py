
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
    
    env.Append(CPPPATH=[wut_path+"/include", devkitpro_path +"/portlibs/wiiu/include/angle"])
    env.Append(LIBPATH=[wut_path+"/lib", devkitpro_path+"/portlibs/ppc/lib/"])
    # env.Append(DEVKITPRO=devkitpro_path)
    env.Append(ENV={"DEVKITPRO": devkitpro_path})
    env.Append(LINKFLAGS=["-specs="+wut_path+"/share/wut.specs"])
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
