#include "models/Theater.h"

Theater::Theater(int id, const QString& name, int capacity, QObject* parent)
    : QObject(parent)
    , m_id(id)
    , m_name(name)
    , m_capacity(capacity)
{
}
