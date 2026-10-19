import sys
import re
import os
import shutil


g_RunFlag=0x0
g_IsVerbose=False
g_ProjectName=""
g_ProjectPath=""


def VerbosePrint(msg: str):
    if g_IsVerbose:
        print("VERBOSE: " + msg)
    pass


F_PROJECT_CREATE=0x01
def ParseSwitches(switches: str):
    global g_RunFlag
    global g_GenerateCmd
    global g_IsVerbose
    global g_ProjectName

    # Turn verbose ASAP
    if switches.find('v') != -1: 
        g_IsVerbose = True

    if not re.match(r"^\-.+", switches):
        if g_ProjectName:
            print("Invalid switches format: ", switches)
            exit(1)
        VerbosePrint("g_ProjectName: " + switches)
        g_ProjectName = switches
        return

    switches = switches[1:]

    for s in switches:
        VerbosePrint("On switch: " + s)
        match s:
            case 'c':
                VerbosePrint("F_PROJCET_CREATE")
                g_RunFlag = g_RunFlag | F_PROJECT_CREATE
    pass


def AppendRunFlag(switch: str):
    global g_RunFlag
    global g_GenerateCmd

    VerbosePrint("Parsing switch: " + switch)

    match switch.lower():
        case _:
            ParseSwitches(switch)


def ConstructProjectPath():
    global g_ProjectName
    global g_ProjectPath
    
    g_ProjectPath = "./Projects/" + g_ProjectName
    VerbosePrint("g_ProjectPath: " + g_ProjectPath)


def AppendMainToProjectCMake():
    pass

def CreateProjectCMake():
    pass


def CreateProject():
    global g_ProjectName
    global g_ProjectPath

    ConstructProjectPath()
    
    if os.path.exists(g_ProjectPath):
        print("Invalid that project already exists: ", g_ProjectPath)
        exit(1)

    os.makedirs(g_ProjectPath)
    os.makedirs(g_ProjectPath + "/Assets")
    os.makedirs(g_ProjectPath + "/Source")
    print("Created new project in path: ", g_ProjectPath)


if __name__ == "__main__":
    for i in range(1, len(sys.argv)):
        AppendRunFlag(sys.argv[i])

    VerbosePrint("g_RunFlag: " + hex(g_RunFlag))

    if (g_RunFlag & F_PROJECT_CREATE):
        CreateProject()
        CreateProjectCMake()
        AppendMainToProjectCMake()
    
    exit(0)
