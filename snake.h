#ifndef SNAKE_H
#define SNAKE_H
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include<unistd.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Network.hpp>


//Numero de vezes cujo programa verirfica seu estado a cada segundo
#define TICKS_PER_SECOND 30 

//Velocidade padrão da cobra
#define DEFAULT_SPEED 30 

//Tamanho de mapa padrão
#define DEFAULT_MAP_X_SIZE 40
#define DEFAULT_MAP_Y_SIZE 30

//Classe Cells realiza o papel de célula da cobra (cada pedaço de seu corpo)
class Cells{

    public:
        //Direções definidas que a cobra pode tomar
        enum directions{
            RIGHT,
            DOWN,
            LEFT,
            UP
        };
    private:

        //Posição atual da célula no mapa
        int pos_x;
        int pos_y;

        //Direção atual da célula
        int _direction;

        //Direção que a celula estava no movimento anterior
        int _last_dir;
    
    public:
        Cells();

        //Funções para definir posição e direção
        void set_pos(int x, int y);
        void set_dir(int dir);
        void set_last_dir(int dir);

        //Funções que retornam posição e direção
        int get_x();
        int get_y();
        int direction();
        int last_dir();
};

//Classe snake gerencia estado da cobra
class Snake{
    public:
        //Direções definidas que a cobra pode tomar (same as Cells::directions)
        enum directions{
            RIGHT,
            DOWN,
            LEFT,
            UP
        };

    private:
        //Atributos da cobra (tamanho e velocidade atual)
        int _size;
        int _speed;

        //Nivel de aceleração da cobra (_speed = defaultSpeed + defaultSpeed*boost_level)
        int boost_level;

        //Variável local representando a velocidade padrão da cobra
        const int defaultSpeed = DEFAULT_SPEED;

        //TRUE = cobra esta viva; FALSE = cobra morreu
        bool lives;

        //Vetor dinâmico de células da cobra
        std::vector<Cells> body;

    public:
        Snake(int start_x, int start_y);

        //Retornam tamanho e se cobra esta viva
        int size();
        bool isAlive();
        
        //Adiciona e remove "n" células à cauda da cobra
        void addCells(int nCells);
        void removeCells(int nCells);

        //Define e retorna a direção da cabeça da cobra (direção da célula body[0])
        void set_direction(int dir);
        int direction();
        
        //Returna atual velocidade da cobra
        int speed();

        //Incrementa e diminui atual nivel ded boost
        void upBoost();
        void downBoost();
        
        //retorna posição da cabeça cobra (body[0])
        int x();
        int y();

        //Atualiza posições das células da cobra
        void move();

        //Mata a cobra
        void kill();
};

//Classe comida, objeto usado para incrementar tamanho da cobra
class Food{
    private:
        //Variaveis de posição da comida no mapa
        int pos_x;
        int pos_y;

    public:
        //Construtor inicializa comida com posições em x e y;
        Food(int x_pos, int y_pos);

        //Define posições das comidas
        void set_pos(int x_pos, int y_pos);
        
        //Retorna posições das comidas
        int x();
        int y();
};

//Classe derivada de Food, representam comidas com efeitos adversos
class Poison: public Food{
    public:
        //Definição dos tipos de venenos
        enum poisonType{
            //Diminui cobra
            SHRINK,
            //Aumenta velocidade da cobra
            SPEEDBOOST
        };

    private:
        //Variável que define qual o tipo de veneno
        int _type;
        int time_limit;
        const int _limit = TICKS_PER_SECOND * 10;
    
    public:
        //Cosntrutor de Poison inicializando com seus parametros classe pai Food
        Poison(int x_pos, int y_pos, int poison_type);

        void tick();
        int get_time_limit();
        int limit();
        void zero_tick();
        //Redefine tipo de veneno
        void set_type(int t);

        //retorna tipo de veneno
        int type(); 
};

//Classe de configurações, utilizada para setar os parâmetros do game antes mesmo dele iniciar
class Game_Config{
    
    //Possiveis dificuldaddes do jogo
    public:
        enum dificulties{
                EASY,
                MEDIUM,
                HARD
        };

    private:    
        //Variáveis de tamanho do mapa
        int map_size_x;
        int map_size_y;

        //Variáveis de dificuldade do jogo
        int _dificulty;
    
    public:
        Game_Config();
        //Por padrão, inicializa com valores padrões de tamanho e na dificuldade mais fácil
        Game_Config(int x_size, int y_size, int dif);

        //Retorna e redefine tamanho do mapa
        int map_x();
        int map_y();
        void set_map_x(int x);
        void set_map_y(int y);
        
        //Retorna e redefine dificuldade
        int dificulty();
        void set_dificulty(int dif);

};

//Classe de mapa com suas definiçẽs e funcões
class Map{
    public:
        enum legends{
            FREE_SPACE,
            SNAKE,
            WALL,
            FOOD,
            POISON
        };

    private:
        int map_x;
        int map_y;
        std::vector< std::vector<int> > map;
    public:
        Map(int x, int y);

        void set_boarders();
        void set_value(int x, int y, int value);
        
        std::vector< std::vector<int> > get_map();

        
};


class Obstacle{
    private:
        std::vector< std::vector<int> >_positions{{0, 0},
                                                  {0, 0},
                                                  {0, 0},
                                                  {0, 0},
                                                  {0, 0}};

    public:
        Obstacle(int map_size_x, int map_size_y);

        std::vector< std::vector<int> > positions();
};

class Game{
    private:
        int _score;
        int _time;
        int limit = TICKS_PER_SECOND;

        Map* maze;
        Game_Config* config;
        Snake* snake;
        Poison* poison;
        Food* food;
        std::vector<Obstacle> obstacles;
    
        sf::RenderWindow* window;
        sf::Event ev;


    public:
        Game(Game_Config _config);
        ~Game();

        const bool running() const;
        void boost();

        int score();
        void add_score(int n);
        void sub_score(int n);
        void set_score(int newScore);

        int time();
        void set_time(int newtime);
        void tickTime();

        void add_food();
        void add_poison();
        void remove_poison();
        void remove_food();

        void tick();
        void move();
        void run();

        void print_in_map();
        void render();
        // Snake get_snake();
        // Game_Config get_config();
        // Map get_maze();
};

#endif

