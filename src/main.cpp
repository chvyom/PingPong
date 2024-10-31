#include <raylib.h>
#include <iostream>

int player_score=0;
int cpu_score=0;

bool gameOver=false;
bool pause=false;

const int screen_width=1280;
const int screen_height=800;

Color Green=Color{38,185,145,255};
Color Dark_Green=Color{20,160,133,255};
Color Light_Green=Color{129,204,184,255};
Color Yellow=Color{243,213,91,255};


class Ball
{
    public:
    float x,y;
    int speed_x,speed_y;
    float radius;
 
    void ResetBall()
    {
        x=GetScreenWidth()/2;
        y=GetScreenHeight()/2;

     
      
        int speedrandom[2]={-1,1};
        speed_x*=speedrandom[GetRandomValue(0,1)];
        speed_y*=speedrandom[GetRandomValue(0,1)];
     
       

    }
    void Draw(){
         DrawCircle(x,y,radius,Yellow);
    }
    void Update(){
        x+=speed_x;
        y+=speed_y;

        if(y+radius>=GetScreenHeight() || y-radius<=0)
        {
            speed_y*=-1;
        }
        if(x+radius>=GetScreenWidth())
        {
               
                cpu_score++;
                ResetBall();
        } 
        if(x-radius<=0)
        {
           
            player_score++;
            ResetBall();
        }
        
    }

    
};

class Paddle
{
    protected:
        void LimitMovement()
        {
             if(y<=0)
            {
                y=0;
            }
            if(y+height>=GetScreenHeight())
            {
                y=GetScreenHeight()-height;
            }   
        }
    public:
        float x,y;
        int speed;
        float width,height;
    
        void Draw()
        {
              DrawRectangle(x,y,width,height,WHITE);
        }
        void Update()
        {
            if(IsKeyDown(KEY_UP))
            {
                y-=speed;
            }
            if(IsKeyDown(KEY_DOWN))
            {
                y+=speed;
            }
            LimitMovement();
           
        }
};
class CpuPaddle:public Paddle{
    public:
        void Update(float ball_y)
          {
              if(y+height>ball_y)
              {
                  y=y-speed;
              }
              if(y+height<ball_y){
                  y=y+speed;
              }

              if(player_score>=5)
              {
                    if(y+height/2>ball_y)
                    {
                        y=y-speed;
                     }
                    if(y+height/2<ball_y){
                         y=y+speed;
                     }
              }
              LimitMovement();
          }
  
};



Ball ball;
Paddle player;
CpuPaddle cpu;

void InitGame()
{
    ball.radius=20;
    ball.x=screen_width/2;
    ball.y=screen_height/2;
    ball.speed_x=7;
    ball.speed_y=7;

    player.width=25;
    player.height=120;
    player.x=screen_width-player.width-10;
    player.y=screen_height/2-player.height/2;
    player.speed=6;

    cpu.width=25;
    cpu.height=120;
    cpu.x=10;
    cpu.y=screen_height/2-cpu.height/2;
    cpu.speed=6;

    cpu_score=0;
    player_score=0;

}

void UpdateGame()
{
    if(!gameOver)
    {  
        if (IsKeyPressed('P')) pause = !pause;

    if (!pause){
      if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height}))
        {
            ball.speed_x*=-1;
        }
         if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.width,cpu.height}))
        {
            ball.speed_x*=-1;
        }
    
     ball.Update();
        player.Update();
        cpu.Update(ball.y);
        if(player_score==10 || cpu_score==10)
            gameOver=true;
    }
}
    else{
        
         if(IsKeyPressed(KEY_ENTER))
            {
                InitGame();
                gameOver = false;
            }

    }

      
}  

void DrawGame()
{
        BeginDrawing();
        ClearBackground(Dark_Green);

        if(!gameOver){
        DrawRectangle(0,0,screen_width/2,screen_height,Green);
        DrawCircle(screen_width/2,screen_height/2,150,Light_Green);
        DrawLine(screen_width/2,0,screen_width/2,screen_height,WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i",cpu_score),screen_width/4-20,20,80,WHITE);
        DrawText(TextFormat("%i",player_score),3*screen_width/4-20,20,80,WHITE);

        if (pause) DrawText("GAME PAUSED", screen_width/2 - MeasureText("GAME PAUSED", 40)/2, screen_height/2 - 40, 40, WHITE);

    }

    else
    {
        if(player_score==10)
            DrawText("YOU WON",GetScreenWidth()/2-MeasureText("YOU WON",40)/2,screen_height/2-30,40,WHITE);
        else if(cpu_score==10)
             DrawText("YOU LOST",GetScreenWidth()/2-MeasureText("YOU LOST",40)/2,screen_height/2-30,40,WHITE);

     DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, WHITE);
    }
    EndDrawing();
}

        
       


int main() 
{
    
    InitWindow(screen_width,screen_height,"PingPong");

    Image icon = LoadImage("src\\Pong.png");
    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    SetWindowIcon(icon);
    UnloadImage(icon);
    
    SetTargetFPS(60);
    
    InitGame();

    

    while (WindowShouldClose()==false)
    {
       
        UpdateGame();
        DrawGame();
        
    }
    

    CloseWindow();

    return 0;
}