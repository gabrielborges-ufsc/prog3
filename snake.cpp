 using namespace std;

#include "snake.h"

using namespace std;

//Class Cells

void Cells::set_pos(int x, int y){
    if(x < 0 || y < 0) return;

    pos_x = x;
    pos_y = y;
};

void Cells::set_dir(int dir){
    
    if(dir < this->RIGHT || dir > this->UP) return;
    _direction = dir;
};

void Cells::set_last_dir(int dir){
    if(dir < this->RIGHT || dir > this->UP) return;
    _last_dir = dir;
}

int Cells::get_x(){
    return pos_x;
};

int Cells::get_y(){
    return pos_y;
};

int Cells::direction(){
    return _direction;
};

int Cells::last_dir(){
    return _last_dir;
};

Cells::Cells(){
    pos_x = 1;
    pos_y = 1;
    _direction = RIGHT;
    _last_dir = RIGHT;
};

//Class Snake

int Snake::size(){
    return _size;
};

bool Snake::isAlive(){
    return lives;
};

int Snake::direction(){
    return body[0].direction();
};

int Snake::speed(){
    return _speed;
};

int Snake::x(){
    return body[0].get_x();
};

int Snake::y(){
    return body[0].get_y();
};

void Snake::addCells(int nCells){
    if(nCells <= 0) return;

    for(int i = 0; i < nCells; i++){
        Cells new_cell;
        
        new_cell.set_dir( this->body.back().direction() );
        new_cell.set_last_dir( new_cell.direction());

        switch (this->body.back().direction())
        {
        case RIGHT:
            new_cell.set_pos(body.back().get_x()-1, body.back().get_y());
            break;
        
        case DOWN:
            new_cell.set_pos(body.back().get_x(), body.back().get_y()-1);
            break;
        
        case LEFT:
            new_cell.set_pos(body.back().get_x()+1, body.back().get_y());
            break;
        
        case UP:
            new_cell.set_pos(body.back().get_x(), body.back().get_y()+1);
            break;
        }

        body.push_back(new_cell);
        _size++;
        //cout << "Celula implementada " << i;
    }
};

//Minimo de cells = 4
void Snake::removeCells(int nCells){
    if(nCells <= 0) return;

    for(int i = 0; i < nCells; i++){
        if(body.size() <= 4) return;

        body.pop_back();
        _size--;
    }

    
}

void Snake::set_direction(int dir){
    if(dir < this->RIGHT || dir > this->UP) return;

    body[0].set_last_dir( body[0].direction() );
    body[0].set_dir(dir);
};

void Snake::upBoost(){
    boost_level++;
    _speed = defaultSpeed + defaultSpeed*boost_level;
}

void Snake::downBoost(){
    if(boost_level <= 0) return;
    boost_level--;
    _speed = defaultSpeed + defaultSpeed*boost_level;
}

void Snake::kill(){
    lives = false;
};

void Snake::move(){
    for(int i = 1; i < body.size(); i++){
        body[i].set_last_dir( body[i].direction() );
        body[i].set_dir( body[i-1].last_dir() );

        switch ( body[i].direction() )
        {
        case RIGHT:
            body[i].set_pos(body[i].get_x()+1, body[i].get_y());
            break;
        
        case DOWN:
            body[i].set_pos(body[i].get_x(), body[i].get_y()-1);
            break;
        
        case LEFT:
            body[i].set_pos(body[i].get_x()-1, body[i].get_y());
            break;
        
        case UP:
            body[i].set_pos(body[i].get_x(), body[i].get_y()+1);
            break;
        }
    }
};

Snake::Snake(int start_x, int start_y){
    //Init cabeça
    Cells head;
    head.set_pos(start_x,start_y);
    head.set_dir(RIGHT);
    head.set_last_dir(RIGHT);
    body.push_back(head);
    _size++;

    addCells(3);

    lives = true;
    boost_level = 0;
    _speed = defaultSpeed;
};   

//Class Food
void Food::set_pos(int x_pos, int y_pos){
    if(x_pos < 0 || y_pos < 0) return;

    pos_x = x_pos;
    pos_y = y_pos;
};

