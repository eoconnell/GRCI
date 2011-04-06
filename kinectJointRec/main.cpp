/*****************************************************************************
*                                                                            *
*  OpenNI 1.0 Alpha                                                          *
*  Copyright (C) 2010 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  OpenNI is free software: you can redistribute it and/or modify            *
*  it under the terms of the GNU Lesser General Public License as published  *
*  by the Free Software Foundation, either version 3 of the License, or      *
*  (at your option) any later version.                                       *
*                                                                            *
*  OpenNI is distributed in the hope that it will be useful,                 *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
*  GNU Lesser General Public License for more details.                       *
*                                                                            *
*  You should have received a copy of the GNU Lesser General Public License  *
*  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.            *
*                                                                            *
*****************************************************************************/


/**
 * Joint Recognition
 *
 * Changes:
 *	* Status updates have been removed to improve readability (PR)
 *	* Added comment blocks for all functions (EO)
 *
 * @project     kinectJointRec
 * @author      CIS 3287 - Computer Vision team
 * @link        http://openni.ninesixweb.com
 */


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include "SceneDrawer.h"
#include <fstream>
#include<stdio.h>
#include<iostream>
//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
xn::Context g_Context;// represents an OpenNi Context Object
xn::DepthGenerator g_DepthGenerator;
xn::UserGenerator g_UserGenerator;
xn::ImageGenerator g_ImageGenerator;
xn::Recorder recorder;
std::ofstream outpnt;

XnBool g_bNeedPose = FALSE;
XnChar g_strPose[20] = "";
XnBool g_bDrawBackground = TRUE;
XnBool g_bDrawPixels = TRUE;
XnBool g_bDrawSkeleton = TRUE;
XnBool g_bPrintID = TRUE;
XnBool g_bPrintState = TRUE;
XnBool g_runfile = TRUE; // Use mock node(T) or kinect(F)
XnBool g_Record = FALSE; // Record a new .oni file(T)


#include <GL/glut.h>

#define GL_WIN_SIZE_X 720
#define GL_WIN_SIZE_Y 480

XnBool g_bPause = false;
XnBool g_bRecord = false;

XnBool g_bQuit = false;

//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------

void CleanupExit()
{
	g_Context.Shutdown();
	rename("recording.tmp.oni", "recording.oni");
	exit (1);
}

/**
 * User_NewUser() - Callback Function
 *
 * Detects the presense of a new user.
 *
 * @param generator
 * @param nId			XnUserID	Identification for the user.
 * @param pCookie
 */

void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("New User %d\n", nId);
	// New user found
	if (g_bNeedPose)
	{
		/**
		 * GetPoseDetectionCap()
		 * 
		 * Gets a PoseDetectionCapability object for accessing
		 * Pose-Detection functionality (allows a user generator to
		 * recognize when a user is in a specific pose)
           */
		
		// starts detection for specified pose and the user id
          g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
          printf("**** looking for pose named: %s\n",g_strPose);
     }
	else
	{
		g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
	}
}

/**
 * User_LostUser() - Callback Fuction
 *
 * Code to execute when a user is lost.
 *
 * @param generator
 * @param nId			XnUserID	Identification for the user.
 * @param pCookie
 */

void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("Lost user %d\n", nId);
}

/**
 * UserPose_PoseDetected() - Callback Function
 *
 * Executes when a pose is detected by OpenNI.
 *
 * @param capability
 * @param strPose		XnChar*	Name of the detected pose.
 * @param nId			XnUserID	User who performed the detected pose.
 * @param pCookie
 */

void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& capability, 
								    const XnChar* strPose,
								    XnUserID nId, void* pCookie)
{
	printf("Pose %s detected for user %d\n", strPose, nId);
	g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
	g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

/**
 * UserCalibration_CalibrationStart() - Calback Function
 *
 * Started Calibration.
 * TODO: Get this to be automatic when a user enters.
 *
 * @param capability
 * @param nId			XnUserID	User Identification.
 * @param pCookie
 */

void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, 
											XnUserID nId,
											void* pCookie)
{
	printf("Calibration started for user %d\n", nId);
}

/**
 * UserCalibration_CalibrationEnd() - Callback Function
 *
 * If calibrated it places a skeleton on the user,
 * otherwise it starts looking for poses again.
 *
 * @param capability
 * @param nId			XnUserID	User Identification.
 * @param bSuccess		XnBool	True if calibration succeeded.
 * @param pCookie
 */
void XN_CALLBACK_TYPE UserCalibration_CalibrationEnd(xn::SkeletonCapability& capability,
										   XnUserID nId,
										   XnBool bSuccess,
										   void* pCookie)
{
	if (bSuccess)
	{
		// Calibration succeeded
		printf("Calibration complete, start tracking user %d\n", nId);
		g_UserGenerator.GetSkeletonCap().StartTracking(nId);
	}
	else
	{
		// Calibration failed
		printf("Calibration failed for user %d\n", nId);
		if (g_bNeedPose)
		{
			g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
		}
		else
		{
			g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
		}
	}
}

/**
 * glutDisplay()
 *
 * This gets passed to glut for displaying each frame.
 *
 * @param void
 */

