#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

//#include "coin1.h" //The 2nd coin file created is included

using namespace simplecpp;
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 0.5
#define BOMB_GAP 6

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG -720 //It can revolve fully //changed from 0 to -720
#define MAX_RELEASE_ANGLE_DEG 720 // changed from (360-RELEASE_ANGLE_STEP_DEG) to 720
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 20 // increased from 0 to 20
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5

class MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;

  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//2nd coin
class Coin1 : public MovingObject {
  double coin1_start_x;
  double coin1_start_y;
  double release_speed;
  double release_angle_deg;
  double coin1_ax;
  double coin1_ay;
  double add_angle=15;


 public:
 Circle coin1_circle;
 Coin1(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin1_ax = argax;
    coin1_ay = argay;
    initCoin();
  }

void initCoin() {
  coin1_start_x = (PLAY_X_START+WINDOW_X+400)/2;
  coin1_start_y = PLAY_Y_HEIGHT;
  coin1_circle.reset(coin1_start_x, coin1_start_y, COIN_SIZE*1.5, COIN_SIZE);
  coin1_circle.setColor(COLOR(93, 240, 245)); //light blue
  coin1_circle.setFill(true);
  addPart(&coin1_circle);

}

void resetCoin(){
    //coin1_circle.hide();
  release_angle_deg+=add_angle;
  if (release_angle_deg>135 || release_angle_deg < 75) release_angle_deg=75;
  double coin1_angle_deg = release_angle_deg;
  double coin1_speed;

        if(coin1_angle_deg==75) {
    coin1_speed = COIN_SPEED*0.5;
    coin1_ax = 20;
    coin1_ay = COIN_G*0.25;
    coin1_start_x = (PLAY_X_START+WINDOW_X-400)/2;
    }

        if(coin1_angle_deg==90) {
    coin1_speed = COIN_SPEED*1.5;
    coin1_ax = 0;
    coin1_ay = COIN_G*1.2;
    coin1_start_x = (PLAY_X_START+WINDOW_X+100)/2;
    }

        if(coin1_angle_deg==105) {
    coin1_speed = COIN_SPEED*2;
    coin1_ax = -20;
    coin1_ay = COIN_G/2;
    coin1_start_x = (PLAY_X_START+WINDOW_X+200)/2;
    }

        if(coin1_angle_deg==120) {
    coin1_speed = COIN_SPEED*2;
    coin1_ax = -15;
    coin1_ay = COIN_G/1.8;
    coin1_start_x = (PLAY_X_START+WINDOW_X+300)/2;
    }

        if(coin1_angle_deg==135) {
    coin1_speed = COIN_SPEED*2;
    coin1_ax = -10;
    coin1_ay = COIN_G/2;
    coin1_start_x = (PLAY_X_START+WINDOW_X+400)/2;
    }

  /*double coin1_speed = COIN_SPEED*2;
  coin1_ax = -10;
  coin1_ay = COIN_G/1.5;*/
  //(Above comment just as a reference to original)
  bool paused = true, rtheta = true;
  reset_all(coin1_start_x, coin1_start_y, coin1_speed, coin1_angle_deg, coin1_ax, coin1_ay, paused, rtheta);

}

}; // End class Coin1


//Class Bomb
class Bomb : public MovingObject {
  double bomb_start_x;
  double bomb_start_y;
  double release_speed;
  double release_angle_deg;
  double bomb_ax;
  double bomb_ay;
  double add_angle=15;


