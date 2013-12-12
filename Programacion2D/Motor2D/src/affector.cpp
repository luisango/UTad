#include "../include/affector.h"
#include "../include/particle.h"	

Affector::Affector( double boundx0, double boundy0, double boundx1, double boundy1 ) : boundx0( boundx0 ), boundy0( boundy0 ), boundx1( boundx1 ), boundy1( boundy1 )
{
	minr = ming = minb = 0;
	maxr = maxg = maxb = 0;
	minvelx = maxvelx = 0;
	minvely = maxvely = 0;
	minangvel = maxangvel = 0;
	mode = ALL_DESACTIVE;
}
	

	
void Affector::AddParticles( Particle* particles )
{
	bool insideBounding = false;

	if( particles->GetX() >= boundx0 && particles->GetX() <=  boundx1 &&
	particles->GetY() >= boundy0 && particles->GetY() <=  boundy1 )
	{
		insideBounding = true;
	}


	for( unsigned int i = 0; i < particlesModified.Size() && insideBounding; i++ )
	{
		if( particles == particlesModified[i] )
		{
			insideBounding = false;
		}
	}

	if( insideBounding )
	{
		particlesModified.Add( particles );
		Apply( particles );
	}
}


void Affector::Apply( Particle* particle)
{	
	if( mode != 0 )
	{
		if( mode & VELX_ACTIVE == VELX_ACTIVE )
		{
			particle->SetVelocityX( RangeRand(minvelx, maxvelx ) );
		}
		
		if( mode & VELY_ACTIVE == VELY_ACTIVE )
		{
			particle->SetVelocityY( RangeRand(minvely, maxvely ) );
		}

		if( mode & VELANG_ACTIVE == VELANG_ACTIVE )
		{
			particle->SetVelocityAngle( RangeRand(minangvel, maxangvel ) );
		}
		if( mode & COLOR_ACTIVE == COLOR_ACTIVE )
		{
			particle->SetColor( RangeRand(minr, maxr ), RangeRand(ming, maxg ), RangeRand(minb, maxb ), particle->GetAlpha() );
		}
	}
}



void Affector::DeleteParticles( Particle* particle)
{
	particlesModified.Remove( particle );
}


int32 Affector::RangeRand ( int32 minRange, int32 maxRange )
{
	int32 range = maxRange - minRange;
	int32 randomValue = 0;
	if( range != 0 )
	{
		randomValue = rand() % range;
	}
	
	randomValue += minRange;

	return randomValue;
}