#!/bin/bash

DOT_FILES=$(ls *.dot 2>/dev/null)

for dot_file in $DOT_FILES; do
    png_file="${dot_file%.dot}.png"
    dot -Tpng "$dot_file" -o "$png_file"
done

echo "Дерево из файла создано успешно!"