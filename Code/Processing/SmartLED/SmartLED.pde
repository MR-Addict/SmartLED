import websockets.*;

WebsocketClient wsc;
ColorPicker cp;

int MatrixWidth = 16;
int MatrixHeight = 16;
int MatrixGrid = 50;

int LastTime;
int Mode=0;

color BGCOLOR = #2B2B2B;
color SCOLOR = #0000FF;
color LINECOLOR = #AAAAAA;
color[][] BLOCK = new color[MatrixWidth][MatrixHeight];
String WSURL="ws://192.168.4.1:81/";
String Menu[] = new String[]{"Reset", "Draw", "Display Time", "Bouncing Ball", "Halo01",
  "Halo02", "DisplaySprite", "Snake Game"};

void setup() {
  size(1200, 840);
  textSize(30);
  textAlign(CENTER);
  wsc = new WebsocketClient(this, WSURL);
  cp=new ColorPicker(840, 20, 400, 300);
  for (int x=0; x<MatrixWidth; x++)
    for (int y=0; y<MatrixHeight; y++)
      BLOCK[x][y]=BGCOLOR;
}

void draw() {
  SendTime();
  background(BGCOLOR);
  DrawFrame();
}

void mousePressed() {
  GetColor();
  GetPos();
  GetMode();
}

void keyPressed() {
  if (key == CODED) {
    int Direction = 0;
    if (keyCode == UP) {
      Direction = 0;
    } else if (keyCode == DOWN) {
      Direction = 3;
    } else if (keyCode == RIGHT) {
      Direction =2;
    } else if (keyCode == LEFT) {
      Direction = 1;
    }
    String MYJSON = "{\"Direction\":"+Direction+'}';
    wsc.sendMessage(MYJSON);
  }
}

void SendTime() {
  if (Mode == 0 && millis()-LastTime>1000) {
    String MYJSON =  "{\"H\":"+hour()+",\"M\":"+minute()+'}';
    wsc.sendMessage(MYJSON);
    LastTime = millis();
  }
}

void DrawFrame() {
  cp.render();
  strokeWeight(5);
  for (int x=0; x<MatrixWidth; x++) {
    for (int y = 0; y<MatrixHeight; y++) {
      fill(BLOCK[x][y]);
      rect(20+x*MatrixGrid, 20+y*MatrixGrid, MatrixGrid, MatrixGrid);
    }
  }
  fill(BGCOLOR);
  for (int i=0; i<8; i++)
    rect(840, 400+50*i, 340, 40);
  fill(#AAAAAA);
  for (int i=0; i<8; i++)
    text(Menu[i], 1000, 430+50*i);
  fill(SCOLOR);
  rect(840, 330, 340, 40);
}

void GetColor() {
  if (mouseX >= 840 &&mouseX < 1180 &&mouseY >= 20 &&mouseY <= 320 ) {
    SCOLOR = get( mouseX, mouseY );
    String MYJSON = "{\"R\":"+(SCOLOR >> 16 & 0xFF)+",\"G\":"+(SCOLOR >> 8 & 0xFF)+",\"B\":"+(SCOLOR & 0xFF)+'}';
    wsc.sendMessage(MYJSON);
  }
}

void GetPos() {
  for (int x=0; x<MatrixWidth; x++) {
    for (int y=0; y<MatrixHeight; y++) {
      if (mouseX>20+x*MatrixGrid&&mouseX<70+x*MatrixGrid&&mouseY>20+y*MatrixGrid&&mouseY<70+y*MatrixGrid) {
        if (mouseButton == LEFT) {
          BLOCK[x][y]=SCOLOR;
          String MYJSON = "{\"X\":"+x+",\"Y\":"+y+",\"LED\":\"ON\"}";
          wsc.sendMessage(MYJSON);
          return;
        } else if (mouseButton == RIGHT) {
          BLOCK[x][y]=BGCOLOR;
          String MYJSON = "{\"X\":"+x+",\"Y\":"+y+",\"LED\":\"OFF\"}";
          wsc.sendMessage(MYJSON);
          return;
        }
      }
    }
  }
}

void GetMode() {
  for (int i=0; i<8; i++) {
    if (mouseX>840&&mouseX<1180&&mouseY>400+50*i&&mouseY<440+50*i) {
      if (i==0||i==1) {
        for (int x=0; x<MatrixWidth; x++)
          for (int y=0; y<MatrixHeight; y++)
            BLOCK[x][y]=BGCOLOR;
        Mode = 6;
      } else Mode = i-2;
      String MYJSON = "{\"Mode\":"+Mode+'}';
      wsc.sendMessage(MYJSON);
    }
  }
}

void webSocketEvent(String message) {
  println("Received: " + message);
}
