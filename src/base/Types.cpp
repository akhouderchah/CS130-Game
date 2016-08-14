#include "Types.h"

const ObjHandle ObjHandle::null;
const ObjHandle::ID_t ObjHandle::MAX_ID = UINT32_MAX;

ObjHandle::ObjHandle() : m_Handle(0)
{
}

ObjHandle::ObjHandle(handle_t handleVal) : m_Handle(handleVal)
{
}

ObjHandle::handle_t ObjHandle::constructRawHandle(ID_t ID, type_t type, version_t version)
{
	handle_t handle = 0;
	handle |= (handle_t)version;
	handle |= ((handle_t)type) << 16;
	handle |= ((handle_t)ID) << 32;

	return handle;
}

ObjHandle ObjHandle::constructHandle(ID_t ID, type_t type, version_t version)
{
	return ObjHandle(constructRawHandle(ID, type, version));
}

ObjHandle::ID_t ObjHandle::GetID() const
{
	handle_t ID = m_Handle & 0xFFFFFFFF00000000;
	return (ID_t)(ID >> 32);
}

ObjHandle::type_t ObjHandle::GetType() const
{
	handle_t type = m_Handle & 0x00000000FFFF0000;
	return (type_t)(type >> 16);
}

ObjHandle::version_t ObjHandle::GetVersion() const
{
	handle_t version = m_Handle & 0x000000000000FFFF;
	return (version_t)version;
}

