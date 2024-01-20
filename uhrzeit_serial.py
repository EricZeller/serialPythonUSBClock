import serial
import time

ser = serial.Serial('COM4', 9600)  # COM9 durch den entsprechenden COM-Port ersetzen
last_time = ""

try:
    while True:
        # Warte auf Anfrage vom Mbed-Gerät
        if ser.read() == b'?':
            current_time = str(int(time.time()))  # Aktuelle Uhrzeit als String erhalten, z.B. '17:48:30'
            ser.write(current_time.encode())  # Die Uhrzeit als Zeichen an das Mbed-Gerät senden
            if last_time != current_time:
                last_time = current_time
                print("\n" + current_time, end = "", flush=True)
            print(".", end="", flush=True)
            time.sleep(1)

    pass
except KeyboardInterrupt:
    pass

ser.close()  # Die serielle Verbindung schließen
