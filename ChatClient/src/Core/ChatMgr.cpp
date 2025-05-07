// ChatMgr.cpp 

#include "pch.h"
#include "ChatMgr.h"

void ChatMgr::AddMessage(const std::wstring& message)
{
    m_History.push_back(message);
    m_PendingMessages.push_back(message);
}

const std::vector<std::wstring>& ChatMgr::GetHistory() const
{
    return m_History;
}

const std::vector<std::wstring>& ChatMgr::GetPendingMessages() const
{
    return m_PendingMessages;
}

void ChatMgr::ClearPendingMessages()
{
    m_PendingMessages.clear();
}
