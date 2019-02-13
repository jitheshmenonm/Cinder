#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "Line.h"
#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;


vec2 mouseLocation(300,200);//target  to start
vector<CLine> Lines = { CLine(vec2(400.0, 600.0), vec2(450.0, 550.0)),
						CLine(vec2(450.0, 550.0), vec2(500.0, 500.0)),
						CLine(vec2(500.0, 500.0), vec2(600.0, 500.0)),
						CLine(vec2(600.0, 500.0), vec2(700.0, 400.0)),
						CLine(vec2(700.0, 400.0), vec2(750.0, 400.0)) };//initial lines to start
vector<CLine> LinesBackward;


class CinderProjectApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void mouseMove(MouseEvent event) override;
	static void prepareSettings(Settings *settings);

	//////////////////////
	void UpdateLines();
	void DoBackwardKinematics();
	void DoForwardKinematics();	
};

void CinderProjectApp::setup()
{
}
void CinderProjectApp::mouseMove(MouseEvent event)
{
	mouseLocation = event.getPos();
	UpdateLines();
}

void CinderProjectApp::mouseDown( MouseEvent event )
{
}

void CinderProjectApp::update()
{
}

void CinderProjectApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize(800, 600);
	settings->setFrameRate(60.0f);
}

void CinderProjectApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	for (auto l : Lines)
			l.Draw();	

	if (!Lines.empty())
	{
		gl::color(1, 0, 0); // set color to red
		gl::drawSolidCircle(Lines.back().m_ptEnd, 2.0);
	}
}

CINDER_APP(CinderProjectApp, RendererGl, &CinderProjectApp::prepareSettings)

/////////////////////////////////////////////////
void CinderProjectApp::UpdateLines()
{
	DoBackwardKinematics();
	DoForwardKinematics();
}

void CinderProjectApp::DoBackwardKinematics()
{
	//Fill up the backward Lines
	LinesBackward.clear();
	vec2 ptCurrentTarget = mouseLocation;
	for (auto itr = Lines.rbegin(); itr != Lines.rend(); itr++)
	{
		CLine lineNew = CLine(ptCurrentTarget, itr->m_ptStart);
		vec2 ptStartNew = lineNew.PointOnLineAtDistance(itr->m_Length);
		LinesBackward.emplace_back(CLine(ptStartNew, ptCurrentTarget));
		ptCurrentTarget = ptStartNew;
	}			
}

void CinderProjectApp::DoForwardKinematics()
{
	//use the LineBackwards to update the Lines
	assert(Lines.size() == LinesBackward.size());
	vec2 ptCurrentStart = Lines[0].m_ptStart;
	auto itrLineBck = LinesBackward.rbegin();
	auto itrLine = Lines.begin();
	for (; itrLineBck != LinesBackward.rend() && itrLine != Lines.end(); itrLine++,itrLineBck++)
	{
		if(itrLine!=Lines.begin())//from second line onwards update the start ppoint as well
			itrLine->m_ptStart = ptCurrentStart;
		CLine lineNew = CLine(ptCurrentStart, itrLineBck->m_ptEnd);
		ptCurrentStart = lineNew.PointOnLineAtDistance(itrLine->m_Length);
		itrLine->m_ptEnd = ptCurrentStart;//recalculate length not needed		
	}
}