 public:
 Circle bomb_circle;
 Bomb(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    bomb_ax = argax;
    bomb_ay = argay;
    initBomb();
  }

void initBomb() {
  bomb_start_x = (PLAY_X_START+WINDOW_X+400)/2;
  bomb_start_y = WINDOW_Y; //BOMB IS RESET TO THE BOTTOMMOST POINT
  bomb_circle.reset(bomb_start_x, bomb_start_y, COIN_SIZE*2.5);
  bomb_circle.setColor(COLOR(87, 101, 107)); //greyish
  bomb_circle.setFill(true);
  addPart(&bomb_circle);

}

void resetBomb(){
    //bomb_circle.hide();
  release_angle_deg+=add_angle;
  double bomb_angle_deg = release_angle_deg;
  double bomb_speed;

    if (release_angle_deg>150 || release_angle_deg < 75) release_angle_deg=60;

        if(bomb_angle_deg==75) {
    bomb_speed = COIN_SPEED*1.25;
    bomb_ax = 0;
    bomb_ay = COIN_G*1.1;
    bomb_start_x = (PLAY_X_START+WINDOW_X-400)/2;
    }

        if(bomb_angle_deg==90) {
    bomb_speed = COIN_SPEED*2;
    bomb_ax = +5;
    bomb_ay = COIN_G*2;
    bomb_start_x = (PLAY_X_START+WINDOW_X)/2;
    }

        if(bomb_angle_deg==105) {
    bomb_speed = COIN_SPEED*2;
    bomb_ax = -20;
    bomb_ay = COIN_G*1.6;
    bomb_start_x = (PLAY_X_START+WINDOW_X+600)/2;
    }

        if(bomb_angle_deg==120) {
    bomb_speed = COIN_SPEED*1.4;
    bomb_ax = -15;
    bomb_ay = COIN_G*1.5;
    bomb_start_x = (PLAY_X_START+WINDOW_X+500)/2;
    }

        if(bomb_angle_deg==135) {
    bomb_speed = COIN_SPEED*2;
    bomb_ax = -10;
    bomb_ay = COIN_G/2;
    bomb_start_x = (PLAY_X_START+WINDOW_X+600)/2;
    }

    if(bomb_angle_deg==150) {
    bomb_speed = COIN_SPEED*1.6;
    bomb_ax = 35;
    bomb_ay = COIN_G*0.4;
    bomb_start_x = (WINDOW_X-100);
    }

  /*double bomb_speed = COIN_SPEED*2;
  bomb_ax = -10;
  bomb_ay = COIN_G/1.5;*/
  //(Above comment just as a reference to original)
  bool paused = true, rtheta = true;
  reset_all(bomb_start_x, bomb_start_y, bomb_speed, bomb_angle_deg, bomb_ax, bomb_ay, paused, rtheta);

}

}; //end Bomb




//coin.h
#ifndef __COIN_H__
#define __COIN_H__



class Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;
  double add_x=75;

 public:

 // Moving parts
  Circle coin_circle;

 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }

  void initCoin();
  void resetCoin();

}; // End class Coin

#endif

//lasso.h


class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  Coin1 *the_coin1; //2nd coin
  Bomb *the_bomb1; //bomb1
  int num_coins;
  int lives=3;

  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  void check_for_coin1(Coin1 *coin);// 2nd coin
  void check_for_bomb1(Bomb *bomb); // bomb1 //timed mode
  void check_for_bomb1_lives(Bomb *bomb); //bomb 1 //normal mode
  int getNumCoins() { return num_coins;}
  int getLives() { return lives;}
  void check_for_life(Circle *life,Circle *L2,Circle *L3);

}; // End class Lasso

#endif

//coin.h

void Coin::initCoin() {
  coin_start_x = (PLAY_X_START+WINDOW_X)/2;
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR(255,223,0)); //golden yellow
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::resetCoin() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  coin_start_x+=add_x;
  if(coin_start_x>WINDOW_X-100) coin_start_x = PLAY_X_START +200;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

//lasso.cpp

void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR(255,98,0)); //light orange
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR(30, 0 ,255)); //Blue Outer Ring after catching
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  the_coin1 = NULL; //2nd coin
  the_bomb1=NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR(0,0,255)); //blue rope

  lasso_band.setColor(COLOR(0, 255, 98)); //greenish
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {
    num_coins++;
    the_coin->resetCoin();
    the_coin = NULL; // debug the initial fault in code;
  }

  if(the_coin1 != NULL) {
    num_coins+=3; //2nd coin //Increase points by three
    the_coin1->resetCoin();
    the_coin1 = NULL; // debug the initial fault in code;
  }
} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  if(getXPos() < PLAY_X_START || getXPos()>WINDOW_X) { yank(); } // left margin added
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
  }
} // End Lasso::check_for_coin()

void Lasso::check_for_coin1(Coin1 *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin1_x = coinPtr->getXPos();
  double coin1_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin1_x);
  double ydiff = (lasso_y - coin1_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin1 = coinPtr;
    the_coin1->getAttachedTo(this);
  }
} // End Lasso::check_for_coin1()

