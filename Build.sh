#!/usr/bin/bash

CMD_NOT_FOUND_RESULT=127
dependencies=("cmake" "python --help" "make")
root_file="./Scripts/Win32/CopyDLLPerProject.ps1"
is_invalid=False

printHelp()
{
    echo "Help manual"
    echo ""
    echo "      command:"
    echo "          $0 [build-switches] --All"
    echo "          $0 [build-switches] --Tests"
    echo "          $0 [build-switches] --Project <project-name>"
    echo ""
    echo ""
    echo "The scripts defaults to debug build type."
}

for dep in "${dependencies[@]}"
do

    2>/dev/null 1>/dev/null $dep 

    result=$?
    if [[ "$result" == "$CMD_NOT_FOUND_RESULT" ]]; then

        echo "error: Missing dependency '$dep' ($result)"
        is_invalid=True

    fi

done

if [[ ! -f "$root_file" ]]; then

    echo "error: Please call the script from the root directory of the engine."
    is_invalid=True

fi

if [[ "$#" -lt 1 ]]; then

    echo "error: Invalid amount of arguments."
    echo ""
    printHelp
    is_invalid=True
    
fi

if [[ "$is_invalid" == True ]]; then

    exit 1
    
fi

python ./Scripts/BuildManagment.py "$@"

result=$?
if [[ "$result" != "0" ]]; then

    printHelp

fi
