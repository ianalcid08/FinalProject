#ifndef _IAISTATE_H_
#define _IAISTATE_H_

/***********************************************
 * Filename:  		IAIState.h
 * Date:      		02/08/2013
 * Mod. Date: 		02/08/2013
 * Mod. Initials:	JT
 * Author:    		Joseph Thomas
 * Purpose:   		Base class for all AI states
 ************************************************/
class CEnemyObject;
class IBossEvent;

class IAIState
{
public:
	virtual ~IAIState(void) = 0 {};

	/*****************************************************************
	* void Initialize(): Initializes all data in the state.
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	virtual void Initialize() = 0;

	/*****************************************************************
	* void Shutdown(): Handles all shutdown code for exiting the state.
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	virtual void Shutdown() = 0;

	/*****************************************************************
	* void Update( float fElapsedTime ): updates the state
	*
	* Ins:			    float _fTime
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	virtual bool Update( float _fTime ) = 0;
};

#endif