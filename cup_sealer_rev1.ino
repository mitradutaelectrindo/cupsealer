#define vra A5          // lama untuk press plastik
#define vrb A4          // timing buat ganti wadah
#define vrc A3
#define swlimit_top   1
#define swlimit_bot   0
#define swlimit_slide 2
#define sen_plastik 7
#define sw_auto     5
#define sw_stop     4
#define sw_stat     3
#define mrak_ccw    12
#define mrak_cw     11
#define mpres_bot   10
#define mpres_top   9
#define mroll       8

boolean jalan=false;
uint8_t st_limtop,st_limbot,st_slide,st_sen,st_stop,st_stat,st_auto,proses;
uint16_t wtun_a,wtun_b,wtun_c;

void setup() {
  // put your setup code here, to run once:
  analogReference(DEFAULT);
  pinMode(swlimit_top,INPUT_PULLUP);
  pinMode(swlimit_bot,INPUT_PULLUP);
  pinMode(swlimit_slide,INPUT);
  pinMode(sen_plastik,INPUT_PULLUP);
  pinMode(sw_stop,INPUT_PULLUP);
  pinMode(sw_stat,INPUT_PULLUP);
  pinMode(sw_auto,INPUT_PULLUP);

  pinMode(mrak_ccw,OUTPUT);
  pinMode(mrak_cw,OUTPUT);
  pinMode(mpres_bot,OUTPUT);
  pinMode(mpres_top,OUTPUT);
  pinMode(mroll,OUTPUT);

  digitalWrite(mrak_ccw, LOW);
  digitalWrite(mrak_cw, LOW);
  digitalWrite(mpres_bot, LOW);
  digitalWrite(mpres_top, LOW);
  digitalWrite(mroll, LOW);

  delay(1000);

  roller(1);
  delay(1000);
  st_sen=lim_roll();
  while(st_sen!=1) { roller(1); st_sen=lim_roll(); }
  roller(0);

  mot_rak(1);
  delay(400);
  st_slide=lim_slide();
  while(st_slide!=1){mot_rak(1); st_slide=lim_slide();}
  mot_rak(0);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  
  st_auto=otom();
  delay(1000);
  //===============================================================================
  while(st_auto==1){
      
      st_stat=stat();
      if(st_stat){jalan=true;}
      
      while(jalan){
        wtun_a=tunda_a();
        wtun_b=tunda_b();
        wtun_c=tunda_c();

        st_limbot=lim_bot();
        while(st_limbot!=1){mot_pres(0,1); st_limbot=lim_bot();}
        mot_pres(0,0);
        delay(wtun_a);
        st_limtop=lim_top();
        while(st_limtop!=1){mot_pres(1,0);st_limtop=lim_top();}
        mot_pres(0,0);

        roller(1);
        mot_rak(1); 
        delay(2000); proses=1;
        while(proses){
          st_sen=lim_roll();
          st_slide=lim_slide();

          if(st_sen){roller(0);}else {roller(1);}
          if(st_slide){mot_rak(0);}else{mot_rak(1);}
          if(st_sen && st_slide){proses=0;}
        }
        delay(wtun_b);
        
        st_stop=stope();
        if(st_stop){jalan=false;}
      }
      
      st_auto=otom();
  }
  //===============================================================================
  while(st_auto==0){
      wtun_a=tunda_a();
      wtun_b=tunda_b();
      st_stat=stat();
      
      if(st_stat){
        st_limbot=lim_bot();
        while(st_limbot!=1){mot_pres(0,1); st_limbot=lim_bot();}
        mot_pres(0,0);
        delay(wtun_a);
        st_limtop=lim_top();
        while(st_limtop!=1){mot_pres(1,0);st_limtop=lim_top();}
        mot_pres(0,0);
        
        roller(1);
        mot_rak(1); 
        delay(2000); proses=1;
        while(proses){
          st_sen=lim_roll();
          st_slide=lim_slide();

          if(st_sen){roller(0);}else {roller(1);}
          if(st_slide){mot_rak(0);}else{mot_rak(1);}
          if(st_sen && st_slide){proses=0;}
        }
        delay(wtun_b);
      }
      st_auto=otom();
    }// end of manual

    
}

uint8_t otom(){ 
  uint8_t state=0;
  uint8_t val=digitalRead(sw_auto); 
  if(val==0){state=1;}else{state=0;}
  return state;
  }

uint8_t stope(){ 
  uint8_t state=0;
  uint8_t val=digitalRead(sw_stop); 
  if(val==0){state=1;}else{state=0;}
  return state;
  }

uint8_t stat(){ 
  uint8_t state=0;
  uint8_t val=digitalRead(sw_stat); 
  if(val==0){state=1;}else{state=0;}
  return state;
  }

uint8_t lim_top(){ 
  uint8_t state=0;
  uint8_t val=digitalRead(swlimit_top); 
  if(val==0){state=1;}else{state=0;}
  return state;
  }

uint8_t lim_bot(){ 
  uint8_t state=0;
  uint8_t val=digitalRead(swlimit_bot); 
  if(val==0){state=1;}else{state=0;}
  return state;
  }
uint8_t lim_slide(){ 
  uint8_t state=0;
  uint8_t val=digitalRead(swlimit_slide); 
  if(val==1){state=1;}else{state=0;}
  return state;
  }

uint8_t lim_roll(){ 
  uint8_t state=0;
  uint8_t val=digitalRead(sen_plastik); 
  if(val==0){state=1;}else{state=0;}
  return state;
  }

uint16_t tunda_a(){
  uint16_t wtunda;
  uint16_t i=analogRead(A5);
  wtunda=i*5;
  return wtunda;
  }

uint16_t tunda_b(){
  uint16_t wtunda;
  uint16_t i=analogRead(A4);
  wtunda=i*5;
  return wtunda;
  }

uint16_t tunda_c(){
  uint16_t wtunda;
  uint16_t i=analogRead(A3);
  wtunda=i;
  return wtunda;
  }
  
void mot_rak(int i){
  if(i==1){digitalWrite(mrak_cw,HIGH);}
  else {digitalWrite(mrak_cw,LOW);}
  }

void mot_pres(int top, int bot){
  if(top==1){digitalWrite(mpres_top,HIGH);}else{digitalWrite(mpres_top,LOW);}
  if(bot==1){digitalWrite(mpres_bot,HIGH);}else{digitalWrite(mpres_bot,LOW);}
  }

void roller(int i){
  if(i==1){digitalWrite(mroll,HIGH);}else {digitalWrite(mroll,LOW);}
  }
