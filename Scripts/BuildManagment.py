import sys


def ValidateSystem():
    pass


def CreateBuildResources():
    ValidateSystem()


if __name__ == "__main__":
    print("Build managment script called")
    if (len(sys.argv) <= 1):
        exit()
