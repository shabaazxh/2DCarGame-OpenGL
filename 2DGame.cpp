/*****************************
    Shahbaaz Hussain    
        w1726400

*****************************/

/* Includes */
#include "include\freeglut.h"
#include <iostream>
#include <vector>
#include <time.h>

/* Vector2 struct to set x,y coords */
struct vector2
{
    float x, y; //x,y pos variables
    vector2() : x(0), y(0) {} //set x,y variables to 0
    vector2(float x_, float y_) : x(x_), y(y_) {} //set constructor variables to struct vairbales

    /* Does not update the variables, returns single calculation at given time*/
    vector2 operator+(const vector2& vector) const {
        return vector2(x + vector.x, y + vector.y);
    }
    /* Updates the variables by adding new value to the current value of x,y */
    vector2 operator+=(const vector2& vector) {
        return vector2(x += vector.x, y += vector.y);
    }
    /* Does not update the variables, returns single calculation at given time*/
    vector2 operator-(const vector2& vector) const {
        return vector2(x - vector.x, y - vector.y);
    }
    /* Updates the variables by subtracing current value by another value and set
    x,y to the result of that calculation */
    vector2 operator-=(const vector2& vector) {
        return vector2(x -= vector.x, y -= vector.y);
    }
};

/* Vector3 struct to set colour on objects */
struct vector3
{
    float x, y, z; //init variables x,y,z (r.g.b) 
    vector3() : x(0), y(0), z(0) {} //set all variables to 0
    /* Constructor parameter variables set to struct variables */
    vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

/* GameObject class to render 2D Game primitives e.g. Player/Obstacles */
class GameObject {
public:
    GameObject();
    //GameObject contructor to set x,y,width, height & colour for glRect()
    GameObject(const GLfloat rotation, const vector2& position, const vector2& size, const vector3& color);

    void draw() const; //draw the glRect, can be called once constructor is used to init GameObject object type
    
    // getter/setters to get position/set position of object
    vector2 get_position() const;
    void set_position(const vector2& position);
    //getter/setter to get the width & height of the glRect takes vector2 for width and height
    vector2 get_size() const;
    void set_Size(const vector2& size);
    //getter/setter for rotation of the GameObject, taking angle to set glRotatef() angle
    GLfloat get_rotation() const;
    void set_rotation(GLfloat angle);
    //set colour of a GameObject, if it needs to be updated from original from constructor
    void set_Colour(const vector3& color);

    // Check collision between objects. Since all objects being rendered are of type GameObject
    // All have m_position.x,y & m_size.x,y and so we can loop through and check their values against the value of the object
    // we wish to check if collsion has occured e.g. the player, so we pass player position & size
    bool isCollision(const vector2& position, const vector2& size) const
    {
        /* if the follwing returns true, there is collision */
        return position.x + size.x > m_position.x &&  // check if x pos + width of player > object x pos
            position.x < m_position.x + m_size.x &&  //  check if x pos of player < object x pos + width
            position.y + size.y > m_position.y &&   //   check if y pos + height of player > object y pos
            position.y < m_position.y + m_size.y;   //   check if player y pos < object y pos + height
    }

private:
    /* Class variabels to store position, size of object, rotation & colour */
    vector2 m_position;
    vector2 m_size;
    GLfloat m_rotation;
    vector3 m_color;
};

GameObject::GameObject() {}
GameObject::GameObject(GLfloat rotation, const vector2& position, const vector2& size, const vector3& color)
{
    /* set class variables to contain the values passed in when contrucutor is initialized */
    m_position = position;
    m_size = size;
    m_rotation = rotation;
    m_color = color;
}

void GameObject::draw() const {

    glPushMatrix(); //save original coordinate system 
    glColor3f(m_color.x, m_color.y, m_color.z);//set the colour of the object
    glTranslatef(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2, 0); //restore the object back to its original position
    glRotatef(m_rotation, 0, 0, 1); //rotate the object
    glTranslatef(-m_size.x / 2, -m_size.y / 2, 0); // make pos x,y the centre of the object
    glRectf(0, 0, m_size.x, m_size.y); //set the width, height of object
    glPopMatrix(); //restore coordinate system
}

vector2 GameObject::get_position() const //returns the position of the object
{
    return m_position;
}

void GameObject::set_position(const vector2& position) //sets the position of the object
{
    m_position = position;
}

vector2 GameObject::get_size() const //gets the size of the object (width, height)
{
    return m_size;
}

void GameObject::set_Size(const vector2& size) //sets the size of the object (width, height)
{
    m_size = size;
}

GLfloat GameObject::get_rotation() const //gets the rotation of the object
{
    return m_rotation;
}

void GameObject::set_rotation(GLfloat angle) //set the rotation of the object
{
    m_rotation = angle;
}

void GameObject::set_Colour(const vector3& color) //sets the colour of the object
{
    m_color = color;
}
/* GameInterface class to create objects relating to the games UI or similar */
class GameInterface
{
public:
    void GameBorder(const vector3& color); //draw a square border on the screen to define play area

