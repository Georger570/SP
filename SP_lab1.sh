#!/bin/bash
echo "Программа подсчёта файлов пользователя"
echo "Программа позволяет узнать количество обычных файлов в заданном каталоге, владельцем которых является задданный пользователь."
echo "Разработчик: Кока Никита"

echo "Текущее имя каталога: $( pwd | tr / "\n" | tail -1 )"
while : 
do
	count=0
	while :
	do
		read -p "Введите имя пользователя: " username
		user=$( grep /$username:/bin/bash /etc/passwd | tr : "\n" | head -1 ) 
		if [ "$user" != "" ]
		then
			break	
		elif [[ "$count" = 2 ]]
		then
			echo "Не правильный ввод 3 раза подряд, выход из программы..." 1>&2
			exit 1
		else 
			count=$(( $count + 1 ))
			echo "Пользователь с данным именем не существует!" 1>&2
		fi
	done
	read -p  "Введите имя каталога: " cat_name
	echo "Подсчёт количества файлов..."
	quantity=$( find $cat_name -type f -user $user | wc -l  )
	echo "Количество файлов: $quantity"
	count=0
	while :
	do
		read -p "Вы хотите продолжить работу? (y/n) " confirmation
		if [ "$confirmation" = "n" ] || [ "$confirmation" = "N" ] ;
		then
			exit 0
			echo "Выход из программы..."
		elif [ "$confirmation" = "y" ] || [ "$confirmation" = "Y" ] ;
		then
			break
		else
			echo "Неправильный ввод, пожалуйста, попробуйте ещё раз" 1>&2
			count=$(( $count + 1 ))
			if [[ "$count" = 3 ]]
			then	
				echo "Не правильный ввод 3 раза подряд, выход из программы..." 1>&2
				exit 1
			fi
		fi
	done
done