void Lasso::check_for_bomb1(Bomb *bombPtr) { // for timed mode
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double bomb1_x = bombPtr->getXPos();
  double bomb1_y = bombPtr->getYPos();
  double xdiff = (lasso_x - bomb1_x);
  double ydiff = (lasso_y - bomb1_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  double bomb_radius= LASSO_RADIUS; // radius till which the bomb must not be there
  if(distance <= bomb_radius) {
    the_bomb1 = bombPtr;
    lasso_loop.reset(getXPos(), getYPos(), bomb_radius); //the loop opens up on detecting a bomb
    lasso_loop.setFill(false);
    num_coins-=5; //if the coin is in the vicinity of lasso, the points get deducted by 5
    wait(0.5); //wait after detecting a bomb
    the_bomb1->resetBomb();
    //the_bomb = NULL;
  }
} // End Lasso::check_for_bomb1()

void Lasso::check_for_bomb1_lives(Bomb *bombPtr) { // for normal mode
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double bomb1_x = bombPtr->getXPos();
  double bomb1_y = bombPtr->getYPos();
  double xdiff = (lasso_x - bomb1_x);
  double ydiff = (lasso_y - bomb1_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  double bomb_radius= LASSO_RADIUS; // radius till which the bomb must not be there
  if(distance <= bomb_radius) {
    lasso_loop.reset(getXPos(), getYPos(), bomb_radius);
    lasso_loop.setFill(false);
    the_bomb1 = bombPtr;
    lives-=1; //if the coin is in the vicinity of lasso, the lives get reduced by one
    wait(0.5); //wait after detecting a bomb
    the_bomb1->resetBomb();
    yank();
  }
} // End Lasso::check_for_bomb1()

void Lasso::check_for_life(Circle *lifePtr,Circle *L2,Circle *L3) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double life_x = lifePtr->getX();
  double life_y = lifePtr->getY();
  double xdiff = (lasso_x - life_x);
  double ydiff = (lasso_y - life_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    if(lives==2) {lives+=1; L3->show();}
    if(lives==1) {lives+=1; L2->show();}
    lifePtr->moveTo(WINDOW_X, WINDOW_Y);
    lifePtr->hide();
  }

}



main_program {

    cout<< "Welcome to The LASSO Game !"<<endl<<endl;

    cout<<"Let's go through the rules of the game."<<endl<<endl;
    cout<<"The game consists of a lasso with the help of which you have to catch the coins." <<endl<<endl;

    cout<<"** The yellow coin, when catched, gives you one point." <<endl;
    cout<<"** The blue coin is a bonus coin, which when catched, gives you three bonus points." <<endl;
    cout<<"** !! BUT BEWARE !!  There is also a grey colored bomb which may cost you 5 points or a life, according to the mode." <<endl<<endl;

    cout<< "The game consists of two modes :-"<<endl<<endl;
    cout<< "1. NORMAL Mode: You have 3 lives in the beginning. Bombs cost you one life. You have to avoid bombs and catch as many coins as you can. There is no time limit. ";
    cout<< "You may also collect a life which appears in the form of a red colored ball. "<<endl;
    cout<< "2. TIMED Mode: You have to catch maximum number of coins in a given time limit. Bombs cost you 5 points."<<endl<<endl;

    cout<<"The current high-score is displayed on the top of the screen. Next can be yours!"<<endl<<endl;

    cout<< "Rest instructions will follow with the game!"<<endl<<endl;

    cout<< "Time to start the game!"<<endl;
    cout<<"!! BEST OF LUCK !! "<<endl<<endl;


  string player_name;
  cout << "Enter your name to begin! --> ";
  getline(cin, player_name); //enter player name;
  string name_display = "Player : " + player_name;

  int mode;

  //alternate code // It may be used if button is not put
  /*cout<< "Choose Mode : "<<endl;
  cout<< "Press 1 for Normal Mode"<<endl;
  cout<< "Press 2 for Timed Mode" << endl;

  int mode;
  cin>>mode;*/

  initCanvas("Lasso", WINDOW_X, WINDOW_Y);

    //creating buttons

    Rectangle outer_box((WINDOW_X)/2,WINDOW_Y/2 ,700,500);
    outer_box.setColor(COLOR("blue"));

    Text choose_mode ((WINDOW_X)/2, PLAY_Y_HEIGHT/2 - 100, "C H O O S E   T H E   M O D E   O F   P L A Y");
    choose_mode.setColor(COLOR(0, 255, 98)); //greenish

    Text normal_text ((WINDOW_X)/2,PLAY_Y_HEIGHT/2 -30, "Normal Mode");
    Rectangle normal_box((WINDOW_X)/2,PLAY_Y_HEIGHT/2 -30,100,50);
    normal_text.setColor(COLOR(255,98,0)); //light orange
    normal_box.setColor(COLOR("blue"));

    Text timed_text ((WINDOW_X)/2,PLAY_Y_HEIGHT/2 +30, "Timed Mode");
    Rectangle timed_box((WINDOW_X)/2,PLAY_Y_HEIGHT/2 +30,100,50);
    timed_text.setColor(COLOR(255,98,0)); //light orange
    timed_box.setColor(COLOR("blue"));

    Text lasso_game ((WINDOW_X)/2,PLAY_Y_HEIGHT/2 +150, "L A S S O");
    Circle lasso_b((WINDOW_X)/2,PLAY_Y_HEIGHT/2 +150,50);
    lasso_game.setColor(COLOR(247, 177, 0)); //yellowish
    lasso_b.setColor(COLOR(247, 0, 128)); //pinkish violet

    while(true){

    long long int touch,touch_x,touch_y;
    touch=getClick();
    touch_x=touch/65536,touch_y=touch%65536;

        if (touch_x>=((WINDOW_X)/2 -50) && touch_x<=((WINDOW_X)/2 +50) && touch_y>= (PLAY_Y_HEIGHT/2 -30-25) && touch_y<= (PLAY_Y_HEIGHT/2 -30+25)) {

            mode=1;
            normal_text.hide();
            normal_box.hide();
            timed_text.hide();
            timed_box.hide();
            outer_box.hide();
            choose_mode.hide();
            lasso_game.hide();
            lasso_b.hide();
            break;

        }

        else if (touch_x>=((WINDOW_X)/2 -50) && touch_x<=((WINDOW_X)/2 +50) && touch_y>= (PLAY_Y_HEIGHT/2 +30-25) && touch_y<= (PLAY_Y_HEIGHT/2 +30+25)) {

            mode=2;
            normal_text.hide();
            normal_box.hide();
            timed_text.hide();
            timed_box.hide();
            outer_box.hide();
            choose_mode.hide();
            lasso_game.hide();
            lasso_b.hide();
            break;

        }
        else continue;

    }
    //end buttons

    //beginning of game modes
    //intentionally separated both the loops completely to control the parameters individually
  if (mode==1) {

  string high_score;
  string highscore_player;

        ifstream HS;
        HS.open("high_score.txt"); // HS - High Score
            HS >> high_score;
        HS.close();

        ifstream HSP;
        HSP.open ("highscore_player.txt"); // HSP - High Score Player
            getline(HSP, highscore_player);
        HSP.close();

    string high_score_display = "High Score -> " + highscore_player + " : " + high_score;


  {
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;


  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR(247, 0, 128)); //pinkish violet
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR(247, 0, 128)); //pinkish violet

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);

  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

  //char Lives[256];
  //sprintf(Lives, "Lives : %d", lasso.getLives());
  Text Lives_display(WINDOW_X-130, PLAY_Y_HEIGHT+50, "Lives : ");
  Circle L1(WINDOW_X-130 + textWidth("Lives : ")/2 +10, PLAY_Y_HEIGHT+50, 10);
  Circle L2(WINDOW_X-130 + textWidth("Lives : ")/2 +35, PLAY_Y_HEIGHT+50, 10);
  Circle L3(WINDOW_X-130 + textWidth("Lives : ")/2 + 60, PLAY_Y_HEIGHT+50, 10);
  L1.setColor(COLOR("red")); L1.setFill(true);
  L2.setColor(COLOR("red")); L2.setFill(true);
  L3.setColor(COLOR("red")); L3.setFill(true);

  Circle life(WINDOW_X, WINDOW_Y, 10);
  life.setColor(COLOR("red")); life.setFill(true);
  life.hide();
  life.moveTo(WINDOW_X-100, 100);

  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);

  //2nd coin
  double coin1_ax = -10;
  double coin1_ay = COIN_G/1.5;
  Coin1 coin1(coin_speed*2, 135, coin1_ax, coin1_ay, paused, rtheta); //coin at 135deg initially;
  coin1.coin1_circle.hide(); //the coin1 is hidden by default

  double bomb1_ax = 0;
  double bomb1_ay = COIN_G/1.5;
  Bomb bomb1(coin_speed*0, 120, bomb1_ax, bomb1_ay, paused, rtheta); //initial speed of bomb = 0
  bomb1.bomb_circle.hide(); //the bomb1 is hidden by default



  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;
  double last_coin_jump_end_1 = 0; //for coin1
  double last_bomb_jump_end_1 = 0; //for bomb1

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  Text display_name(WINDOW_X -100, PLAY_Y_HEIGHT +20, name_display) ;
  Text HighScore (WINDOW_X -150, 20, high_score_display);
  HighScore.setColor (COLOR(255,98,0)); //light orange

  Text start_game((PLAY_X_START +WINDOW_X)/2, WINDOW_Y/2,"Press S to start the game");
  Text Controls((PLAY_X_START +WINDOW_X)/2, WINDOW_Y/2 +20,"Press C to see the controls");
  Rectangle box1((PLAY_X_START +WINDOW_X)/2, WINDOW_Y/2+10,200,60);
  box1.setColor(COLOR(255,98,0)); //light orange
  Text Control (PLAY_X_START+100, PLAY_Y_HEIGHT+80, "To see controls, press C");