    //Display text on screen
    void writeBitMapString(const vector2& position, // Writing text onto the screen
        const vector3& color, void* font, char* string);

    GLfloat get_Points(); //get the current amount of points
    void set_Points(GLfloat points); //set the current points
    void Grid(GLfloat xLine, GLfloat yLine, const vector3& LineColour); //drawing a grid on screen (menu interaction)

    GLfloat getGrid() const; // return the value of the grid to determine if it's off or on
    void set_Grid(GLfloat control); //control whether the grid is on or off

    void GameInstructions(); // print instructions into game terminal

private:
    GLfloat m_points; //stores the points in the game
    GLint m_Grid = 0.0;//determine if grid is on or off 1.0 == on 0.0 == off
};
/* Game border function */
void GameInterface::GameBorder(const vector3& color)
{
    glColor3f(color.x, color.y, color.z); //set colour of border
    glBegin(GL_LINE_LOOP); //OpenGL rendering type
    /*set coordinates for the sqaure border box using glVertex2f */
    glVertex2f(50, 50); //bottom left
    glVertex2f(50, 450); //top left
    glVertex2f(450, 450); //top right
    glVertex2f(450, 50); //bottom right
    glEnd();
}
/* Writng text to screen & positioning function */
void GameInterface::writeBitMapString(const vector2& position, 
    const vector3& color, void* font, char* string)
{
    glColor3f(color.x, color.y, color.z); //set colour of text
    glRasterPos2f(position.x, position.y); //position the text on the screen

    char* c;
    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c); //create the text on the screen
}

GLfloat GameInterface::get_Points() //returns the amount of points currently in the game
{
    return m_points;
}

void GameInterface::set_Points(GLfloat points)//sets the points in the game
{
    m_points = points;
}
/* Drawng grid through menu interaction function */
void GameInterface::Grid(GLfloat xLine, GLfloat yLine, const vector3&LineColour)
{
    if(m_Grid) //if grid is on == 1.0 -> then render the grid
    {
        for (int i = 0; i < 9; i++) //loop 9 times -> to draw 9 lines on x axis
        {
            glColor3f(LineColour.x, LineColour.y, LineColour.z); //set the colour of the grid
            glLineWidth(2.0); //set the colour of the grid
            glBegin(GL_LINES); //Set OpenGL rendering type to GL_LINES
            glVertex2f(xLine, 50); //set begin pos of the line 
            glVertex2f(xLine, 450); //set end pos of othe line
            glEnd();
            xLine += 43; //increment the x value by 43 to draw multiple lines across the x axis
        }

        for (int j = 0; j < 9; j++) //loop through 9 times to draw lines on the y axis
        {
            glColor3f(LineColour.x, LineColour.y, LineColour.z); //set the colour of the grid
            glLineWidth(2.0); //set the colour of the grid
            glBegin(GL_LINES); //Set OpenGL rendering type to GL_LINES
            glVertex2f(450, yLine); //set begin pos of the line 
            glVertex2f(50, yLine);  //set end pos of othe line
            glEnd();
            yLine += 43; //increment the y value by 43 to draw multiple lines across the x axis
        }
    }
}
void GameInterface::set_Grid(GLfloat control) //change  the variable m_Grid to control grid being on or off
{
    m_Grid = control;
}

GLfloat GameInterface::getGrid() const //get the current state of the grid -> is it on or off? (Can tell visually but for debugging can be useful)
{
    return m_Grid;
}
/* Game instructions printing to the terminal function */
void GameInterface::GameInstructions()
{
    std::cout << "Player Controls: " << std::endl;
    std::cout << "Up Arrow Key - Move player forwards " << std::endl;
    std::cout << "Down Arrow Key - Move player backwards " << std::endl;
    std::cout << "Left/Right Arrow Key - Move player left or right " << std::endl;
    std::cout << "R key to turn car " << std::endl;
    std::cout << "Right Click to choose MENU options " << std::endl;
}

