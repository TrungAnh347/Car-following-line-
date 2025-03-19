#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo_in, servo_out;
int last_time_in = 0, last_time_out = 0;
int car_in = 0, car_out = 0;
const int hong_ngoai[6] = {2, 3, 4, 5, 6, 7};

void setup() {
    lcd.init();
    lcd.backlight();
    lcd.begin(16, 2);
    for (int i = 0; i < 6; i++) {
        pinMode(hong_ngoai[i], INPUT);
    }
    servo_in.attach(8);
    servo_out.attach(9);
    servo_in.write(0);
    servo_out.write(0);
    lcd.setCursor(0, 0);
}

void display_lcd() {
    lcd.print(car_in ? "In: T" : "In: F");
    delay(10);
    lcd.print(car_out ? " Out: T" : " Out: F");
    delay(10);
}

void loop() {
    int read_hn[6];
    for (int i = 0; i < 6; i++) {
        read_hn[i] = digitalRead(hong_ngoai[i]);
    }
    int empty = read_hn[2] + read_hn[3] + read_hn[4] + read_hn[5];
    lcd.setCursor(0, 1);
    lcd.print("Empty: ");
    lcd.print(empty);
    int current_time_in = millis();
    int current_time_out = millis();
    
    if (empty && read_hn[0] == 0) {
        last_time_in = current_time_in;
        servo_in.write(90);
        car_in = 1;
    }
    if (read_hn[1] == 0) {
        last_time_out = current_time_out;
        servo_out.write(90);
        car_out = 1;
    }
    if (current_time_in - last_time_in >= 3000) {
        servo_in.write(0);
        car_in = 0;
    }
    if (current_time_out - last_time_out >= 3000) {
        servo_out.write(0);
        car_out = 0;
    }
    lcd.setCursor(0, 0);
    display_lcd();
}