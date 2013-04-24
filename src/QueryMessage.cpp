#include "QueryMessage.h"
#include "Project.h"
#include <rct/Serializer.h>

QueryMessage::QueryMessage(Type type)
    : ClientMessage(MessageId), mType(type), mFlags(0), mMax(-1), mMinLine(-1), mMaxLine(-1), mBuildIndex(0)
{
}

void QueryMessage::encode(Serializer &serializer) const
{
    serializer << mRaw << mQuery << mContext << mType << mFlags << mMax
               << mMinLine << mMaxLine << mBuildIndex << mPathFilters << projects();
}

void QueryMessage::decode(Deserializer &deserializer)
{
    List<String> projects;
    deserializer >> mRaw >> mQuery >> mContext >> mType >> mFlags >> mMax
                 >> mMinLine >> mMaxLine >> mBuildIndex >> mPathFilters >> projects;
    setProjects(projects);
}

unsigned QueryMessage::keyFlags(unsigned queryFlags)
{
    unsigned ret = Location::NoFlag;
    if (!(queryFlags & QueryMessage::NoContext))
        ret |= Location::ShowContext;
    if (queryFlags & QueryMessage::CursorInfoIncludeReferences)
        ret |= Project::Cursor::IncludeReferences;
    if (queryFlags & QueryMessage::CursorInfoIncludeTarget)
        ret |= Project::Cursor::IncludeTarget;
    return ret;
}

Match QueryMessage::match() const
{
    unsigned flags = Match::Flag_StringMatch;
    if (mFlags & MatchRegexp)
        flags |= Match::Flag_RegExp;

    return Match(mQuery, flags);
}
