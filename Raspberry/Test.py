import speech_recognition as sr
for i in range(len(sr.Microphone.list_microphone_names())):
    print(i, sr.Microphone.list_microphone_names()[i])