#!/usr/bin/bash

CMD_NOT_FOUND_RESULT=127
dependencies=("cmake" "python --help")
root_file="./Scripts/Win32/CopyDLLPerProject.ps1"
flag=False

printHelp()
{
    echo "Help:"
}

for dep in "${dependencies[@]}"
do

    2>/dev/null 1>/dev/null $dep 

    result=$?
    if [[ "$result" == "$CMD_NOT_FOUND_RESULT" ]]; then

        echo "error: Missing dependency '$dep' ($result)"
        flag=True

    fi

done

if [[ ! -f "$root_file" ]]; then

    echo "error: Please call the script from the root directory of the engine."
    flag=True

fi

if [[ "$#" -lt 1 ]]; then

    echo "error: Invalid amount of arguments."
    echo ""
    printHelp
    flag=True
    
fi

if [[ "$flag" == True ]]; then

    exit 1
    
fi

python ./Scripts/BuildManagment.py "$@"
