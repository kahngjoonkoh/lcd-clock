#include <LiquidCrystal.h>
#include <OneButton.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int selectTime = 7;
const int changeTime = 8;
OneButton selectB(selectTime, true);
OneButton changeB(changeTime, true);

int hours = 0;
int minutes = 0;
int seconds = 0;
int milliseconds = 0;
String clock_time[4] = {"00", "00", "00", "000"};
unsigned long time;

bool done = true;
bool add_minute = false;
bool morning = true;
bool changing = false;
int order = 0;
int added_seconds = 0;
bool hChanged = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  screen_display();
}

void loop() {
  selectB.tick();
  changeB.tick();
  time = millis();
  milliseconds = time%1000;
  int accumalated_seconds = (time - milliseconds)/1000;
  accumalated_seconds += added_seconds;
  seconds = accumalated_seconds%60;

  if(seconds == 0 and done == false){
    add_minute = true;
  }

  if(seconds != 0){
    done = false;
  }


  if(add_minute == true){
    add_minute = false;
    done = true;
    minutes+=1;
  }

  if(minutes == 60){
    minutes-=60;
    hours+=1;
  }

  if(hours == 12 && hChanged == false){
    if(morning == true){
      morning = false;
      hChanged = true;
    }
    else{
      if(morning == false && hChanged == false){
        morning = true;
        hChanged = true;
      }
    }
  }

  if(hours == 13){
    hours -= 12;
  }
  if(hours != 12){
    hChanged = false;
  }

  clock_time[0] = String(hours);
  clock_time[1] = String(minutes);
  clock_time[2] = String(seconds);
  clock_time[3] = String(milliseconds)+"    ";
  
  if(hours <=9){
    clock_time[0] = "0" + String(hours);
  }
  
  if(minutes <=9){
    clock_time[1] = "0" + String(minutes);
  }
  
  if(seconds <=9){
    clock_time[2] = "0" + String(seconds);
  }
  
  //Serial.println(time);
  change_time();
  screen_display();
}

void screen_display(){
    
  lcd.setCursor(0,0);
  lcd.print("Time ");

  lcd.setCursor(13, 0);

  if(morning == true){
    lcd.print(" AM");
  }
  else{
    lcd.print(" PM");
  }
  
  lcd.setCursor(0, 1);
  
  if(changing and (order==1 or order==2)){
    lcd.print("  ");
  }
  else{
    lcd.print(clock_time[0]);
  }
  
  lcd.setCursor(2, 1);
  
  if(time%1000 > 500){
    lcd.print(":");
  }
  else{
    lcd.print(" ");
  }
  
  lcd.setCursor(3, 1);
  
  if(changing and (order==0 or order==2)){
    lcd.print("  ");
  }
  else{
    lcd.print(clock_time[1]);
  }
  
  lcd.setCursor(5, 1);

  if(time%1000 > 500){
    lcd.print(":");
  }
  else{
    lcd.print(" ");
  }
  
  lcd.setCursor(6, 1);
  
  if(changing and (order==0 or order==1)){
    lcd.print("  ");
  }
  else{
    lcd.print(clock_time[2]);
  }
  
  lcd.setCursor(8, 1);
  lcd.print(" ");
  
  lcd.setCursor(9, 1);
  if(changing){
    lcd.print("   ");
  }
  else{
    lcd.print(clock_time[3]);
  }

  if(not changing){
    lcd.setCursor(5, 0);
    lcd.print("        ");
  }
}

void change_time(){
  selectB.attachDoubleClick(change_boolean);
  
  if(changing and order==0){
    lcd.setCursor(5, 0);
    lcd.print("change:h ");
    changeB.attachClick(addH);
    selectB.attachDoubleClick(change_off);
    selectB.attachClick(skip_order);
  }
  
  if(changing and order==1){
    lcd.setCursor(5, 0);
    lcd.print("change:m ");
    changeB.attachClick(addM);
    selectB.attachDoubleClick(change_off);
    selectB.attachClick(skip_order);
  }

  if(changing and order==2){
    Serial.print(order);
    lcd.setCursor(5, 0);
    lcd.print("change:s ");
    changeB.attachClick(addS);
    selectB.attachDoubleClick(change_off);
    selectB.attachClick(skip_order);
  }
}

void change_boolean(){
  changing = true;
}

void addH(){
  hours+=1;
}

void addM(){
  minutes+=1;
}

void addS(){
  added_seconds += 1;
}

void skip_order(){
  if(order>=2){
    change_off();
  }
  else{
  order+=1;
  }
}

void change_off(){
  Serial.print("executed");
  changing = false;
  order = 0;
  lcd.setCursor(5, 0);
  lcd.print("        ");
}
