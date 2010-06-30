#ifndef MEMORYHOLDER_H
#define MEMORYHOLDER_H

#include <QtCore>

template <class Member>
class MemoryHolder
{
public:
    MemoryHolder(int cacheSize=10000) : cacheSize(cacheSize) {

    }

    void changeCacheSize(int cacheSize) {
        this->cacheSize = cacheSize;
    }

    bool isInMemory(const QString &name)
    {
        QString n2 = name.toLower();

        QMutexLocker lock(&memberMutex);
        return members.contains(n2) || nonExistentMembers.contains(n2);
    }

    void addMemberInMemory(const Member &m)
    {
        memberMutex.lock();

        nonExistentMembers.remove(m.name);
        if (!members.contains(m.name))
            cachedMembersOrder.push_front(m.name);

        members[m.name] = m;

        memberMutex.unlock();
    }

    void cleanCache()
    {
        cachedMembersMutex.lock();

        while(cachedMembersOrder.size() > cacheSize) {
            removeMemberInMemory(cachedMembersOrder.takeLast());
        }

        cachedMembersMutex.unlock();
    }

    void removeMemberInMemory(const QString &name)
    {
        memberMutex.lock();

        members.remove(name);

        memberMutex.unlock();
    }
    void addNonExistant(const QString &name)
    {
        memberMutex.lock();

        nonExistentMembers.insert(name.toLower());

        memberMutex.unlock();
    }
    /* Precondition: the member must be in memory. */
    bool exists(const QString &name) const
    {
        QString n2 = name.toLower();

        {
            QMutexLocker m(&memberMutex);

            if (members.contains(n2)) {
                return true;
            }
            else
                return false;

//            if (nonExistentMembers.contains(n2))
//                return false;
        }
    }

    Member member (const QString &name2) const {
        QString name = name2.toLower();

        if (exists(name)) {
            QMutexLocker m(&memberMutex);
            return members.value(name);
        }

        qDebug() << "Critical! Unreachable code reached";
        return Member(name);
    }

protected:
    QHash<QString, Member> members;
    QSet<QString> nonExistentMembers;
    mutable QMutex memberMutex;
    QLinkedList<QString> cachedMembersOrder;
    mutable QMutex cachedMembersMutex;
    int cacheSize;
};

#endif // MEMORYHOLDER_H