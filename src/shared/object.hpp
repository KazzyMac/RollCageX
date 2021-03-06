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

#ifndef _RCX_OBJECT_H
#define _RCX_OBJECT_H
#include <ode/common.h>

#include "racetime_data.hpp"
#include "file_3d.hpp"
#include "script.hpp"
#include "component.hpp"
#include "space.hpp"

//object: one "thing" on the track, from a complex building to a tree, spawning
//will be controlled by a custom scripting langue in future versions, the most
//important role of "object" is to store the ode space for the spawned object
//(keeps track of the geoms in ode that describes the components) and joint
//group (for cleaning up object)

//template for spawning
class Object_Template:public Racetime_Data
{
	public:
		static Object_Template *Load(const char *path);
		void Spawn(dReal x, dReal y, dReal z);

	private:
		Object_Template(const char*); //just set some default values
		//placeholder for script data, now just variables

		//script to be run when spawning object
		Script *spawn;

		//temporary graphics
		file_3d *graphics_debug1;
		file_3d *graphics_debug2;
		file_3d *graphics_debug3;

		//temporary solution
		bool box;
		bool funbox;
		bool flipper;
		bool NH4;
		bool building;
		bool sphere;
		bool pillar;
};

//can be added/removed at runtime ("racetime")
class Object
{
	public:
		virtual ~Object(); //(virtual makes sure also inherited classes calls this destructor)
		static void Destroy_All();

		//removes empty objects
		static void Events_Step();

		//for increasing/decreasing activity counter
		void Increase_Activity();
		void Decrease_Activity();
	private:
		Object();
		//the following are either using or inherited from this class
		friend class Object_Template; //needs access to constructor
		friend bool load_track (const char *);
		friend class Car;

		//things to keep track of when cleaning out object
		unsigned int activity; //counts geoms,bodies and future stuff (script timers, loops, etc)

		Component *components;
		dSpaceID selected_space;

		//to allow acces to the two above pointers
		friend class Component; //components
		friend class Geom; //selected space
		friend class Space; //selected space

		//placeholder for more data
			
		//used to find next/prev object in dynamically allocated chain
		//set next to null in last object in chain
		static Object *head;
		Object *prev, *next;
};

#endif
