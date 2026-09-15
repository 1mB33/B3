import sys


def PrintHelpMessage():
    pass


def ValidateSystem():
    pass


def CreateBuildResources():
    ValidateSystem()


if __name__ == "__main__":
    if (len(sys.argv) <= 1):
        PrintHelpMessage()
        exit()
