#include "Window.h"
#include "GUIManager.h"
#include "../include/Renderer.h"
#include "../include/Image.h"

Window::Window()
{
	m_canvas					= NULL;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
bool Window::init( const std::string name, const Vector2& position, const std::string& backgroungImage )
{
	m_name					= name;
	m_position			= position;
	m_canvas				= new Image( backgroungImage.c_str() );
	m_size					= Vector2( (float)m_canvas->GetWidth(), (float)m_canvas->GetHeight() );

	return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
bool Window::init( const std::string name )
{
	m_name					= name;
	m_position			= Vector2( 0, 0 );
	m_size					= GUIManager::instance().getScreenSize();

	return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Window::update()
{

}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Window::render()
{
	if( m_canvas && m_visible )
	{
		Vector2 pos = getAbsolutePosition();

		Renderer::Instance().SetBlendMode( Renderer::ALPHA );
		Renderer::Instance().DrawImage( m_canvas, pos.x, pos.y );
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Window::onInputEvent( const Message& message )
{
	switch( message.type )
	{
	case mtKeyDown:
		{
			const MessageKeyDown* messagePointer = static_cast<const MessageKeyDown*>(&message);				
			NOTIFY_LISTENERS( onKeyDown( this, messagePointer->keyCode ) );
			break;
		}
	case mtPointerButtonUp:
			NOTIFY_LISTENERS( onClick( this ) );
			break;
	}	
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Window::destroy()
{
	if( m_canvas )
	{
		delete m_canvas;
		m_canvas = NULL;
	}
}