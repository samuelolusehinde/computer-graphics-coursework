#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "maths.hpp"

//Defines several possible options for camera movement. Used as abstraction to 
//stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


//An abstract camera class that processes input and calculates the corresponding Euler Angles, 
//Vectors and Matrices for use in OpenGL
class Camera
{
public:
	//Camera Attributes
	Vec3 Position;
	Vec3 Front;
	Vec3 Up;
	Vec3 Right;
	Vec3 WorldUp;
	//Euler Angles
	float Yaw;
	float Pitch;
	//Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	//Constructor with vectors
	Camera(Vec3 position = Vec3(0.0f, 0.0f, 0.0f), Vec3 up = Vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH) : Front(Vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	//Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
		Front(Vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = Vec3(posX, posY, posZ);
		WorldUp = Vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}



	Mat4 GetViewMatrixCustonm()
	{
		Vec3 Pos = { Position.x, Position.y, Position.z };
		Vec3 FrontCust = { Front.x, Front.y, Front.z };

		{
			return LookAt(Pos, Pos + FrontCust, {Up.x, Up.y, Up.z});
		}
	}

	Mat4 GetViewMatrixQuat()
	{
		Quat Xrot(0.0f, 1.0f, 0.0f, Yaw);
		Quat Yrot(1.0f, 0.0f, 0.0f, Pitch);

		Mat4 rotation = QuaternionToMatrix((Xrot)); // inverse of the camera's rotation
		Mat4 rotation2 = QuaternionToMatrix((Yrot)); // inverse of the camera's rotation
		

		Mat4 translation = Translate(Identity(), Vec4(-Position.x, -Position.y, -Position.z, 1.0f)); // inverse of the camera's position
		Mat4 view = Multiply( Multiply( rotation, rotation2 ) , translation);
		return view;
	}



	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position = Position + ( Front * velocity);
		if (direction == BACKWARD)
			Position = Position - ( Front * velocity );
		if (direction == LEFT)
			Position = Position - (Right * velocity);
		if (direction == RIGHT)
			Position = Position + (Right * velocity);
	}

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		//Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	void updateCameraVectors()
	{
		//Calculate the new Front vector
		Vec3 front;
		front.x = cos(toRadians(Yaw)) * cos(toRadians(Pitch));
		front.y = sin(toRadians(Pitch));
		front.z = sin(toRadians(Yaw)) * cos(toRadians(Pitch));
	
		Front = front.normalize();

		//Also re-calculate the Right and Up vector
		//Normalize the vectors, because their length gets closer to 0 
		//the more you look up or down which results in slower movement.
		Right = Front.cross(WorldUp).normalize();
		Up = Right.cross(Front).normalize();
	}
};





class ThirdPersonCamera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	bool isFixed;
	float FixedHeight;


	float PlayerAngle = 0;
	glm::vec3 PlayyerPosition;


	ThirdPersonCamera(bool fixedJHeight = 0, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		isFixed = fixedJHeight;
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	ThirdPersonCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	inline glm::mat4 GetViewMatrix()
	{
		updateCameraVectors();
		return 	glm::lookAt(Position, PlayyerPosition, glm::vec3(0.0f, 1.0f, 0.0f));

	}

	Mat4 GetViewMatrixCustonm()
	{
		Vec3 Pos = { Position.x, Position.y, Position.z };
		Vec3 FrontCust = { Front.x, Front.y, Front.z };

		{
			return LookAt(Pos, Pos + FrontCust, { Up.x, Up.y, Up.z });
		}
	}

	inline void SetFixedHeight(bool isFixed, float Height)
	{
		this->isFixed = isFixed;
		this->FixedHeight = Height;
	}

	inline void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;

		if (Pitch < 87)
		{
			if (direction == FORWARD)
				Position += Front * velocity;
			if (direction == BACKWARD)
				Position -= Front * velocity;
		}

		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;

		if (isFixed)
		{
			Position.y = FixedHeight;
		}
	}

	inline void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			Pitch = glm::clamp(Pitch, 15.0f, 45.0f);
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	inline void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}



private:
	inline void updateCameraVectors()
	{
		//sets view
		float m_angle_around = Yaw;
		float angle = glm::radians(m_angle_around + PlayerAngle);
		float Distance = 40.0f;

		float hDist = Distance * glm::cos(glm::radians(Pitch));
		float vDist = Distance * glm::sin(glm::radians(Pitch));
		float offsetX = hDist * glm::sin(angle);
		float offsetZ = hDist * glm::cos(angle);

		glm::vec3 m_focal_point = PlayyerPosition;
		Position = glm::vec3(offsetX, vDist, offsetZ) + m_focal_point;


	}
};



#endif