int Food::x(){
    return pos_x;
};

int Food::y(){
    return pos_y;
};

Food::Food(int x_pos, int y_pos){
    
    if(x_pos < 0 || y_pos < 0){
        pos_x = 1;
        pos_y = 1;
    
    }else{
        pos_x = x_pos;
        pos_y = y_pos;
    }
};

//Class Poison
void Poison::set_type(int t){
    if(t < SHRINK && t > SPEEDBOOST) return;

    _type = t;
};

int Poison::type(){
    return _type;
};

int Poison::get_time_limit(){
    return time_limit;
};

void Poison::zero_tick(){
    time_limit = 0;
};

void Poison::tick(){
    time_limit++;
};

Poison::Poison(int x_pos, int y_pos, int poison_type):Food(x_pos, y_pos){
    set_type(poison_type);
};

int Poison::limit(){
    return _limit;
}

//Class Game_Config
int Game_Config::map_x(){
    return map_size_x;
};

int Game_Config::map_y(){
    return map_size_y;
};

int Game_Config::dificulty(){
    return _dificulty;
};

void Game_Config::set_map_x(int x){
    if(x > 10){
        map_size_x = x;
    }
};

void Game_Config::set_map_y(int y){
    if(y > 10){
        map_size_y = y;
    }
};

void Game_Config::set_dificulty(int dif){
    if(dif < EASY || dif > HARD) return;

    _dificulty = dif;
};

Game_Config::Game_Config(int x_size, int y_size, int dif){
    set_map_x(x_size);
    set_map_y(y_size);
    set_dificulty(dif);
};

Game_Config::Game_Config(){
    set_map_x(DEFAULT_MAP_X_SIZE);
    set_map_y(DEFAULT_MAP_Y_SIZE);
    set_dificulty(EASY);
}

//class Map
void Map::set_boarders(){

    for(int i = 0; i < map.size(); i++){
        for(int j = 0; j < map[0].size(); j++){
            if(i == 0 || i == map.size()-1 || j == 0 || j == map[0].size() -1){
                map[i][j] = WALL;
            }
            else{
                map[i][j] = FREE_SPACE;
            }
        }
    }
};

void Map::set_value(int x, int y, int value){
    if(map_x > x && map_y > y && (value >= FREE_SPACE && value <= POISON)){
        map[x][y] = value;
    }
};

vector<vector<int>> Map::get_map(){
    return map;
}

Map::Map(int x, int y){
    if(x > 0 && y > 0){
        map.resize(x);
        for(int i = 0; i < x; i++ ){
            map[i].resize(y);
        }
    }
    map_x = x;
    map_y = y;

    set_boarders();
};

//class Obstacle
vector< vector<int> > Obstacle::positions(){
    return _positions;
};

Obstacle::Obstacle(int map_size_x, int map_size_y){
    srand(time(0));
    int rand_x = rand()%map_size_x;
    int rand_y = rand()%map_size_x;

    while(rand_x == 0 || rand_x >= map_size_x-3 || rand_y == 0 || rand_y >= map_size_y-3
    || (rand_x == 2 && rand_y == 3) || (rand_x == 2 && rand_y == map_size_y-2) || 
    (rand_x == 1 && rand_y == map_size_y-3) || (rand_x >= map_size_x-5 && rand_y <= 3) ||
    (rand_x >= map_size_x-4 && rand_y <= 4))
    {
        rand_x = rand()%map_size_x;
        rand_y = rand()%map_size_x;
    }

    _positions[0][0] = rand_x;
    _positions[0][1] = rand_y;
    _positions[1][0] = rand_x+1;
    _positions[1][1] = rand_y;
    _positions[2][0] = rand_x+2;
    _positions[2][1] = rand_y;
    _positions[3][0] = rand_x;
    _positions[3][1] = rand_y+1;
    _positions[4][0] = rand_x;
    _positions[4][1] = rand_y+2;
};

//Class Game
int Game::score(){
    return _score;
};

int Game::time(){
    return _time;
};

void Game::add_score(int n){
    _score+=n;
};

