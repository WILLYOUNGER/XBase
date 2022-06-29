#include "XHttpStruct.h"

std::map<XNETSTRUCT::XSocket, std::list<XNETSTRUCT::XResponse>*> XNETSTRUCT::XResponse::m_reply;