for(;;){
    //creating a new event for starting the game
    XEvent start;

    char C = charFromEvent(start);

    if(C=='s') {

    start_game.hide(); //hide the initial text
    Controls.hide();
    box1.hide();

    break;}

    else if (C=='c') {
    //Text text00(250,30, "The game is paused");
        Text text0(250,50, "CONTROLS:");
        Text text1(250,70, "T = Throw");
        Text text2(250,90, "Y = Yank");
        Text text3(250,110, "L = Loop");
        Text text4(250,130, "[ = Decrease Angle");
        Text text5(250,150, "] = Increase Angle");
        Text text6(250,170, "'=' = Increase Speed");
        Text text7(250,190, " '-' = Decrease Speed");
        Text text8(250,210, " Q = Quit Game");

        Rectangle box(250,130, 200,200);
        box.setColor(COLOR(255,98,0)); //light orange
        wait(15);continue;}
    else continue;}


  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }



    XEvent e;

    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);



      switch(c) {

      case 't':
	lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	lasso.check_for_coin1(&coin1);
	lasso.check_for_life(&life,&L2,&L3);
	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'c':
    {

    Text text0(250,50, "CONTROLS:");
    Text text1(250,70, "T = Throw");
    Text text2(250,90, "Y = Yank");
    Text text3(250,110, "L = Loop");
    Text text4(250,130, "[ = Decrease Angle");
    Text text5(250,150, "] = Increase Angle");
    Text text6(250,170, "'=' = Increase Speed");
    Text text7(250,190, " '-' = Decrease Speed");
    Text text8(250,210, " Q = Quit Game");
    Rectangle box(250,120, 200,200);

    //Timer for resuming the game
    for(int i=5; i>0; i--){
    char resume[256];
    sprintf(resume,"Resuming in : %d ",i);
    Text Resume(250,30, resume);
    Resume.setColor(COLOR("red"));
    wait(1);}

    continue;}
      case 'q':
      {
        Text game_over((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2 -10, "YOU EXITED FROM THE GAME");
        game_over.setColor(COLOR("red"));

        char score[256];
        sprintf(score, "Your Score: %d", lasso.getNumCoins());
        Text score_exit((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2 +10, score);

        Rectangle game_over_box((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2, textWidth("YOU EXITED FROM THE GAME")+10, 2*textHeight()+10 );

            stringstream ss;
        ss << lasso.getNumCoins() ;
        string str = ss.str(); // convert int to string
        //cout<< str;
        if (str>= high_score) {

            high_score= str; //==lasso.getNumCoins();
            highscore_player= player_name;
        }

        ofstream HSP_o;
    HSP_o.open ("highscore_player.txt", ios::in | ios::out | ios::trunc); // HSP - High Score Player

        HSP_o << highscore_player;
        HSP_o.close();



        ofstream HS_o;
    HS_o.open ("high_score.txt", ios::in | ios::out | ios::trunc); // HS - High Score


        HS_o << high_score;
        HS_o.close();

        wait(2);
        exit(0);
      }//end of case 'q'
      default:
	break;
      }
    }

    lasso.nextStep(stepTime);

    //start coin------------------------------------
    coin.nextStep(stepTime);

    if((coin.getYPos() == PLAY_Y_HEIGHT)) coin.coin_circle.hide();

    if((coin.getYPos() < PLAY_Y_HEIGHT)) coin.coin_circle.show();


    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin();
      last_coin_jump_end = currTime;
    }
    //end coin----------------------------------------

    //start coin1--------------------------------------
    coin1.nextStep(stepTime);


        if((coin1.getYPos() == PLAY_Y_HEIGHT)) coin1.coin1_circle.hide();

        if((coin1.getYPos() < PLAY_Y_HEIGHT)) coin1.coin1_circle.show();

        if(coin1.isPaused()) {
            if((currTime-last_coin_jump_end_1) >= 5*COIN_GAP) {
            coin1.unpause();
            }
        }

        if((coin1.getYPos() > PLAY_Y_HEIGHT) || (coin1.getYPos() < 0)) {
            coin1.resetCoin();
            coin1.coin1_circle.hide();
            last_coin_jump_end_1 = currTime;
        }

        if((coin1.getXPos() < PLAY_X_START) || (coin1.getXPos() > WINDOW_X )) {
            coin1.resetCoin();
            coin1.coin1_circle.hide();
            last_coin_jump_end_1 = currTime;
        }
        //end coin1-------------------------------------

    //start bomb1----------------------------------------------

    bomb1.nextStep(stepTime);

    lasso.check_for_bomb1_lives(&bomb1);

        //display the lives (red dots);
        if(lasso.getLives()==2) {L3.hide();}
        if(lasso.getLives()==1) {L2.hide();}
        if(lasso.getLives()==0) {L1.hide();}

    if((bomb1.getYPos() >= PLAY_Y_HEIGHT)) bomb1.bomb_circle.hide();

        if((bomb1.getYPos() < PLAY_Y_HEIGHT)) bomb1.bomb_circle.show();

        if((bomb1.getYPos() > WINDOW_Y) || (bomb1.getYPos() < 0)) {
            bomb1.resetBomb();
            bomb1.bomb_circle.hide();
            last_bomb_jump_end_1 = currTime;
        }

        if((bomb1.getXPos() < PLAY_X_START) || (bomb1.getXPos() > WINDOW_X )) {
            bomb1.resetBomb();
            bomb1.bomb_circle.hide();
            last_bomb_jump_end_1 = currTime;
        }

        if(bomb1.isPaused()) {
            if((currTime-last_bomb_jump_end_1) >= BOMB_GAP) {
            bomb1.unpause();
            }
        }

        //end bomb1-------------------------------------------------

    // showing a life-----------------------------------------------


    if ((fabs(currTime-40)<0.8*stepTime || fabs(currTime-60)<0.8*stepTime) && lasso.getLives()<3) { //To show a life earlier
        life.moveTo(200,100);
        life.show();
    }

    if ((fabs(currTime-44)<0.8*stepTime || fabs(currTime-64)<0.8*stepTime)) {
        life.hide();
        life.moveTo(WINDOW_X, WINDOW_Y);
    }

    if(fmod(currTime,20)<0.8*stepTime && lasso.getLives()<3) {
        double moveTo_x=fmod(10*currTime,800);
        double moveTo_y=fmod(currTime,100);

        if(fabs(moveTo_x)<10) {moveTo_x=WINDOW_X/2;}
        if(fabs(moveTo_y)<10) {moveTo_y=50;}

        life.moveTo(moveTo_x,moveTo_y);
        life.show();
    }

    if(fmod(currTime+16,20)<0.8*stepTime && lasso.getLives()<3) { //currTime+16 = 20k ie currTime=20(k-1)+4 //after 4*1.85( 1 currTime=1.85 sec) seconds the life disappears

        life.hide();
        life.moveTo(WINDOW_X, WINDOW_Y);

    }


    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    currTime += stepTime;
    wait(stepTime);


    if (lasso.getLives()==0) {
        Text game_over((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2-10, "THE GAME IS OVER");
        game_over.setColor(COLOR("red"));

        char score[256];
        sprintf(score, "Your Score: %d", lasso.getNumCoins());
        Text score_exit((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2 +10, score);

        Rectangle game_over_box((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2, textWidth("THE GAME IS OVER")+10, 2*textHeight()+10);

        //record high score
        stringstream ss;
        ss << lasso.getNumCoins() ;
        string str = ss.str();
        //cout<< str;
        if (str>= high_score) {

            high_score= str; //lasso.getNumCoins();
            highscore_player= player_name;
        }

            ofstream HSP_o;
        HSP_o.open ("highscore_player.txt", ios::in | ios::out | ios::trunc); // HSP - High Score Player


        HSP_o << highscore_player;
        HSP_o.close();

            ofstream HS_o;
        HS_o.open ("high_score.txt", ios::in | ios::out | ios::trunc); // HS - High Score

        HS_o << high_score;
        HS_o.close();


    wait(2);
    exit(0);
    }

  } // End for(;;)

  wait(3);
  } //End of initCanvas

} //end of normal mode

 else if (mode==2) {

 string high_score;
  string highscore_player;

        ifstream HS;
        HS.open("high_score_timed.txt"); // HS - High Score
        HS >> high_score;
        HS.close();

        ifstream HSP;
        HSP.open ("highscore_player_timed.txt"); // HSP - High Score Player
    getline(HSP, highscore_player); // to input the second name also from the file else the second name was not displaying

        HSP.close();

    string high_score_display = "High Score -> " + highscore_player + " : " + high_score;


  {

  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;
  float time=90;


  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR(0, 255, 98)); //greenish
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR(0, 255, 98)); //greenish

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);

  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

  char Time[256];
  sprintf(Time,"Time Remaining: %0.0f", time);
  Text time_display(WINDOW_X-100, PLAY_Y_HEIGHT+50, Time);
  time_display.setColor(COLOR("red"));

  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);

  //2nd coin
  double coin1_ax = -10;
  double coin1_ay = COIN_G/1.5;
  Coin1 coin1(coin_speed*2, 135, coin1_ax, coin1_ay, paused, rtheta); //coin at 135deg initially;
  coin1.coin1_circle.hide(); //the coin1 is hidden by default

  double bomb1_ax = 0;
  double bomb1_ay = COIN_G/1.5;
  Bomb bomb1(coin_speed*0, 75, bomb1_ax, bomb1_ay, paused, rtheta); //initial speed of bomb = 0
  bomb1.bomb_circle.hide(); //the bomb1 is hidden by default



  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;
  double last_coin_jump_end_1 = 0; //for coin1
  double last_bomb_jump_end_1 = 0; //for bomb1

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  Text display_name(WINDOW_X -100, PLAY_Y_HEIGHT +20, name_display) ;
  Text HighScore (WINDOW_X -150, 20, high_score_display);
  HighScore.setColor (COLOR("red"));

  Text start_game((PLAY_X_START +WINDOW_X)/2, WINDOW_Y/2,"Press S to start the game");
  Text Controls((PLAY_X_START +WINDOW_X)/2, WINDOW_Y/2 +20,"Press C to see the controls");
  Rectangle box1((PLAY_X_START +WINDOW_X)/2, WINDOW_Y/2+10,200,60);
  Text Control (PLAY_X_START+100, PLAY_Y_HEIGHT+80, "To see controls, press C");
  box1.setColor(COLOR("red"));

