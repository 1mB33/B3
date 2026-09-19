import sys
import re
import os
import shutil


g_IsVerbose=False
g_RunFlag=0
g_ProjectBuildName="All"
g_ProjectBuildDir=""
g_Indention=2
g_GenerateCmd="cmake"


def IsTool(name):
    return shutil. which(name) is not None


def VerbosePrint(msg: str):
    if g_IsVerbose:
        print("VERBOSE: " + msg)
    pass


F_BUILD_ALL=0x01
F_BUILD_TESTS=0x02
F_BUILD_DEBUG=0x04
F_BUILD_RELEASE=0x08
F_BUILD_VERBOSE=0x10
def ParseSwitches(switches: str):
    global g_RunFlag
    global g_GenerateCmd
    global g_IsVerbose

    if switches.find('v') != -1: 
        g_IsVerbose = True

    if not re.match(r"^\-.+", switches):
        VerbosePrint("Invalid switches format: ", switches)

    switches = switches[1:]

    for s in switches:
        VerbosePrint("On switch: " + s)
        match s:
            case 'r':
                if g_RunFlag & F_BUILD_DEBUG:
                    VerbosePrint("Got F_BUILD_RELEASE ('r'), but F_BUILD_DEBUG ('d') is already ON")
                    continue
                VerbosePrint("F_BUILD_RELEASE")
                g_RunFlag = g_RunFlag | F_BUILD_RELEASE
                g_GenerateCmd = g_GenerateCmd + " -DCMAKE_BUILD_TYPE=Release"
            case 'd':
                if g_RunFlag & F_BUILD_RELEASE:
                    VerbosePrint("Got F_BUILD_RELEASE ('r'), but F_BUILD_DEBUG ('d') is already ON")
                    continue
                VerbosePrint("F_BUILD_DEBUG")
                g_RunFlag = g_RunFlag | F_BUILD_DEBUG
                g_GenerateCmd = g_GenerateCmd + " -DCMAKE_BUILD_TYPE=Debug"
            case 'v':
                VerbosePrint("F_BUILD_VERBOSE")
                g_RunFlag = g_RunFlag | F_BUILD_VERBOSE
    pass


def AppendRunFlag(switch: str):
    global g_RunFlag

    VerbosePrint("Parsing switch: " + switch)

    match switch.lower():
        case "--all":
            VerbosePrint("F_BUILD_ALL")
            g_RunFlag = g_RunFlag | F_BUILD_ALL;
        case "--tests":
            VerbosePrint("F_BUILD_TESTS")
            g_RunFlag = g_RunFlag | F_BUILD_TESTS;
        case _:
            ParseSwitches(switch)


def ValidateSystem():
    global g_GenerateCmd

    # Prefer ninja over make
    if IsTool("ninja"):
        g_GenerateCmd = g_GenerateCmd + " -G Ninja"
    pass


def CreateBuildResources():
    global g_GenerateCmd
    global g_ProjectBuildDir
    global g_RunFlag
    global g_Indention

    g_ProjectBuildDir = "./Build/" + g_ProjectBuildName + "/"
    if g_RunFlag & F_BUILD_TESTS:
        g_ProjectBuildDir = g_ProjectBuildDir + 'Tests/'
        g_Indention += 1
    if g_RunFlag & F_BUILD_RELEASE:
        g_ProjectBuildDir = g_ProjectBuildDir + 'Release/'
        g_Indention += 1
    if g_RunFlag & F_BUILD_DEBUG:
        g_ProjectBuildDir = g_ProjectBuildDir + 'Debug/'
        g_Indention += 1

    VerbosePrint("CMake dir: " + g_ProjectBuildDir)

    ValidateSystem()

    if not os.path.exists(g_ProjectBuildDir):
        os.makedirs(g_ProjectBuildDir)

    indention = ""
    for i in range(0, g_Indention):
        indention = indention + "../"

    g_GenerateCmd = g_GenerateCmd + " " + indention

    os.chdir(g_ProjectBuildDir)
    VerbosePrint("g_GenerateCmd: " + g_GenerateCmd)
    generateResult = os.system(g_GenerateCmd)
    VerbosePrint("generateResult: " + str(generateResult))
    for i in range(0, g_Indention):
        os.chdir("..")

    pass


if __name__ == "__main__":
    for i in range(1, len(sys.argv)):
        AppendRunFlag(sys.argv[i])

    VerbosePrint("g_RunFlag: " + hex(g_RunFlag))

    CreateBuildResources()

    os.chdir(g_ProjectBuildDir)
    if (g_RunFlag & (F_BUILD_TESTS | F_BUILD_ALL)):
        indention = ""
        for i in range(0, g_Indention):
            indention = indention + "../"
        os.system("cmake --build .")
        shutil.copy("compile_commands.json", indention + "compile_commands.json")

    
    exit(0)
