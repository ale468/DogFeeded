#include <EEPROM.h>
#include <LiquidCrystal.h>
/*#include <RTClib.h>*/

LiquidCrystal lcd(12, 13, 2, 3, 4, 5);
//RTC_DS1307 rtc;
bool controle = 0;
bool proErro = 0;
bool prosseguir = 0;
byte horas = 0;
byte minutos = 0;
byte horaRTC = 0;
byte minutoRTC = 0;
//int rtcHoraMin[] = {now.hour(),now.minute()};
byte numeroRefeicoes = 0;
long tempo;
int tempoPoteCheio;
byte status = 0;

void setup() {
  pinMode(6, INPUT); //Esquerda
  pinMode(7, INPUT); //Direita
  pinMode(8, INPUT); //OK
  pinMode(15, OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  status = EEPROM.read(999);
}

void loop() {
  if(status==0){
    configurarHora();
  }
  else if(status==1){
    bemVindo();
  }else if (status==2){
    refeicoes(); 
  }else{
    ModoEspera();
  }
}

void bemVindo(){
     lcd.clear();
     lcd.setCursor(6, 0);
     lcd.print("Seja");
     lcd.setCursor(3, 1);
     lcd.print("bem-vindo!");
     delay(2000);
     lcd.clear();
     delay(800);
     
    while(digitalRead(8)==0){
            lcd.setCursor(1,0);
      lcd.print("Aperte OK para ");
      lcd.setCursor(0,1);
      lcd.print("despejar a racao");     
    }   
    while(digitalRead(8)==1){}
        delay(30);
      lcd.clear();
    tone(15,1000);
    delay(150);
    noTone(15); 
    tempo = millis();
    lcd.setCursor(0,0);
    lcd.print("Aperte OK quando");
    lcd.setCursor(2,1);
    lcd.print("quiser parar");
    while(digitalRead(8)==0){}
    while(digitalRead(8)==1){}
    delay(30);
        tone(15,1000);
        delay(150);
        noTone(15);
      Serial.println(tempo);
      Serial.println(millis());
    tempoPoteCheio = millis() - tempo;
    Serial.println(tempoPoteCheio);
        lcd.clear();
        delay(5000);
        status = 2;
  }

void refeicoes() {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Quantas");
    lcd.setCursor(0, 1);
    lcd.print("refeicoes?");

    while (digitalRead(8) != 1) {
      lcd.setCursor(11, 1);
      numeroRefeicoes = analogRead(14) / 204 + 1;
      lcd.print(numeroRefeicoes);
      delay(50);
      lcd.setCursor(11,1);
      lcd.print(" ");
      delay(50);
    }
    while(digitalRead(8)== 1){
      tone(15,1000);
      delay(100);
      noTone(15);
      while(digitalRead(8)== 1){}
    }
    lcd.clear();
    byte i = 0;
  byte j = 0;
    while (i != numeroRefeicoes) {
      if(i>0){
    EEPROM.write(j, horas);
    j++;
    EEPROM.write(j, minutos);
    j++;
    }
      lcd.clear();
      controle = 0;
      if (proErro == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Horario");
        lcd.print(i + 1);
        lcd.print(": __:__");
        proErro = 1;
      }
      while (digitalRead(8) == 0) {

        if (digitalRead(6) == HIGH) {
          controle = 0;
        }
        if (digitalRead(7) == HIGH) {
          controle = 1;
        }
        if (controle == 0) {
          delay(20);
          lcd.setCursor(10, 0);
          lcd.print("  ");
          delay(20);

          lcd.setCursor(10, 0);
          horas = analogRead(14) / 44.47;
          lcd.print(horas);

          if (horas < 10) {
            lcd.setCursor(11, 0);
            lcd.print('_');
          }
          if (horas == 0) {
            lcd.setCursor(11, 0);
            lcd.print(0);
          }

        }
        if (controle == 1) {
          delay(20);
          lcd.setCursor(13, 0);
          lcd.print("  ");
          delay(20);
          lcd.setCursor(13, 0);
          minutos = analogRead(14) / 17.33;
          lcd.print(minutos);

          if (minutos < 10) {
            lcd.setCursor(14, 0);
            lcd.print('_');
          }
          if (minutos == 0) {
            lcd.setCursor(14, 0);
            lcd.print(0);
          }
      prosseguir = 1;
        }
    if(prosseguir == 0){
      while(digitalRead(8) == 1){
               delay(30);
               tone(15,1000);
             delay(60);
             noTone(15);
               delay(60);
               tone(15,1000);
             delay(60);
             noTone(15);
               delay(150);
         lcd.setCursor(0,1);
         lcd.print("Hora Incompleta!");
               lcd.setCursor(0,1);
               delay(500);
               lcd.print("                ");
      }
    }
      }  while(digitalRead(8)== 1){
          tone(15,1000);
          delay(100);
          noTone(15);
          while(digitalRead(8)== 1){}
         }
      delay(1000);

      i++;
      proErro = 0;
    prosseguir = 0;

    }

    lcd.clear();
  status = 3;
  }

void configurarHora(){
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Insira a hora");
    lcd.setCursor(6, 1);
    lcd.print("__:__");
       
  while (digitalRead(8) == 0) {

        if (digitalRead(6) == HIGH) {
          controle = 0;
        }
        if (digitalRead(7) == HIGH) {
          controle = 1;
        }
        if (controle == 0) {
          delay(20);
          lcd.setCursor(6, 1);
          lcd.print("  ");
          delay(20);

          lcd.setCursor(6, 1);
          horaRTC = analogRead(14) / 44.47;
          lcd.print(horaRTC);

          if (horaRTC < 10) {
            lcd.setCursor(7, 1);
            lcd.print('_');
          }
          if (horaRTC == 0) {
            lcd.setCursor(7, 1);
            lcd.print(0);
          }

        }
        if (controle == 1) {
          delay(20);
          lcd.setCursor(9, 1);
          lcd.print("  ");
          delay(20);
          lcd.setCursor(9, 1);
          minutoRTC = analogRead(14) / 17.33;
          lcd.print(minutoRTC);

          if (minutoRTC < 10) {
            lcd.setCursor(10, 1);
            lcd.print('9');
          }
          if (minutoRTC == 0) {
            lcd.setCursor(10, 1);
            lcd.print(0);
          }
      prosseguir = 1;
        }
    if(prosseguir == 0){
      while(digitalRead(8) == 1){
               delay(30);
               tone(15,1000);
             delay(60);
             noTone(15);
               delay(60);
               tone(15,1000);
             delay(60);
             noTone(15);
               delay(150);
         lcd.setCursor(2,1);
         lcd.print("!!");
               lcd.setCursor(12,1);
               lcd.print("!!");
               lcd.setCursor(2,1);
               delay(700);
               lcd.print("  ");
               lcd.setCursor(12,1);
         lcd.print("  ");
      }
    }prosseguir = 0;
      }controle=0;
     status = 1;
  // rtc.adjust(DateTime(2014, 1, 21, 3, 30, 0));
}
void ModoEspera(){
  long i = 0;
  lcd.clear();
  while(i!=-1){
    
  lcd.setCursor(0,0);
  //lcd.print(now.hour());
  lcd.print(":");
  //lcd.print(now.minute());
    lcd.setCursor(0,1);
  lcd.print("racao as ");
    
    while(digitalRead(6)==1 && digitalRead(7)==1){
      i = millis();
      if(i-millis()==5000){
        configurarHora();
        EEPROM.write(999,3);
      }
    }
    while(digitalRead(7)==1 && digitalRead(8)==1){
      i = millis();
      if(i-millis()==5000){
        bemVindo();
        EEPROM.write(999,3);
      }
    }
    while(digitalRead(6)==1 && digitalRead(8)==1){
      i = millis();
      if(i-millis()==5000){
        refeicoes();
        status = 3;
      }
    }
  }
}
void AbrirMotor(){
  
}
void FecharMotor(){
  
}
void assobiar(){
  for(int i = 500;i<800;i=i+4)
  {
    tone(15,i*4);
    delay(3);
  }
  noTone(15);
  delay(60);
  for(int i = 600;i<1100;i=i+7)
  {
    tone(15,i*3);
    delay(3);
  }
  noTone(15);
  int i = 1;
}