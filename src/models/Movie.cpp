#include "models/Movie.h"

Movie::Movie(int id, const QString& title, int duration,
             const QString& genre, QObject* parent)
    : QObject(parent)
    , m_id(id)
    , m_title(title)
    , m_duration(duration)
    , m_genre(genre)
{
}
