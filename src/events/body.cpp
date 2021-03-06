/*
 * RCX Copyright (C) 2009-2010 Mats Wahlberg ("Slinger")
 *
 * This program comes with ABSOLUTELY NO WARRANTY!
 *
 * This is free software, and you are welcome to
 * redistribute it under certain conditions.
 *
 * See license.txt and README for more info
 */

#include <SDL/SDL.h>
#include <ode/ode.h>
#include "../shared/body.hpp"
#include "event_lists.hpp"
#include "../shared/printlog.hpp"

void Body::TMP_Events_Step(Uint32 step)
{
	Body *body;
	Geom *geom, *next;

	while (Buffer_Event_List::Get_Event(&body))
	{
		//first of all, remove all connected (to this body) geoms:
		//ok, this is _really_ uggly...
		//ode lacks a "dBodyGetGeom" routine (why?! it's easy to implement!)...
		//solution: loop through all geoms remove all with "force_to_body"==this body
		for (geom=Geom::head; geom; geom=next)
		{
			next=geom->next; //needs this after possibly destroying the geom (avoid segfault)
			if (geom->force_to_body == body)
				delete geom;
		}

		delete body;
	}
}