for(;;){

    //creating a new event for starting the game
    XEvent start;

    char C = charFromEvent(start);

    if(C=='s') {

    start_game.hide(); //hide the initial text
    Controls.hide();
    box1.hide();

    break;
    }

    else if (C=='c') {
    //Text text00(250,30, "The game is paused");
        Text text0(250,50, "CONTROLS:");
        Text text1(250,70, "T = Throw");
        Text text2(250,90, "Y = Yank");
        Text text3(250,110, "L = Loop");
        Text text4(250,130, "[ = Decrease Angle");
        Text text5(250,150, "] = Increase Angle");
        Text text6(250,170, "'=' = Increase Speed");
        Text text7(250,190, " '-' = Decrease Speed");
        Text text8(250,210, " Q = Quit Game");

        Rectangle box(250,130, 200,200);
        box.setColor(COLOR("red"));
        wait(15);
        continue;}
    else continue;}


  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }



    XEvent e;

    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);



      switch(c) {

      case 't':
	lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	lasso.check_for_coin1(&coin1);
	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'c':
    {

    Text text0(250,50, "CONTROLS:");
    Text text1(250,70, "T = Throw");
    Text text2(250,90, "Y = Yank");
    Text text3(250,110, "L = Loop");
    Text text4(250,130, "[ = Decrease Angle");
    Text text5(250,150, "] = Increase Angle");
    Text text6(250,170, "'=' = Increase Speed");
    Text text7(250,190, " '-' = Decrease Speed");
    Text text8(250,210, " Q = Quit Game");
    Rectangle box(250,120, 200,200);

    //Timer for resuming the game
    for(int i=5; i>0; i--){
    char resume[256];
    sprintf(resume,"Resuming in : %d ",i);
    Text Resume(250,30, resume);
    Resume.setColor(COLOR("red"));
    wait(1);}

    continue;}
      case 'q':
      {
        Text game_over((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2-10, "YOU EXITED FROM THE GAME");
        game_over.setColor(COLOR("red"));

        char score[256];
        sprintf(score, "Your Score: %d", lasso.getNumCoins());
        Text score_exit((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2 +10, score);

        Rectangle game_over_box((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2, textWidth("YOU EXITED FROM THE GAME")+10, 2*textHeight()+10 );

            stringstream ss;
        ss << lasso.getNumCoins() ;
        string str = ss.str();
        //cout<< str;
        if (str>= high_score) {

            high_score= str; //lasso.getNumCoins();
            highscore_player= player_name;
        }

    ofstream HSP_o;
    HSP_o.open ("highscore_player_timed.txt", ios::in | ios::out | ios::trunc); // HSP - High Score Player

        HSP_o << highscore_player;
        HSP_o.close();


    ofstream HS_o;
    HS_o.open ("high_score_timed.txt", ios::in | ios::out | ios::trunc); // HS - High Score

        HS_o << high_score;
        HS_o.close();

        wait(2);
        exit(0);
      }//end of case 'q'
      default:
	break;
      }
    }

    lasso.nextStep(stepTime);

    //start coin-------------------------------------
    coin.nextStep(stepTime);

    if((coin.getYPos() == PLAY_Y_HEIGHT)) coin.coin_circle.hide();

    if((coin.getYPos() < PLAY_Y_HEIGHT)) coin.coin_circle.show();

    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin();
      last_coin_jump_end = currTime;
    }
    //end coin--------------------------------------------------------

    //start coin1------------------------------------------------------
    coin1.nextStep(stepTime);

        if((coin1.getYPos() == PLAY_Y_HEIGHT)) coin1.coin1_circle.hide();

        if((coin1.getYPos() < PLAY_Y_HEIGHT)) coin1.coin1_circle.show();

        if(coin1.isPaused()) {
            if((currTime-last_coin_jump_end_1) >= 5*COIN_GAP) {
            coin1.unpause();
            }
        }

        if((coin1.getYPos() > PLAY_Y_HEIGHT) || (coin1.getYPos() < 0)) {
            coin1.resetCoin();
            coin1.coin1_circle.hide();
            last_coin_jump_end_1 = currTime;
        }

        if((coin1.getXPos() < PLAY_X_START) || (coin1.getXPos() > WINDOW_X )) {
            coin1.resetCoin();
            coin1.coin1_circle.hide();
            last_coin_jump_end_1 = currTime;
        }
    //end coin1------------------------------------------------------

    //start bomb1----------------------------------------------------
    bomb1.nextStep(stepTime);
    lasso.check_for_bomb1(&bomb1);
      charPressed.setMessage(msg);
    if((bomb1.getYPos() >= PLAY_Y_HEIGHT)) bomb1.bomb_circle.hide();

        if((bomb1.getYPos() < PLAY_Y_HEIGHT)) bomb1.bomb_circle.show();

        if(bomb1.isPaused()) {
            if((currTime-last_bomb_jump_end_1) >= BOMB_GAP) {
            bomb1.unpause();
            }
        }

        if((bomb1.getYPos() > WINDOW_Y) || (bomb1.getYPos() < 0)) {
            bomb1.resetBomb();
            bomb1.bomb_circle.hide();
            last_bomb_jump_end_1 = currTime;
        }

        if((bomb1.getXPos() < PLAY_X_START) || (bomb1.getXPos() > WINDOW_X )) {
            bomb1.resetBomb();
            bomb1.bomb_circle.hide();
            last_bomb_jump_end_1 = currTime;
        }

        //end bomb1-------------------------------------------------



    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    time=90- 1.85*currTime; // to match the time in seconds according to my laptop
    sprintf(Time,"Time Remaining: %0.0f", time);
    time_display.setMessage(Time);

    currTime += stepTime;
    wait(stepTime);

    if (time<=0) {
        Text game_over((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2-10, "THE TIME IS OVER");
        game_over.setColor(COLOR("red"));

        char score[256];
        sprintf(score, "Your Score: %d", lasso.getNumCoins());
        Text score_exit((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2 +10, score);

        Rectangle game_over_box((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2, textWidth("THE TIME IS OVER")+10, 2*textHeight()+10 );

        //record high score
        stringstream ss;
        ss << lasso.getNumCoins() ;
        string str = ss.str();
        //cout<< str;
        if (str>= high_score) {

            high_score= str; //lasso.getNumCoins();
            highscore_player= player_name;
        }

        ofstream HSP_o;
        HSP_o.open ("highscore_player_timed.txt", ios::in | ios::out | ios::trunc); // HSP - High Score Player


        ofstream HS_o;
        HS_o.open ("high_score_timed.txt", ios::in | ios::out | ios::trunc); // HS - High Score

        HS_o << high_score;
        HS_o.close();


        HSP_o << highscore_player;
        HSP_o.close();
    wait(2);
    exit(0);
    }


  } // End for(;;)

  wait(3);
  } //End of initCanvas

 } // end of timed mode

 else;
} // End main_program
