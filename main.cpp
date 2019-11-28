#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_image.h"
#include <time.h>
#include <string>
#define BLOCOS 45 //Define quantidade de blocos
int cor,corr,corrr;
const int LARGURA_TELA = 600;
const int ALTURA_TELA = 400;
const int TELA_BITS = 32;

SDL_Surface *image = NULL;
SDL_Surface *tela = NULL;

SDL_Surface *carrega_imagem( std::string NOMEIMAGEM )
{
    //Carrega imagem
    SDL_Surface* carregaimagem = NULL;

    //Imagem otimizada que sera usada
    SDL_Surface* otimizaImagem = NULL;

    //Carrega imagem usando SDL
    carregaimagem = IMG_Load( NOMEIMAGEM.c_str() );

    //Se a imagem for carregada
    if( carregaimagem != NULL )
    {
        //Cria image
        otimizaImagem = SDL_DisplayFormat( carregaimagem );

        //limpa buffer
        SDL_FreeSurface( carregaimagem );
    }


    return otimizaImagem;
}

void aplica( int x, int y, SDL_Surface* fonte, SDL_Surface* destino )
{
    //Deslocamento retangulo
    SDL_Rect desloca;


    desloca.x = x;
    desloca.y = y;


    SDL_BlitSurface( fonte, NULL, destino, &desloca );
}

bool init()
{
    //Inicia o SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Seta Tamanho da tela
    tela = SDL_SetVideoMode( LARGURA_TELA, ALTURA_TELA, TELA_BITS, SDL_SWSURFACE );

    //Caso acontece algum erro
    if( tela == NULL )
    {
        return false;
    }

    //Nome janela
    SDL_WM_SetCaption( "Tela inicia", NULL );


    return true;
}

void limpa_tela()
{
    //limpa a imagem
    SDL_FreeSurface( image );

    //sai do sdl
    SDL_Quit();
}


bool colisao(float Ax, float Ay, float Al, float Aal, float Bx, float By, float Bl, float Bal) //faz a checkagem da colisao
{
  if ( Ay+Aal < By ) return false;
  else if ( Ay > By+Bal ) return false;
  else if ( Ax+Al < Bx ) return false;
  else if ( Ax > Bx+Bl ) return false;

  return true;
}

struct Bloco
{

  float x;//posicao
  float y;//posicao
  float largura;
  float altura;
  bool status;
};


