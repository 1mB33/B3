import sys
import os


g_IsVerbose=True
g_RunFlag=0
g_ProjectBuildName="All"
g_ProjectBuildDir=""
g_HasFiles=False
g_GenerateCmd="cmake ../../"


def IsTool(name):
    from shutil import which
    return which(name) is not None


def VerbosePrint(msg: str):
    if g_IsVerbose:
        print("VERBOSE: " + msg)
    pass


F_BUILD_ALL=0x01
F_BUILD_TESTS=0x02
def AppendRunFlag(switch: str) -> int:
    global g_RunFlag

    VerbosePrint("Parsing switch: " + switch)

    match switch.lower():
        case "--all":
            g_RunFlag = g_RunFlag | F_BUILD_ALL;
        case "--tests":
            g_RunFlag = g_RunFlag | F_BUILD_TESTS;


def ValidateSystem():
    global g_ProjectBuildDir
    global g_GenerateCmd
    global g_HasFiles
    
    generateCachePath = g_ProjectBuildDir + "CMakeCache.txt"
    if os.path.exists(generateCachePath):
        g_HasFiles = True
        return

    # Prefer ninja over make
    if IsTool("ninja"):
        g_GenerateCmd = g_GenerateCmd + " -G Ninja"
    pass


def CreateBuildResources():
    global g_ProjectBuildDir

    g_ProjectBuildDir = "./Build/" + g_ProjectBuildName + "/"

    ValidateSystem()

    if not os.path.exists(g_ProjectBuildDir):
        os.makedirs(g_ProjectBuildDir)

    if not g_HasFiles:
        os.chdir(g_ProjectBuildDir)
        generateResult = os.system(g_GenerateCmd)
        VerbosePrint("generateResult: " + str(generateResult))
        os.chdir("../..")

    pass


if __name__ == "__main__":
    VerbosePrint("Build script called")
    for i in range(1, len(sys.argv)):
        AppendRunFlag(sys.argv[i])
    VerbosePrint("g_RunFlag: " + hex(g_RunFlag))

    CreateBuildResources()

    os.chdir(g_ProjectBuildDir)
    if (g_RunFlag & (F_BUILD_TESTS | F_BUILD_ALL)):
        os.system("cmake --build .")
    
    exit(0)
