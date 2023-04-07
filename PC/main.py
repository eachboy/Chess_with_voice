import board, pieces, ai
import serial
import speech_recognition as sr

uno = serial.Serial('COM3', 9600)
mega = serial.Serial('COM5', 9600)

def color(n):
    if n == 1:
        b = bytes(n, 'utf-8')
        uno.write(b) 
    elif n == 2:
        b = bytes(n, 'utf-8')
        uno.write(b)  
    elif n == 3:
        b = bytes(n, 'utf-8')
        uno.write(b)  
    elif n == 4: 
        # yellow
        b = bytes(n, 'utf-8')
        uno.write(b)  
    elif n == 5:
        # purple
        b = bytes(n, 'utf-8')
        uno.write(b) 
    elif n == 6:
        # blink red
        b = bytes(n, 'utf-8')
        uno.write(b) 

def speech():
    r = sr.Recognizer()
    with sr.Microphone(device_index=1) as sourse:
        try:
            print('Говорите...')
            color(2)
            audio = r.listen(sourse, phrase_time_limit=5, timeout=7)
            color(4)
            qwery = (r.recognize_google(audio, language='ru-RU')).title()
        except:
            color(6)
            return 'Ошибка ввода. Повторите попытку'
        else:
            color(4)
            print(qwery)
            word = trans(qwery)
            print(word)
            if len(word)==4: 
                color(2)
                return word
            else:
                color(6)
                'Ошибка ввода'

def trans(word):
    try:
        slovar = {'А':'A', 'A':'A', 'B':'B', 'C':'C', 'С':'C', 'Д':'D', 'D':'D', 'Е':'E', 'E':'E', 'G':'G', 'Ф':'F','F':'F', 'Г':'G', 'Х':'H','H':'H', '1':'1', '2':'2', '3':'3', '4':'4', ' ':' ', '5':'5', '6':'6', '7':'7', '8':'8', 'ОДИН':'1', 'ДВА':'2', 'ТРИ':'3', 'ЧЕТЫРЕ':'4', 'ПЯТЬ':'5', 'ШЕСТЬ':'6', 'СЕМЬ':'7', 'ВОСЕМЬ':'8'}
        mess = word.upper().replace(' ', '')
        new_mess = ''
        if mess[0] in 'АО' or mess[0] in 'AO':
            if 'АОДИН' in mess:
                mess = mess.replace('AОДИН', 'A1')
            if 'АДВА' in mess:
                mess = mess.replace('АДВА', 'A2')
            if 'АТРИ' in mess:
                mess = mess.replace('АТРИ', 'A3')
            if 'АЧЕТЫРЕ' in mess:
                mess = mess.replace('АЧЕТЫРЕ', 'A4')
            if 'АПЯТЬ' in mess:
                mess = mess.replace('АПЯТЬ', 'A5')
            if 'ОПЯТЬ' in mess:
                mess = mess.replace('ОПЯТЬ', 'A5')
            if 'АШЕСТЬ' in mess:
                mess = mess.replace('АШЕСТЬ', 'A6')
            if 'АСЕМЬ' in mess:
                mess = mess.replace('АСЕМЬ', 'A7')
            if 'АВОСЕМЬ' in mess:
                mess = mess.replace('АВОСЕМЬ', 'A8')
        for i in range(len(mess)):
            new_mess += slovar.get(mess[i])
        return new_mess
    except:
        color(6)
        return 'Ошибка ввода'

def get_user_move():
    print("Пример хода: A2 A4")
    move_str = input("Ваш ход: ")
    if move_str == '12345':
        move_str = speech()

    try:
        xfrom = letter_to_xpos(move_str[0:1])
        yfrom = 8 - int(move_str[1:2])
        xto = letter_to_xpos(move_str[2:3])
        yto = 8 - int(move_str[3:4])
        return ai.Move(xfrom, yfrom, xto, yto, False)
    except ValueError:
        color(6)
        print("Неверный формат. Пример: A2 A4")
        return get_user_move()