void mostrajogo(){
  {
    //inicializa SDL
  SDL_Init(SDL_INIT_EVERYTHING);




  //Nome JANELA
  SDL_WM_SetCaption( "JOGO", NULL );

  //Tamanho Janela
  SDL_SetVideoMode(600,400,32, SDL_OPENGL );

  //Sombras
  glShadeModel(GL_SMOOTH);

  //Booleana jogo
  bool executando = true;

  SDL_Event event;

  int contagem = 0;

  float personX = 300; //posicao x do retangulo
  float personY = 370; //posicao y do retangulo
  float personlarg = 80; //largura do retangulo
  float personalt = 20; //altura do retangulo

  bool esq = false,dir = false; //estado do botao

  //BOLA
  float bolaX = 300; //posicao x
  float bolaY = 280; //posicao y
  float bolaCA = 30; //comprimento e altura da bola

  float vellX = 0.3; // velocidade x
  float vellY = 0.3; // velocidade y


  //Elementos bloco

  Bloco blocos[BLOCOS]; //Cria Vetor de blocos

  for ( int n = 0, x = 4, y = 10; n < BLOCOS; n++, x += 66 ) //atravessa todo o vetor
    {
      if ( x > 560 ) //se o x estiver perto da borda direita
        {
          x = 4; //Volta para a esquerda
          y += 25; //Move para baixo
        }
      blocos[n].x = x; //Seta posicao atual do bloco x
      blocos[n].y = y; //posicao y
      blocos[n].largura = 60; //largura
      blocos[n].altura = 20; //altura
      blocos[n].status = true; //Seta estado do bloco para verdadeiro
    }


  //Loop do jogo
  while ( executando )
    {
      //EVENTOS
      while ( SDL_PollEvent(&event) )
        {
          //se a janela for fechada
          if ( event.type == SDL_QUIT )
            {
              executando = false;
            }

          //SE PRECIONAR ESQ FECHA A JANELA
          if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE )
            {
              executando = false;
            }

          if ( event.type == SDL_KEYDOWN ) //CHECKA SE BOTAO FOI PRESSIONADO PARA BAIXO
            {
              if ( event.key.keysym.sym == SDLK_LEFT ) //Checka seta para esquerda
                {
                  esq = true;
                }

              else if ( event.key.keysym.sym == SDLK_RIGHT ) //Checka seta para direita
                {
                  dir = true;
                }

            }

          else if ( event.type == SDL_KEYUP ) //Checka se uma tecla foi solta
            {
              if ( event.key.keysym.sym == SDLK_LEFT ) //Seta esquerda
                {
                  esq = false;
                }

              else if ( event.key.keysym.sym == SDLK_RIGHT ) //Seta direita
                {
                  dir = false;
                }
            }


        }

      //LOGICA MOVIMENTO
      if ( esq == true ) //Se seta esquerda for pressionada
        {
          personX -= 1; //Move para esquerda
        }

      if ( dir == true ) //Se seta direita for pressionada
        {
          personX += 1; //Move para direita
        }
      //limita as bordas para o retangulo nao passar
      if ( personX < 0 )
        {
          personX = 0;
        }

      if ( personX+personlarg > 600 )
        {
          personX = 600-personlarg;
        }

      //Logica bola
      bolaX += vellX; //move a bola no eixo x primeiro

      for ( int n = 0; n < BLOCOS; n++ ) //Percorre o vetor de tijolos
        {
          if ( blocos[n].status == true ) //Se o tijolo nao foi atingido
            {
              if ( colisao(bolaX,bolaY,bolaCA,bolaCA,blocos[n].x,blocos[n].y,blocos[n].largura, blocos[n].altura) == true ) //Checka colisao
                {
                  vellX =- vellX; //Muda velocidade x da bola
                  blocos[n].status = false; //Quebra o bloco
                  contagem +=1;
                  vellY *= 1.015;
                  vellX *= 1.015;

                  break; //Para de chekar a colisao no eixo x
                }
            }
        }

      bolaY += vellY; //move a bola no eixo y primeiro

      for ( int n = 0; n < BLOCOS; n++ ) //Percorre o vetor de tijolos
        {
          if ( blocos[n].status == true ) //Se o tijolo nao foi atingido
            {
              if ( colisao(bolaX,bolaY,bolaCA,bolaCA,blocos[n].x,blocos[n].y,blocos[n].largura, blocos[n].altura) == true ) //Checka colisao
                {
                  vellY =- vellY; //Muda a velocidade y da bola
                  blocos[n].status = false; //Quebra tijolo
                  contagem += +1;
                  vellY *= 1.015;
                  vellX *= 1.015;

                  break; //Para de chackar a colisao no eixo y
                }
            }
        }

      if ( bolaX < 0 ) //Checka se a bola bateu na borda esquerda
        {

          vellX = -vellX; //nega a velocidade x
        }

      else if ( bolaX+bolaCA>600 )
        {
          vellX = -vellX;//nega a velocidade x
        }

      if ( bolaY < 0 )
        {
          vellY = -vellY; //nega a velocidade y
        }

      if ( bolaY+bolaCA > 400 ) //Renicia o jogo se a bola bater na parte inferior
        {


          personX = 300;
          personY = 370;
          personlarg = 80;
          personalt = 20;

          esq = false,dir = false;


          bolaX = 300;
          bolaY = 280;
          bolaCA = 30;

          vellX = 0.3;
          vellY = 0.3;
          contagem = 0;

          for ( int n = 0; n < BLOCOS; n++ )
            {
              blocos[n].status = true;
            }
        }
        else if ( contagem == 45 ) //Se todos os blocos acabarem
            {


              personX = 300;
              personY = 370;
              personlarg = 80;
              personalt = 20;

              esq = false,dir = false;


              bolaX = 300;
              bolaY = 280;
              bolaCA = 30;

              vellX = 0.3;
              vellY = -0.3;
              contagem = 0;

              for ( int n = 0; n < BLOCOS; n++ )
                {
                  blocos[n].status = true;
                }
            }

      if ( colisao(bolaX,bolaY,bolaCA,bolaCA,personX,personY,personlarg,personalt) == true ) //checka se a bola bateu no retangulo
        {
          vellY *= 1.015;
          vellX *= 1.015;
          vellY = -vellY;
        }



      //RENDERIZA TELA
      glClear(GL_COLOR_BUFFER_BIT);

      glPushMatrix();

      glOrtho(0,600,400,0,-1,1);

      glColor4ub(255,255,255,255);

      glBegin(GL_QUADS);

      glVertex2f(personX,personY);
      glVertex2f(personX+personlarg,personY);
      glVertex2f(personX+personlarg,personY+personalt);
      glVertex2f(personX,personY+personalt);

      glEnd();
      //gera as cores da bola
      cor=rand()%255;
      corr=rand()%255;
      corrr=rand()%255;

      glColor4ub(cor,corr,corrr,255);
      glBegin(GL_QUADS);
      glVertex2f(bolaX,bolaY);
      glVertex2f(bolaX+bolaCA,bolaY);
      glVertex2f(bolaX+bolaCA,bolaY+bolaCA);
      glVertex2f(bolaX,bolaY+bolaCA);
      glEnd();

      glColor4ub(0,0,255,255);

      glBegin(GL_QUADS);

      for ( int n = 0; n < BLOCOS; n++ )
        {
          if ( blocos[n].status == true )
          {


               //gera as cores dos blocos
               cor=rand()%255;
               corr=rand()%255;
               corrr=rand()%255;
               glColor4ub(cor,corr,corrr,255);








              glVertex2f(blocos[n].x,blocos[n].y);
              glVertex2f(blocos[n].x+blocos[n].largura,blocos[n].y);
              glVertex2f(blocos[n].x+blocos[n].largura,blocos[n].y+blocos[n].altura);
              glVertex2f(blocos[n].x,blocos[n].y+blocos[n].altura);

            }
        }

      glEnd();

      glPopMatrix();

      SDL_GL_SwapBuffers();

      SDL_Delay(1);
    }
}
  SDL_Quit();
}

int main( int argc, char* args[] )
 {
     //Inicializa
    if( init() == false )
    {
        return 1;
    }

    //Carrega imagem
    image = carrega_imagem( "menu.png" );

    //Caso a imagem falhe
    if( image == NULL )
    {
        return 1;
    }

    //Applica a imagem
    aplica( 0, 0, image, tela );

    //Atualizada a tela
    if( SDL_Flip( tela ) == -1 )
    {
        return 1;
    }

    //Fecha imagem depois de 3 segundos
   SDL_Delay( 3000 );
   mostrajogo();

  return 0;
}