std::vector<GameObject> obstacle; //Store the obstacle objects in vector which are of type GameObject
GameObject player(0,vector2(50,50), vector2(80, 20), vector3(1, 1, 1)); //Player creation/properties set
GameObject Exit(0,vector2(370,450), vector2(80, 20), vector3(0, 1, 0)); //Exit door creation/properties set
GameObject Begin(0, vector2(50,30), vector2(80, 20), vector3(1, 0, 0)); //Begin door creation/properties set
GameInterface GameUI; //Make an instance of GameInterface called GameUI so we can change UI components throughout the code/add/remove


GLsizei windowHeight = 500, windowWidth = 500; //window width, height for the game
GLfloat moveSpeed = 20.0f; //player movementSpeed

/* Reshape function to set viewport/ set viewport scaling */
void reshape(int width, int height)
{
    if (height == 0) //make sure the window height is not 0 -> This can lead game to break if height == 0
        height = 1;

    glMatrixMode(GL_PROJECTION); //Using GL_PROJECTION so we can adjust camera to view the game
    glViewport(0, 0, width, height); //set the viewport size, 0-500 on x-axis and then 0-500 on y-axis changed from normalized coordinates

    glLoadIdentity();//Load identity matrix

    /* Ensure content scales correctly on the screen and objects are not stretched */
    if (width <= height)
        //ortho sets the world coordinate system
        glOrtho(0.0f, windowWidth, 0.0f, windowHeight * height / width, 1.0, -1.0);
    else
        glOrtho(0.0f, windowWidth * width / height, 0.0f, windowHeight, 1.0, -1.0);

    /* Model view allows for object transformation */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* SpecialKeys to handle Player movement input */
void specialKeys(int key, int x, int y)
{
    vector2 PlayerLocation = player.get_position(); //get the current position of the player

    if (key == GLUT_KEY_UP && PlayerLocation.y <= 430) //check if up arrow key is pressed + player pos is <= 430
    {
        player.set_position(player.get_position() + vector2(0, moveSpeed));//set thep player position to the current position + movementSpeed -> moving the player up in y axis
    }
    else if (key == GLUT_KEY_DOWN && PlayerLocation.y >= 70)//check if down arrow key is pressed + player pos is >= 70
    {
        player.set_position(player.get_position() - vector2(0, moveSpeed));//set the player position to the current position - movementSpeed -> moving the player down in y axis
    }

    else if (key == GLUT_KEY_LEFT && PlayerLocation.x >= 70)//check if left arrow key is pressed + player pos is >= 70
    {
        player.set_position(player.get_position() - vector2(moveSpeed, 0));//set player position to current position - moveSpeed on x axis, moving player to the left
    }

    else if (key == GLUT_KEY_RIGHT && PlayerLocation.x <= 350)//check if right arrow key is pressed + player pos is <= 350
    {
        player.set_position(player.get_position() + vector2(moveSpeed, 0));//set player position to current position + moveSpeed on x axis, mmoving player to the right
    }
    //Loop obstacle vector, check position of objects against player position
    for (int i = 0; i < obstacle.size(); i++) {
        if (obstacle[i].isCollision(player.get_position(), player.get_size())) { //check if player collides with any obstacle
            GameUI.set_Points(GameUI.get_Points() - 10.0f);//if collision, set the points to the current points - 10.0f
        }
    }
    //Check if player has collided with exit door
    if (Exit.isCollision(player.get_position(), player.get_size())) 
    {
        //Reload game components if true
        GameUI.set_Points(GameUI.get_Points() + 10.0f); //set points to current points + 10
        player.set_position(vector2(50, 50));//set position of player back to the beginning
        int max = 400; //define max for random
        int min = 100; //define min for random
        int range = (max - min) + 1; //set the range between max random and min random
        for (int i = 0; i < obstacle.size(); i++) //loop through obstacle vector
        {
            //generate random number between 0.1-1 for the colours
            GLfloat r = rand() % (2) + 0.1; 
            GLfloat g = rand() % (2) + 0.1;
            GLfloat b = rand() % (2) + 0.1;
            obstacle[i].set_position(vector2(rand() % range + min, rand() % range + min));//set the position of each obstacle in vector to random
            obstacle[i].set_Colour(vector3(r, g, b)); //set the colour of each obstacle inside vector to a random colour
        }
        std::cout << "** You have Won the level **" << std::endl; // print to the console, you have won if player makes to exit
    }
    //check game points
    if (GameUI.get_Points() <= 0) //check if the current amount of game points is <= to 0
    {
        player.set_position(vector2(50, 50)); //if so, game is lost and player is reset to beginning
        GameUI.set_Points(60.0); //reset the points back to 60
    }
    glutPostRedisplay();
}
/* Check player keyboard input for rotation/quit game */
void keyboard(unsigned char key, int x, int y)
{
    //switch case to see which key was pressed, switch the case according to the key
    switch (key)
    {
    case 'r': //set player rotation if r pressed
        player.set_rotation(player.get_rotation() + 90.0f); //set rotation to current rotation + 90
        break;
    case 'q': //quit game if q pressed
        std::cout << "You quit the game" << std::endl; //print message to console letting user know, the game has been exited
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
/* Check if GridMenu, checking if Grid is to be turned On or Off */
void GridMenu(GLint id)
{
    if (id == 1) //check if the first menu item is selected
    {
        GameUI.set_Grid(1.0); //if selected, Grid on
    }
    else //if not first menu item
    {
        GameUI.set_Grid(0.0); //Grid off
    }
    glutPostRedisplay();
}

/* Dislay function for rendering all OpenGL components */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glMatrixMode(GL_MODELVIEW); //set model view to allow us to transform objects
    glLoadIdentity();//identity matrix needed for object transformation

    /* Render 6 obstacles from obstacles vector */
    for (int i = 0; i < obstacle.size(); i++)
    {
        obstacle[i].draw(); //call draw function for object obstacle. All obstacles are of type GameObject which has the draw function
    }


    /* Rendering game components */
    GameUI.GameBorder(vector3(1, 1, 1)); //display the game border
    GameUI.Grid(80.0f, 80.0f, vector3(1,1,1));//display the grid
    player.draw();//draw rhe player
    Exit.draw();//draw the exit door
    Begin.draw();//draw the starting door

    /* Score */
    char text[32]; //new text variable of type char to store text
    sprintf_s(text, "Score: %.0f", GameUI.get_Points()); //takes the char variable, the format of the text and our current points
    GameUI.writeBitMapString(vector2(55,455), vector3(1,1,1),GLUT_BITMAP_HELVETICA_18, text);//create bitmapString using our text variable to display points

    glutSwapBuffers(); //swap buffer, swaps between buffers to keep window up to date with drawing/events 
    glFlush();
}
/* init function to clear current window */
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);   //clear the window
    glClear(GL_COLOR_BUFFER_BIT); //glClear with GL_COLOR_BUFFER_BIT to specifiy we will be using colour
}

