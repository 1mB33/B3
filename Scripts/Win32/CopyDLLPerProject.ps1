param (
   [Parameter(Mandatory=$true)][string]$root_path,
   [Parameter(Mandatory=$true)][string]$project_list
)

$dlls = Get-ChildItem -Path $root_path -Filter *.dll -File

# Write-Output $root_path
# Write-Output $project_list
# Write-Output $dlls

$project_list.Split(";") | ForEach {  
    foreach ($dll in $dlls) 
    {
        Copy-Item $dll -Destination $root_path\$_\
    }
}
