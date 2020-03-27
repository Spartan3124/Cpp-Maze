//Name: Caleb Fischer
//File: 350Midterm 10/27/19
#include <iostream>
#include <vector>
#include <ctime>
/*
ALGORITHM: build_maze
INPUT: (r,c) - the starting point

let UNVISITED be the container of unvisited cells:
initially this should be all cells except for (r,c)

let PATH be the container containing only (r,c)

let VISITED be a container of visited cells:
initially this should contain only (r,c)

let PUNCHED be an empty container of "(r0,c0) -- (r1,c1)"
that represents punched walls.
WARNING: If "(r0,c0) -- (r1,c1)" is in this container.
then "(r1,c1) -- (r0,c0)" is also in the container.

while PATH is not empty:
look at the last step stored in PATH -- call it x
look at all the surrounding cells around x
the available cells to go to are the
cells to the north, south, east, west of x
which are within the grid and not visited yet,
i.e., in UNVISITED.

if there is at least one available cell:
randomly choose one available cell -- call it y
store y in VISITED and add that to PATH
remove y from UNVISITED

store x--y (the punched wall) in PUNCHED; y--x
should be there too

else:
there are are no available cells
we have to go backwards -- remove x from PATH
*/

//row col is effectively a key
//Cells == nodes
class Cell
{
public:
    Cell(int key, Cell * prev = NULL, Cell * next = NULL)
        : k_(key), prev_(prev), next_(next)
    {
        visited_ = 0;
        n_ = 1;
        s_ = 1;
        e_ = 1;
        w_ = 1;
    }
    
    // Cell(const Cell & c2)
    //     : r(c2.r), c(c2.c)
    // {
    // }
    int k_;
    Cell * next_; // pointer to next Cell
    Cell * prev_; // pointer to previous Cell
    bool visited_;
    //walls
    bool n_, s_, e_, w_;

};


class Maze
{
public:
    Maze()
        : mstart_(new Cell(-1)), mend_(new Cell(-1))
    {
        mstart_->next_ = mend_;
        mend_->prev_ = mstart_;
    }
    
    ~Maze()
    {
        Cell * m = mstart_;
        while (m != NULL)
        {
            Cell * q = m->next_;
            delete m;
            m = q;
        }
    }

    void pushFront(int key)
    {
        Cell * c = new Cell(key);

        //original head points to new head
        mstart_->next_->prev_ = c;

        //new cell points to original head
        c->next_ = mstart_->next_;
        //c points to &pstart
        c->prev_ = mstart_;
        mstart_->next_ = c;
    }

    void pushBack(int key)
    {
        Cell * c = new Cell(key);
        
        //new cell points to original head
        mend_->prev_->next_ = c;
        c->next_ = mend_;
        //c points to &pstart
        c->prev_ = mend_->prev_;
        mend_->prev_ = c;
    }
    
    Cell * mstart_;
    Cell * mend_;
};

// The maze is n-by-n and (r,c) is the starting point of the
// maze.
void initialize_maze(int n, int r, int c, Maze & maze)
{
    //row * n + column
    for (int i = 0; i < n * n; ++i)
    {
        maze.pushBack(i);
    }
}


class Path
{
public:
    Path()
        : pstart_(new Cell(-1)), pend_(new Cell(-1))
    {
        pstart_->next_ = pend_;
        pend_->prev_ = pstart_;
    }
    
    ~Path()
    {
        Cell * p = pstart_;
        while (p != NULL)
        {
            Cell * q = p->next_;
            delete p;
            p = p->next_;
        }
    }

    void push(int key, int n, Maze & m)
    {
        Cell * mc = m.mstart_->next_;
        for (int i = 0; i < n * n; ++i)
        {
            if (mc->k_ == key) break;
            mc = mc->next_;
        }
        
        mc->visited_ = 1;

        Cell * c = new Cell(key);
//original head points to new head
        pstart_->next_->prev_ = c;

        //new cell points to original head
        c->next_ = pstart_->next_;
        //c points to &pstart
        c->prev_ = pstart_;
        //pstart_->next_->prev_ = c;
        pstart_->next_ = c;
    }

