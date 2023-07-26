#include "mbed.h"

BufferedSerial pc(USBTX, USBRX);  // Verwende BufferedSerial statt Serial

DigitalOut SerialDataInA(PA_9);
DigitalOut ShiftClock(PA_8);
DigitalOut LatchClock(PB_5);

DigitalOut led1(PA_5);
DigitalOut led2(PA_6);
DigitalOut led3(PA_7);
DigitalOut led4(PB_6);

Timeout timeout;
InterruptIn button1(PA_1);
InterruptIn button2(PA_4);
InterruptIn button3(PB_0);

unsigned int counter = 0;  
unsigned int seconds = 0;
unsigned int hours = 0;
unsigned int minutes = 0;
unsigned int day = 0;
unsigned int month = 0;
unsigned int year = 0;
unsigned int unixstamp = 0;
bool dot2 = true;
bool dot4 = true; 
int blinkrate = 100;
int digit1 = 0;
int digit2 = 0;
int digit3 = 0;
int digit4 = 0;
enum statusTyp{hhmm, mmss, ddmm, yyyy};
statusTyp status;

void isrTimeout();
void isrButton1();
void isrButton2();
void isrButton3();


void shift(){
    ShiftClock = 0;
    ShiftClock = 1;
    ShiftClock = 0;
}
void clear(){
    SerialDataInA = 0;
    for(int i=0; i < 20; i++){ // 16
        ShiftClock = 1;
        ShiftClock = 0;
    }
}
void display(int index, int array[]){
    clear();
    for(int i=0; i < 8; i++){
        SerialDataInA = array[i]; 
        shift();
    }

    for(int i=0; i < 4; i++){
        shift();
    }
    if(index == 1){
         SerialDataInA = 0;
         shift();
         SerialDataInA = 0;
         shift();
         SerialDataInA = 0;
         shift();
         SerialDataInA = 1;
         shift();
    }
    if(index == 2){
         SerialDataInA = 0;
         shift();
         SerialDataInA = 0;
         shift();
         SerialDataInA = 1;
         shift();
         SerialDataInA = 0;
         shift();
    }
    if(index == 3){
         SerialDataInA = 0;
         shift();
         SerialDataInA = 1;
         shift();
         SerialDataInA = 0;
         shift();
         SerialDataInA = 0;
         shift();
    }
    if(index == 4){
         SerialDataInA = 1;
         shift();
         SerialDataInA = 0;
         shift();
         SerialDataInA = 0;
         shift();
         SerialDataInA = 0;
         shift();
    }
    LatchClock = 1;
    LatchClock = 0;
}
int sd_number_0[] = {1, 1, 0, 0, 0, 0, 0, 0};
int sd_number_1[] = {1, 1, 1, 1, 1, 0, 0, 1};
int sd_number_2[] = {1, 0, 1, 0, 0, 1, 0, 0};
int sd_number_3[] = {1, 0, 1, 1, 0, 0, 0, 0};
int sd_number_4[] = {1, 0, 0, 1, 1, 0, 0, 1};
int sd_number_5[] = {1, 0, 0, 1, 0, 0, 1, 0};
int sd_number_6[] = {1, 0, 0, 0, 0, 0, 1, 0};
int sd_number_7[] = {1, 1, 1, 1, 1, 0, 0, 0};
int sd_number_8[] = {1, 0, 0, 0, 0, 0, 0, 0};
int sd_number_9[] = {1, 0, 0, 1, 0, 0, 0, 0};

