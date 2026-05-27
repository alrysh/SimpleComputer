#!/bin/bash

echo "Нажимайте клавиши. Для выхода нажмите Ctrl+C."

while true; do
    # -s: не отображать нажатые символы
    # -n 1: читать 1 символ
    # -r: не интерпретировать backslash-escape-последовательности
    read -rn1 key

    # Проверка, если это ESC-последовательность (начинается с ESC)
    if [[ $key == $'\e' ]]; then
        read -rsn2 -t 0.001 rest
        key+=$rest
    fi

    # Показать, какие байты пришли
    echo -n "Коды: "
    echo -n "$key" | hexdump -v -e '/1 "%02X "'
    echo
done
