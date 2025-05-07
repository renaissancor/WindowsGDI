// ChatMgr.h 
#pragma once

#include "Singleton.h" 

#include <string>
#include <vector>

class ChatMgr : public Singleton<ChatMgr>
{
	friend class Singleton<ChatMgr>; // private access to Singleton
private:
    std::vector<std::wstring> m_History;         // All messages
    std::vector<std::wstring> m_PendingMessages; // Messages waiting to send

public:
    void AddMessage(const std::wstring& message);

    const std::vector<std::wstring>& GetHistory() const;
    const std::vector<std::wstring>& GetPendingMessages() const;

    void ClearPendingMessages();
};
