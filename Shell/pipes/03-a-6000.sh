git clone https://github.com/avelin/fmi-os.git
cp fmi-os/exercises/data/emp.data ~

# Брой редове
awk 'END {print NR}' emp.data

# Трети ред
awk 'NR == 3 {print $0}' emp.data

# Последното поле на всеки ред
awk '{print $NF}' emp.data

# Последното поле на последния ред
awk 'END {print $NF}' emp.data

# Всеки ред, който има повече от 4 полета
awk 'NF > 4 {print $0}' emp.data

# Всеки ред, чието последно поле е по-голямо от 4
awk '$NF > 4 {print $0}' emp.data

# Общия брой полета във всички редове
awk '{print NF}' emp.data

# Броя редове, в които се среща низът Beth
awk '/Beth/ {i+=1} END {print i}' emp.data

# Най-голямото трето поле и редът, който го съдържа
awk '$NF > i {i = $NF; r = NR} END {printf i" "r"\n"}' emp.data

# Всеки ред, който има поне едно поле
awk 'NF > 1 {print $0}' emp.data

# Всеки ред, който има повече от 17 знака
awk '/^.{18,}$/ {print $0}' emp.data

# Броя на полетата във всеки ред и самият ред
awk '{printf NF" "$0"\n"}' emp.data

# Първите две полета от всеки ред, с разменени места
awk '{print $2"\t"$1}' emp.data

# Всеки ред така, че първите две полета да са с разменени места
awk '{temp = $1; $1 = $2; $2 = temp; print $0}' emp.data

# Всеки ред така, че на мястото на първото поле да има номер на реда
awk '{$1 = NR; print $0}' emp.data

# Всеки ред без второто поле
awk '{
for(i = 1; i <= NF; i++) {
    if(i != 2) {
        printf "%s ", $i; 
    }
}
printf "\n";
}' emp.data

# За всеки ред, сумата от второ и трето поле
awk '{print $2 + $3}' emp.data

# Сумата на второ и трето поле от всеки ред
awk 'BEGIN {sum=0} {sum+=$2 + $3} END{print sum}' emp.data