void glutDisplay (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the OpenGL viewpoint
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	xn::SceneMetaData sceneMD;//Represents a MetaData object for generators producing scene analysis maps
	xn::DepthMetaData depthMD;
	g_DepthGenerator.GetMetaData(depthMD);
	glOrtho(0, depthMD.XRes(), depthMD.YRes(), 0, -1.0, 1.0);

	glDisable(GL_TEXTURE_2D);

	if (!g_bPause)
	{
		// Read next available data
		g_Context.WaitAnyUpdateAll();
		if(g_Record)
			recorder.Record();
		}

		// Process the data
		g_DepthGenerator.GetMetaData(depthMD);
		// Get the pixels that belong to a user
		g_UserGenerator.GetUserPixels(0, sceneMD);
		DrawDepthMap(depthMD, sceneMD);

		// Swaps the buffers of the current window if double buffered.
		glutSwapBuffers();
}

/**
 * glutIdle()
 *
 * Does this only get called if nothing moves in the frame?
 *
 */
void glutIdle (void)
{
	if (g_bQuit) {
		CleanupExit();
	}

	// Display the frame
	glutPostRedisplay();
}

/**
 * glutKeyboard()
 *
 * Perform actions based off of what key the user presses.
 * This gets passed to glut to handle input.
 *
 * @param key	unsigned char		name/value of the key pressed
 * @param x	int				purpose unknown (Possibly mouse coords)
 * @param y	int				purpose unknown
 */

void glutKeyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
			// Escape
			CleanupExit();
		case 'b':
			// Draw background?
			g_bDrawBackground = !g_bDrawBackground;
			break;
		case 'x':
			// Draw pixels at all?
			g_bDrawPixels = !g_bDrawPixels;
			break;
		case 's':
			// Draw Skeleton?
			g_bDrawSkeleton = !g_bDrawSkeleton;
			break;
		case 'i':
			// Print label?
			g_bPrintID = !g_bPrintID;
			break;
		case 'l':
			// Print ID & state as label, or only ID?
			g_bPrintState = !g_bPrintState;
			break;
		case'p':
			g_bPause = !g_bPause;
			break;
		case'r':
			g_bRecord = !g_bRecord;
			if(!g_bRecord)
				outpnt<<"$\n";
				printf("Record on/off\n");
			break;
	}
} // end glutKeyboard

/**
 * glInit()
 *
 * Configuration for glut.
 *
 * @param pargc	int*		points to the number of command line args.
 * @param argv		char**	points to the command line args.
 */

void glInit (int * pargc, char ** argv)
{
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	glutCreateWindow ("Prime Sense User Tracker Viewer");
	//glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

#define SAMPLE_XML_PATH "KinConfig.xml"

#define CHECK_RC(nRetVal, what)										\
	if (nRetVal != XN_STATUS_OK)									\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));\
		return nRetVal;												\
	}

int main(int argc, char **argv)
{
	// Handles to a registered callback function
	XnCallbackHandle hUserCallbacks, hCalibrationCallbacks, hPoseCallbacks;

	outpnt.open("kindat"); // openfile to save data
	std::cout<<"\n\n\nKEYBOARD TOGGLE:(focus must be on GL window)\n b-background \n "
			 "x-draw all pixels \n s-Skeleton \n i-print labels \n l- print state \n "
			 "p-pause \n r-record data points\n";

	// must be called before any OpenNI function ( except xnInitFromXmlFile() )
	g_Context.Init();

	if(g_runfile){
		// Uses an xml file to set config of nodes
		//g_Context.InitFromXmlFile("KinConfig.xml");
		// Playing sample recording. Replace file name to any file to play.
		//g_Context.OpenFileRecording("SkeletonRec.oni");
		g_Context.OpenFileRecording("PRrec4w.oni");
	}

	g_DepthGenerator.Create(g_Context);	// creates a depth generator
	g_UserGenerator.Create(g_Context);		// creates a user generator
	g_ImageGenerator.Create(g_Context);	// creates a image generator

	// Returns the first found existing node of the specified type(pointer to context. ,type,handle to node)
	g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
	g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
	g_Context.FindExistingNode(XN_NODE_TYPE_IMAGE,g_ImageGenerator);
        
	if(g_Record){
		recorder.Create(g_Context);	// creates a recorder node
		recorder.SetDestination(XN_RECORD_MEDIUM_FILE,"recording.tmp.oni");
		recorder.AddNodeToRecording(g_DepthGenerator);
		//recorder.AddNodeToRecording(g_ImageGenerator);
	}

	/**
	 * Register____Callbacks()
	 *
	 * For your information.
	 *
      * @param detection start	XnCallbackHandle
      * @param detection end		XnCallbackHandle
	 * @param null				What the hell is a pCookie?
	 * @param callback handle	XnCallbackHandle
      */
	g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);//registers to user callbacks which are defined above
	g_UserGenerator.GetSkeletonCap().RegisterCalibrationCallbacks(UserCalibration_CalibrationStart, UserCalibration_CalibrationEnd, NULL, hCalibrationCallbacks);

	if (g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
	{
		g_bNeedPose = TRUE;
		if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
		{
			printf("Pose required, but not supported\n");
			return 1;
		}
		g_UserGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks(UserPose_PoseDetected, NULL, NULL, hPoseCallbacks);
		g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
	}

	g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
	g_Context.StartGeneratingAll();//Make sure all generators are generating data.
	

	glInit(&argc, argv);
	glutMainLoop();
        
	outpnt.close();
        
}
