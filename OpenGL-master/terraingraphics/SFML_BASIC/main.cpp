
/////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 
 
#include "SFML/Graphics.hpp" 
#include "SFML/Graphics/Shader.hpp"
#include "SFML/OpenGL.hpp" 
#include <iostream> 
  
 
#include "Terrain.h"
#include "Camera.h"




int main() 
{ 
    // Create the main window 
	bool wireframe = false;
	bool ortographic = false;
    int width=600,height=600;
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
	sf::RenderWindow App(sf::VideoMode(width, height, 32), "SFML OpenGL", sf::Style::Close, Settings);
	//sf::RenderWindow App(sf::VideoMode(width, height, 32), "SFML OpenGL"); 
    // Create a clock for measuring time elapsed     
    sf::Clock Clock; 

	aiVector3D position(0,10,-30);
	Camera camera;
    camera.Init(position); //create a camera
      
    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.3f, 0.3f, 0.6f, 0.f); //background colour
    glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); 
   
    //// Setup a perspective projection & Camera position 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
	glEnable(GL_DEPTH_TEST); // check for depth
	glEnable(GL_NORMALIZE);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_COLOR_MATERIAL);
	GLfloat ambientLight_color[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat diffuseLight_color[] = { 0.5, 0.5, 0.5, 0.5 };
	GLfloat light_position[] = { 8.0, 20.0, 0.0, 0.0 };
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight_color); // set color of diffuse component
	glLightfv(GL_LIGHT0, GL_SPECULAR, ambientLight_color); // set color of specular component
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   // set position
	///////////////////////////setting material
	GLfloat materialAmbDiff[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // create an array of RGBA values


	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiff); // set the diffuse & ambient reflection colour for the front of faces

	GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // create an array of RGBA values (White)
	GLfloat materialShininess[] = { 20.0f }; // select value between 0-128, 128=shiniest


	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular); // set the colour of specular reflection
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess); // set shininess of the material
    //set up a 3D Perspective View volume
    gluPerspective(90.f, (float)width/height, 1.f, 300.0f);//fov, aspect, zNear, zFar 
 
	sf::Texture grassTex, waterTex, snowTex;
	std::string image = "grass2.png";
	if (!grassTex.loadFromFile(image))
	{
		std::cout << "Could not load" << image;
		char c;
		std::cin >> c;
	}
	image = "water.jpg";
	if (!waterTex.loadFromFile(image))
	{
		std::cout << "Could not load" << image;
		char c;
		std::cin >> c;
	}
	image = "snow.jpg";
	if (!snowTex.loadFromFile(image))
	{
		std::cout << "Could not load" << image;
		char c;
		std::cin >> c;
	}

	//load & bind the shader
	sf::Shader shader;
	//all the lighting & texture blending code should  be put in 'fragment.glsl'
	if(!shader.loadFromFile("vertex.glsl","fragment.glsl")){
        //exit(1);
		std::cin;
    }
	shader.setParameter("grassTex", grassTex);
	shader.setParameter("waterTex", waterTex);
	shader.setParameter("snowTex", snowTex);

	//Create our Terrain
	Terrain terrain;
	terrain.Init();
	shader.setParameter("heightestPoint", terrain.tallestPoint );
    // Start game loop 
    while (App.isOpen()) 
    { 
        // Process events 
        sf::Event Event; 
        while (App.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                App.close(); 
   
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                App.close(); 

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::I)){
				wireframe = !wireframe;

			}
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::O)){
				ortographic = !ortographic;

			}
             
			//update the camera
			camera.Update(Event);
 
			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
    
		
        } 
           
        //Prepare for drawing 
        // Clear color and depth buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   
        // Apply some transformations 
        //initialise the worldview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (ortographic == true)
		{

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-30, 30, -40, 30, 300, -300);
			glMatrixMode(GL_MODELVIEW);
		

		}
		else
		{

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90.f, (float)width / height, 1.f, 300.0f);//fov, aspect, zNear, zFar 
			glMatrixMode(GL_MODELVIEW);
		}
	

	
	

		//get the viewing transform from the camera
		camera.ViewingTransform();


		//make the world spin
		//TODO:probably should remove this in final
		static float ang=0.0;
		ang+=0.01f;
		//glRotatef(ang*2,0,1,0);//spin about y-axis
		
		
		//sf::Texture::bind(&grassTex);
		//sf::Texture::bind(&snowTex);
		GLfloat light_position[] = { 8.0, 20.0, 0.0, 0.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		sf::Shader::bind(&shader);
		
		//draw the world
		terrain.Draw();
	
		   
        // Finally, display rendered frame on screen 
        App.display(); 
    } 
   
    return EXIT_SUCCESS; 
}
