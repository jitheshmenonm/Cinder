#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "CBoid.h"
#include "CQuad.h"

using namespace ci;
using namespace ci::app;
using namespace std;

//Not rebuild Quadtree every frame but move the objects to the correct nodes
//Only make quads when required(already doing in py)
//Quadtree node factory using pools
//nodes of two types one for leaf carrying data and other just a pointer
//Use linkedlist for immediate neighbours instead of quadtree

#define WINDOWSIZEX 800
#define WINDOWSIZEY 600

#define OBSTACLECIRCLERADIUS HALFBOIDSIZE

bool bSeekTarget = false;
static vec2 globalTarget = vec2(800.0,600.0);
std::vector<CBoid> boids;
std::vector<vec2> obstacleCircles;

///Quad tree functions
extern void BuildQuadTree(CBoid* ptrRoot);
extern void DestroyQuadTree(CBoid* ptrRoot);
extern void UpdateQuadTree(std::vector<CBoid>& boids);
CQuad * ptrRoot;
///

static bool blogtofile = false;
void Logtofile(std::string name, float value)
{
	if (blogtofile)
	{
		std::ofstream myfile;
		myfile.open("D:\\TEMP\\boid.txt", std::ios::out | std::ios::app);
		myfile << name << value << std::endl;
		myfile.close();
	}
}

bool double_equals(double a, double b)
{
	double epsilon = 0.001;
	return std::abs(a - b) < epsilon;
}

void DrawBoidAndObstacles()
{
	gl::color(Color(0, 0, 1)); // blue
	for (auto boid : boids)
		boid.Draw();
	gl::color(Color(1, 0, 0)); // red
	for (auto oc : obstacleCircles)
		gl::drawSolidCircle(vec2(oc.x,oc.y), OBSTACLECIRCLERADIUS);
	
}
void UpdateBoidPositions()
{
	for (auto & boid : boids)
	{
		if(bSeekTarget)
			boid.Seek(globalTarget);
		boid.UpdateLocation();
	}		
}

class CinderProjectApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void mouseMove( MouseEvent event) override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;
};

void CinderProjectApp::setup()
{
	setWindowPos(100, 100);
	setWindowSize(WINDOWSIZEX, WINDOWSIZEY);
	ptrRoot = new CQuad(glm::vec2(0.0, 0.0), glm::vec2((float)WINDOWSIZEX, (float)WINDOWSIZEY));
	ptrRoot->SetRoot();
}

void CinderProjectApp::mouseDown( MouseEvent event )
{
	if (event.isLeftDown())
		obstacleCircles.emplace_back((float)event.getX(), (float)event.getY());
		
	if (event.isRightDown())
		globalTarget = vec2((float)event.getX(), (float)event.getY());
	if (event.isMiddleDown())
		boids.clear();			
}
void CinderProjectApp::keyDown(KeyEvent event)
{
	float min = 100.0;
	float max = WINDOWSIZEX-100.0f;
	float xCenter = static_cast <float> (min + (rand() % static_cast<int>(max - min + 1)));
	min = 100.0;max = WINDOWSIZEY-100.0;
	float yCenter = static_cast <float> (min + (rand() % static_cast<int>(max - min + 1)));

	if (event.getChar() == 'a')
		boids.emplace_back(xCenter, yCenter);
	else if (event.getChar() == 's')
		bSeekTarget = true;
}
void CinderProjectApp::mouseMove(MouseEvent event)
{
	if(bSeekTarget)
		globalTarget= vec2((float)event.getX(), (float)event.getY());
}

void CinderProjectApp::update()
{
	UpdateBoidPositions();
	UpdateQuadTree(boids);
}

void CinderProjectApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	DrawBoidAndObstacles();
}

CINDER_APP( CinderProjectApp, RendererGl )