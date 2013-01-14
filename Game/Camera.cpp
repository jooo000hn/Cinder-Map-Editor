#include "Camera.h"

#include "cinder/app/App.h"

using namespace ci;
using namespace ci::app;

using namespace ly;

ly::Camera* ly::Camera::sInstance = NULL;

ly::Camera* ly::Camera::get()
{
	if ( !sInstance ) sInstance = new ly::Camera();
	return sInstance;
}

void ly::Camera::setZoom( float zoom )
{
	mBody.position.z = zoom;
}

void ly::Camera::setAngle( float angleX )
{
	mPivot.rotation.x = angleX;
}

void ly::Camera::setFov( float fov )
{
	mFov = fov;
	mCinderCamera.setPerspective( mFov, 1024.0f/768.0f, 1.0f, 4000.0f );
}

ci::Ray ly::Camera::rayIntoScene( ci::Vec2i screenPoint )
{
	float windowWidth = 1024;
	float windowHeight = 768;
	float aspect = windowWidth / windowHeight;
	float u = screenPoint.x / windowWidth;
	float v = screenPoint.y / windowHeight;
	Ray ray = mCinderCamera.generateRay(u , 1.0f - v, aspect );
	return ray;
}

ly::Camera::Camera()
{
	mFov = 45.0f;
	mCinderCamera.setPerspective( mFov, 1024.0f/768.0f, 1.0f, 4000.0f );
	
	mPivot.rotation.x	= -13.0f;
	mBody.position.z	= 30.0f;
	
	mBody.parent = &mPivot;
	mPivot.parent = this;
}

ly::Camera::~Camera() {}

ci::Vec3f ly::Camera::globalPosition()
{
	return mBody.globalPosition();
}

void ly::Camera::update( const float deltaTime )
{	
	Node::update( deltaTime );
	mPivot.update( deltaTime );
	mBody.update( deltaTime );
	
	mCinderCamera.setWorldUp( Vec3f::yAxis() );
	mCinderCamera.setEyePoint( mBody.globalPosition() );
	mCinderCamera.setOrientation( Quatf( mBody.transform() ) );
	
}