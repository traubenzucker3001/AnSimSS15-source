/*
 * VirtualObject.h
 *
 *  Created on: 28.09.2015
 *      Author: Daniel
 */

#ifndef VIRTUALOBJECT_H_
#define VIRTUALOBJECT_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <CVK_2/CVK_Framework.h>

#include "RigidBody.h"

/** \brief virtual object
 *
 * connection between physical and graphical representation
 */
class VirtualObject
{
private:
	RigidBody* physicBody;		/**< RigidBody as physics component of this object */

	CVK::Node* graphicBody;		/**< CVK Geometry as graphics component of this object */

	glm::vec3 position;			/**< position */

	glm::mat4 modelMatrix;		/**< model matrix */

	int id;						/**< identification number */


public:
	/** \brief default constructor
	 *
	 * creates a virtual object instance
	 * @param posIN initial position
	 * @param bodyCount counts number of initialized bodies
	 * @param massIN body mass
	 * @param staticIN static object (true) or not (false) (not observed at the moment)
	 * @param sizeIN box half size or sphere radius
	 * @param idIN identification number
	 */
	VirtualObject(glm::vec3 posIN, int bodyCount, float massIN, bool staticIN, float sizeIN, int idIN);

	/** \brief destructor
	 *
	 * destroys a virtual object instance
	 */
	~VirtualObject();

	/** \brief update modelmatrix
	 *
	 * update modelmatrix when simulation is running on cpu
	 * @return void
	 */
	void updateMM();

};

#endif /* VIRTUALOBJECT_H_ */
