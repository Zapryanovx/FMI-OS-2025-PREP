mkdir dir5
echo -e '1\n2\n3' > ~/dir5/file1
echo -e 's\na\nf\nd' > ~/dir5/file2
echo -e '3\n2\n1\n45\n42\n14\n1\n52' > ~/dir5/file3

# Статистика за броя редове, думи и символи за всеки един файл
wc dir5/* | head -n -1

# Статистика за броя редове и символи за всички файлове
wc -l -c dir5/* | head -n -1

# Общия брой редове на трите файла
wc -l dir5/* | tail -n 1