def get_valid_user_move(board):
    while True:
        move = get_user_move()
        valid = False
        possible_moves = board.get_possible_moves(pieces.Piece.WHITE)
        if (not possible_moves):
            return 0

        for possible_move in possible_moves:
            if (move.equals(possible_move)):
                move.castling_move = possible_move.castling_move
                valid = True
                break

        if (valid):
            break
        else:
            color(6)
            print("Неверный ход.")
    return move

def letter_to_xpos(letter):
    letter = letter.upper()
    if letter == 'A':
        return 0
    if letter == 'B':
        return 1
    if letter == 'C':
        return 2
    if letter == 'D':
        return 3
    if letter == 'E':
        return 4
    if letter == 'F':
        return 5
    if letter == 'G':
        return 6
    if letter == 'H':
        return 7
    raise ValueError("Неверная буква.")

def stop():
    check = True
    while check:
        line = mega.readline().decode('utf-8').rstrip()
        if line == 'continue':
            check = False

def to_arduino(hod):
    print("hod",hod)
    next_x = hod[1]
    next_y = hod[3]
    next_pos = next_x + next_y
    for i in range(len(figures)):
        if next_pos == figures[i][1]:
            hod = 'e' + next_x + str(int(figures[i][2])//10) + next_y + str(int(figures[i][2])%10) + hod

    b = bytes(hod, 'utf-8')
    mega.write(b)

    print(board.to_string())  
    
    check = True
    while check:
        line = mega.readline().decode('utf-8').rstrip()
        # if line == 'stop':
        #     color(1)
        #     stop()
        if line == 'next_turn' or line == 'Time_up':
            check = False
            # if line == 'Time_up':
            #     color(1)
            #     for i in range(len(figures)):
            #         if figures[i][2] != figures[i][1]:
            #             hod_p = str(figures[i][2]//10) + str(figures[i][1]//10) + str(figures[i][2]%10) + str(figures[i][1]%10) 
            #             b = bytes(hod_p, 'utf-8')
            #             mega.write(b)
            #             check = True
            #             while check:
            #                 line = mega.readline().decode('utf-8').rstrip()
            #                 if line == 'next_turn':
            #                       check = False


board = board.Board.new()
print(board.to_string())

figures = [
    # const, current, eat
    ["11", "11", "110"],
    ["21", "21", "310"],
    ["31", "31", "29"],
    ["41", "41", "49"],
    ["51", "51", "69"],
    ["61", "61", "89"],
    ["71", "71", "510"],
    ["81", "81", "710"],
    ["12", "12", "91"],
    ["22", "22", "93"],
    ["32", "32", "95"],
    ["42", "42", "97"],
    ["52", "52", "211"],
    ["62", "62", "311"],
    ["72", "72", "511"],
    ["82", "82", "711"],

    ["17", "17", "92"],
    ["27", "27", "94"],
    ["37", "37", "96"],
    ["47", "47", "98"],
    ["57", "57", "111"],
    ["67", "67", "311"],
    ["77", "77", "511"],
    ["87", "87", "711"],
    ["18", "18", "210"],
    ["28", "28", "410"],
    ["38", "38", "19"],
    ["48", "48", "49"],
    ["58", "58", "69"],
    ["68", "68", "79"],
    ["78", "78", "610"],
    ["88", "88", "810"]
]

# Использовать, когда команда не отправляется на Arduino
# hod_p = '11'
# b = bytes(hod_p, 'utf-8')
# mega.write(b)

# stop()

while True:
    move = get_valid_user_move(board)
    if (move == 0):
        if (board.is_check(pieces.Piece.WHITE)):
            print("Шах и мат! Чёрные выиграли!")
            break
        else:
            print("Ничья")
            break

    board.perform_move(move)
    
    hod_1 = move.to_string() #Ход человека
    print(hod_1)
    hod = "".join(map(str, hod_1))
    to_arduino(hod)

    ai_move = ai.AI.get_ai_move(board, [])
    if (ai_move == 0):
        if (board.is_check(pieces.Piece.BLACK)):
            print("Шах и мат! Белые выиграли!")
            break
        else:
            print("Ничья")
            break
    
    hod_1 = ai_move.to_string() #Ход ИИ
    hod = "".join(map(str, hod_1))
    to_arduino(hod)

    board.perform_move(ai_move)
    print(board.to_string())