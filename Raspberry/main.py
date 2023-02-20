import board, pieces, ai
import serial
import speech_recognition as sr


# Использовать, когда команда не отправляется на Arduino
# hod_p = '11'
# if __name__ == '__main__':
#     post = serial.Serial('/dev/ttyUSB0', 9600)
#     post.flush()
#     c = bytes(hod_p, 'utf-8')
#     post.write(c)

figures = [
    # const, current, eat
    ["11", "11", "99"],
    ["21", "21", "99"],
    ["31", "31", "99"],
    ["41", "41", "99"],
    ["51", "51", "99"],
    ["61", "61", "99"],
    ["71", "71", "99"],
    ["81", "81", "99"],
    ["12", "12", "99"],
    ["22", "22", "99"],
    ["32", "32", "99"],
    ["42", "42", "99"],
    ["52", "52", "99"],
    ["62", "62", "99"],
    ["72", "72", "99"],
    ["82", "82", "99"],

    ["17", "17", "99"],
    ["27", "27", "99"],
    ["37", "37", "99"],
    ["47", "47", "99"],
    ["57", "57", "99"],
    ["67", "67", "99"],
    ["77", "77", "99"],
    ["87", "87", "99"],
    ["18", "18", "99"],
    ["28", "28", "99"],
    ["38", "38", "99"],
    ["48", "48", "99"],
    ["58", "58", "99"],
    ["68", "68", "99"],
    ["78", "78", "99"],
    ["88", "88", "99"]
]


def speech():
    r = sr.Recognizer()
    with sr.Microphone(device_index=1) as sourse:
        try:
            print('Говорите...')
            audio = r.listen(sourse, phrase_time_limit=5, timeout=7)
            qwery = (r.recognize_google(audio, language='ru-RU')).title()
        except:
            return 'Ошибка ввода. Повторите попытку'
        else:
            print(qwery)
            word = trans(qwery)
            print(word)
            if len(word)==4: 
                return word
            else:
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
        return 'Ошибка ввода'



def get_user_move():
    print("Пример хода: A2 A4")
    move_str = input("Ваш ход: ")
    if move_str == '12345':
        move_str = speech()

    try:
        xfrom = letter_to_xpos(move_str[0:1])
        yfrom = 8 - int(move_str[1:2]) # The board is drawn "upside down", so flip the y coordinate.
        xto = letter_to_xpos(move_str[2:3])
        yto = 8 - int(move_str[3:4]) # The board is drawn "upside down", so flip the y coordinate.
        return ai.Move(xfrom, yfrom, xto, yto, False)
    except ValueError:
        print("Неверный формат. Пример: A2 A4")
        return get_user_move()

# Returns a valid move based on the users input.
def get_valid_user_move(board):
    while True:
        move = get_user_move()
        valid = False
        possible_moves = board.get_possible_moves(pieces.Piece.WHITE)
        # No possible moves
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
            print("Неверный ход.")
    return move

# Converts a letter (A-H) to the x position on the chess board.
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

def to_arduino(hod):
    print("hod",hod)
    next_x = hod[1]
    next_y = hod[3]
    next_pos = next_x + next_y
    for i in range(len(figures)):
        if next_pos == figures[i][1]:
            hod_p = 'e' + next_x + str(int(figures[i][2])//10) + next_y + str(int(figures[i][2])%10)
            if __name__ == '__main__':
                post = serial.Serial('/dev/ttyUSB0', 9600)
                post.flush()
                c = bytes(hod_p, 'utf-8')
                post.write(c)

            if __name__ == '__main__':
                ArAnswer = serial.Serial('/dev/ttyUSB0', 9600)
                ArAnswer.flush()
                check = True
                while check:
                    line = ArAnswer.readline().decode('utf-8').rstrip()
                    if line == 'next_turn':
                        check = False

    if __name__ == '__main__':
        post = serial.Serial('/dev/ttyUSB0', 9600)
        post.flush()
        c = bytes(hod, 'utf-8')
        post.write(c)

    print(board.to_string())  
    
    if __name__ == '__main__':
        ArAnswer = serial.Serial('/dev/ttyUSB0', 9600)
        ArAnswer.flush()
        check = True
        while check:
            line = ArAnswer.readline().decode('utf-8').rstrip()
            if line == 'next_turn' or line == 'Time_up':
                check = False
                # if line == 'Time_up':
                #     for i in range(len(figures)):
                #         if figures[i][2] != figures[i][1]:
                #             hod_p = str(figures[i][2]//10) + str(figures[i][1]//10) + str(figures[i][2]%10) + str(figures[i][1]%10) 
                #             if __name__ == '__main__':
                #                 post = serial.Serial('/dev/ttyUSB0', 9600)
                #                 post.flush()
                #                 c = bytes(hod_p, 'utf-8')
                #                 post.write(c)


board = board.Board.new()
print(board.to_string())

while True:
    move = get_valid_user_move(board)
    if (move == 0):
        if (board.is_check(pieces.Piece.WHITE)):
            print("Checkmate. Black Wins.")
            break
        else:
            print("Stalemate.")
            break

    board.perform_move(move)
    
    hod_1 = move.to_string() #Ход человека
    print(hod_1)
    hod = "".join(map(str, hod_1))
    to_arduino(hod)

    ai_move = ai.AI.get_ai_move(board, [])
    if (ai_move == 0):
        if (board.is_check(pieces.Piece.BLACK)):
            print("Checkmate. White wins.")
            break
        else:
            print("Stalemate.")
            break
    
    hod_1 = ai_move.to_string() #Ход ИИ
    hod = "".join(map(str, hod_1))
    to_arduino(hod)

    board.perform_move(ai_move)
    
    print(board.to_string())