int sd_number_0_dot[] = {0, 1, 0, 0, 0, 0, 0, 0};
int sd_number_1_dot[] = {0, 1, 1, 1, 1, 0, 0, 1};
int sd_number_2_dot[] = {0, 0, 1, 0, 0, 1, 0, 0};
int sd_number_3_dot[] = {0, 0, 1, 1, 0, 0, 0, 0};
int sd_number_4_dot[] = {0, 0, 0, 1, 1, 0, 0, 1};
int sd_number_5_dot[] = {0, 0, 0, 1, 0, 0, 1, 0};
int sd_number_6_dot[] = {0, 0, 0, 0, 0, 0, 1, 0};
int sd_number_7_dot[] = {0, 1, 1, 1, 1, 0, 0, 0};
int sd_number_8_dot[] = {0, 0, 0, 0, 0, 0, 0, 0};
int sd_number_9_dot[] = {0, 0, 0, 1, 0, 0, 0, 0};


void display4(int seg1, int seg2, int seg3, int seg4, int timer){
    for (int i = 0; i<timer/4; i++) {
        if (seg1 == 1) {
            display(1, sd_number_1); 
        } else if (seg1 == 2) {
            display(1, sd_number_2); 
        } else if (seg1 == 3) {
            display(1, sd_number_3); 
        } else if (seg1 == 4) {
            display(1, sd_number_4); 
        } else if (seg1 == 5) {
            display(1, sd_number_5); 
        } else if (seg1 == 6) {
            display(1, sd_number_6); 
        } else if (seg1 == 7) {
            display(1, sd_number_7); 
        } else if (seg1 == 8) {
            display(1, sd_number_8); 
        } else if (seg1 == 9) {
            display(1, sd_number_9); 
        } else if (seg1 == 0) {
            display(1, sd_number_0); 
        }
        timeout.attach(&isrTimeout, 1ms);

        if (dot2 == true) {
        
        if (seg2 == 1) {
            display(2, sd_number_1_dot); 
        } else if (seg2 == 2) {
            display(2, sd_number_2_dot); 
        } else if (seg2 == 3) {
            display(2, sd_number_3_dot); 
        } else if (seg2 == 4) {
            display(2, sd_number_4_dot); 
        } else if (seg2 == 5) {
            display(2, sd_number_5_dot); 
        } else if (seg2 == 6) {
            display(2, sd_number_6_dot); 
        } else if (seg2 == 7) {
            display(2, sd_number_7_dot); 
        } else if (seg2 == 8) {
            display(2, sd_number_8_dot); 
        } else if (seg2 == 9) {
            display(2, sd_number_9_dot); 
        } else if (seg2 == 0) {
            display(2, sd_number_0_dot); 
        }
        } else if (dot2 == false){

        if (seg2 == 1) {
            display(2, sd_number_1); 
        } else if (seg2 == 2) {
            display(2, sd_number_2); 
        } else if (seg2 == 3) {
            display(2, sd_number_3); 
        } else if (seg2 == 4) {
            display(2, sd_number_4); 
        } else if (seg2 == 5) {
            display(2, sd_number_5); 
        } else if (seg2 == 6) {
            display(2, sd_number_6); 
        } else if (seg2 == 7) {
            display(2, sd_number_7); 
        } else if (seg2 == 8) {
            display(2, sd_number_8); 
        } else if (seg2 == 9) {
            display(2, sd_number_9); 
        } else if (seg2 == 0) {
            display(2, sd_number_0); 
        }
        }

        timeout.attach(&isrTimeout, 1ms);

        if (seg3 == 1) {
            display(3, sd_number_1); 
        } else if (seg3 == 2) {
            display(3, sd_number_2); 
        } else if (seg3 == 3) {
            display(3, sd_number_3); 
        } else if (seg3 == 4) {
            display(3, sd_number_4); 
        } else if (seg3 == 5) {
            display(3, sd_number_5); 
        } else if (seg3 == 6) {
            display(3, sd_number_6); 
        } else if (seg3 == 7) {
            display(3, sd_number_7); 
        } else if (seg3 == 8) {
            display(3, sd_number_8); 
        } else if (seg3 == 9) {
            display(3, sd_number_9); 
        } else if (seg3 == 0) {
            display(3, sd_number_0); 
        }
        timeout.attach(&isrTimeout, 1ms);

        if (seg4 == 1) {
            display(4, sd_number_1); 
        } else if (seg4 == 2) {
            display(4, sd_number_2); 
        } else if (seg4 == 3) {
            display(4, sd_number_3); 
        } else if (seg4 == 4) {
            display(4, sd_number_4); 
        } else if (seg4 == 5) {
            display(4, sd_number_5); 
        } else if (seg4 == 6) {
            display(4, sd_number_6); 
        } else if (seg4 == 7) {
            display(4, sd_number_7); 
        } else if (seg4 == 8) {
            display(4, sd_number_8); 
        } else if (seg4 == 9) {
            display(4, sd_number_9); 
        } else if (seg4 == 0) {
            display(4, sd_number_0); 
        }
        timeout.attach(&isrTimeout, 1ms);
    }
}