    void pop()
    {
        //start -> c0 -> c1 -> start
        //delete c0
        Cell * c0 = pstart_->next_;
        pstart_->next_->next_->prev_ = pstart_;
        pstart_->next_ = pstart_->next_->next_;
        delete c0;
    }
    
    Cell * pstart_;
    Cell * pend_;
};

void print_path(const Path & p)
{
    // Cell * phead = p.pstart_->next_; 
    // while (phead != NULL)
    // {
    //     std::cout << phead->k_ << ' '; 
    //     phead = phead->next_;
    // }
    // std::cout << std::endl;

    // Cell * pend = p.pend_->prev_;
    // while (pend != NULL)
    // {
    //     std::cout << pend->k_ << ' '; 
    //     pend = pend->prev_;
    // }
    // std::cout << std::endl;
}

void print_cell(int n, Cell *& pc)
{
    for (int col = 0; col < n; ++col)
    {
        std::cout << (pc->w_ == 1 ? '|' : ' ');
        std::cout << ' ';
        pc = pc->next_;
    }
    std::cout << '|';
    std::cout << std::endl;
}


void print_line(int n, Cell *& pc)
{
    std::cout << '+';
    for (int row = 0; row < n; ++row)
    {
        std::cout << (pc->n_ == 1 ? '-' : ' ');
        std::cout << '+';
        pc = pc->next_;
    }
    std::cout << std::endl;
}


void print_line_s(int n, Cell *& pc)
{
    std::cout << '+';
    for (int row = 0; row < n; ++row)
    {
        std::cout << (pc->s_ == 1 ? '-' : ' ');
        std::cout << '+';
        pc = pc->next_;
    }
    std::cout << std::endl;
}

void prev_n(int n, Cell *& pc)
{
    for (int j = 0; j < n; ++j)
    {
        pc = pc->prev_;
    }
}

void print_maze(int n, const Maze & maze)
{
    Cell * pc = maze.mstart_->next_;
    Cell * pcSaved = pc;
    for (int i = 0; i < n; ++i)
    {
        pc = pcSaved;
        print_line(n, pc);
        pcSaved = pc;

        //resets pc to print cells
        prev_n(n, pc);
        print_cell(n, pc);
    }
    prev_n(n, pc);
    print_line_s(n, pc);
}

bool quit(int n, Maze & maze)
{
    Cell * pc = maze.mstart_->next_;
    
    for (int i = 0; i < n * n; ++i)
    {
        if (pc->visited_ == 0) return 0;
        pc = pc->next_;
    }
    return 1;
}


bool isTop(int n, int key)
{
    return (key < n ? 1 : 0);
}


bool isBottom(int n, int key)
{
    return (key >= (n * (n - 1)) && key < n * n ? 1 : 0);
}


bool isLeft(int n, int key)
{
    for (int i = 0; i < n * n; i += n)
    {
        if (key == i) return 1;
    }
    return 0;
}


bool isRight(int n, int key)
{
    for (int i = n - 1; i < n * n; i += n)
    {
        if (key == i) return 1;
    }
    return 0;
}


bool checkValidMoves(bool & north, bool & south, bool & east, bool & west,
                     int xKey, int n, Cell *& x)
{
    Cell * xp = x;
    if (isTop(n, xKey))
    {
        north = 0;
    }
    else
    {
        for (int i = 0; i < n; ++i)
        {
            xp = xp->prev_;
        }
        north = (!xp->visited_);
    }

    xp = x;
    if (isBottom(n, xKey))
    {
        south = 0;
    }
    else
    {
        for (int i = 0; i < n; ++i)
        {
            xp = xp->next_;
        }
        south = (!xp->visited_);
    }

    xp = x;
    if (isLeft(n, xKey))
    {
        west = 0;
    }
    else
    {
        xp = xp->prev_;
        west = (!xp->visited_);
    }

    xp = x;
    if (isRight(n, xKey))
    {
        east = 0;
    }
    else
    {
        xp = xp->next_;
        east = (!xp->visited_);
    }

    return (north || south || east || west);
}


