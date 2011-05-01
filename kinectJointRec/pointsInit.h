/*
 * File:   pointsInit.h
 * Author: COMPUTER VISION
 *
 */

#ifndef POINTSINIT_H
#define	POINTSINIT_H


GLubyte indices_cube[6][4] = {
	{0,1,2,3},	// back
	{0,3,4,5},	// right
	{0,5,6,1},	// top
	{1,6,7,2},	// left
	{7,4,3,2},	// bottom
	{4,7,6,5}		// front
};


/***********************************************************
 *	BODY POINTS
 ***********************************************************/

GLfloat vertices_robody[8][3] = {
	{(2),(2),(-2)},
	{(-2),(2),(-2)},
	{(-2),(-2),(-2)},
	{(2),(-2),(-2)},
	{(2),(-2),(2)},
	{(2),(2),(2)},
	{(-2),(2),(2)},
	{(-2),(-2),(2)}
};

GLfloat color_robody[6][3] = {
	{1.0f, 0.0f, 0.0f},	// back		red
	{1.0f, 1.0f, 0.0f},	// right
	{0.0f, 1.0f, 1.0f},	// top
	{0.0f, 1.0f, 0.0f},	// left
	{0.0f, 0.0f, 1.0f},	// bottom
	{1.0f, 0.0f, 0.0f}	// front
};

/***********************************************************
 *	HEAD POINTS
 ***********************************************************/

GLfloat vertices_rohead[8][3] = {
	{(1),(1),(2)},
	{(-1),(1),(2)},
	{(-1),(-1),(2)},
	{(1),(-1),(2)},
	{(1),(-1),(4)},
	{(1),(1),(4)},
	{(-1),(1),(4)},
	{(-1),(-1),(4)}
};

GLfloat color_rohead[6][3] = {
	{0.0f, 0.0f, 1.0f},	// back		blue
	{1.0f, 1.0f, 0.0f},	// right
	{0.0f, 1.0f, 1.0f},	// top
	{0.0f, 1.0f, 0.0f},	// left
	{0.0f, 0.0f, 1.0f},	// bottom
	{1.0f, 0.0f, 0.0f}	// front
};

/***********************************************************
 *	LEFT ARM POINTS
 ***********************************************************/

GLfloat vertices_roleftarm[8][3] = {
	{(-4),(2),(0)},
	{(-4),(1),(0)},
	{(-2),(1),(0)},
	{(-2),(2),(0)},
	{(-2),(2),(1)},
	{(-4),(2),(1)},
	{(-4),(1),(1)},
	{(-2),(1),(1)}
};

GLfloat color_roleftarm[6][3] = {
	{0.0f, 1.0f, 0.0f},	// back		green
	{1.0f, 1.0f, 0.0f},	// right
	{0.0f, 1.0f, 1.0f},	// top
	{0.0f, 1.0f, 0.0f},	// left
	{0.0f, 0.0f, 1.0f},	// bottom
	{1.0f, 0.0f, 0.0f}	// front		
};

/***********************************************************
 *	RIGHT ARM POINTS
 ***********************************************************/

GLfloat vertices_rorightarm[8][3] = {
	{(-4),(-2),(0)},
	{(-4),(-1),(0)},
	{(-2),(-1),(0)},
	{(-2),(-2),(0)},
	{(-2),(-2),(1)},
	{(-4),(-2),(1)},
	{(-4),(-1),(1)},
	{(-2),(-1),(1)}
};

GLfloat color_rorightarm[6][3] = {
	{0.0f, 1.0f, 0.0f},	// back		green
	{1.0f, 1.0f, 0.0f},	// right		
	{0.0f, 1.0f, 1.0f},	// top		
	{0.0f, 1.0f, 0.0f},	// left		
	{0.0f, 0.0f, 1.0f},	// bottom		
	{1.0f, 0.0f, 0.0f}	// front		
};

#endif	/* POINTSINIT_H */