void Game::sub_score(int n){
    _score-=n;
};

void Game::set_score(int newScore){
    _score = newScore;
};

void Game::set_time(int newtime){
    if(newtime >= 0) _time = newtime;
};

void Game::tickTime(){
    _time++;
};

void Game::add_food(){
    std::srand(std::time(0));
    int rand_x = 10;
    int rand_y=10;
    
    do
    {
    rand_x = std::rand()%config->map_x();
    rand_y = std::rand()%config->map_y();
    }
    while(maze->get_map()[rand_x][rand_y] != Map::FREE_SPACE);

    food = new Food(rand_x, rand_y);
};

void Game::add_poison(){
    std::srand(std::time(0));
    int rand_x=10, rand_y=10, rand_type=0;
    
    rand_type = std::rand()%2;
    do
    {
    rand_x = std::rand()%config->map_x();
    rand_y = std::rand()%config->map_y();
    }
    while(maze->get_map()[rand_x][rand_y] != Map::FREE_SPACE);

    poison = new Poison(rand_x, rand_y, rand_type);
};

void Game::remove_food(){
    if(food != nullptr) delete food;
};

void Game::remove_poison(){
    if(poison != nullptr) delete poison;
};

Game::Game(Game_Config _config){
    cout << _config.map_x() << "  " << _config.map_y() << "  " << _config.dificulty();
    config = new Game_Config(_config.map_x(), _config.map_y(), _config.dificulty());
    window = new sf::RenderWindow(sf::VideoMode(10*_config.map_x(),10*_config.map_y()), "Ultimate Snake",sf::Style::Titlebar | sf::Style::Close);
    maze = new Map(config->map_x(), config->map_y());
    snake = new Snake(10,10);
    run();
    //obstacles.push_back(Obstacle(_config.map_x(),_config.map_y()));
};

Game::~Game(){
    delete window;
    delete maze;
    delete config;
    delete snake;
}

const bool Game::running() const{
    return window->isOpen();
}

void Game::tick(){
    if(food == nullptr)
        {
            add_food();
        }

    if(poison == nullptr){
            if(poison->get_time_limit() == poison->limit()){
                add_poison();
                poison->zero_tick();
            }
            else{poison->tick();}
        }
        while(window->pollEvent(ev))
        {
            switch (ev.type)
            {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                if(ev.key.code == sf::Keyboard::Right){
                    snake->set_direction(Snake::RIGHT);
                }else if(ev.key.code == sf::Keyboard::Down){
                    snake->set_direction(Snake::DOWN);
                }else if(ev.key.code == sf::Keyboard::Up){
                    snake->set_direction(Snake::UP);
                }else if(ev.key.code == sf::Keyboard::Left){
                    snake->set_direction(Snake::LEFT);
                }
                break;
            default:
                break;
            }
        }

        usleep(1000000/TICKS_PER_SECOND);
        tickTime();
}

void Game::boost(){
    limit = limit*0.66;
}

void Game::print_in_map()
{
    maze->set_boarders();
    if(food != nullptr){
        maze->set_value(food->x(), food->y(), Map::FOOD);
    }
    if(poison != nullptr){
        maze->set_value(poison->x(), poison->y(), Map::POISON);
    }
}
void Game::render
void Game::move(){
    snake->move();
    if(snake->x() == food->x() && snake->y()==food->y()){
        snake->addCells(1);
        add_score(10);
        delete food;
    }else if(snake->x() == poison->x() && snake->y()==poison->y()){
        sub_score(10);
        if(poison->type() == Poison::SHRINK){
            snake->removeCells(4);  
        }
        else if(poison->type() == Poison::SPEEDBOOST){
            boost();
        }
        delete poison;
        
    }else if(snake->x() == 0 || snake->x() == config->map_x()-1 || snake->y() == 0 || snake->y() == config->map_y()-1){
        snake->kill();
    }
}
void Game::run(){
    while(snake->isAlive())
    {
        tick();
        if(_time == limit){
            this->move();
            set_time(0);
            print_in_map();
            render();
        }
    }
}