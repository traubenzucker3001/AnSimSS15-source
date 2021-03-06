/*
 * RigidBody.h
 *
 *  Created on: 01.09.2015
 *      Author: Daniel
 */


#ifndef RIGIDBODY_H_
#define RIGIDBODY_H_


//includes
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

#include "ForceActor.h"



/** \brief RigidBody
 *
 * class for rigid body representation
 */
class RigidBody {

protected:

	float mMass;							/**< body mass */
	float mInverseMass;						/**< body inverse mass */

	glm::vec3 mPosition;					/**< body position */
	glm::vec3 mVelocity;					/**< body velocity */
	glm::quat mRotationQuat;					/**< body rotation quaternion */
	glm::mat3 mRotationMat;					/**< body rotation matrix */
	glm::mat3 mInertiaTensor;				/**< body inertia tensor */
	glm::vec3 mInitInverseInertTensDiagon;		/**< initial inverse inertia tensor diagonal */
	glm::mat3 mInverseInertiaTensor;			/**< inverse inertia tensor */
	glm::vec3 mStartingDirection;				/**< direction in which the rocket will start */

	glm::vec3 mAngularVelocity;				/**< angular velocity */
	glm::vec3 mAngularMomentum;				/**< angular momentum */
	glm::vec3 mLinearMomentum;				/**< linear momentum */
	glm::vec3 mTorque;						/**< Torque */

	glm::vec3 mForce;						/**< body force */
	glm::mat4 mTransformMatrix;				/**< transformation matrix */

	std::vector<ForceActor*> mForces;

public:

	/** \brief constructor
	 *
	 * creates a rigid body instance with given parameters
	 * @param massIN mass
	 * @param posIN position
	 * @param rotIN rotation
	 */
	RigidBody(float massIN, glm::vec3 posIN);

	/** \brief standard constructor
	*
	* creates a rigidBody instance
	*/
	RigidBody();

	/** \brief destructor
	 *
	 * destroys a rigid body instance
	 */
	~RigidBody();

	/** \brief iterate
	 *
	 * perform linear and angular step to update velocity, position and rotation
	 * @param duration time length of a simulation step
	 * @param gravity gravity in world
	 * @return void
	 */
	void iterate(float duration, float gravity);

	/** \brief update rotations matrix
	 *
	 * update rotation matrix with new rotation quaternion
	 * @return void
	 */
	void updateRotMatrix();

	/** \brief update inverse inertia tensor
	 *
	 * update inertia tensor with new rotation matrix
	 * @return void
	 */
	void updateInverseInertiaTensor();

	/** \brief update momenta
	 *
	 * apply gravity and update linear and angular momenta
	 * @param duration time length of a simulation step
	 * @param gravity gravity in world
	 * @return void
	 */
	void updateMomenta(float duration, float gravity);

	/** \brief calculates torque
	 *
	 * calculates torque from given Forces of mForces
	 * @return void
	 */
	void calculateTorque();

	/** \brief adds force to the mforces vector
	 *
	 * @param forces ForceActor with a vector for the point to apply
	 * 		the force in model space and the force itself
	 * @return void
	 */
	void addForce(ForceActor* force);

	/** \brief resets the rigid body
	 *
	 * @param newPosition	the place where to set the rigid body
	 * @param rotQuat	how the rigid body should be rotated in the world
	 * @return void
	 */
	void reset(glm::vec3 newPosition, glm::quat rotQuat);

	/** \brief calculate mForce
	 *
	 * calculate mForce from given Forces of mForces
	 * @return void
	 */
	void calculateForce();

	/** \brief prints information about Rigidbody
	*
	* prints some information about position, forces, momenta etc. onto the console.
	* @return void
	*/
	void printInfo();


	//getter + settter
	const glm::vec3& getPosition() const {
		return mPosition;
	}

	void setPosition(const glm::vec3& position) {
		this->mPosition = position;
	}

	const glm::mat3& getRotationMat() const {
		return mRotationMat;
	}

	void setRotationMat(const glm::mat3& rotationMat) {
		this->mRotationMat = rotationMat;
	}

	const glm::quat& getRotationQuat() const {
		return mRotationQuat;
	}

	void setRotationQuat(const glm::quat& rotationQuat) {
		this->mRotationQuat = rotationQuat;
	}

	void setInertiaTensor(const glm::mat3& inertia){
		this->mInertiaTensor = inertia;
	}

	const glm::vec3& getStartingDirection() const {
		return mStartingDirection;
	}

	void setStartingDirection(const glm::vec3& startingDirection) {
		mStartingDirection = startingDirection;
	}
};


#endif /* RIGIDBODY_H_ */