void build_maze(int n, int r, int c, Maze & m)
{
    initialize_maze(n, r, c, m);
    srand((unsigned int) time(NULL));
    Path p;
 
    p.push((r * n + c), n, m);
    Cell * x;

    bool north, south, east, west = 1;
    while (!quit(n, m))
    {
        x = m.mstart_->next_;
        print_path(p);
            //print_maze(n, m);
        north = 1;
        south = 1;
        east = 1;
        west = 1;

        //checks if moves are open
        //sets x in maze to path head
        for (int i = 0; i < n * n; ++i)
        {
            if (x->k_ == p.pstart_->next_->k_) break;
            x = x->next_;
        }
        
        //sets current cell to visited
        x->visited_ = 1;
        int xKey = x->k_;
        //std::cout << "naboveLoop: " << n; 
        if(!checkValidMoves(north, south, east, west, xKey, n, x))
        {
            p.pop();
        }
        else
        {
            //check if one is unvisited, else pop()
            // std::cout << "north: " << north << '\n';
            // std::cout << "south: " << south << '\n';
            // std::cout << "east: " << east << '\n';
            //std::cout << "west: " << west << '\n';

            bool moveComplete = 0;
            while (!moveComplete)
            {
                //std::cout << "here!";
                int s = rand() % 4;
                //std::cout << "s: " << s << '\n';
                switch(s)
                {
                    case 0:
                        if (!north) break;
                        //int newX = x->k_ - n;

                        //sets north wall to 0 and new x s wall to 0
                        //adds next x to path
                        x->n_ = 0;
                        for (int i = 0; i < n; ++i)
                        {
                            x = x->prev_;
                        }
                        xKey = x->k_;
                        x->s_ = 0;
                        p.push(xKey, n, m);

                        //std::cout << "Moved North\n";
                        moveComplete = 1;
                        break;
                        
                    case 1:
                        if (!south) break;

                        //sets s wall to 0 and new x n wall to 0;
                        //adds next x to path
                        x->s_ = 0;
                        for (int i = 0; i < n; ++i)
                        {
                            x = x->next_;
                        }
                        xKey = x->k_;
                        x->n_ = 0;
                        p.push(xKey, n, m);

                        //std::cout << "Moved South\n";
                        moveComplete = 1;
                        break;
                        
                    case 2:
                        if (!east) break;
                        x->e_= 0;
                        x = x->next_;
                        xKey = x->k_;
                        x->w_ = 0;
                        p.push(xKey, n, m);

                        //std::cout << "Moved East\n";
                        moveComplete = 1;
                        break;
                        
                    case 3:
                        if(!west) break;
                        //std::cout << "WEST";
                        x->w_= 0;
                        x = x->prev_;
                        xKey = x->k_;
                        x->e_ = 0;
                        p.push(xKey, n, m);

                        //std::cout << "Moved West\n";
                        moveComplete = 1;
                        break;
                }
            }
        }
    }
//Unvisited class containing array of booleans
    //Path class with PATH.push for forwards, PATH.pop() backwards.
    //Punched walls.  Cell (2,3), punch east see cell (2,4). record ((2, 3), (2, 4))
    //return punchwall vect
}


int main()
{
    srand((unsigned int) time(NULL)); 
            
// Maze is in a grid of n-by-n cells. The starting point is (r, c).
    int n, r, c;
    std::cin >> n >> r >> c;
    //std::vector<int> x(n, n);
    int size = n * n;
    Maze maze;
    //Cell cell(3, 3);
    //maze.pushBack(3);
    build_maze(n, r, c, maze);
    //std::vector<PunchedWall> punched_walls;//= build_maze(n, r, c);
    print_maze(n, maze);
    return 0;
}
