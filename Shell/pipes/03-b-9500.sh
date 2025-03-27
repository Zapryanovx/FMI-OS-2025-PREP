cat ssa-input.txt | awk '
$1 == "Array" { 
    array = $2 
} 

$1 == "physicaldrive" { 
    drive = $2 
} 

$2 == "Temperature" && $1 == "Current" {
    ct = $4 
} 

$2 == "Temperature" && $1 == "Maximum" {
    mt = $4; 
    printf "%s-%s %s %s\n", array, drive, ct, mt 
}'