void isrTimeout(){

}

void isrButton1(){
    if (status == hhmm) {
        status = mmss;
    } else if(status == mmss){
        status = ddmm;
    } else if(status == ddmm){
        status = yyyy;
    } else if (status == yyyy) {
        status = hhmm;
    }
    timeout.attach(&isrTimeout, 50ms);
}


int main() {
    status = hhmm;
    blinkrate = 100;
    pc.set_baud(9600);  // Setze die Baudrate auf 9600
    pc.set_format(8, SerialBase::None, 1);  // Setze Datenbits auf 8, Parität auf None und Stopbits auf 1
    button1.fall( &isrButton1 );

    while (true) {
        
        if (seconds % 10 == 0) {
            pc.write("?", 1);  // Anfrage nach aktueller Uhrzeit senden

            if (pc.readable()) {
                char timeStr[11];
                pc.read(timeStr, sizeof(timeStr));  // Den empfangenen String einlesen
                timeStr[sizeof(timeStr) - 1] = '\0';  // Null-Terminierung hinzufügen
                const char *str = timeStr; 
                unixstamp = atoi(str);  
                set_time(unixstamp);
            }
        }
        time_t rawtime = time(NULL);
        struct tm *timeinfo;
        timeinfo = localtime(&rawtime);
        // Extrahiere Tag, Monat und Jahr aus der Struktur
        day = timeinfo->tm_mday;
        month = timeinfo->tm_mon + 1; // Monate werden von 0 bis 11 gezählt, daher +1
        year = timeinfo->tm_year + 1900; // Jahre seit 1900

        // Extrahiere Stunden, Minuten und Sekunden aus der Struktur
        hours = timeinfo->tm_hour + 2;
        minutes = timeinfo->tm_min; 
        seconds = timeinfo->tm_sec;

        //Sekundenanzeige
        if (seconds < 15) {
            led1 = 1;
            led2 = 1;
            led3 = 1;
            led4 = 1;
        } else if (seconds < 30) {
            led1 = 0;
        } else if (seconds < 45) {
            led2 = 0;
        } else if (seconds < 58) {
            led3 = 0;
        } else if (seconds == 59) {
            led4 = 0;
        }        


        // Anzeigen der Stunden und Minuten

        if (status == hhmm){
            digit1 = hours / 10;
            digit2 = hours % 10;
            digit3 = minutes / 10;
            digit4 = minutes % 10;
            dot2 = true;
        } else if(status == mmss){
            digit1 = minutes / 10;
            digit2 = minutes % 10;
            digit3 = seconds / 10;
            digit4 = seconds % 10; 
        } else if(status == ddmm){
            digit1 = day / 10;
            digit2 = day % 10;
            digit3 = month / 10;
            digit4 = month % 10;
        } else if (status == yyyy) {
            digit1 = year / 1000;
            digit2 = year / 100 % 10;
            digit3 = year / 10 % 10;
            digit4 = year % 10;
            dot2 = false;
        }

        

        display4(digit1, digit2, digit3, digit4, blinkrate);

        if (counter < 10) {
            counter++;
        } else if (counter >= 9) {
            counter = 0;
        }
    }
}
