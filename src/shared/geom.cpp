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

#include <ode/ode.h>
#include "geom.hpp"
#include "printlog.hpp"
#include "track.hpp"
#include "internal.hpp"
#include "../events/event_lists.hpp"
#include "../loaders/conf.hpp"

Geom *Geom::head = NULL;

//allocates a new geom data, returns its pointer (and uppdate its object's count),
//ads it to the component list, and ads the data to specified geom (assumed)
Geom::Geom (dGeomID geom, Object *obj): Component(obj) //pass object argument to base class constructor
{
	printlog(2, "configuring Geom class");

	//increase object activity counter
	object_parent->Increase_Activity();

	//parent object
	if (obj->selected_space)
	{
		printlog(2, "(adding to selected space)");
		dSpaceAdd (obj->selected_space, geom);
	}
	else //add geom to global space
	{
		dSpaceAdd (space, geom);
	}

	//add it to the geom list
	next = Geom::head;
	Geom::head = this;
	prev = NULL;

	//one more geom after this
	if (next)
		next->prev = this;
	else
		printlog(2, "(first registered)");

	//add it to the geom
	dGeomSetData (geom, (void*)(Geom*)(this));
	geom_id = geom;

	//now lets set some default values...
	//event processing (triggering):
	colliding = false; //no collision event yet
	
	f_3d = NULL; //default, isn't rendered
	//collision contactpoint data
	mu = internal.mu;
	mu_rim = internal.mu;
	erp = internal.erp;
	cfm = internal.cfm;
	slip = internal.slip;

	wheel = false; //not a wheel
	bounce = 0.0; //no bouncyness

	collide = true; //on collision, create opposing forces

	//events:
	//for force handling (disable)
	buffer_event=false;
	force_to_body=NULL; //when true, points at wanted body
	sensor_event=false;

	//debug variables
	flipper_geom = 0;
	TMP_pillar_geom =false; //not a demo pillar geom
}
//destroys a geom, and removes it from the list
Geom::~Geom ()
{
	//lets just hope the given pointer is ok...
	printlog(2, "clearing Geom class");

	//remove all events
	Buffer_Event_List::Remove(this);
	Sensor_Event_List::Remove(this);

	//1: remove it from the list
	if (!prev) //head in list, change head pointer
	{
		printlog(2, "(geom is head)");
		Geom::head = next;
	}
	else //not head in list, got a previous link to update
		prev->next = next;

	if (next) //not last link in list
		next->prev = prev;
	else
		printlog(2, "(geom is last)");

	dGeomDestroy(geom_id);

	//decrease activity and check if 0
	object_parent->Decrease_Activity();
}

