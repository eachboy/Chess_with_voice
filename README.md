# Шахматы с голосовым управлением

## Требуемые компоненты
- Компьютер/ПК с Windows 10/11
- Arduino Mega 2560
- Arduino Uno

## Требуемые библиотеки
```
pip install PyAudio
pip install SpeechRecognition
pip install numpy
pip install pyserial
pip install --upgrade google-cloud-speech
```

## Требуемые языки программирования
- Python 3.9+
- Arduino C


## Требуемые программы
- Visual Studio Code
- Arduino IDE 1.8.xx или 2.x.x


## Требуемые размеры шахматной доски
1. Размеры доски - 400 х 400 мм.
2. Размеры клетки - 43 х 43 мм.
3. Диаметр основания фигуры - 31 мм.

## Описание работы программы
- Вводится голосовая команда игроком (пример: A2 A4)
- На вход компьютер принимает голосовую команду
- Если команда корректна, то программа обрабатывает её. В противном случае отправляется команда на Arduino Mega 2560, которая отправляет сообщение о неверной команде на дисплей. Игроку предоставляется возможность ввода повторного ввода команды
- После обработки команда отправляется на Arduino Mega 2560, которая в свою очередь управляет моторами
- Если фигура съедена, то кран переставляет фигуру на отведённую позицию, а затем делает ход на указанную координату.
- После хода игрока, программа на компьютер обрабатывает ход искусственного интеллекта
- Обработанный ход искуственного интеллекта отправляется на Arduino Mega 2560, после чего Arduino передаёт сигнал для движения моторами.
- Если фигура съедена, то кран переставляет фигуру на отведённую позицию, а затем делает ход на указанную координату.
- Если игрок или исскусственный интеллект делают ход, завершающий игру, то Arduino запускает команду, которая возвращает все фигуры на исходные позиции.
#

## Для начала
### 1. Компьютер
- Используя флешку, установите на неё код программы из папки "Python"

- Установите на компьютер Arduino IDE(при установке потребуется установить драйвера, которые программа предложит сама установить их).
- Установите все библиотеки, написанные выше.
- Переходите к работе с Arduino.

### 2. Arduino Mega 2560
- Откройте программу Arduino IDE.
- Откройте в программе код из папки "Arduino/Mega".
- Загрузите файл "maga.ino"

### 3. Arduino Uno
- Вставьте в программу код из папки "Arduino/Uno".
#
## Объединение плат
#### После выше указанных действий, выполните подключение платы Arduino к компьютеру через USB-кабель. После проверьте подключение плат между собой:
1. Подключите Arduino Mega и Arduino Uno к компьютеру
2. Зайдите в дисптчер устройств
3. Выберете пункт
4. После того, как Вы нашли порт, измените его в файле "main.py", который находится в папке "PC":
    - Где есть комментарий "Uno" - вместо надписи "COM" введите имя порта, к которому подключена Arduino Uno (например, "COM12") 
    - В остальных местах - имя порта, к которому подключена Arduino Mega

#

## Работа с модулями 
1. Чтобы программа распознавала речь, нужно использовать библиотеки PyAudio и SpeechRecognition. SpeechRecognition работает через библиотеку PyAudio, поэтому их обе необходимо скачать. 
2. Сперва нужно определить ID микрофона, который используется для ввода голоса. Чтобы определить ID, нужно написать следующий код
```
import speech_recognition as s_r
for i in range(len(s_r.Microphone.list_microphone_names())):
    print(i, '-', s_r.Microphone.list_microphone_names()[i])
```
3. Чтобы добавить библиотеку Speech_Recognition в программу, напишите следующую строку
```
import speech_recognition as s_r
```
4. ID, который мы получили из программы выше, нужно ввести после 'device_index=' без пробелов в функцию, которая отвечает за распознавание речи
```
def speech():
    r = s_r.Recognizer()
    with sr.Microphone(device_index=1) as sourse:
```
5. Чтобы начать распознавание речи, вводим строки. Переменная audio активирует и запоминает фразу, которую говорит пользователь. Время на ввод фразы в данном случае составляет 5 секунд. Если фраза так и не была сказана, то через 7 секунд ввод с голоса прекратится. Переменная qwery содержит в себе строку с фразой, которую вы сказали раньше.
Модуль Google-Cloud-Speech используется для наиболее точного распознавания речи. Данный модуль требует обязательного подключения к сети Wi-Fi
```
audio=r.listen(sourse, phrase_time_limit=5, timeout=7)
qwery=r.recognize_google(audio, language='ru-RU')
```
6. Так как вся речь содержится в функции, то нужно из не возвращать нашу строку с фразой
```
return qwery
```
7. Наша получаенная строка состоит из русских букв, однако программа обрабатывает команды с латинскими буквами. Для этого в коде есть функция trans(), которая фильтрует полученную строку в латинские буквы и арабские цифры с помощью словаря. Функция возвращает переменную new_mess, содержащую в себе новую команду, которая будет обработана программой и отправлена на Arduino Mega 2560
```
slovar = {'А':'A', 'A':'A', 'B':'B', 'C':'C', 'С':'C', 'Д':'D', 'D':'D', 'Е':'E', 'E':'E', 'G':'G', 'Ф':'F','F':'F', 'Г':'G', 'Х':'H','H':'H', '1':'1', '2':'2', '3':'3', '4':'4', ' ':' ', '5':'5', '6':'6', '7':'7', '8':'8', 'ОДИН':'1', 'ДВА':'2', 'ТРИ':'3', 'ЧЕТЫРЕ':'4', 'ПЯТЬ':'5', 'ШЕСТЬ':'6', 'СЕМЬ':'7', 'ВОСЕМЬ':'8'}
```