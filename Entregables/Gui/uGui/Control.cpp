#include "Control.h"
#include <list>
#include <algorithm>
#include "GUIManager.h"

int Control::s_id = 1000;

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
Control::Control()
{
	m_parent					= NULL;
	m_position				= Vector2( 0, 0 );
	m_size						= Vector2( 0, 0 );	
	m_enabled					= true;
	m_visible					= true;
	m_focus						= false;
	m_pointerIsOver		= false;

	s_id++;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
bool Control::isPointInside( const Vector2& point ) const
{
	Vector2 pos = getAbsolutePosition();
	if( point.x >= pos.x && point.x <= pos.x + m_size.x &&
		point.y >= pos.y && point.y <= pos.y + m_size.y )
	{
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
Vector2 Control::getAbsolutePosition() const
{
	if( m_parent )
		return m_parent->getAbsolutePosition() + m_position;

	return m_position;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Control::processUpdate()
{
	update();

	std::list<Control*>::iterator it = m_children.begin();
	for( ; it != m_children.end(); ++it )
	{
		Control* control = *it;
		control->processUpdate();
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Control::processRender()
{
	render();

	std::list<Control*>::iterator it = m_children.begin();
	for( ; it != m_children.end(); ++it )
	{
		Control* control = *it;
		control->processRender();
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Control::processDestroy()
{
	// libera sus recursos propios
	destroy();

	// recorre hijos y los destruye
	std::list<Control*>::iterator it = m_children.begin();
	for( ; it != m_children.end(); ++it )
	{
		Control* control = *it;
		control->processDestroy();
	}

	delete this;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
bool Control::injectInput( const Message& message )
{
	bool messageHandled = false;

	// primero vemos si es para algun hijo
	std::list<Control*>::iterator it = m_children.begin();
	for( ; it != m_children.end(); ++it )
	{
		Control* control = *it;
		messageHandled = control->injectInput( message );
		if( messageHandled )
			break;
	}

	// si no lo ha procesado ning�n hijo puede ser para este control
	if( !messageHandled )
	{
		switch( message.type )
		{
		case mtPointerMove:
			{
				const MessagePointerMove* messagePointer = static_cast<const MessagePointerMove*>(&message);
				if( isPointInside( Vector2( messagePointer->x, messagePointer->y) ))
				{
					m_pointerIsOver = true;
					onInputEvent( message );
					messageHandled = true;
				}
				else
				{
					m_pointerIsOver = false;
				}
			}
			break;

		case mtPointerButtonDown:
			{
				const MessagePointerButtonDown* messagePointer = static_cast<const MessagePointerButtonDown*>(&message);
				if( isPointInside( Vector2( messagePointer->x, messagePointer->y) ))
				{
					m_pointerIsOver = true;
					onInputEvent( message );
					messageHandled = true;
				}
				else
				{
					m_pointerIsOver = false;
				}
			}
			break;

		case mtPointerButtonUp:
			{
				const MessagePointerButtonUp* messagePointer = static_cast<const MessagePointerButtonUp*>(&message);
				if( isPointInside( Vector2( messagePointer->x, messagePointer->y) ))
				{
					m_pointerIsOver = true;
					onInputEvent( message );
					messageHandled = true;
				}
				else
				{
					m_pointerIsOver = false;
				}
			}
			break;
		case mtKeyDown:
            {
                const MessageKeyDown* messagePointer = static_cast<const MessageKeyDown*>(&message);
			    onInputEvent( message );
            }
            break;
		}
	}

	return messageHandled;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Control::setParent( Control* parent )
{
	parent->addControl( this );
	m_parent = parent;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Control::setEventListener( IEventListener* eventListener )
{ 
	m_eventListeners.push_back( eventListener ); 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
string Control::getName() const
{ 
	return m_name; 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Control::setVisible( bool show )
{ 
	m_visible = show; 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
bool Control::isVisible() const
{ 
	return m_visible; 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Control::setEnabled( bool enable )
{ 
	m_enabled = enable; 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
bool Control::isEnabled() const
{ 
	return m_enabled; 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Control::setPosition( const Vector2& position )
{ 
	m_position = position; 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
Vector2 Control::getPosition() const
{ 
	return m_position; 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
Vector2 Control::getSize() const
{ 
	return m_size; 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Control::setFocus( bool focus )
{ 
	m_focus = focus; 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
bool Control::hasFocus() const
{ 
	return m_focus; 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Control::addControl( Control* control )
{ 
	m_children.push_back( control ); 
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
void Control::removeControl( Control* control )
{
	m_children.remove( control );
}

//------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------
Control* Control::findControlByName( const string& name )
{
	if( m_name == name )
		return this;

	std::list<Control*>::iterator it = m_children.begin();
	for( ; it != m_children.end(); ++it )
	{
		Control* control = *it;
		Control* foundControl = control->findControlByName( name );
		if( foundControl )
			return foundControl;
	}

	return NULL;
}