int main(int argc, char** argv)
{
    GameUI.GameInstructions(); //run the game insturctions function
    GameUI.set_Points(60.0f); //set the points of the game

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);//set window size
	glutInitWindowPosition(100, 100);
	glutCreateWindow("2DCarGame");//set window title
	init();
	glutDisplayFunc(display);//call our display function here for rendering
	glutReshapeFunc(reshape);//call reshape function to scale window correctly
    glutSpecialFunc(specialKeys);//register specialkeys so window understand arrow key input
    glutKeyboardFunc(keyboard);//register keyboard input so that window understands keyboard input

    // Add menu 
    glutCreateMenu(GridMenu);//init the menu, passing our function GridMenu as the menu to display
    glutAddMenuEntry("Grid on", 1); //Set text and option in the menu
    glutAddMenuEntry("Grid off",2);//Set text and option in the menu
    glutAttachMenu(GLUT_RIGHT_BUTTON);//map the menu to the right mouse button

    //set max and min x,y pos values
    srand(time(0));
    int max = 400;
    int min = 100;
    int range = (max - min) + 1;
    
    // Push back 6 GameObject objects into obstacle vector with random x, y pos between 100-400 and colours
    srand(time(NULL));
    for (int i = 0; i < 6; i++)
    {
        /* Randomly generate r,g,b colours from 0.1-1*/
        GLfloat r = rand() % (2) + 0.1;
        GLfloat g = rand() % (2) + 0.1;
        GLfloat b = rand() % (2) + 0.1;
        obstacle.push_back(GameObject(0,
            vector2(rand()%range + min, rand() % range + min),
            vector2(50,50),vector3(r,g,b)));
    }
	glutMainLoop();
	return 0;
}

