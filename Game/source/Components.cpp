#include "stdafx.h"
#include "Entity.h"
#include "EntitiesSystem.h"
#include "Quadtree.h"
#include <ctime>
#include "Scripts.h"

#include "Components.h"

using namespace stdio_fw;

///////////////////////////////////////////
// Component
Component::Component()
{
	m_type = CompType::COMP_UNKNOWN;
}

Component::~Component()
{
	m_parent = NULL;
}
///////////////////////////////////////////


///////////////////////////////////////////
// Transform
Transform::Transform(Vec3 _position, Vec3 _rotation, Vec3 _scale)
{
	m_position = _position;
	m_rotation = _rotation;
	m_scale = _scale;

	m_type = CompType::COMP_TRANSFORM;
}

Transform::~Transform()
{

}

void Transform::Update()
{

}
///////////////////////////////////////////


///////////////////////////////////////////
// Renderer
Renderer::Renderer(Image* _sprite)
{
	m_sprite = _sprite;
	m_bound = Rect(0, 0, m_sprite->getWidth(), m_sprite->getHeight());

	m_type = CompType::COMP_RENDERER;
}

Renderer::~Renderer()
{
	m_sprite->unloadImage();
	SAFE_DEL(m_sprite);
}

void Renderer::Update()
{

}

void Renderer::UpdateBound()
{
	m_bound = Rect(0, 0, m_sprite->getWidth(), m_sprite->getHeight());
}
///////////////////////////////////////////


///////////////////////////////////////////
// Rigidbody2D
Rigidbody2D::Rigidbody2D()
{
	m_type = CompType::COMP_RIGIDBODY2D;
}

Rigidbody2D::~Rigidbody2D()
{

}

void Rigidbody2D::Update()
{

}
///////////////////////////////////////////


///////////////////////////////////////////
// Collider2D
Collider2D::Collider2D(Rect _bound)
{
	m_type = CompType::COMP_COLLIDER2D;
	m_bound = _bound;

	m_collisionObject = NULL;
}

Collider2D::~Collider2D()
{

}

void Collider2D::Update()
{ 
	m_bound.x = m_parent->m_transform->m_position.x - m_bound.width / 2;
	m_bound.y = m_parent->m_transform->m_position.y - m_bound.height / 2;

	std::vector<Entity*> detectEntityList;
	detectEntityList = EntitiesSystem::GetInstance()->m_quadtree->Retrieve(m_parent);


	for(int i = 0; i < detectEntityList.size(); i++)
	{
		std::vector<Component*> collider2dList = detectEntityList[i]->GetComponents(CompType::COMP_COLLIDER2D);

		for(int j = 0; j < collider2dList.size(); j++)
		{
			if(CheckAABB(static_cast<Collider2D*>(collider2dList[j])))
			{
				m_collisionObject = detectEntityList[i];
				
				if(m_collisionObject->IsTaggedAs("Tank"))
				{
					TankController* tankController = static_cast<TankController*>(m_parent->GetComponent(CompType::COMP_TANKCONTROLLER));
					if(tankController)
						tankController->m_lockDirection = tankController->m_direction;
				}

				detectEntityList.clear();
				return;
			}
		}
	}
}

bool Collider2D::CheckAABB(Collider2D* _collider2d)
{
	return !(m_bound.x + m_bound.width < _collider2d->m_bound.x
		|| m_bound.y + m_bound.height < _collider2d->m_bound.y
		|| m_bound.x > _collider2d->m_bound.x + _collider2d->m_bound.width
		|| m_bound.y > _collider2d->m_bound.y + _collider2d->m_bound.height);
}
///////////////////////////////////////////


///////////////////////////////////////////
// Animator
Animator::Animator()
{
	m_type = CompType::COMP_ANIMATOR;
}

Animator::~Animator()
{

}

void Animator::Update()
{

}

void Animator::SetFrameList(int _count, ...)
{
	char* t;
	va_list ap;
	va_start(ap, _count);

	for(int i = 0; i < _count; i++)
	{
		t = va_arg(ap, char*);

		Image* image = new Image(t);
		image->loadImage();
		m_frameList.push_back(image);

		m_frameTime.push_back(-1);
	}

	va_end(ap);
}

void Animator::SetFrameTime(int _count, ...)
{

}
///////////////////////////////////////////